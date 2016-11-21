#include "LDBCReader.h"
#include "TransactionalBFS.h"
#include "TransactionManager.h"
#include "Index.h"

#include "LDBCTransactionalQuery.cpp"
#include "global.h"

#include <thread>
#include <iostream>

using namespace std;

int main() {

//  typedef Visitor VisitorType;
  typedef Index IndexType;
//  typedef LdbcQuery * LdbcQueryPtr;
  GraphType g;

  cout << "Begin testing\n"; 
  LDBCReader reader(g);
  reader.readDirectory("../tests/ldbc/social_network_1000");

  auto persons = reader.getPersonList();
  cout << "Finish reading \n";

  LocksManager LkManager;
  LkManager.buildLockMap(g);
  TransactionManager TmManager;
  
//  IndexType Index(g);
//  auto NumIndices =  Index.buildVertexIndex("id");
//  cout <<"Indexing vertex " << NumIndices << "\n";
//
//  auto IndexEntry = Index.getVertexIndex( "420","PERSON" );
//  if (IndexEntry.second)  {
//    cout << (IndexEntry.first)->getPropertyValue("firstName").first
//          << "\t" << (IndexEntry.first)->getId() << endl;
//  }
//  else 
//    cout <<"Vertex Not found\n";

//  std::vector<LdbcQueryPtr> Queries;

  LdbcQuery1 q1(1);
  q1.setParam("gender", "male");
//  Queries.push_back(&q1);

  LdbcQuery2 q2(2);
  q2.setRange("creationDate", "", "2012-07-16T10:00:00.255"); 
//  Queries.push_back(&q2);


  LdbcQuery3 q3(3);
  q3.setPropertyRange("name", "Austria", "Angola"); 
  q3.setRange("creationDate", "2010-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 
//  Queries.push_back(&q3);

  LdbcQuery4 q4(4);
  q4.setRange("creationDate", "2010-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 
//  Queries.push_back(&q4);

  LdbcQuery5 q5(5);
  q5.setRange("joinDate", "2012-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 
//  Queries.push_back(&q5);

  LdbcQuery6 q6(6);
  q6.setParam("name", "Peter_Pan");
//  Queries.push_back(&q6);

  LdbcQuery7 q7(7);
//  Queries.push_back(&q7);

  LdbcQuery8 q8(8);
//  Queries.push_back(&q8);

  LdbcQuery9 q9(9);
  q9.setRange("creationDate", "", "2011-07-16T23:59:00.255"); 
//  Queries.push_back(&q9);

  LdbcQuery10 q10(10);
  q10.setRange("birthday", "0000-04-21", "0000-05-22");
//  Queries.push_back(&q10);

  LdbcQuery11 q11(11);
  q11.setRange("workFrom", "", "2014");
  q11.setParam("name", "China");
//  Queries.push_back(&q11);

  LdbcQuery12 q12(12);
  q12.setParam("name", "Person");
//  Queries.push_back(&q12);

  LdbcQuery13 q13(13);
//  Queries.push_back(&q13);

  LdbcQuery14 q14(14);
//  Queries.push_back(&q14);

//        q1.runQuery(g, Input[0], LockManager);
//        q2.runQuery(g, Input[0], LockManager);
//        q3.runQuery(g, Input[2], LockManager);
//        q4.runQuery(g, Input[3], LockManager);
//        q5.runQuery(g, Input[4], LockManager);
//        q6.runQuery(g, Input[5], LockManager);
//        q7.runQuery(g, Input[6], LockManager);
//        q8.runQuery(g, Input[7], LockManager);
//        q9.runQuery(g, Input[8], LockManager);
//        q10.runQuery(g, Input[9], LockManager);
//        q11.runQuery(g, Input[10], LockManager);
//        q12.runQuery(g, Input[11], LockManager);
//        q13.runQuery(g, Input[12], Input[0], LockManager);
//        q14.runQuery(g, Input[13], Input[1], LockManager);


    std::vector<TranPointer> TranxList;
    for (auto i= 0; i < 14; i++) {
      auto TxEntryPtr = TmManager.addTransaction();
      auto TxPtr = TxEntryPtr.second;
      TranxList.push_back(TxPtr);
    }

    vector<std::thread> threads;

      threads.push_back(std::thread([&]{q1.runQuery(g, persons[0], *(TranxList[0]));}));
      threads.push_back(std::thread([&]{q2.runQuery(g, persons[0], *(TranxList[1]));}));
      threads.push_back(std::thread([&]{q3.runQuery(g, persons[0], *(TranxList[2]));}));
      threads.push_back(std::thread([&]{q4.runQuery(g, persons[0], *(TranxList[3]));}));
      threads.push_back(std::thread([&]{q5.runQuery(g, persons[0], *(TranxList[4]));}));
      threads.push_back(std::thread([&]{q6.runQuery(g, persons[0], *(TranxList[5]));}));

    for_each(threads.begin(), threads.end(), std::mem_fn(&thread::join)); 

//  
//  for_each(threads.begin(), threads.end(), std::mem_fn(&thread::join)); 
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

