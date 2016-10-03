#include "LDBCReader.h"
#include "TranxBreadthFirstSearch.h"
#include "TransactionManager.h"

#include <thread>
#include <iostream>

using namespace std;

#define _DEBUG_ENABLE_  true

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

  auto TxPtr = TmManager.addTransaction();

  vector<std::thread> threads;
  for (auto i= 0; i < 10; i++) {
    threads.push_back(std::thread(tranxBreadthFirstSearch, std::ref(g), 0, std::ref(Visitor), TxPtr, LkManager));
  }
  
  for_each(threads.begin(), threads.end(), std::mem_fn(&thread::join)); 

  cout <<"finish testing\n";
  fflush(stdout);
//  tranxBreadthFirstSearch(g, 0, Visitor, TxPtr, LkManager);

}

