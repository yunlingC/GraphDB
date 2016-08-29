//===-- transactions/TranxBreadthFirstSearch.h - BFS class --*- C++ -*-===//
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

#ifndef _TRANX_BREADTHFIRSTSEARCH_H_
#define _TRANX_BREADTHFIRSTSEARCH_H_

/// Local includes.
#include "GraphType.h"
#include "Visitor.h"
#include "Transaction.h"

#include <queue>
#include <unordered_map>
#include <vector>
#include <GDBench/Utils.h>

//TODO
/// No skip-lock in this tranxBFS
/// back-off one by one /read or write - stack
/// update graph->add/delete nodes/edges
/// remove std
/// Macros of spinning on locks
/// add comments

	typedef std::pair<GraphType::VertexPointer, bool> VisitPair;
	typedef Transaction TransactionType;

	/// get read locks one by one and store in locklists,
	/// then release locks in reverse order one by one before return.
	void tranxBreadthFirstSearch(GraphType & Graph,
	                        const GraphType::VertexDescriptor & StartVertex,
	                        Visitor & GraphVisitor,
	                        TransactionType & Tranx
							) {
		auto ScheduledVertex = Graph.getVertexPointer(StartVertex);

		///change 1
		if ( ScheduledVertex  == nullptr ) {
			std::cerr << "No such vertex in graph \n";
			exit(1);
		}

		// Start traversing the graph from here.
		std::queue<GraphType::VertexPointer> VertexQueue;
		/// True means visited and false means not visited.
		std::map<GraphType::VertexPointer, bool> ColorMap;

		VertexQueue.push(ScheduledVertex);
		GraphVisitor.visitStartVertex(ScheduledVertex);

		ColorMap.insert(VisitPair(ScheduledVertex, false));

		GraphType::VertexPointer TargetVertex = nullptr;

		while ( !VertexQueue.empty() ) {
			ScheduledVertex = VertexQueue.front();  VertexQueue.pop();

			/// spin on it unless get the lock or checkOn() return false by RagManager

			if ( !Tranx.getVertexLock(ScheduledVertex, Pp, SH)) {
				/// TODO
			}

			}

			Tranx.registerVertexLock(ScheduledVertex, Pp, SH);

			if (GraphVisitor.visitVertex(ScheduledVertex))
				return;

			/// Set to visited.
			ColorMap[ScheduledVertex] = true;

			if (!Tranx.getVertexLock(ScheduledVertex, NE, SH)){/// TODO }

			Tranx.registerVertexLock(ScheduledVertex, NE, SH);

			auto NextEdge = ScheduledVertex->getNextEdge();
			while ( NextEdge != nullptr ) {
				/// Get the target node.

				if (!Tranx.getEdgeLock(NextEdge, FV, SH)) {
					/// TODO
				}

				if (!Tranx.getEdgeLock(NextEdge, SV, SH)) {
					/// TODO
				}

				Tranx.registerEdgeLock(NextEdge, FV, SH);
				Tranx.registerEdgeLock(NextEdge, SV, SH);

				TargetVertex = NextEdge->getTarget(ScheduledVertex);
				bool RevisitFlag = GraphVisitor.discoverVertex(TargetVertex);
				bool BranchMatch = GraphVisitor.scheduleBranch(ScheduledVertex, NextEdge, TargetVertex);
				bool TypeMatch =  GraphVisitor.scheduleEdge(NextEdge);
				bool DirectionMatch = GraphVisitor.visitDirection(TargetVertex, NextEdge);

				if( BranchMatch )
					return;

				if ( ColorMap.find(TargetVertex ) == ColorMap.end() || RevisitFlag ) {
					/// queue the target for visitation
					GraphVisitor.scheduleTree(ScheduledVertex, NextEdge, TargetVertex);

					if( TypeMatch && DirectionMatch )   {
						///control the vertex to be visited filtered by type
						VertexQueue.push(TargetVertex);

						ColorMap.insert(VisitPair(TargetVertex,false));
					}
				} else {
					GraphVisitor.revisitVertex( TargetVertex );
				}

				if (!Tranx.getEdgeLock(NextEdge, FNE, SH)) {
					/// TODO
				}

				if ( !Tranx.getEdgeLock(NextEdge, SNE, SH)) {
					/// TODO
				}

				Tranx.registerEdgeLock(NextEdge, FNE, SH);
				Tranx.registerEdgeLock(NextEdge, SNE, SH);

				/// Get the next edge from the scheduled vertex.
				NextEdge = NextEdge->getNextEdge(ScheduledVertex);
			}
		}
		GraphVisitor.finishVisit();
	}

#endif /*_TRANX_BREADTHFIRSTSEARCH_H */
