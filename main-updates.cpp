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

  Graph g;
  cout << "Begin testing\n";

  LDBCReader reader(g);
  reader.readDirectory("../tests/ldbc/social_network_10/New");
 
  std::cout << "Finish reading\n";
  LockManager lockManager;
  lockManager.buildLockMap(g);
  cout << "LockMap built\n";

  TransactionManager transManager;

  ///Q16 is for insert: insert a new people make is MOHAMMED 's new friend
  Query16 Q16;
  Q16.setPersonProperty("firstName", "Luis Filipe"); 

//  Q18 is for reading people MOHAMMED 's friends number: 7 orginally
  Query18 Q18;
  Q18.setPersonId(0);

  Q18.runQuery(g, transManager, lockManager, 1);
  Q16.runQuery(g, transManager, lockManager, 1);
  Q18.runQuery(g, transManager, lockManager, 1);

 // MyFile << "\n\n++++++++++++++Multithread+++++++++++++++\n\n";
  vector<thread> threads;
//  threads.push_back(thread([&] {Q15.runQuery(g, transManager, lockManager, 1);}));
//  threads.push_back(thread([&] {Q18.runQuery(g, transManager, lockManager, 1);}));
  threads.push_back(thread([&] {Q16.runQuery(g, transManager, lockManager, 1);}));
  threads.push_back(thread([&] {Q16.runQuery(g, transManager, lockManager, 1);}));
//  threads.push_back(thread([&] {Q17.runQuery(g, transManager, lockManager, 1);}));
//  threads.push_back(thread([&] {Q17.runQuery(g, transManager, lockManager, 1);}));

  for_each(threads.begin(), threads.end(),
           std::mem_fn(&thread::join));

  Q18.runQuery(g, transManager,lockManager,  1);

  cout << "finish testing\n";
  MyFile.close();
  return 0;
}
