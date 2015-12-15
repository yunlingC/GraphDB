//===-- algorithm/BreadthFirstSearch.h - BFS search ----*- C++ -*-===//
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

#include "GraphType.h"

#include <queue>
#include <unordered_map>

#define UNDEFINE -1

namespace algorithm {

  void breadthFirstSearch(GraphType & Graph
                          , const GraphType::VertexDescriptor & StartVertex
                          , std::vector<GraphType::VertexDescriptor> & BFSTree
                          ) {
  
    typedef std::pair<GraphType::VertexPointer, bool> VisitPair;
  
    // Start traversing the graph from here. 
    std::queue<GraphType::VertexPointer> VertexQueue;
    /// True means visited and false means not visited.
    std::unordered_map<GraphType::VertexPointer, bool> ColorMap;
  
  //  BFSTree.reserve(Graph.getAllVertices().size());
  
    auto ScheduledVertex = Graph.getVertexPointer(StartVertex);
  
    VertexQueue.push(ScheduledVertex);               
  
    BFSTree[StartVertex] = UNDEFINE;
  
    ColorMap.insert(VisitPair(ScheduledVertex, false));
  
    GraphType::VertexPointer TargetVertex = nullptr;
  
    while ( !VertexQueue.empty() ) {
      ScheduledVertex = VertexQueue.front();  VertexQueue.pop();
  
      /// Set to visited.    
      ColorMap[ScheduledVertex] = true;
  
      auto NextEdge = ScheduledVertex->getNextEdge();
      while ( NextEdge != nullptr ) {                  
        /// Get the target node.
        TargetVertex = NextEdge->getTarget(ScheduledVertex);     
  
        if (ColorMap.find(TargetVertex) == ColorMap.end()) {
         	  /// Queue up the target for visitation
  	        VertexQueue.push(TargetVertex);
  
            BFSTree[TargetVertex->getId()] = ScheduledVertex->getId();
  
  	        ColorMap.insert(VisitPair(TargetVertex,false));
        } 
        // Get the next edge from the scheduled vertex. 
        NextEdge = NextEdge->getNextEdge(ScheduledVertex);
      }
    }
  };
}

#endif /* _BREADTH_FIRST_SEARCH_H_ */
