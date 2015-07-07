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
  typedef std::unordered_map<std::string, std::pair<std::string, std::string>> BranchMapType;

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

  BranchMapType BranchMap = {
    {"KNOWS", make_pair("firstName", "Andrew")}, 
    {"HAS_INTEREST", make_pair("id", "10")}
  };

  Query17 Q17;
  Q17.setPropertyList(VertexProp);
  Q17.setBranchMap(BranchMap);

  cout << "setup\n";
  Q17.runQuery(g, transManager, lockManager, 1);

  Query18 Q18;
  Q18.setPersonId(31409);   
  Q18.runQuery(g, transManager, lockManager, 1);

  vector<thread> threads;
  threads.push_back(thread([&] {Q18.runQuery(g, transManager, lockManager, 1);}));
  threads.push_back(thread([&] {Q17.runQuery(g, transManager, lockManager, 1);}));

  for_each(threads.begin(), threads.end(),
           std::mem_fn(&thread::join));

  Q18.runQuery(g, transManager, lockManager, 1);

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
  CCFile.close();
  return 0;
}
