#include <algorithm>
#include <iostream>
#include <thread>

#include "LDBCReader.h"
//#include "LinkedList.h"
//#include "LdbcQueryDescription.h"
#include "LocksManager.h"
//#include "LdbcUpdateDescription.h"
#include "LdbcConcurrentQuery.h"

int main() {

  typedef GraphType Graph;
  typedef LocksManager LockManagerType;
///
  typedef std::thread thread;
  typedef PropertyList<FixedString, FixedString> PropertyListType;
  typedef std::pair<std::string, std::string> VertexPairType;
  typedef std::pair<std::string, PropertyListType> EdgePairType;
  typedef std::map<VertexPairType, EdgePairType> BranchMapType;


  Graph g;
  LDBCReader reader(g);
  reader.readDirectory("../tests/ldbc/social_network_50/New");
  cout << "Finish reading \n";

  LockManagerType LockManager;
  LockManager.buildLockMap(g);
  cout << "LockMap built\n";


  cout <<"Begin testing \n";

  LdbcQuery1 q1;
  q1.setParam("firstName", "Peng");
//  q1.runQuery(g, 31359, LockManager);

  LdbcQuery2 q2;
  q2.setRange("creationDate", "", "2012-07-16T10:00:00.255"); 
//  q2.runQuery(g, 31359, LockManager);

  LdbcQuery3 q3;
  q3.setPropertyRange("name", "Austria", "Angola"); 
  q3.setRange("creationDate", "2010-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 
//  q3.runQuery(g, 31359, LockManager);

  LdbcQuery4 q4;
  q4.setRange("creationDate", "2010-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 
//  q4.runQuery(g, 31359, LockManager);

  LdbcQuery5 q5;
  q5.setRange("joinDate", "2012-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 
//  q5.runQuery(g, 31359, LockManager);

  LdbcQuery6 q6;
  q6.setParam("name", "Rumi");
//  q6.runQuery(g, 31359, LockManager);

  LdbcQuery7 q7;
//  q7.runQuery(g, 31359, LockManager);

  LdbcQuery8 q8;
//  q8.runQuery(g, 31359, LockManager);

  LdbcQuery9 q9;
  q9.setRange("creationDate", "", "2011-07-16T23:59:00.255"); 
//  q9.runQuery(g, 31359, LockManager);

  LdbcQuery10 q10;
  q10.setRange("birthday", "0000-04-21", "0000-05-22");
//  q10.runQuery(g, 31359, LockManager);

  LdbcQuery11 q11;
  q11.setRange("workFrom", "", "2014");
  q11.setParam("name", "China");
//  q11.runQuery(g, 31359, LockManager);

  LdbcQuery12 q12;
  q12.setParam("name", "Person");
//  q12.runQuery(g, 31359, LockManager);

  LdbcQuery13 q13;
//  q13.runQuery(g, 31359, 31366, LockManager);

  LdbcQuery14 q14;
//  q14.runQuery(g, 31359, 31408, LockManager);

///write operations 
  TransactionManager transManager;

  PropertyListType VertexProp; 
  VertexProp.set("firstName", "Senator");
  VertexProp.set("id", "1234567");


  PropertyListType EdgeProp1, EdgeProp2; 
  EdgeProp1.set("null", "null");
  EdgeProp2.set("null", "null");

  EdgePairType NewEdge1, NewEdge2;
  NewEdge1 = make_pair("KNOWS", EdgeProp1);
  NewEdge2 = make_pair("HAS_INTEREST", EdgeProp2);

  BranchMapType NewBranch= {
    {make_pair("PERSON", "5497558144580"),  NewEdge1},
    {make_pair("TAG", "10"), NewEdge2} 
  };

  Query17 Q17;
  Q17.setPropertyList(VertexProp);
  Q17.setBranchMap(NewBranch);
//  Q17.runQuery(g, transManager, LockManager, 1);

  vector<thread> threads;
  threads.push_back( thread([&]{q1.runQuery(g, 31359, LockManager);}));
  threads.push_back( thread([&]{q2.runQuery(g, 31359, LockManager);}));
  threads.push_back( thread([&]{q3.runQuery(g, 31359, LockManager);}));
  threads.push_back( thread([&]{q4.runQuery(g, 31359, LockManager);}));
  threads.push_back( thread([&]{q5.runQuery(g, 31359, LockManager);}));
  threads.push_back( thread([&]{q6.runQuery(g, 31359, LockManager);}));
  threads.push_back( thread([&]{q7.runQuery(g, 31359, LockManager);}));
  threads.push_back( thread([&]{q8.runQuery(g, 31359, LockManager);}));
  threads.push_back( thread([&]{q9.runQuery(g, 31359, LockManager);}));
  threads.push_back( thread([&]{q10.runQuery(g, 31359, LockManager);}));
  threads.push_back( thread([&]{q11.runQuery(g, 31359, LockManager);}));
  threads.push_back( thread([&]{q12.runQuery(g, 31359, LockManager);}));
  threads.push_back( thread([&]{q13.runQuery(g, 31359, 31366, LockManager);}));
  threads.push_back( thread([&]{q14.runQuery(g, 31359, 31408, LockManager);}));
  threads.push_back( thread([&]{Q17.runQuery(g, transManager, LockManager, 1);}));

  for_each(threads.begin(), threads.end(),
           std::mem_fn(&thread::join));

  LdbcFile.close();

  cout << "Finish testing\n ";

}
