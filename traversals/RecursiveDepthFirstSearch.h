//===-- traversals/RecursiveDepthFirstSearch.h - DFS search ----*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is a simple recursive DFS search.
///
//===----------------------------------------------------------------------===//
#ifndef _RECURSIVE_DEPTH_FIRST_SEARCH_H_
#define _RECURSIVE_DEPTH_FIRST_SEARCH_H_

/// Include the GraphType class.
#include "GraphType.h"
#include "Visitor.h"

void recursiveDepthFirstSearch(GraphType & Graph,
			       GraphType::VertexDescriptor VertexSourceId,
             Visitor & GraphVisitor, 
			       std::map<GraphType::VertexPointer, bool> & VisitedColor) {

  typedef std::pair<GraphType::VertexPointer, bool> ColorMapPair;

  GraphType::VertexPointer CurrentVertex =
    Graph.getVertexPointer(VertexSourceId);
  
  VisitedColor.insert(ColorMapPair(CurrentVertex, true));

  bool VertexMatch = GraphVisitor.visitVertex(CurrentVertex);
  if (VertexMatch == true)
    return ;

  auto NextEdge = CurrentVertex->getNextEdge();
  while (NextEdge != nullptr) {
    GraphType::VertexPointer TargetVertex =
      NextEdge->getTarget(CurrentVertex);

    GraphVisitor.visitEdge(NextEdge);

    bool RevisitFlag = GraphVisitor.discoverVertex(TargetVertex);

    GraphVisitor.scheduleBranch(CurrentVertex, NextEdge, TargetVertex);
    bool TypeMatch = GraphVisitor.scheduleEdge(NextEdge);

    // Get color and check if false.
    auto VisitedVertex = VisitedColor.find(TargetVertex);
    if (VisitedVertex == VisitedColor.end() || RevisitFlag) {
      if (TypeMatch) {
        recursiveDepthFirstSearch(Graph, TargetVertex->getId(), 
                                GraphVisitor, VisitedColor);
        VisitedColor.insert(ColorMapPair(TargetVertex, true));
      }
    }
    else {
      GraphVisitor.revisitVertex(TargetVertex);
    }
    NextEdge = NextEdge->getNextEdge(CurrentVertex);
  }
};

void recursiveDepthFirstSearch(GraphType & Graph,
		      GraphType::VertexDescriptor StartVertex,
                      Visitor & GraphVisitor ) {

  typedef std::map<GraphType::VertexPointer, bool> ColorMap;
  ColorMap Color;
  
  GraphVisitor.visitStartVertex( Graph.getVertexPointer(StartVertex));
  recursiveDepthFirstSearch(Graph, StartVertex, GraphVisitor, Color);
  GraphVisitor.finishVisit();
};
#endif /* _RECURSIVE_DEPTH_FIRST_SEARCH_H_ */
