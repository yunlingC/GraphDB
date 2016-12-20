//===-- transactions/TransactionalBFS.h - BFS class --*- C++ -*-===//
//
//                     CAESR Graph Database
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the main class for BFS in transactions.
///
//===----------------------------------------------------------------------===//

#ifndef _TRANSACTIONAL_BFS_H_
#define _TRANSACTIONAL_BFS_H_

/// Local includes.
#include "GraphType.h"
#include "Visitor.h"
#include "Transaction.h"
#include "LocksManager.h"
#include "global.h"

#include <queue>
#include <unordered_map>
#include <vector>

class TransactionalBFS {
public:
  typedef GraphType::VertexPointer  VertexPointer;
  typedef GraphType::EdgePointer EdgePointer;
  typedef GraphType::VertexDescriptor VertexDescriptor;
  typedef Lock::MutexPointer MutexPointer;
  typedef unsigned int IdType;
	typedef std::pair<VertexPointer, bool> VisitPair;
	typedef Transaction* TransactionPointer;
  typedef LocksManager   LockManagerType;

public:

  template<typename ObjectPointer>
  bool checkLock(ObjectPointer ObjPtr
               , MutexType Mutextype
               , LockType Locktype
               , TransactionPointer TxPtr
               , LockManagerType & LockManager
               ) {
    auto MutexPtr = ObjPtr->getLockPtr()->getMutexPtr(Mutextype);
//    auto wait = LockManager.checkWaitOn(TxPtr->getId(), MutexPtr, Locktype);
//    if (wait) {
//      TxPtr->waitOn(ObjPtr, Mutextype, Locktype);
//    }
//    else {
//      TxPtr->abort();
//      return false;
//    }
    return true;
  }

  bool getVertexLock(VertexPointer VertexPtr
                    , MutexType Mutextype
                    , LockType Locktype
                    , TransactionPointer TxPtr
                    , LockManagerType & LockManager
                    ) {
    bool getLock = TxPtr->getVertexLock(VertexPtr, Mutextype, Locktype);
#ifdef _NO_WAIT_
    return getLock;
#else
    if (!getLock) {
      checkLock<VertexPointer>(VertexPtr, Mutextype, Locktype, TxPtr, LockManager);
    }
    return true;
#endif
  }

  bool getEdgeLock(EdgePointer EdgePtr
                  , MutexType Mutextype
                  , LockType Locktype
                  , TransactionPointer TxPtr
                  , LockManagerType & LockManager
                  )  {
    bool getLock = TxPtr->getEdgeLock(EdgePtr, Mutextype, Locktype);
#ifdef _NO_WAIT_
    return getLock;
#else
    if (!getLock) {
      checkLock<EdgePointer>(EdgePtr, Mutextype, Locktype, TxPtr, LockManager);
    }
    return true;
#endif
  }

