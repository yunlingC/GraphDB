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


#include "GraphType.h"

void depthFirstSearch(GraphType & Graph,
                      GraphType::VertexDescriptor StartVertexId) {


  typedef std::map<GraphType::VertexPointer, bool> ColorMap;
  typedef std::pair<GraphType::VertexPointer, bool> ColorMapPair;
  typedef std::pair<GraphType::VertexPointer, GraphType::EdgeList> StackEntry;
  typedef std::vector<StackEntry> VertexStack;

  ColorMap VisitedMap;
  VertexStack Stack;

  // Get outgoing edges of StartVertex.
  GraphType::VertexPointer StartVertex =
    Graph.getVertexPointer(StartVertexId);
  
  Stack.push_back(StackEntry(StartVertex, Graph.getOutEdges(StartVertex)));

  while ( !Stack.empty() ) {
    StackEntry& Back = Stack.back();
    auto CurrentVertex = Back.first;
    auto OutEdges = Back.second;
    Stack.pop_back();

    std::cout << "==> vid: " << CurrentVertex->getId() << "\n";

    auto VisitedVertex = VisitedMap.find(CurrentVertex);
    if ( VisitedVertex == VisitedMap.end() ) {
      // Not visited yet.
      VisitedMap.insert(ColorMapPair(CurrentVertex, true));

      //      std::cout << "# of out edges: " << OutEdges.size() << endl;
      for (auto EdgeIterator = OutEdges.begin(); 
           EdgeIterator != OutEdges.end();
           ++EdgeIterator) {
        auto TargetVertex = (*EdgeIterator)->getTarget(CurrentVertex);
        //        std::cout << " + iterate edges: " << (*EdgeIterator)->getId() << " vid: " 
        //                  << TargetVertex->getId() << ", outedges: " << Graph.getOutEdges(TargetVertex).size() << "\n";

        
        Stack.push_back(StackEntry(TargetVertex,
                                   Graph.getOutEdges(TargetVertex) 
                                   ));
                                    
      }
    }
    
  }
}
                      

#endif /* _DEPTH_FIRST_SEARCH_H_ */
