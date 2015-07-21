#include <algorithm>
#include <iostream>
#include <thread>

#include "LDBCReader.h"
#include "LinkedList.h"
#include "LdbcQueryDescription.h"

int main() {

  typedef GraphType Graph;

  Graph g;
  LDBCReader reader(g);
  reader.readDirectory("../tests/ldbc/social_network_50/New");
  cout << "Finish reading \n";

  cout <<"Begin testing \n";

  LdbcQuery1 q1;
  q1.setParam("firstName", "Peng");
  q1.runQuery(g, 31359);

  LdbcQuery2 q2;
  q2.setRange("creationDate", "", "2012-07-16T10:00:00.255"); 
  q2.runQuery(g, 31359);

  LdbcQuery3 q3;
  q3.setPropertyRange("name", "Austria", "Angola"); 
  q3.setRange("creationDate", "2010-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 
  q3.runQuery(g, 31359);

  LdbcQuery4 q4;
  q4.setRange("creationDate", "2010-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 
  q4.runQuery(g, 31359);

  LdbcQuery5 q5;
  q5.setRange("joinDate", "2012-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 
  q5.runQuery(g, 31359);

  LdbcQuery6 q6;
  q6.setParam("name", "Rumi");
  q6.runQuery(g, 31359);

  LdbcQuery7 q7;
  q7.runQuery(g, 31359);

  LdbcQuery8 q8;
  q8.runQuery(g, 31359);

  LdbcQuery9 q9;
  q9.setRange("creationDate", "", "2011-07-16T23:59:00.255"); 
  q9.runQuery(g, 31359);

  LdbcQuery10 q10;
  q10.setRange("birthday", "0000-04-21", "0000-05-22");
  q10.runQuery(g, 31359);

  LdbcQuery11 q11;
  q11.setRange("workFrom", "", "2014");
  q11.setParam("name", "China");
  q11.runQuery(g, 31359);

  LdbcQuery12 q12;
  q12.setParam("name", "Person");
  q12.runQuery(g, 31359);

  LdbcQuery13 q13;
  q13.runQuery(g, 31359, 31366);

  LdbcQuery14 q14;
  q14.runQuery(g, 31359, 31408);

  cout << "Finish testing\n ";

}
