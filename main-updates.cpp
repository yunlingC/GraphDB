#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <queue>
#include <thread>
//#include <boost/thread.hpp>
#include "GDBench/util.h"
#include "LinkedList.h"
#include "QueryDescription.h"
#include "UpdateDescription.h"
#include "QueryRandomizer.h"


int main() {

  typedef GraphType Graph;
  typedef std::thread thread;

  Graph g;
  cout << "Begin testing\n";

  GDReader reader(g);
  reader.readFile("../tests/gd/sndata.gd");

  LockManager lockManager;
  lockManager.buildLockMap(g);
  cout << "LockMap built\n";

  TransactionManager transManager;
  ///Q15 is for update : update people MOHAMMED ROBESON with age = 20
  /**
  Query15 Q15;
  Q15.setPersonProperty("name","MOHAMMED ROBESON"); 
  Q15.setLockManager(lockManager);
  Q15.setSleepTime(1);
 */ 
  ///Q16 is for insert: insert a new people make is MOHAMMED 's new friend
  Query16 Q16;
  Q16.setPersonProperty("name","MOHAMMED ROBESON"); 
  Q16.setLockManager(lockManager);
  Q16.setSleepTime(2);

  //Q18 is for reading people MOHAMMED 's friends number: 7 orginally
  Query18 Q18;
  Q18.setPersonId(0);
  Q18.setLockManager(lockManager);

  /**
  ///Q17 is for delete:delete MOHAMMED 's friend 
  Query17 Q17;
  Q17.setPersonProperty("name","MOHAMMED ROBESON"); 
  Q17.setLockManager(lockManager);
  Q17.setSleepTime(3);
*/
  ///without concurrency: results is in gd_excution.log (not logfile for rollback!)
  Q18.runQuery(g, transManager, 1);
//  Q15.runQuery(g, transManager, 1);

  cout << "VertexLock num " << lockManager.getVertexLockMap().size() << "\n";
  Q16.runQuery(g, transManager, 1);
  cout << "VertexLock num " << lockManager.getVertexLockMap().size() << "\n";
//  Q18.runQuery(g, 1);
//  Q17.runQuery(g, transManager, 1);
  Q18.runQuery(g, transManager, 1);

  ///In order for observe how locks is working, run every thread twice;
  // sleep time could be changed in order to see how data are locked and released
  /*
  MyFile << "\n\n++++++++++++++Multithread+++++++++++++++\n\n";
  vector<thread> threads;
  threads.push_back(thread([&] {Q15.runQuery(g, transManager, 1);}));
  threads.push_back(thread([&] {Q18.runQuery(g, transManager, 1);}));
  threads.push_back(thread([&] {Q18.runQuery(g, transManager, 1);}));
//  threads.push_back(thread([&] {Q18.runQuery(g,  1);}));
//  threads.push_back(thread([&] {Q18.runQuery(g,  1);}));
//  threads.push_back(thread([&] {Q18.runQuery(g,  1);}));
//  threads.push_back(thread([&] {Q18.runQuery(g,  1);}));
  threads.push_back(thread([&] {Q16.runQuery(g, transManager,1);}));
  threads.push_back(thread([&] {Q16.runQuery(g, transManager,1);}));
  threads.push_back(thread([&] {Q17.runQuery(g, transManager,1);}));
  threads.push_back(thread([&] {Q17.runQuery(g, transManager,1);}));

  for_each(threads.begin(), threads.end(),
           std::mem_fn(&thread::join));

  Q18.runQuery(g, transManager, 1);
*/
  cout << "finish testing\n";
  MyFile.close();
  return 0;
}
