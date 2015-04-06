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
                      GraphType::VertexDescriptor StartVertex,
                      Visitor & GraphVisitor ) {


  typedef std::pair<GraphType::VertexPointer, bool> VisitPair;
  GraphType::VertexPointer ScheduledVertex = Graph.getVertexPointer(StartVertex);
  //pass hint
  passNodeHint(ScheduledVertex);

  std::vector<GraphType::VertexPointer> VertexStack;
  std::map<GraphType::VertexPointer, bool> ColorMap;

  VertexStack.push_back(ScheduledVertex);
  GraphVisitor.visitStartVertex(ScheduledVertex);

  ColorMap.insert(VisitPair(ScheduledVertex, false));

  GraphType::VertexPointer TargetVertex = nullptr;

  while ( !VertexStack.empty() ) {
    ScheduledVertex = VertexStack.back();  VertexStack.pop_back();
    bool VertexMatch = GraphVisitor.visitVertex(ScheduledVertex);
    if(VertexMatch == true)
      return ;
 
    ColorMap[ScheduledVertex] = true;

    auto NextEdge = ScheduledVertex->getNextEdge();
    while( NextEdge != nullptr ){
    
      TargetVertex = NextEdge->getTarget(ScheduledVertex);
      bool RevisitFlag = GraphVisitor.discoverVertex(TargetVertex);
      bool BranchMatch = GraphVisitor.scheduleBranch(ScheduledVertex, NextEdge, TargetVertex);
      bool TypeMatch = GraphVisitor.scheduleEdge(NextEdge);
      bool DirectionMatch = GraphVisitor.visitDirection(TargetVertex, NextEdge);

      if(BranchMatch == true)
        break;

      if ( ColorMap.find(TargetVertex) == ColorMap.end() || RevisitFlag ) {

        GraphVisitor.scheduleTree(ScheduledVertex, NextEdge, TargetVertex);

        if( TypeMatch && DirectionMatch) {
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
