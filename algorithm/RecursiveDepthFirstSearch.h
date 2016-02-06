//===-- algorithm/RecursiveDepthFirstSearch.h - DFS search ----*- C++ -*-===//
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

#include "GraphType.h"

#define UNDEFINE -1

namespace algorithm {

  /// Recursive DFS with stack
  void recursiveDepthFirstSearch(GraphType & Graph
  			                        , GraphType::VertexDescriptor StartVertex 
  			                        , std::map<GraphType::VertexPointer, bool> & VisitedColor
                                , std::vector<GraphType::VertexDescriptor> & DFSTree
                                ) {
  
    typedef std::pair<GraphType::VertexPointer, bool> ColorMapPair;
    typedef std::vector<GraphType::EdgePointer> EdgeList;
  
    GraphType::VertexPointer CurrentVertex =
      Graph.getVertexPointer(StartVertex);
    

    VisitedColor.insert(ColorMapPair(CurrentVertex, true));
  
    EdgeList Edges;
    auto InEdges = Graph.getInEdges(CurrentVertex);
    Edges = Graph.getOutEdges(CurrentVertex);
    Edges.insert( Edges.end(), InEdges.begin(), InEdges.end() );  
  
    // Iterate over all the edges.
    for ( auto EdgeIterator = Edges.begin(), Iter_end = Edges.end();  
        EdgeIterator != Iter_end; ++EdgeIterator ) {

      GraphType::VertexPointer TargetVertex =
        (*EdgeIterator)->getTarget(CurrentVertex);

      auto VisitedVertex = VisitedColor.find(TargetVertex);
      if ( VisitedVertex == VisitedColor.end() ) {
        DFSTree[TargetVertex->getId()] = CurrentVertex->getId();
        recursiveDepthFirstSearch(Graph, TargetVertex->getId(), VisitedColor, DFSTree);
      } 
    }
  };
  
  void recursiveDFS(GraphType & Graph
  		              , GraphType::VertexDescriptor StartVertex
                    , std::vector<GraphType::VertexDescriptor> & DFSTree
                    ){
    typedef std::map<GraphType::VertexPointer, bool> ColorMap;
    ColorMap Color;

    DFSTree[StartVertex] = UNDEFINE;
    recursiveDepthFirstSearch(Graph, StartVertex, Color, DFSTree);
  };
}

#endif /* _RECURSIVE_DEPTH_FIRST_SEARCH_H_ */
