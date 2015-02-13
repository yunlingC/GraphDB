#include "Vertex.h"
#include "Edge.h"
#include "GraphType.h"
#include "FixedString.h"
#include "macros.h"
#include "GDReader.h"
#include "LinkedList.h"

#include <iostream>
#include <queue>

#include "breadth_first_search.h"
#include "CustomVisitor.h"
#include "QueryDescription.h"

int main() {

  typedef GraphType Graph;

  Graph g;
  //  g.allocVertexMemory(50);
  //  g.allocEdgeMemory(100);
  cout << "Begin testing\n";

  GDReader reader(g);
  reader.readFile("../tests/gd/sndata.gd");

  //createGraph(g);
  Graph::VertexPtr vp0 = g.getVertexPointer(0);
  MAGIC_SOURCE_NODE(vp0);
  MAGIC_PREFETCH_TRIGGER;
  cout << "BFS start\n";

  cout << "Query 1\n";
  Query1 Q1("name", "KIRA VERLATO", g);
  cout << endl;

 
  cout << "===============================\n";
  cout << "Query 2\n";
//  Query2 Q2("wpurl", "http://www.uwaterloo.ca/webpage15.html", g);
  Query2 Q2(14, g); 
  cout << endl;

  cout << "===============================\n";
  cout << "Query 3\n";
  Query3 Q3(4, g);
  cout << endl;

  cout << "===============================\n";
  cout << "Query 4\n";
  Query4 Q4("pid", "5", g);

  cout << "End of 4 queries\n";

  cout << "===============================\n";

  cout << "Query 13\n";
  Query12 Q12(4, g);

  return 0;
}
