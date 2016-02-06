//===-- algorithm/DepthFirstSearch.h - DFS search --------------*- C++ -*-===//
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

#include "GraphType.h"

#define UNDEFINE -1

namespace algorithm {

  /// DFS with stack 
  void depthFirstSearch(GraphType & Graph
                        , GraphType::VertexDescriptor StartVertex
                        , std::vector<GraphType::VertexDescriptor> & DFSTree
                        ) {
  
    typedef std::pair<GraphType::VertexPointer, bool> VisitPair;
  
    std::vector<GraphType::VertexPointer> VertexStack;
    std::unordered_map<GraphType::VertexPointer, bool> ColorMap;
  
    GraphType::VertexPointer ScheduledVertex = Graph.getVertexPointer(StartVertex);
    VertexStack.push_back(ScheduledVertex);
  
    DFSTree[StartVertex] = UNDEFINE;
  
    ColorMap.insert(VisitPair(ScheduledVertex, false));
  
    GraphType::VertexPointer TargetVertex = nullptr;
  
    while ( !VertexStack.empty() ) {
      ScheduledVertex = VertexStack.back();  VertexStack.pop_back();
  
      ColorMap[ScheduledVertex] = true;
  
      auto NextEdge = ScheduledVertex->getNextEdge();
      while ( NextEdge != nullptr ){
      
        TargetVertex = NextEdge->getTarget(ScheduledVertex);
  
        if ( ColorMap.find(TargetVertex) == ColorMap.end()) {
  
            VertexStack.push_back(TargetVertex);
  
            DFSTree[TargetVertex->getId()] = ScheduledVertex->getId();
  
            ColorMap.insert(VisitPair(TargetVertex, false));
        }
        NextEdge = NextEdge->getNextEdge(ScheduledVertex);
      }
    }
  };
}
                      
#endif /* _DEPTH_FIRST_SEARCH_H_ */
