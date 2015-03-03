//#include "macros.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <thread>

//#include "GDReader.h"
#include "LinkedList.h"
#include "breadth_first_search.h"
#include "QueryDescription.h"
#include "QueryRandomizer.h"


int main() {

  typedef GraphType Graph;
  typedef Graph::VertexDescriptor VertexDescriptor;

  Graph g;
  //  g.allocVertexMemory(50);
  //  g.allocEdgeMemory(100);
  cout << "Begin testing\n";

  GDReader reader(g);
  reader.readFile("../tests/gd/sndata100.gd");

  //createGraph(g);
  //  Graph::VertexPtr vp0 = g.getVertexPointer(0);

  QueryRandomizer rand(reader);

  unsigned int counter = 0;

  string name = rand.getName();
  string pid  = rand.getPid();
  VertexDescriptor webId = rand.getAWebpageIndex();
  VertexDescriptor personId1 = rand.getAPersonIndex();
  VertexDescriptor personId2 = rand.getAPersonIndex();
  VertexDescriptor personId3 = personId2;
  while( personId3 == personId2)             //for query of connection with p1 and p2
    personId3 = rand.getAPersonIndex();

  cout << "BFS start\n";

  Query1 Q1;
  Query2 Q2;
  Query3 Q3;
  Query4 Q4;
  Query5 Q5;
  Query6 Q6;
  Query7 Q7;
  Query8 Q8;
  Query9 Q9;
  Query10 Q10;
  Query11 Q11;
  Query12 Q12;
  Query13 Q13;

  /// 1: BFS  2: DFS
  Q1.runQuery(g, "name", name, 1);
  Q1.runQuery(g, "name", name, 2);
  Q2.runQuery(g, webId, 1);
  Q2.runQuery(g, webId, 2);
  Q3.runQuery(g, personId1, 1);
  Q3.runQuery(g, personId1, 2);
  Q4.runQuery(g, "pid", pid, 1);
  Q4.runQuery(g, "pid", pid, 2);
  Q5.runQuery(g, personId1, 1);
  Q5.runQuery(g, personId1, 2);
  Q6.runQuery(g, personId1, 1);
  Q6.runQuery(g, personId1, 2);
  Q7.runQuery(g, personId1, 1);
  Q7.runQuery(g, personId1, 2);
  Q8.runQuery(g, personId2, personId3, 1);
  Q8.runQuery(g, personId2, personId3, 2);
  Q9.runQuery(g, personId2, personId3, 1);
  Q9.runQuery(g, personId2, personId3, 2);
  Q10.runQuery(g, personId2, personId3, 1);
  Q10.runQuery(g, personId2, personId3, 2);
  Q11.runQuery(g, personId2, personId3, 1);
  Q11.runQuery(g, personId2, personId3, 2);
  Q12.runQuery(g, personId1, 1);
  Q12.runQuery(g, personId1, 2);
  Q13.runQuery(g, personId1, 1);
  Q13.runQuery(g, personId1, 2);
  /**
  vector<thread> threads;
  threads.push_back( thread([&]{Q1.runQuery(g, "name", "KIRA VERLATO", 1);}));
  threads.push_back( thread([&]{Q2.runQuery(g, 63, 1);}));
  threads.push_back( thread([&]{Q3.runQuery(g, 4, 1);}));
  threads.push_back( thread([&]{Q4.runQuery(g, "pid", "5", 1);}));
  threads.push_back( thread([&]{Q5.runQuery(g, 0, 1);}));
  threads.push_back( thread([&]{Q6.runQuery(g, 0, 1);}));
  threads.push_back( thread([&]{Q7.runQuery(g, 0, 1);}));
  threads.push_back( thread([&]{Q8.runQuery(g, 0, 2, 1);}));
  threads.push_back( thread([&]{Q9.runQuery(g, 0, 99, 1);}));
  threads.push_back( thread([&]{Q10.runQuery(g, 0, 1, 1);}));
  threads.push_back( thread([&]{Q11.runQuery(g, 0, 1, 1);}));
  threads.push_back( thread([&]{Q12.runQuery(g, 4, 1);}));
  threads.push_back( thread([&]{Q13.runQuery(g, 0, 1);}));

//  threads.push_back( thread( &Query::runQuery3, Q));
//  threads.push_back( thread( &Query::runQuery4, Q));
//  threads.push_back( thread( &Query::runQuery12, Q));

  for_each(threads.begin(), threads.end(),
           std::mem_fn(&thread::join));

  vector<thread> dthreads;
  dthreads.push_back( thread([&]{Q1.runQuery(g, "name", "KIRA VERLATO", 2);}));
  dthreads.push_back( thread([&]{Q2.runQuery(g, 63, 2);}));
  dthreads.push_back( thread([&]{Q3.runQuery(g, 4, 2);}));
  dthreads.push_back( thread([&]{Q4.runQuery(g, "pid", "5", 2);}));
  dthreads.push_back( thread([&]{Q5.runQuery(g, 0, 2);}));
  dthreads.push_back( thread([&]{Q6.runQuery(g, 0, 2);}));
  dthreads.push_back( thread([&]{Q7.runQuery(g, 0, 2);}));
  dthreads.push_back( thread([&]{Q8.runQuery(g, 0, 2, 2);}));
  dthreads.push_back( thread([&]{Q9.runQuery(g, 0, 99, 2);}));
  dthreads.push_back( thread([&]{Q10.runQuery(g, 0, 1, 2);}));
  dthreads.push_back( thread([&]{Q11.runQuery(g, 0, 1, 2);}));
  dthreads.push_back( thread([&]{Q12.runQuery(g, 4, 2);}));
  dthreads.push_back( thread([&]{Q13.runQuery(g, 0, 2);}));


//  threads.push_back( thread( &Query::runQuery3, Q));
//  threads.push_back( thread( &Query::runQuery4, Q));
//  threads.push_back( thread( &Query::runQuery12, Q));

  for_each(dthreads.begin(), dthreads.end(),
           std::mem_fn(&thread::join));

*/

  return 0;
}
