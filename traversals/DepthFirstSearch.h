//===-- traversals/DepthFirstSearch.h - DFS search --------------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is a  DFS search as taken from boost.
///
//===----------------------------------------------------------------------===//
#ifndef _DEPTH_FIRST_SEARCH_H_
#define _DEPTH_FIRST_SEARCH_H_

#include "Visitor.h"
#include "GraphType.h"

#define _STATS_ true

void depthFirstSearch(GraphType & Graph,
                      GraphType::VertexDescriptor StartVertex,
                      Visitor & GraphVisitor ) {


  typedef std::pair<GraphType::VertexPointer, bool> VisitPair;
  GraphType::VertexPointer ScheduledVertex = Graph.getVertexPointer(StartVertex);

  std::vector<GraphType::VertexPointer> VertexStack;
  std::unordered_map<GraphType::VertexPointer, bool> ColorMap;

  VertexStack.push_back(ScheduledVertex);
  GraphVisitor.visitStartVertex(ScheduledVertex);

  /// ColorMap is used to not enter a loop (in acyclic graphs)
  ColorMap.insert(VisitPair(ScheduledVertex, false));

  GraphType::VertexPointer TargetVertex = nullptr;

  while ( !VertexStack.empty() ) {
    ScheduledVertex = VertexStack.back();  VertexStack.pop_back();

    bool VertexMatch = GraphVisitor.visitVertex(ScheduledVertex);

#ifdef _STATS_
    GraphVisitor.countVertex(ScheduledVertex);
#endif

    if (VertexMatch == true)
      return ;
 
    ColorMap[ScheduledVertex] = true;


    auto NextEdge = ScheduledVertex->getNextEdge();
    while (NextEdge != nullptr) {
    
#ifdef _STATS_
    GraphVisitor.countEdge(NextEdge);
#endif
      TargetVertex = NextEdge->getTarget(ScheduledVertex);
      /// Revisit flag is set when one vertex appears in different depths 
      /// and count depth is needed.
      bool RevisitFlag = GraphVisitor.discoverVertex(TargetVertex);
      bool BranchMatch = GraphVisitor.scheduleBranch(ScheduledVertex, NextEdge, TargetVertex);
      /// Type (label) and direction decide to visit later or not
      bool TypeMatch = GraphVisitor.scheduleEdge(NextEdge);
      bool DirectionMatch = GraphVisitor.visitDirection(TargetVertex, NextEdge);

      /// Break out of the loop but not return
      if (BranchMatch == true)
        break;

      if ( ColorMap.find(TargetVertex) == ColorMap.end() || RevisitFlag ) {

        GraphVisitor.scheduleTree(ScheduledVertex, NextEdge, TargetVertex);

        if ( TypeMatch && DirectionMatch) {
          VertexStack.push_back(TargetVertex);
          ColorMap.insert(VisitPair(TargetVertex, false));
        } else {
          //VisitedMap.insert(ColorMapPair(TargetVertex, true));
          GraphVisitor.revisitVertex(TargetVertex);
        }
      }
      NextEdge = NextEdge->getNextEdge(ScheduledVertex);
    
    }
  }
  GraphVisitor.finishVisit();
}
                      

#endif /* _DEPTH_FIRST_SEARCH_H_ */
