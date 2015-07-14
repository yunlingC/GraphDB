#include <algorithm>
#include <iostream>
#include <thread>

#include "LDBCReader.h"
#include "LinkedList.h"
#include "LdbcQueryDescription.h"


int main() {

  typedef GraphType Graph;
  //typedef Graph::VertexDescriptor VertexDescriptor;
  //typedef Graph::VertexPointer VertexPointer;

  Graph g;
  cout << "Begin testing\n";

  LDBCReader reader(g);
  reader.readDirectory("../tests/ldbc/social_network_50/New");

  cout << "Finish reading \n";

//  passPtr(0,0,0,0,0,0);
  //start prefetching trigger
  
//  SimRoiStart();
//  SimRoiEnd();

//  LdbcQuery1 q1;
//  q1.runQuery(g, 0);
//
//  LdbcQuery2 q2;
//  q2.runQuery(g, 0);
//
//  LdbcQuery3 q3;
//  q3.runQuery(g, 0);
//
//  LdbcQuery4 q4;
//  q4.runQuery(g, 0);
//
//  LdbcQuery5 q5;
//  q5.runQuery(g, 0);
//
//  LdbcQuery6 q6;
//  q6.runQuery(g, 0);
//
//  LdbcQuery7 q7;
//  q7.runQuery(g, 1);
//
//  LdbcQuery8 q8;
//  q8.runQuery(g, 2);
//
//  LdbcQuery9 q9;
//  q9.runQuery(g, 0);

//  LdbcQuery10 q10;
//  q10.runQuery(g, 31359);

//  LdbcQuery11 q11;
//  q11.runQuery(g, 0);
//
//    LdbcQuery12 q12;
//    q12.runQuery(g, 31359);
//
  LdbcQuery13 q13;
  q13.runQuery(g, 0, 4);


//  LdbcQuery14 q14;
//  q14.runQuery(g, 31359, 31399);



//  Visitor vis;
//  VertexPointer vertex;
//  vis.visitVertex(vertex) {
//    cout << "==>vid " << vertex->getId() << " ==>pid " << vertex->getPropertyValue(key).first << endl;
//  }

//  breadthFirstSearch(g, 0, vis);
//  cout << "Finish testing\n ";

}
