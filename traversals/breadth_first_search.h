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
#include "VertexVisitor.h"

/// This needs to be removed.
using namespace std;

void breadth_first_search(GraphType::VertexDescriptor StartVertex, GraphType & Graph, VertexVisitor & Visitor) {
  typedef pair<GraphType::VertexPtr, bool> VisitPair;
  auto ScheduledVertex = Graph.getVertexPointer(StartVertex);

  std::cout << "================= BFS ===================== \n";
  // Start traversing the graph from here. 
  std::queue<GraphType::VertexPtr> VertexQueue;
  std::map<GraphType::VertexPtr, bool> ColorMap; // true = visited, false = not visited

  VertexQueue.push(ScheduledVertex);
  ColorMap.insert(VisitPair(ScheduledVertex,false));

  GraphType::VertexPtr TargetVertex = nullptr;

  while ( !VertexQueue.empty() ) {
    ScheduledVertex = VertexQueue.front();  VertexQueue.pop();
    Visitor.visitVertex(ScheduledVertex);
    // Set to visited.    
    ColorMap[ScheduledVertex] = true;

    auto NextEdge = ScheduledVertex->getNextEdge();
    while ( NextEdge != nullptr ) {
      // Get the target
      TargetVertex = NextEdge->getTarget(ScheduledVertex);
      if ( ColorMap.find(TargetVertex) == ColorMap.end() ) {
	// queue the target for visitation
	VertexQueue.push(TargetVertex);
	Visitor.scheduleVertex(TargetVertex);
	ColorMap.insert(VisitPair(TargetVertex,false));
      }
      // Update NextEdge from ScheduledVertex
      NextEdge = NextEdge->getNextEdge(ScheduledVertex);
    }
  }
  std::cout << "================= END BFS ===================== \n";
};

#endif /* _BREADTH_FIRST_SEARCH_H_ */
