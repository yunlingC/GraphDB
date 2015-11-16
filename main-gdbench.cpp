//#include "macros.h"
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <queue>
#include <thread>
#include <string>
#include <time.h>

#include "LinkedList.h"
#include "QueryDescription.h"
#include "QueryRandomizer.h"

int main(int argc, char *argv[]) {

  typedef GraphType Graph;

  Graph g;
//  cout << "Begin testing\n";

#ifdef _FIXALLOC_
  g.allocVertexMemory(5000);
  g.allocEdgeMemory(40000);
#endif /* _FIXALLOC */

  std::cout << "Begin testing\n";

  GDReader reader(g);
  reader.setPrintInterval(10000);
  reader.readFile("../tests/gd/sndata5000.gd");

  cout << "Finish reading\n";

  unsigned int STEP = 100;
  if (argc > 1) {
    if ((std::stoi(argv[1]) > 1) && 
        (std::stoi(argv[1]) < 5000)) {

        STEP = std::stoi(argv[1]);
    }
  }
//  g.dump();

//  std::cout <<" start deleting \n";

//  GraphType::EdgeDescriptor  Edge1 = 25; 
//  GraphType::VertexDescriptor Vertex1 = 9;
//  
//  std::vector<unsigned int> EdgeList = { 10, 15, 20, 25, 40,  
//                                         60, 80, 100, 200, 300, 
//                                         400, 500, 600, 700, 800,
//                                         900
//                                        };
//
//  std::vector<unsigned int> VertexList = { 5, 10, 15, 20, 30, 50, 80};

  
//  for (auto & EdgeId : EdgeList) {
//    auto res = g.removeBranch(EdgeId);
//    std::cout << "remove edge with id " << res << "\n";
//  }
//
//  for (auto & VertexId : VertexList) {
//    auto res2 = g.removeVertex(VertexId);
//    std::cout << "remove vertex with id " << res2 << "\n";
//  }

//  for (unsigned int i = 0; i < 39000; i+= STEP) {
//    auto res = g.removeBranch(i);
////    std::cout << "remove edge with id " << res << "\n";
//  }
//
//  for (unsigned int j = 1; j < 5000; j+= STEP) {
//    auto res2 = g.removeVertex(j);
////    std::cout << "remove vertex with id " << res2 << "\n";
//  }
//
////  std::cout <<"recover all the deletion \n";
//
//  g.recoverAll();


//  g.dump();

//  g.recoverAll();

//  g.dump();

  Query1 Q1;
  Q1.setPersonProperty("name", "A");

  SimRoiStart();

  Q1.runQuery(g, 1);

  SimRoiEnd();

//  AddressVisitor AddrVisitor;
//
//  SimRoiStart();
//
//  breadthFirstSearch(g, 0, AddrVisitor);
//
//  SimRoiEnd();

//  std::cout << "\nVertex\n";
//  unsigned int i = 0;
//  for (auto & addr : AddrVisitor.VertexAddrDis) {
//      std::cout << addr/4 << ",";
//      if (i%1000 == 0 ) 
//        std::cout << std::endl;
//      i++;
//  }
//
//  i = 0;
//  std::cout << "\n\nEdge\n";
//  for (auto & addr : AddrVisitor.EdgeAddrDis) {
//      std::cout << addr/4 << ",";
//      if (i%1000 == 0) 
//        std::cout << std::endl;
//      i++;
//  }

  cout << "\nfinish testing\n";
  myfile.close();
  return 0;
}
