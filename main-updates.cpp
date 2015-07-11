#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <queue>
#include <thread>
//#include <boost/thread.hpp>
#include "GDBench/util.h"
#include "LDBCReader.h"
#include "LinkedList.h"
//#include "LdbcQueryDescription.h"
#include "LdbcUpdateDescription.h"
#include "QueryRandomizer.h"


int main() {

  typedef GraphType Graph;
  typedef std::thread thread;
  typedef LocksManager LockManager;
  typedef PropertyList<FixedString, FixedString> PropertyListType;
  typedef std::pair<std::string, std::string> VertexPairType;
  typedef std::pair<std::string, PropertyListType> EdgePairType;
  typedef std::map<VertexPairType, EdgePairType> BranchMapType;

  Graph g;
  cout << "Begin testing\n";

  LDBCReader reader(g);
  reader.readDirectory("../tests/ldbc/social_network_50/New");
 
  std::cout << "Finish reading\n";
  LockManager lockManager;
  lockManager.buildLockMap(g);
  cout << "LockMap built\n";

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
//     make_pair("KNOWS", nullptr)}:, 
    {make_pair("TAG", "10"), NewEdge2} 
//     make_pair("HAS_INTEREST", nullptr)}
  };

  Query17 Q17;
  Q17.setPropertyList(VertexProp);
  Q17.setBranchMap(NewBranch);

  cout << "setup\n";
  Q17.runQuery(g, transManager, lockManager, 1);

  Query18 Q18;
  Q18.setPersonId(31409);   
  Q18.runQuery(g, transManager, lockManager, 1);

//  Query17 q17;
//  Query18 q18;
 
//  Q17.runQuery(g, transManager, lockManager, 1);
//  Q18.runQuery(g, transManager, lockManager, 1);

//  vector<thread> threads;
//  threads.push_back(thread([&] {Q18.runQuery(g, transManager, lockManager, 1);}));
//  threads.push_back(thread([&] {Q17.runQuery(g, transManager, lockManager, 1);}));
//
//  for_each(threads.begin(), threads.end(),
//           std::mem_fn(&thread::join));

//  Query16 Q16;
//  Q16.setPersonProperty("firstName", "Luis Filipe"); 
//
////  Q18 is for reading people MOHAMMED 's friends number: 7 orginally
//  Query18 Q18;
//  Q18.setPersonId(0);
//
//  ///new test 
//  Query17 Q17;
//  Q17.setVertexProperty();
//  Q17.setBranchMap();
//
//  Q17.runQuery(g, transManager, lockManager, 1);
//  Q18.runQuery(g, transManager, lockManager, 1);
//  Q16.runQuery(g, transManager, lockManager, 1);
//  Q18.runQuery(g, transManager, lockManager, 1);
//
// // MyFile << "\n\n++++++++++++++Multithread+++++++++++++++\n\n";
//  vector<thread> threads;
////  threads.push_back(thread([&] {Q15.runQuery(g, transManager, lockManager, 1);}));
////  threads.push_back(thread([&] {Q18.runQuery(g, transManager, lockManager, 1);}));
//  threads.push_back(thread([&] {Q16.runQuery(g, transManager, lockManager, 1);}));
//  threads.push_back(thread([&] {Q16.runQuery(g, transManager, lockManager, 1);}));
////  threads.push_back(thread([&] {Q17.runQuery(g, transManager, lockManager, 1);}));
////  threads.push_back(thread([&] {Q17.runQuery(g, transManager, lockManager, 1);}));
//
//  for_each(threads.begin(), threads.end(),
//           std::mem_fn(&thread::join));
//
//  Q18.runQuery(g, transManager,lockManager,  1);
//
  cout << "finish testing\n";
  return 0;
}
