#include <iostream>

#include "LDBCReader.h"
#include "QueryDescription.h"

int main( int argc, char * argv[]) {

  typedef GraphType Graph;

  Graph g;
  LDBCReader reader(g);

  //reader.readDirectory("../tests/ldbc/social_network_100/New");
  reader.readDirectory("/home/y49cui/research/graph/ldbc-graphdb/graphdb/tests/ldbc/social_network_100/New");

  g.dump();

  AddressVisitor AddrVisitor;
  breadthFirstSearch(g, 0, AddrVisitor);

  std::cout << "\nVertex\n";
  unsigned int i = 0;
  for (auto & addr : AddrVisitor.VertexAddrDis) {
      std::cout << addr/4 << "\t";
      if (i%10 == 0 ) 
        std::cout << std::endl;
      i++;
  }

  i = 0;
  std::cout << "\n\nEdge\n";
  for (auto & addr : AddrVisitor.EdgeAddrDis) {
      std::cout << addr/4 << "\t";
      if (i%10 == 0) 
        std::cout << std::endl;
      i++;
  }

  cout << "\nfinish testing\n";
  myfile.close();

}
