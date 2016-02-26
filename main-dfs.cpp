//===-- main-dfs.cpp - Main driver file for testing DFS   ch ----*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Driver file for testing recursive DFS.
///
//===----------------------------------------------------------------------===//
#include "Vertex.h"
#include "Edge.h"
#include "GraphType.h"
#include "FixedString.h"
#include "macros.h"
#include "GDBReader.h"
#include "LinkedList.h"
#include "Visitor.h"

#include <iostream>
#include <queue>

//#include "RecursiveDepthFirstSearch.h"
#include "DepthFirstSearch.h"

/// Test
#include "tests/gs/g1.h"

using namespace std;

class DFSVisitor : public Visitor {
public:
  virtual bool visitVertex(VertexPointer VertexPtr) {
    std::cout << VertexPtr->getId() << "=>" ;
    return false;
  }

};

int main() {

  typedef GraphType Graph;

  Graph g;
  DFSVisitor v;
  cout << "Begin testing\n";
  GDBReader reader(g);
  reader.readFile("../tests/gd/sndata.gd");

  //createGraph(g);
  Graph::VertexPointer vp0 = g.getVertexPointer(0);
  cout << "DFS start: " << vp0 << "\n";
  depthFirstSearch(g, 0, v);

  return 0;
}
