#include "LDBCReader.h"
#include "TransactionalBFS.h"
#include "TransactionManager.h"
#include "Index.h"

//#include "LDBCTransactionalQuery.cpp"
#include "global.h"

#include <thread>
#include <iostream>

using namespace std;

int main() {

//  typedef Visitor VisitorType;
  GraphType g;

  cout << "Begin testing\n"; 
  LDBCReader reader(g);
  reader.readDirectory("../tests/ldbc/social_network_100");

  auto persons = reader.getPersonList();
  cout << "Finish reading \n";

  LocksManager LkManager;
  LkManager.buildLockMap(g);
  TransactionManager TmManager;
  
  IndexType Index(g);
  Index.buildVertexIndex("id");

//  VisitorType Visitor;

//  auto TxId = TmManager.addTransaction();

//  auto TxEntryPtr = TmManager.addTransaction();
//  auto TxPtr = TxEntryPtr.second;

//  LdbcQuery1 Q1;
//  Q1.setParam("firstName", "Bruno");
//  Q1.runQuery(g, persons[0]);
//
// 
//  LdbcQuery2 q2;
//  Q2.runQuery(g, persons[0]);
//
//  vector<std::thread> threads;
//  for (auto i= 0; i < 2; i++) {
//    auto TxEntryPtr = TmManager.addTransaction();
//    auto TxPtr = TxEntryPtr.second;
//    threads.push_back(std::thread([&]{Q1.runQuery(g, persons[0], *TxPtr);}));
//  }
//  
//  for_each(threads.begin(), threads.end(), std::mem_fn(&thread::join)); 
//
//  LkManager.dumpMaps();

  cout <<"finish testing\n";
  fflush(stdout);

}

