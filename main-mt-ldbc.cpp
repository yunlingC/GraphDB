#include <iostream>

#include "LDBCReader.h"
#include "QueryDescription.h"


int main( int argc, char * argv[]) {

  typedef GraphType Graph;

  Graph g;

#ifdef _FIXALLOC_
  g.allocVertexMemory(40000);
  g.allocEdgeMemory(80000);
#endif /* _FIXALLOC */


  LDBCReader reader(g);
  //reader.readDirectory("../tests/ldbc/social_network_100/New");
  reader.readDirectory("/home/y49cui/research/graph/ldbc-graphdb/graphdb/tests/ldbc/social_network_100/New");

//  g.dump();

  cout << "Finish reading\n";

  unsigned int STEP = 100;
  if (argc > 1) {
    if ((std::stoi(argv[1]) > 1) && 
        (std::stoi(argv[1]) < 5000)) {

        STEP = std::stoi(argv[1]);
    }
  }

  srand(time(NULL));

  for (unsigned int i = 0; i < 74990; i+= STEP) {
    auto EdgeId = rand()%74990;

    while (g.Edges.find(EdgeId) != g.Edges.end()) {
      EdgeId = rand()%74990;
    }

    g.Edges.insert(EdgeId);

    auto res = g.removeBranch(EdgeId);
//    auto res = g.removeBranch(i);
//    std::cout << "remove edge with id " << res << "\n";
//    fflush(stdout);
  }

  
  for (unsigned int j = 1; j < 36800; j+= STEP) {
    auto VertexId = rand()%36800 + 1;

    while (g.Vertices.find(VertexId) != g.Vertices.end()) {
      VertexId = rand()%36800;
    }
    g.Vertices.insert(VertexId);

    auto res2 = g.removeVertex(VertexId);
//    auto res2 = g.removeVertex(j);
//    std::cout << "remove vertex with id " << res2 << "\n";
//    fflush(stdout);
  }


  g.recoverAll();
//

//  LdbcQuery1 q1(1);
//  q1.setParam("firstName", "Hermann");
//  q1.runQuery(g, Input[i]);

  /// Calculate the distance between every two objects.
  AddressVisitor AddrVisitor;
  breadthFirstSearch(g, 0, AddrVisitor);

  std::cout << "\nVertex\n";
  unsigned int i = -1;
  for (auto & addr : AddrVisitor.VertexAddrDis) {
      std::cout << addr/4 << ",";
       if (i%1000 == 0 ) 
        std::cout << std::endl;
      i++;

  }

  i = 0;
  std::cout << "\n\nEdge\n";
  for (auto & addr : AddrVisitor.EdgeAddrDis) {
      std::cout << addr/4 << ",";
     if (i%1000 == 0) 
        std::cout << std::endl;
      i++;

  }

  cout << "\nfinish testing\n";

}
