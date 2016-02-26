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
//#include "Filter.h"

void recursiveDepthFirstSearch(GraphType & Graph,
			       GraphType::VertexDescriptor VertexSourceId,
                               Visitor & GraphVisitor,  /// new
			       std::map<GraphType::VertexPointer, bool> &
			       VisitedColor) {

  typedef std::pair<GraphType::VertexPointer, bool> ColorMapPair;
  typedef std::map<GraphType::VertexPointer, bool> ColorMap;
  typedef std::vector<GraphType::EdgePointer> EdgeList;

  GraphType::VertexPointer CurrentVertex =
    Graph.getVertexPointer(VertexSourceId);
  
  VisitedColor.insert(ColorMapPair(CurrentVertex, true));

  //  std::cout << "==> vid: " << CurrentVertex->getId() << "\n";

  bool VertexMatch = GraphVisitor.visitVertex(CurrentVertex);
  if(VertexMatch == true)
    return ;

  EdgeList Edges;
  Edges = Graph.getOutEdges(CurrentVertex);

  /* 
  //FIXME: There is some issue here that YC needs to fix.
  unsigned int direction = GraphVisitor.visitDirection(CurrentVertex);
  switch (direction) {
    case 1:
      Edges = Graph.getOutEdges(CurrentVertex);
      break;
    case 2:
      Edges = Graph.getInEdges(CurrentVertex);
      break;
    case 0: {
      auto InEdges = Graph.getInEdges(CurrentVertex);
      Edges = Graph.getOutEdges(CurrentVertex);
      Edges.insert( Edges.end(), InEdges.begin(), InEdges.end() );  
      break;

            }
    default:
      std::cout <<"ERROR: Direction Unknown\n";
  }
  */

  // Iterate over all the edges.
  for ( auto EdgeIterator = Edges.begin();  EdgeIterator != Edges.end();
       ++EdgeIterator ) {
    GraphType::VertexPointer TargetVertex =
      (*EdgeIterator)->getTarget(CurrentVertex);

    bool EdgeMatch = GraphVisitor.visitEdge(*EdgeIterator);

    bool RevisitFlag = GraphVisitor.discoverVertex(TargetVertex);

    bool BranchMatch = GraphVisitor.scheduleBranch(CurrentVertex, *EdgeIterator, TargetVertex);
    bool TypeMatch = GraphVisitor.scheduleEdge(*EdgeIterator);


    // Get color and check if false.
    auto VisitedVertex = VisitedColor.find(TargetVertex);
    if ( VisitedVertex == VisitedColor.end() || RevisitFlag ) {
//      std::cout << "Vertex " << TargetVertex->getId() << " comes to recursive\n";
      if( TypeMatch) {
        recursiveDepthFirstSearch(Graph, TargetVertex->getId(), GraphVisitor, VisitedColor);
      }
      else {
      VisitedColor.insert(ColorMapPair(TargetVertex, true));
      bool RevisitMatch = GraphVisitor.revisitVertex(TargetVertex);
      }
    }
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