	void breadthFirstSearch(GraphType & Graph
                        , const VertexDescriptor & StartVertex
                        , Visitor & GraphVisitor
                        , TransactionPointer  Tranx
                        , LockManagerType & LockManager
                        ) {

		auto ScheduledVertex = Graph.getVertexPointer(StartVertex);

    assert(ScheduledVertex != nullptr && "Vertex pointer invalid" );

		// Start traversing the graph from here.
		std::queue<VertexPointer> VertexQueue;
		/// True means visited and false means not visited.
		std::unordered_map<VertexPointer, bool> ColorMap;

		VertexQueue.push(ScheduledVertex);
		GraphVisitor.visitStartVertex(ScheduledVertex);

		ColorMap.insert(VisitPair(ScheduledVertex, false));

		VertexPointer TargetVertex = nullptr;

		while ( !VertexQueue.empty() ) {
			ScheduledVertex = VertexQueue.front();  VertexQueue.pop();

      /// If false, this lock canNOT be acquired and transaction aborts
#ifdef _TRANX_STATS_
      auto MutexPtr = ScheduledVertex->getLockPtr()->getMutexPtr(T_Property);
      Tranx->visitMutex(MutexPtr);
#endif
			if (!getVertexLock(ScheduledVertex, T_Property, T_SH, Tranx, LockManager)) {
#ifdef _TRANX_STATS_
        Tranx->abortMutex(MutexPtr);
#endif
        Tranx->abort();
        return;
      }

			if (GraphVisitor.visitVertex(ScheduledVertex))
				return;

			/// Set to visited.
			ColorMap[ScheduledVertex] = true;

#ifdef _TRANX_STATS_
      auto NEMutexPtr = ScheduledVertex->getLockPtr()->getMutexPtr(T_NextEdge);
      Tranx->visitMutex(NEMutexPtr);
#endif
			if (!getVertexLock(ScheduledVertex, T_NextEdge, T_SH, Tranx, LockManager)) {
#ifdef _TRANX_STATS_
        Tranx->abortMutex(NEMutexPtr);
#endif
        Tranx->abort();
        return;
      }

			auto NextEdge = ScheduledVertex->getNextEdge();
			while ( NextEdge != nullptr ) {
				/// Get locks on the target node.

#ifdef _TRANX_STATS_
      auto FVMutexPtr = NextEdge->getLockPtr()->getMutexPtr(T_FirstVertex);
      Tranx->visitMutex(FVMutexPtr);
#endif
				if (!getEdgeLock(NextEdge, T_FirstVertex, T_SH, Tranx, LockManager)) {

#ifdef _TRANX_STATS_
        Tranx->abortMutex(FVMutexPtr);
#endif
          Tranx->abort();
          return;
				}

#ifdef _TRANX_STATS_
      auto SVMutexPtr = NextEdge->getLockPtr()->getMutexPtr(T_SecondVertex);
      Tranx->visitMutex(SVMutexPtr);
#endif
				if (!getEdgeLock(NextEdge, T_SecondVertex, T_SH, Tranx, LockManager)) { 
#ifdef _TRANX_STATS_
          Tranx->abortMutex(SVMutexPtr);
#endif
          Tranx->abort();
          return;
				}

				TargetVertex = NextEdge->getTarget(ScheduledVertex);
				bool RevisitFlag = GraphVisitor.discoverVertex(TargetVertex);
				bool BranchMatch = GraphVisitor.scheduleBranch(ScheduledVertex, NextEdge, TargetVertex);
				bool TypeMatch =  GraphVisitor.scheduleEdge(NextEdge);
				bool DirectionMatch = GraphVisitor.visitDirection(TargetVertex, NextEdge);

				if ( BranchMatch )
					return;

				if ( ColorMap.find(TargetVertex ) == ColorMap.end() || RevisitFlag ) {
					/// queue the target for visitation
					GraphVisitor.scheduleTree(ScheduledVertex, NextEdge, TargetVertex);

					if ( TypeMatch && DirectionMatch )   {
						///control the vertex to be visited filtered by type
						VertexQueue.push(TargetVertex);
						ColorMap.insert(VisitPair(TargetVertex,false));
					}
				} else {
					GraphVisitor.revisitVertex( TargetVertex );
				}

        /// Get shared locks on next edge

#ifdef _TRANX_STATS_
      auto FNEMutexPtr = NextEdge->getLockPtr()->getMutexPtr(T_FirstNextEdge);
      Tranx->visitMutex(FNEMutexPtr);
#endif
				if (!getEdgeLock(NextEdge, T_FirstNextEdge, T_SH, Tranx, LockManager)) {
#ifdef _TRANX_STATS_
          Tranx->abortMutex(FNEMutexPtr);
#endif
          Tranx->abort();
          return;
				}

#ifdef _TRANX_STATS_
      auto SNEMutexPtr = NextEdge->getLockPtr()->getMutexPtr(T_SecondNextEdge);
      Tranx->visitMutex(SNEMutexPtr);
#endif
				if ( !getEdgeLock(NextEdge, T_SecondNextEdge, T_SH, Tranx, LockManager)) {

#ifdef _TRANX_STATS_
          Tranx->abortMutex(SNEMutexPtr);
#endif
          Tranx->abort();
          return;
				}

				/// Get the next edge from the scheduled vertex.
				NextEdge = NextEdge->getNextEdge(ScheduledVertex);
			}
		}
		GraphVisitor.finishVisit();
	}
};

#endif /*_TRANSACTIONAL_BFS_H */
