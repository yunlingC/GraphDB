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

void breadth_first_search(GraphType::VertexDescriptor StartVertex, 
                          GraphType & Graph, VertexVisitor & Visitor) {

  typedef pair<GraphType::VertexPointer, bool> VisitPair;
  auto ScheduledVertex = Graph.getVertexPointer(StartVertex);

  std::cout << "================= BFS ===================== \n";
  // Start traversing the graph from here. 
  std::queue<GraphType::VertexPointer> VertexQueue;
  /// True means visited and false means not visited.
  std::map<GraphType::VertexPointer, bool> ColorMap;

  VertexQueue.push(ScheduledVertex);
  ColorMap.insert(VisitPair(ScheduledVertex,false));

  GraphType::VertexPointer TargetVertex = nullptr;

  while ( !VertexQueue.empty() ) {
    ScheduledVertex = VertexQueue.front();  VertexQueue.pop();
    Visitor.visitVertex(ScheduledVertex);

    // Set to visited.    
    ColorMap[ScheduledVertex] = true;

    auto NextEdge = ScheduledVertex->getNextEdge();
    while ( NextEdge != nullptr ) {
      /// Get the target node.
      TargetVertex = NextEdge->getTarget(ScheduledVertex);
      if ( ColorMap.find(TargetVertex) == ColorMap.end() ) {
	// queue the target for visitation
	VertexQueue.push(TargetVertex);
	Visitor.scheduleVertex(TargetVertex);
	ColorMap.insert(VisitPair(TargetVertex,false));
      }
      // Get the next edge from the scheduled vertex. 
      NextEdge = NextEdge->getNextEdge(ScheduledVertex);
    }
  }
  std::cout << "================= END BFS ===================== \n";
};

#endif /* _BREADTH_FIRST_SEARCH_H_ */
