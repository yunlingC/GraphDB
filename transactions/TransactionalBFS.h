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
//#include "TransactionManager.h"
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
#ifdef _WAIT_DIE_
    ///True - wait
    return MutexPtr->checkTx(TxPtr->getId());
#elif defined _DEADLOCK_DETECTION_
    return LockManager.checkWaitOn(TxPtr, MutexPtr, Locktype);
#endif
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
    if (getLock) {
      return true;
    }

#ifdef _TRANX_STATUS_
    std::cout << "Transaction\t" << TxPtr->getId()
              << "\tfails in getting vertex lock on\t" << VertexPtr->getId()
              << "\tand needs a check\n";
#endif
    bool retValue = false;
    LockManager.startDetect();
    if (checkLock<VertexPointer>(VertexPtr, Mutextype, Locktype, TxPtr, LockManager)) {
#ifdef _TRANX_STATUS_
      std::cout << "Transaction\t" << TxPtr->getId() 
                << "\t waits for vertex Lock\t" << VertexPtr->getId()
                << "\n";
#endif
      retValue =  TxPtr->waitOn(VertexPtr, Mutextype, Locktype);
    }
    else {
#ifdef _TRANX_STATUS_
      std::cout << "Transaction\t" << TxPtr->getId() 
                << "\t aborts on vertex Lock\t" << VertexPtr->getId()
                << "\n";
#endif
//      TxPtr->abort(VertexPtr, Mutextype, Locktype);
//      return false;
    }

    LockManager.endDetect();
    return retValue;
//    return true;
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
    if (getLock) {
      return true;
    }

#ifdef _TRANX_STATUS_
    std::cout << "Transaction\t" << TxPtr->getId()
              << "\tfails in getting edge lock on\t" << EdgePtr->getId()
              << "\tand needs a check\n";
#endif

    bool retValue = false;
    LockManager.startDetect();
    if (checkLock<EdgePointer>(EdgePtr, Mutextype, Locktype, TxPtr, LockManager)) {
#ifdef _TRANX_STATUS_
//      std::cout << "Transaction\t" << TxPtr->getId() 
//                << "\t waits for edge Lock\t" << EdgePtr->getId()
//                << "\n";
#endif
      retValue = TxPtr->waitOn(EdgePtr, Mutextype, Locktype);
    }
    else {
#ifdef _TRANX_STATUS_
      std::cout << "Transaction\t" << TxPtr->getId() 
                << "\t aborts on Lock\t" << EdgePtr->getId()
                << "\n";
#endif
//      TxPtr->abort(EdgePtr, Mutextype, Locktype);
//      return false;
    }
    LockManager.endDetect();
    return retValue;
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
			if (!getVertexLock(ScheduledVertex, T_Property, T_SH, Tranx, LockManager)) {
        Tranx->abort(ScheduledVertex, T_Property, T_SH);
        return;
      }

			if (GraphVisitor.visitVertex(ScheduledVertex))
				return;

			/// Set to visited.
			ColorMap[ScheduledVertex] = true;

			if (!getVertexLock(ScheduledVertex, T_NextEdge, T_SH, Tranx, LockManager)) {
        Tranx->abort(ScheduledVertex, T_NextEdge, T_SH);
        return;
      }

			auto NextEdge = ScheduledVertex->getNextEdge();
			while ( NextEdge != nullptr ) {
				/// Get locks on the target node.

				if (!getEdgeLock(NextEdge, T_FirstVertex, T_SH, Tranx, LockManager)) {
          Tranx->abort(NextEdge, T_FirstVertex, T_SH);
          return;
				}

				if (!getEdgeLock(NextEdge, T_SecondVertex, T_SH, Tranx, LockManager)) { 
          Tranx->abort(NextEdge, T_SecondVertex, T_SH);
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
				if (!getEdgeLock(NextEdge, T_FirstNextEdge, T_SH, Tranx, LockManager)) {
          Tranx->abort(NextEdge, T_FirstNextEdge, T_SH);
          return;
				}

				if ( !getEdgeLock(NextEdge, T_SecondNextEdge, T_SH, Tranx, LockManager)) {
          Tranx->abort(NextEdge, T_SecondNextEdge, T_SH);
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
