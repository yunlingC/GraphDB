#include "LDBCReader.h"
#include "LdbcRDFSQueryDescription.h"
#include "LdbcQueryDescription.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {

  /// argv[0] ./main-rdfs-ldbc
  /// argv[1] query number 1:14
  /// argv[2] traversal method
  
  //typedef Graph::VertexDescriptor VertexDescriptor;
  //typedef Graph::VertexPointer VertexPointer;
  typedef GraphType Graph;

  if(argc < 3) {
    cout <<"Error: At least 3 arguments\n";
    cout <<" 1. query number 1-14 \n"
         <<" 2. traversal  1-BFS 2-DFS  \n"
         <<" 3. parameter #  \n";
    return 1;
  }

  unsigned int QId = atoi(argv[1]);
  unsigned int TId = atoi(argv[2]);

  if (QId > 14 || QId < 1 || (TId != 1 && TId != 2)) {
    cout <<"Wrong arguments \n";
    return 1;
  }

  Graph g; 
  cout << "Begin testing\n"; 
  LDBCReader reader(g);
  reader.readDirectory("../tests/ldbc/social_network_100");

  auto persons = reader.getPersonList();

  cout << "Finish reading \n";

  /**
  LdbcQuery1 q1;
  q1.setParam("firstName", "Peng");
  q1.runQuery(g, persons[0]);

  LdbcQuery2 q2;
  q2.setRange("creationDate", "", "2012-07-16T10:00:00.255"); 
  q2.runQuery(g, persons[0]);

  LdbcQuery3 q3;
  q3.setPropertyRange("name", "Austria", "Angola"); 
  q3.setRange("creationDate", "2010-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 
  q3.runQuery(g, persons[0]);

  LdbcQuery4 q4;
  q4.runQuery(g, persons[0]);

  LdbcQuery5 q5;
  q5.runQuery(g, persons[0]);

  LdbcQuery6 q6;
  q6.runQuery(g, persons[0]);

//  LdbcQuery7 q7;
//  q7.runQuery(g, persons[1]);

  LdbcRDFSQuery7 Q7;
  Q7.runQuery(g, persons[2]);

  LdbcRDFSQuery8 q8;
  q8.runQuery(g, persons[4]);

*/
  LdbcRDFSQuery2 q2;
  q2.runQuery(g, persons[0]);

  LdbcRDFSQuery9 q9;
  q9.runQuery(g, persons[0]);

/**
  LdbcQuery10 q10;
  q10.runQuery(g, persons[0]);

  LdbcQuery11 q11;
  q11.runQuery(g, persons[0]);
*/
//  LdbcRDFSQuery13 Q13;
//  Q13.runQuery(g, persons[0], persons[8]);
//
//  LdbcQuery13 q13;
//  q13.runQuery(g, persons[0], persons[8]);

//  vector<Query*> QueryList;

 /** 
  QueryList.push_back(&q1);
  QueryList.push_back(&q2);
  QueryList.push_back(&q3);
  QueryList.push_back(&q4);
  QueryList.push_back(&q5);
  QueryList.push_back(&q6);
  QueryList.push_back(&q7);
  QueryList.push_back(&q8);
  QueryList.push_back(&q9);
  QueryList.push_back(&q10);
  QueryList.push_back(&q11);
  QueryList.push_back(&q12);
  */

//  QueryList.push_back(&q13);
//  QueryList.push_back(&q14);

//  cout << "query # " << QId << "\n";

//  auto Query = QueryList[QId - 1];
  ///TODO big hole here
//  Query->runQuery(g, TId);

  cout << "Finish testing\n ";

}
