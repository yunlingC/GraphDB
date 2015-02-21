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
  reader.readFile("../tests/gd/sndata.gd");

  //createGraph(g);
  //  Graph::VertexPtr vp0 = g.getVertexPointer(0);
  cout << "BFS start\n";

  Query1 Q1;
  Q1.setPersonProperty("name" , "KIRA VERLATO");

  Query2 Q2;
  Query3 Q3;

  Query4 Q4;
  Q4.setPersonProperty("pid", "5");

  Query12 Q12;
   
  Q1.runQuery(g);
  Q2.runQuery(g, 14);
  Q3.runQuery(g, 4);
  Q4.runQuery(g);
  Q12.runQuery(g, 4);

/*
  vector<thread> threads;
  threads.push_back( thread([&]{Q1.runQuery(g);}));
  threads.push_back( thread([&]{Q2.runQuery(g, 14);}));
  threads.push_back( thread([&]{Q3.runQuery(g, 4);}));
  
//  threads.push_back( thread( &Query::runQuery3, Q));
//  threads.push_back( thread( &Query::runQuery4, Q));
//  threads.push_back( thread( &Query::runQuery12, Q));

  for_each(threads.begin(), threads.end(),
           std::mem_fn(&thread::join));

*/

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
