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
  reader.readFile("../tests/gd/sndata100000.gd");

  //createGraph(g);
  //  Graph::VertexPtr vp0 = g.getVertexPointer(0);

  QueryRandomizer rand(reader);

  string name[5], pid[5] ;
  VertexDescriptor webId[5], personId1[5], personId2[5], personId3[5];
  
  for(auto i = 0; i < 5; i++) {
    name[i] = rand.getName();
    pid[i]  = rand.getPid();
    webId[i] = rand.getAWebpageIndex();
    personId1[i] = rand.getAPersonIndex();
    personId2[i] = rand.getAPersonIndex();
    personId3[i] = personId2[i];
    while( personId3[i] == personId2[i])     //for query of connection with p1 and p2
    personId3[i] = rand.getAPersonIndex();
  }

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
/* 
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
*/
  vector<thread> threads[5];
  vector<thread> dthreads[5];
  for (auto j = 0; j < 5; j++){
  threads[j].push_back( thread([&]{Q1.runQuery(g, "name", name[j], 1);}));
  threads[j].push_back( thread([&]{Q2.runQuery(g, webId[j], 1);}));
  threads[j].push_back( thread([&]{Q3.runQuery(g, personId1[j], 1);}));
  threads[j].push_back( thread([&]{Q4.runQuery(g, "pid", pid[j], 1);}));
  threads[j].push_back( thread([&]{Q5.runQuery(g, personId1[j], 1);}));
  threads[j].push_back( thread([&]{Q6.runQuery(g, personId1[j], 1);}));
  threads[j].push_back( thread([&]{Q7.runQuery(g, personId1[j], 1);}));
  threads[j].push_back( thread([&]{Q8.runQuery(g, personId2[j], personId3[j], 1);}));
  threads[j].push_back( thread([&]{Q9.runQuery(g, personId2[j], personId3[j], 1);}));
  threads[j].push_back( thread([&]{Q10.runQuery(g, personId2[j], personId3[j], 1);}));
  threads[j].push_back( thread([&]{Q11.runQuery(g, personId2[j], personId3[j], 1);}));
  threads[j].push_back( thread([&]{Q12.runQuery(g, personId1[j], 1);}));
  threads[j].push_back( thread([&]{Q13.runQuery(g, personId1[j], 1);}));


  for_each(threads[j].begin(), threads[j].end(),
           std::mem_fn(&thread::join));

  dthreads[j].push_back( thread([&]{Q1.runQuery(g, "name", name[j], 2);}));
  dthreads[j].push_back( thread([&]{Q2.runQuery(g, webId[j], 2);}));
  dthreads[j].push_back( thread([&]{Q3.runQuery(g, personId1[j], 2);}));
  dthreads[j].push_back( thread([&]{Q4.runQuery(g, "pid", pid[j], 2);}));
  dthreads[j].push_back( thread([&]{Q5.runQuery(g, personId1[j], 2);}));
  dthreads[j].push_back( thread([&]{Q6.runQuery(g, personId1[j], 2);}));
  dthreads[j].push_back( thread([&]{Q7.runQuery(g, personId1[j], 2);}));
  dthreads[j].push_back( thread([&]{Q8.runQuery(g, personId2[j], personId3[j], 2);}));
  dthreads[j].push_back( thread([&]{Q9.runQuery(g, personId2[j], personId3[j], 2);}));
  dthreads[j].push_back( thread([&]{Q10.runQuery(g, personId2[j], personId3[j], 2);}));
  dthreads[j].push_back( thread([&]{Q11.runQuery(g, personId2[j], personId3[j], 2);}));
  dthreads[j].push_back( thread([&]{Q12.runQuery(g, personId1[j], 2);}));
  dthreads[j].push_back( thread([&]{Q13.runQuery(g, personId1[j], 2);}));


  for_each(dthreads[j].begin(), dthreads[j].end(),
           std::mem_fn(&thread::join));
  }

  cout << "finish testing\n";
  return 0;
}
