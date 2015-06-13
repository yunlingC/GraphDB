#include <algorithm>
#include <iostream>
#include <thread>

//#include "GDBench/util.h"
#include "LDBCReader.h"
#include "LinkedList.h"
#include "LdbcQueryDescription.h"


int main() {

  typedef GraphType Graph;

  Graph g;
  cout << "Begin testing\n";

  LDBCReader reader(g);
  reader.readDirectory("../tests/ldbc/social_network_part/New");
  cout << "Finish reading \n";

//  SimRoiStart();
//  g.handleAddr();

//  LdbcQuery1 q1;
//  auto persons = reader.getPersonList();
//  cout << "start person " << persons[1] << endl;

//  q1.runQuery(g, persons[1]);

//  SimRoiEnd();
//  LdbcFile.close();
//
//  auto persons = reader.getPersonList();
//  cout << "start person " << persons[1] << endl;

//  passPtr(0,0,0,0,0,0);
  //start prefetching trigger
  
//  LdbcQuery1 q1;

//  SimRoiStart()
//  q1.runQuery(g, persons[1]);
//  SimRoiEnd();

//  LdbcQuery2 q2;
//  q2.runQuery(g, persons[1]);

//  LdbcQuery4 q4;
//  q4.runQuery(g, persons[0]);

//  LdbcQuery5 q5;
//  q5.runQuery(g, persons[0]);

//  LdbcQuery6 q6;
//  q6.runQuery(g, persons[0]);

//  LdbcQuery7 q7;
//  q7.runQuery(g, persons[1]);

//  LdbcQuery8 q8;
//  q8.runQuery(g, persons[2]);

//  LdbcQuery9 q9;
//  q9.runQuery(g, persons[0]);

//  LdbcQuery10 q10;
//  q10.runQuery(g, persons[0]);

//  LdbcQuery11 q11;
//  q11.runQuery(g, persons[0]);

//  LdbcQuery13 q13;
//  q13.runQuery(g, persons[0], persons[4]);
  
//  LdbcQuery3 q3;
//  q3.runQuery(g, persons[0]);
//  Visitor vis;
//  VertexPointer vertex;
//  vis.visitVertex(vertex) {
//    cout << "==>vid " << vertex->getId() << " ==>pid " << vertex->getPropertyValue(key).first << endl;
//  }

//  breadthFirstSearch(g, 0, vis);
//  cout << "Finish testing\n ";

}
