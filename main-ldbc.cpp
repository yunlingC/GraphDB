#include "LDBCReader.h"
#include "LdbcQueryDescription.h"

#include <iostream>

using namespace std;

int main() {

  typedef GraphType Graph;
  //typedef Graph::VertexDescriptor VertexDescriptor;
  //typedef Graph::VertexPointer VertexPointer;
  Graph g; 
  cout << "Begin testing\n"; 
  LDBCReader reader(g);
  reader.readDirectory("../tests/ldbc/social_network_100");

  auto persons = reader.getPersonList();

  cout << "Finish reading \n";

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

  LdbcQuery7 q7;
  q7.runQuery(g, persons[1]);

  LdbcQuery8 q8;
  q8.runQuery(g, persons[2]);

  LdbcQuery9 q9;
  q9.runQuery(g, persons[0]);

  LdbcQuery10 q10;
  q10.runQuery(g, persons[0]);

  LdbcQuery11 q11;
  q11.runQuery(g, persons[0]);

  LdbcQuery13 q13;
  q13.runQuery(g, persons[0], persons[4]);

  cout << "Finish testing\n ";

}
