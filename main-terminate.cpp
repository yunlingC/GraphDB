//#include "macros.h"
#include "GDReader.h"
#include "LinkedList.h"
#include "breadth_first_search.h"
#include "QueryDescription.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <thread>

int main() {

  typedef GraphType Graph;

  Graph g;
  //  g.allocVertexMemory(50);
  //  g.allocEdgeMemory(100);
  cout << "Begin testing\n";

  GDReader reader(g);
  reader.readFile("../tests/gd/sndata-big.gd");

  //createGraph(g);
  //  Graph::VertexPtr vp0 = g.getVertexPointer(0);
  cout << "BFS start\n";

  Query Q(g);
  Q.setPersonProperty("name" , "KIRA VERLATO");
  Q.setWebId(14);
  Q.setPersonId(4);
  Q.setQuery4Property("pid" , "5");


  vector<thread> threads;
  threads.push_back( thread( &Query::runQuery1, Q));
  threads.push_back( thread( &Query::runQuery2, Q));
  threads.push_back( thread( &Query::runQuery3, Q));
  threads.push_back( thread( &Query::runQuery4, Q));
  threads.push_back( thread( &Query::runQuery12, Q));

  for_each(threads.begin(), threads.end(),
           std::mem_fn(&thread::join));



/*
  Q.runQuery1();

//  Query Q2;
  Q.runQuery2(); 

//  Query Q3;
  Q.runQuery3();

//  Query Q4;
  Q.runQuery4();

//  Query1 Q12;
  Q.runQuery12();
*/
  return 0;
}
