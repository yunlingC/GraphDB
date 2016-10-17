#include "LDBCReader.h"
#include "TranxBreadthFirstSearch.h"
#include "TransactionManager.h"
#include "global.h"

#include <thread>
#include <iostream>

using namespace std;

int main() {

  typedef Visitor VisitorType;
  GraphType g;

  cout << "Begin testing\n"; 
  LDBCReader reader(g);
  reader.readDirectory("../tests/ldbc/social_network_100");

  auto persons = reader.getPersonList();
  cout << "Finish reading \n";

  LocksManager LkManager;
  LkManager.buildLockMap(g);
  TransactionManager TmManager;
  VisitorType Visitor;

//  auto TxId = TmManager.addTransaction();

//  auto TxEntryPtr = TmManager.addTransaction();
//  auto TxPtr = TxEntryPtr.second;

  vector<std::thread> threads;
  for (auto i= 0; i < 2; i++) {
    auto TxEntryPtr = TmManager.addTransaction();
    auto TxPtr = TxEntryPtr.second;
    threads.push_back(std::thread(tranxBreadthFirstSearch, std::ref(g), 0, std::ref(Visitor), TxPtr, LkManager));
  }
  
  for_each(threads.begin(), threads.end(), std::mem_fn(&thread::join)); 

  LkManager.dumpMaps();

  cout <<"finish testing\n";
  fflush(stdout);

}

