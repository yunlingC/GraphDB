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

/// Standard includes.
#include <queue>
#include <map>

/// Local includes.
#include "GraphType.h"
#include "Visitor.h"

/// This needs to be removed.
using namespace std;

void breadthFirstSearch(GraphType & Graph, const GraphType::VertexDescriptor & StartVertex, Visitor & GraphVisitor) {

  typedef pair<GraphType::VertexPointer, bool> VisitPair;
  auto ScheduledVertex = Graph.getVertexPointer(StartVertex);

  // Start traversing the graph from here. 
  std::queue<GraphType::VertexPointer> VertexQueue;
  /// True means visited and false means not visited.
  std::map<GraphType::VertexPointer, bool> ColorMap;

  VertexQueue.push(ScheduledVertex);               
  GraphVisitor.visitStartVertex(ScheduledVertex);

  ColorMap.insert(VisitPair(ScheduledVertex,false));

  GraphType::VertexPointer TargetVertex = nullptr;

  while ( !VertexQueue.empty() ) {
    ScheduledVertex = VertexQueue.front();  VertexQueue.pop();
    bool VertexMatch = GraphVisitor.visitVertex(ScheduledVertex);
    if(VertexMatch == true)
      return;
      
    // Set to visited.    
    ColorMap[ScheduledVertex] = true;

    auto NextEdge = ScheduledVertex->getNextEdge();
    while ( NextEdge != nullptr ) {                  
      /// Get the target node.
      TargetVertex = NextEdge->getTarget(ScheduledVertex);     
      bool RevisitFlag = GraphVisitor.discoverVertex(TargetVertex);             

      bool BranchMatch = GraphVisitor.scheduleBranch(ScheduledVertex, NextEdge, TargetVertex);
      bool TypeMatch =  GraphVisitor.scheduleEdge(NextEdge);
      bool DirectionMatch = GraphVisitor.visitDirection(TargetVertex, NextEdge);

      if(BranchMatch == true)
        return;

      if ( ColorMap.find(TargetVertex) == ColorMap.end() || RevisitFlag) {
	// queue the target for visitation
        bool TreeMatch = GraphVisitor.scheduleTree(ScheduledVertex, NextEdge, TargetVertex);

        if(TypeMatch && DirectionMatch)   {//control the vertex to be visited filtered by type
	        VertexQueue.push(TargetVertex);
//          std::cout << "=>target " << TargetVertex->getPropertyValue("id").first << endl;
        }
	      ColorMap.insert(VisitPair(TargetVertex,false));
      } else {
        bool RevisitMatch = GraphVisitor.revisitVertex( TargetVertex );
      }
      // Get the next edge from the scheduled vertex. 
      NextEdge = NextEdge->getNextEdge(ScheduledVertex);
    }
  }
  GraphVisitor.finishVisit();
};

#endif /* _BREADTH_FIRST_SEARCH_H_ */
