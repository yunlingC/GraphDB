//===-- traversals/breadth_first_search.h - BFS search ----*- C++ -*-===//
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
#ifndef _BREADTH_FIRST_SEARCH_H_
#define _BREADTH_FIRST_SEARCH_H_

/// Standard includes.
#include <queue>
#include <map>

/// Local includes.
#include "GraphType.h"
#include "Visitor.h"

/// This needs to be removed.
using namespace std;

void breadth_first_search(GraphType & Graph, const GraphType::VertexDescriptor & StartVertex, Visitor & GraphVisitor) {

  typedef pair<GraphType::VertexPointer, bool> VisitPair;
  auto ScheduledVertex = Graph.getVertexPointer(StartVertex);

//  std::cout << "================= BFS ===================== \n";
  // Start traversing the graph from here. 
  std::queue<GraphType::VertexPointer> VertexQueue;
  /// True means visited and false means not visited.
  std::map<GraphType::VertexPointer, bool> ColorMap;

  VertexQueue.push(ScheduledVertex);               
//  GraphVisitor.discover(ScheduledVertex);
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
      bool EdgeMatch = GraphVisitor.visitEdge(NextEdge);
      /// Get the target node.
      TargetVertex = NextEdge->getTarget(ScheduledVertex);     
      GraphVisitor.discoverVertex(TargetVertex);             
      bool TypeMatch =  GraphVisitor.scheduleEdge(NextEdge);
      bool DirectionMatch = GraphVisitor.visitDirection(TargetVertex, NextEdge);
      bool BranchMatch = GraphVisitor.scheduleBranch(ScheduledVertex, NextEdge, TargetVertex);
      if(BranchMatch == true)
        return;

      if ( ColorMap.find(TargetVertex) == ColorMap.end() ) {
	// queue the target for visitation
        if(TypeMatch == true)   //control the vertex to be visited filtered by type
	        VertexQueue.push(TargetVertex);
	      ColorMap.insert(VisitPair(TargetVertex,false));
      }
      // Get the next edge from the scheduled vertex. 
      NextEdge = NextEdge->getNextEdge(ScheduledVertex);
    }
  }
  GraphVisitor.finishVisit();
  std::cout << "================= END BFS ===================== \n";
};

#endif /* _BREADTH_FIRST_SEARCH_H_ */
