//#include "macros.h"
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <queue>
#include <thread>

#include "GDBench/util.h"
//#include "GDReader.h"
#include "LinkedList.h"
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
  reader.readFile("../tests/gd/sndata1000.gd");

  //createGraph(g);
  //  Graph::VertexPtr vp0 = g.getVertexPointer(0);
  cout << "begin randomizing\n";
  QueryRandomizer rander(reader);

  string name[5], pid[5] ;
  VertexDescriptor webId[5], personId1[5], personId2[5], personId3[5];
 
  ///not random now 
  for(auto i = 0; i < 1; i++) {
    name[i] = rander.getName();
    pid[i]  = rander.getPid();
    webId[i] = rander.getAWebpageIndex();
    personId1[i] = rander.getAPersonIndex();
    personId2[i] = rander.getAPersonIndex();
    personId3[i] = personId2[i];

    ///TODO maybe infinite loop !!
    while( personId3[i] == personId2[i])     //for query of connection with p1 and p2
    personId3[i] = rander.getAPersonIndex();
//    cout << name[i] << endl << pid[i] << endl << webId[i] << endl << personId1[i] << endl << personId2[i] << endl << personId3[i] << endl;
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
  Query14 Q14;


  g.handleAddr();
  //start prefetching trigger
 
  auto id = 0; //only one set of input right now
  Q1.setPersonProperty("name", name[id]);
  Q2.setWebId(webId[id]);
  Q3.setPersonId(personId1[id]);
  Q4.setPersonProperty("pid", pid[id]);
  Q5.setPersonId(personId1[id]);
  Q6.setPersonId(personId1[id]);
  Q7.setPersonId(personId1[id]);
  Q8.setEndPoints(personId2[id], personId3[id]);
  Q9.setEndPoints(personId2[id], personId3[id]);
  Q10.setEndPoints(personId2[id], personId3[id]);
  Q11.setEndPoints(personId2[id], personId3[id]);
  Q12.setPersonId(personId1[id]);
  Q13.setPersonId(personId1[id]);
  Q14.setPersonProperty("name", name[id]);
/**
  Q8.runQuery(g, 1);
  Q8.runQuery(g, 2);

  vector<Query*> QueryList;
  QueryList.push_back(&Q1);
  QueryList.push_back(&Q2);
  QueryList.push_back(&Q3);
  QueryList.push_back(&Q4);
  QueryList.push_back(&Q5);
  QueryList.push_back(&Q6);
  QueryList.push_back(&Q7);
  QueryList.push_back(&Q8);
  QueryList.push_back(&Q9);
  QueryList.push_back(&Q10);
  QueryList.push_back(&Q11);
  QueryList.push_back(&Q12);
  QueryList.push_back(&Q13);

  cout << "begin query\n";
  for (auto i = 0; i < 13; i++) {
    for (auto j = 0; j < 13; j++) {
      for(auto k = 0; k < 13; k++) {
        for(auto m = 0; m < 13; m++) {

          auto query1 = QueryList.at(i);
          auto query2 = QueryList.at(i);
          auto query3 = QueryList.at(k);
          auto query4 = QueryList.at(m);

          vector<thread> MultiThread;

          auto opt = rand() % 2  + 1;
          MultiThread.push_back(thread([&]{query1->runQuery(g, opt);}) );
          MultiThread.push_back(thread([&]{query2->runQuery(g, opt);}) );
          MultiThread.push_back(thread([&]{query3->runQuery(g, opt);}) );
          MultiThread.push_back(thread([&]{query4->runQuery(g, opt);}) );

          for_each(MultiThread.begin(), MultiThread.end(),
             std::mem_fn(&thread::join));
        }
      }
    }
  }
  */
  /// 1: BFS  2: DFS
  SimRoiStart();
  /*
  Q1.runQuery(g,  1);
  Q1.runQuery(g,  2);
  Q2.runQuery(g,  1);
  Q2.runQuery(g, 2);
  Q3.runQuery(g, 1);
  Q3.runQuery(g, 2);
  Q4.runQuery(g, 1);
  Q4.runQuery(g, 2);
  Q5.runQuery(g, 1);
  Q5.runQuery(g, 2);
  Q6.runQuery(g, 1);
  Q6.runQuery(g, 2);
  Q7.runQuery(g, 1);
  Q7.runQuery(g, 2);
  Q8.runQuery(g, 1);
  Q8.runQuery(g, 2);
  Q9.runQuery(g, 1);
  */
  Q9.runQuery(g, 2);
  /*
  Q10.runQuery(g, 1);
  Q10.runQuery(g, 2);
  Q11.runQuery(g, 1);
  Q11.runQuery(g, 2);
  Q12.runQuery(g, 1);
  Q12.runQuery(g, 2);
  Q13.runQuery(g, 1);
  Q13.runQuery(g, 2);
  Q14.runQuery(g, 1);
  Q14.runQuery(g, 2);
  */
  SimRoiEnd();
  /**
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
*/
  cout << "finish testing\n";
  myfile.close();
  return 0;
}
