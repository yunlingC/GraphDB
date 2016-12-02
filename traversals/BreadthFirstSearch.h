//===-- traversals/BreadthFirstSearch.h - BFS search ----*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is a breadth first search.
///
//===----------------------------------------------------------------------===//
#ifndef _BREADTHFIRSTSEARCH_H_
#define _BREADTHFIRSTSEARCH_H_

/// Local includes.
#include "GraphType.h"
#include "Visitor.h"

/// Standard includes.
#include <queue>
#include <unordered_map>

#define _STATS_ true

void breadthFirstSearch(GraphType & Graph, 
                        const GraphType::VertexDescriptor & StartVertex, 
                        Visitor & GraphVisitor) {

  typedef std::pair<GraphType::VertexPointer, bool> VisitPair;

  std::queue<GraphType::VertexPointer> VertexQueue;

  auto ScheduledVertex = Graph.getVertexPointer(StartVertex);

  /// True means visited and false means not visited.
  std::unordered_map<GraphType::VertexPointer, bool> ColorMap;

  VertexQueue.push(ScheduledVertex);               

  GraphVisitor.visitStartVertex(ScheduledVertex);

  ColorMap.insert(VisitPair(ScheduledVertex, false));

  GraphType::VertexPointer TargetVertex = nullptr;

  while (!VertexQueue.empty()) {
    ScheduledVertex = VertexQueue.front();  VertexQueue.pop();

    bool VertexMatch = GraphVisitor.visitVertex(ScheduledVertex);
#ifdef _STATS_
    GraphVisitor.countVertex(ScheduledVertex);
#endif

    if (VertexMatch == true)
      return;
      
    /// Set to visited.    
    ColorMap[ScheduledVertex] = true;


    auto NextEdge = ScheduledVertex->getNextEdge();

    while (NextEdge != nullptr) {                  

#ifdef _STATS_
    GraphVisitor.countEdge(NextEdge);
#endif
      /// Get the target node.
      TargetVertex = NextEdge->getTarget(ScheduledVertex);     
      bool RevisitFlag = GraphVisitor.discoverVertex(TargetVertex);             

      bool BranchMatch = GraphVisitor.scheduleBranch(ScheduledVertex, NextEdge, TargetVertex);
      bool TypeMatch =  GraphVisitor.scheduleEdge(NextEdge);
      bool DirectionMatch = GraphVisitor.visitDirection(TargetVertex, NextEdge);

      if (BranchMatch == true)
        return;

      if (ColorMap.find(TargetVertex) == ColorMap.end()|| RevisitFlag) {
       	/// Queue up the target for visitation
        GraphVisitor.scheduleTree(ScheduledVertex, NextEdge, TargetVertex);

        /// Determine whether the vertex to be visited based on the filters 
        if (TypeMatch && DirectionMatch)   {
	        VertexQueue.push(TargetVertex);

	        ColorMap.insert(VisitPair(TargetVertex,false));
        }
      } else {
        GraphVisitor.revisitVertex(TargetVertex);
      }
      // Get the next edge from the scheduled vertex. 
      NextEdge = NextEdge->getNextEdge(ScheduledVertex);
    }
  }
  GraphVisitor.finishVisit();
};

#endif /* _BREADTH_FIRST_SEARCH_H_ */
