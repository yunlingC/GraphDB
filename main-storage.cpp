//===-- main-storage.cpp - Main driver for neo4j storage -------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This file contains the driver code to construct the graph using a 
/// neo4j-like storage for in-memory databases.
///
//===----------------------------------------------------------------------===//

#include "Vertex.h"
#include "Edge.h"
#include "GraphType.h"
#include "FixedString.h"
#include "macros.h"
#include "GDBReader.h"
#include "LinkedList.h"

#include <iostream>
#include <queue>

#include "breadth_first_search.h"
/// Test
//#include "tests/gs/g1.h"

using namespace std;

class VisitedVertex : public VertexVisitor {
public:
  VisitedVertex() { }
  virtual bool visitVertex(VertexPointer vp) {
    cout << "visited vertex: " << vp->getId() << endl;
    cout << "print property list\n";
    Vertex::PropertyListType pl = vp->getPropertyList();
    pl.print();
    return true;
  }
};
// void bfs(GraphType::VertexDescriptor vs, GraphType & g) {
//   typedef pair<GraphType::VertexPtr, bool> VisitPair;
//   cout << "================= BfS ===================== \n";
//   cout << "+ vertex: " << vs << "\n";

//   GraphType::VertexPtr np = g.getVertexPointer(vs);

//   // Start traversing the graph from here. 
//   std::queue<GraphType::VertexPtr> Q;
//   std::map<GraphType::VertexPtr, bool> C; // true = visited, false = not visited

//   Q.push(np);
//   C.insert(VisitPair(np,false));

//   GraphType::VertexPtr targetVertex = nullptr;

//   while (!Q.empty()) {
//     np = Q.front();  Q.pop();
//     cout << "vid: " << np->getId() << "\n";

//     // Set to visited.    
//     C[np] = true;

//     GraphType::EdgePtr nextEdge = np->getNextEdge();
//     while (nextEdge != nullptr) {
//       // Get the target
//       targetVertex = nextEdge->getTarget(np);
//       if (C.find(targetVertex) == C.end()) {
// 	// queue the target for visitation
// 	//	cout << "push: " << targetVertex->getId() << "\n";
// 	Q.push(targetVertex);
// 	C.insert(VisitPair(targetVertex,false));
//       }
//       // Update nextEdge from np
//       nextEdge = nextEdge->getNextEdge(np);
//     }
//   }
//   cout << "================= END BFS ===================== \n";
// };

int main() {

  typedef GraphType Graph;

  Graph g;
  //  g.allocVertexMemory(50);
  //  g.allocEdgeMemory(100);
  cout << "Begin testing\n";

  GDBReader reader(g);
  reader.readFile("../tests/gd/sndata.gd");

  //createGraph(g);

  VisitedVertex VV;

  Graph::VertexPtr vp0 = g.getVertexPointer(0);
  // Let sniper know fo source node.
  MAGIC_SOURCE_NODE(vp0);
  MAGIC_PREFETCH_TRIGGER;
  cout << "BFS start\n";
  breadth_first_search(vp0->getId(), g, VV);


  return 0;
}
