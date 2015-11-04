//#include "macros.h"
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <queue>
#include <thread>
#include <string>
#include <time.h>

//#include "GDBench/util.h"
//#include "GDReader.h"
#include "LinkedList.h"
#include "QueryDescription.h"
#include "QueryRandomizer.h"

int main(int argc, char *argv[]) {

  typedef GraphType Graph;

  Graph g;
  cout << "Begin testing\n";

#ifdef _FIXALLOC_
  g.allocVertexMemory(1200);
  g.allocEdgeMemory(20000);
#endif /* _FIXALLOC */
  cout << "Begin testing\n";


  GDReader reader(g);
  reader.setPrintInterval(10000);
  reader.readFile("../tests/gd/sndata1000.gd");

  cout << "Finish reading\n";

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
  return 0;
}
