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

void depthFirstSearch(GraphType & Graph,
                      GraphType::VertexDescriptor StartVertexId,
                      Visitor & GraphVisitor ) {


  typedef std::map<GraphType::VertexPointer, bool> ColorMap;
  typedef std::pair<GraphType::VertexPointer, bool> ColorMapPair;
  typedef std::pair<GraphType::VertexPointer, GraphType::EdgeList> StackEntry;
  typedef std::vector<StackEntry> VertexStack;
  typedef std::vector<GraphType::EdgePointer> EdgeList;

  ColorMap VisitedMap;
  VertexStack Stack;

  // Get outgoing edges of StartVertex.
  GraphType::VertexPointer StartVertex =
    Graph.getVertexPointer(StartVertexId);

  GraphVisitor.visitStartVertex( StartVertex );

  Stack.push_back(StackEntry(StartVertex, Graph.getOutEdges(StartVertex)));

  while ( !Stack.empty() ) {
    StackEntry& Back = Stack.back();
    auto CurrentVertex = Back.first;
    auto OutEdges = Back.second;
    Stack.pop_back();

    VisitedMap.insert(ColorMapPair(CurrentVertex, true));

    bool VertexMatch = GraphVisitor.visitVertex(CurrentVertex);
    if(VertexMatch == true)
      return ;
 
    EdgeList Edges;
    auto InEdges = Graph.getInEdges(CurrentVertex);
    Edges = Graph.getOutEdges(CurrentVertex);
    Edges.insert(Edges.end(), InEdges.begin(), InEdges.end());
    for (auto EdgeIterator = Edges.begin(); 
      EdgeIterator != Edges.end();
      ++EdgeIterator) {
      auto TargetVertex = (*EdgeIterator)->getTarget(CurrentVertex);

      bool RevisitFlag = GraphVisitor.discoverVertex(TargetVertex);

      bool BranchMatch = GraphVisitor.scheduleBranch(CurrentVertex, *EdgeIterator, TargetVertex);
      if(BranchMatch == true)
        break;

      bool TypeMatch = GraphVisitor.scheduleEdge(*EdgeIterator);

      bool DirectionMatch = GraphVisitor.visitDirection(TargetVertex, *EdgeIterator);


      auto VisitedVertex = VisitedMap.find(TargetVertex);
        
      if ( VisitedVertex == VisitedMap.end() || RevisitFlag ) {
        bool TreeMatch = GraphVisitor.scheduleTree(CurrentVertex, *EdgeIterator, TargetVertex);

        if( TypeMatch && DirectionMatch) {
         Stack.push_back(StackEntry(TargetVertex,
                                   Graph.getOutEdges(TargetVertex) 
                                   ));
        }
      } else {
        VisitedMap.insert(ColorMapPair(TargetVertex, true));
        bool RevisitMatch = GraphVisitor.revisitVertex(TargetVertex);
      }
    }   
  }
  GraphVisitor.finishVisit();
}
                      

#endif /* _DEPTH_FIRST_SEARCH_H_ */
