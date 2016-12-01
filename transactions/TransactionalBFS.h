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
  typedef GraphType::VertexDescriptor VertexDescriptor;
  typedef unsigned int IdType;
	typedef std::pair<VertexPointer, bool> VisitPair;
	typedef Transaction* TransactionType;
  typedef LocksManager   LockManagerType;

public:
	void breadthFirstSearch(GraphType & Graph
                        , const VertexDescriptor & StartVertex
                        , Visitor & GraphVisitor
                        , TransactionType  Tranx
                        , LockManagerType & LockManager
                        ) {

		auto ScheduledVertex = Graph.getVertexPointer(StartVertex);
    IdType TxId = Tranx->getId();

#if _DEBUG_ENABLE_
		if (ScheduledVertex  == nullptr) {
			std::cerr << "Error: No such vertex in graph \n";
      return ;
//			exit(1);
		}
#endif
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
      auto MutexPtr = LockManager.getVertexLockPointer(ScheduledVertex->getId(), T_Property);
      Tranx->visitMutex(MutexPtr);
#endif
			if (!(LockManager.getVertexLock(ScheduledVertex->getId(), T_Property, T_SH, TxId))) {
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
      auto NEMutexPtr = LockManager.getVertexLockPointer(ScheduledVertex->getId(), T_NextEdge);
      Tranx->visitMutex(NEMutexPtr);
#endif
			if (!(LockManager.getVertexLock(ScheduledVertex->getId(), T_NextEdge, T_SH, TxId))){
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
      auto FVMutexPtr = LockManager.getEdgeLockPointer(NextEdge->getId(), T_FirstVertex);
      Tranx->visitMutex(FVMutexPtr);
#endif
				if (!LockManager.getEdgeLock(NextEdge->getId(), T_FirstVertex, T_SH, TxId)) {

#ifdef _TRANX_STATS_
        Tranx->abortMutex(FVMutexPtr);
#endif
          Tranx->abort();
          return;
				}

#ifdef _TRANX_STATS_
      auto SVMutexPtr = LockManager.getEdgeLockPointer(NextEdge->getId(), T_SecondVertex);
      Tranx->visitMutex(SVMutexPtr);
#endif
				if (!LockManager.getEdgeLock(NextEdge->getId(), T_SecondVertex, T_SH, TxId)) { 
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
      auto FNEMutexPtr = LockManager.getEdgeLockPointer(NextEdge->getId(), T_FirstNextEdge);
      Tranx->visitMutex(FNEMutexPtr);
#endif
				if (!LockManager.getEdgeLock(NextEdge->getId(), T_FirstNextEdge, T_SH, TxId)) {
#ifdef _TRANX_STATS_
          Tranx->abortMutex(FNEMutexPtr);
#endif
          Tranx->abort();
          return;
				}

#ifdef _TRANX_STATS_
      auto SNEMutexPtr = LockManager.getEdgeLockPointer(NextEdge->getId(), T_SecondNextEdge);
      Tranx->visitMutex(SNEMutexPtr);
#endif
				if ( !LockManager.getEdgeLock(NextEdge->getId(), T_SecondNextEdge, T_SH, TxId)) {

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
