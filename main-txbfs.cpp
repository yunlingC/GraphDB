#include "LDBCReader.h"
#include "TransactionalBFS.h"
#include "TransactionManager.h"
//#include "Index.h"

#include "LDBCTransactionalQuery.cpp"
#include "global.h"

#include <thread>
#include <iostream>

using namespace std;


int main(int argc, char *argv[]) {

//  typedef Visitor VisitorType;
  typedef Index IndexType;
//  typedef LdbcQuery * LdbcQueryPtr;
  typedef LdbcQuery::TransactionPointerType TransactionPointerType;
  typedef std::vector<GraphType::VertexDescriptor> VertexIdListType;
  typedef GraphType::PropertyListType PropertyListType;

  int InputSize = 1000;
  int run = 1;
  

  ///arg: 0-main-txbfs 
  ///     1- single thread (1) or multithread(2)
  //      2 (optional) file size
  if ( argc < 2) {
    std::cout <<"Need one more arg: single thread(1) or multithread(2)\n";
    exit(1);
  } else {
    run = std::stoi(argv[1]);
    if (argc > 2)
      InputSize = std::stoi(argv[2]);
//    if ( (InputSize != 1000) && (InputSize != 5000) ) {
//      std::cout <<"Arg error : should be: 1000 or 5000\n";
//      exit(1);
//    }
  }

  GraphType g;

  cout << "Begin testing\n"; 
  LDBCReader reader(g);
  reader.readDirectory("../tests/ldbc/social_network_"+std::to_string(InputSize));

  VertexIdListType persons = reader.getPersonList();
  cout << "Finish reading \n";

  LocksManager LkManager;
  LkManager.buildLockMap(g);
  TransactionManager TmManager;
  
  IndexType Index(g);
  auto NumIndices =  Index.buildVertexIndex("id");
  cout <<"Indexing vertex " << NumIndices << "\n";

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

  /** Update queries definitions**/
  LdbcQueryAddVertex q15(15);
  PropertyListType VertexProp;
  VertexProp.set("firstName", "Senator");
  VertexProp.set("id", "1234567");
  q15.initVertex("PERSON", VertexProp);

  q15.initEdge(true, "PERSON_IS_LOCATED_IN", "PLACE", "1", "null", "null"); 
  q15.initEdge(true, "HAS_INTEREST", "TAG", "10", "null", "null"); 
  q15.initEdge(true, "STUDY_AT", "ORGANISATION", "1580", "classYear", "2010"); 

  LdbcQueryAddEdge q16(16);
  q16.initEdge("LIKES_POST", "creationDate", "2015-12-08T00:47:05.399+0000") ;
  q16.getVertexId("78038", "PERSON", "8", "POST");

  LdbcQueryAddEdge q17(17);
  q17.initEdge("LIKES_COMMENTS", "creationDate", "2015-9-08T00:47:05.399+0000") ;
  q17.getVertexId("78038", "PERSON", "68719476749", "COMMENT");

  LdbcQueryAddVertex q18(18);
  PropertyListType ForumProp;
  ForumProp.set("id", "2015");
  ForumProp.set("title", "Boring Game");
  ForumProp.set("creationDate", "2016-9-08T00:47:05.399+0000");
  q18.initVertex("FORUM", ForumProp);

  q18.initEdge(true, "HAS_MODERATOR", "PERSON", "78038", "null", "null"); 
  q18.initEdge(true, "FORUM_HAS_TAG", "TAG", "10", "null", "null"); 

  LdbcQueryAddEdge q19(19);
  q19.initEdge("HAS_MEMBER", "joinDate", "2015-12-08T00:47:05.399+0000") ;
  q19.getVertexId("2015", "FORUM", "78038", "PERSON");

  LdbcQueryAddVertex q20(20);
  PropertyListType PostProp;
  PostProp.set("id", "4023");
  q20.initVertex("POST", PostProp);

  q20.initEdge(true, "POST_HAS_CREATOR", "PERSON", "78038", "null", "null"); 
  q20.initEdge(true, "POST_HAS_TAG", "TAG", "10", "null", "null"); 
 
  LdbcQueryAddVertex q21(21);
  PropertyListType CommentProp;
  CommentProp.set("id", "65432");
  CommentProp.set("creationDate", "2015-11-08T00:47:05.399+0000");
  q21.initVertex("COMMENT", CommentProp);

  q21.initEdge(true, "COMMENT_IS_LOCATED_IN", "PLACE", "1", "null", "null"); 
  q21.initEdge(true, "COMMENT_HAS_CREATOR", "PERSON", "78038", "null", "null"); 
  q21.initEdge(true, "COMMENT_HAS_TAG", "TAG", "10", "null", "null"); 

  LdbcQueryAddEdge q22(22);
  q22.initEdge("KNOWS", "creationDate", "2015-10-08T00:47:05.399+0000") ;
  q22.getVertexId("78038", "PERSON", "PERSON", "420");
  /*** End of Update queries definition ***/

  std::vector<TransactionPointerType> TranxList;
  for (auto i= 0; i < 14; i++) {
    auto TxEntryPtr = TmManager.addTransaction();
    auto TxPtr = TxEntryPtr.second;
    TranxList.push_back(TxPtr);
  }


  if (run == 1) {
    q1.runQuery(g, persons[0], (TranxList[0]), LkManager);
  
    q2.runQuery(g, persons[0], (TranxList[0]), LkManager);
  
    q3.runQuery(g, persons[0], (TranxList[0]), LkManager);
  
    q4.runQuery(g, persons[0], (TranxList[0]), LkManager);
  
    q5.runQuery(g, persons[0], (TranxList[0]), LkManager);
  
    q6.runQuery(g, persons[0], (TranxList[0]), LkManager);
  
    q7.runQuery(g, persons[0], (TranxList[0]), LkManager);
  
    q8.runQuery(g, persons[0], (TranxList[0]), LkManager);
  
    q9.runQuery(g, persons[0], (TranxList[0]), LkManager);
  
    q10.runQuery(g, persons[0], (TranxList[0]), LkManager);
  
    q11.runQuery(g, persons[0], (TranxList[0]), LkManager);
  
    q12.runQuery(g, persons[0], (TranxList[0]), LkManager);
  
    q13.runQuery(g, persons[0], persons[10], (TranxList[0]), LkManager);
    
    q14.runQuery(g, persons[0], persons[10], (TranxList[0]), LkManager);
  
    q15.runQuery(g, persons[0], (TranxList[0]), LkManager, Index);
  
    q16.runQuery(g, persons[0], (TranxList[0]), LkManager, Index);

    q17.runQuery(g, persons[0], (TranxList[0]), LkManager, Index);
  
    q18.runQuery(g, persons[0], (TranxList[0]), LkManager, Index);

    q19.runQuery(g, persons[0], (TranxList[0]), LkManager, Index);
  
    q20.runQuery(g, persons[0], (TranxList[0]), LkManager, Index);

    q21.runQuery(g, persons[0], (TranxList[0]), LkManager, Index);

    q22.runQuery(g, persons[0], (TranxList[0]), LkManager, Index);
  } 
  else if (run == 2)  {

    vector<std::thread> threads;

    threads.push_back(std::thread([&]{q1.runQuery(g, persons[0], (TranxList[0]), LkManager);}));
    threads.push_back(std::thread([&]{q2.runQuery(g, persons[0], (TranxList[1]), LkManager);}));
    threads.push_back(std::thread([&]{q3.runQuery(g, persons[0], (TranxList[2]), LkManager);}));
    threads.push_back(std::thread([&]{q4.runQuery(g, persons[0], (TranxList[3]), LkManager);}));
    threads.push_back(std::thread([&]{q5.runQuery(g, persons[0], (TranxList[4]), LkManager);}));
    threads.push_back(std::thread([&]{q6.runQuery(g, persons[0], (TranxList[5]), LkManager);}));
    threads.push_back(std::thread([&]{q7.runQuery(g, persons[0], (TranxList[6]), LkManager);}));
    threads.push_back(std::thread([&]{q8.runQuery(g, persons[0], (TranxList[7]), LkManager);}));
    threads.push_back(std::thread([&]{q9.runQuery(g, persons[0], (TranxList[8]), LkManager);}));
    threads.push_back(std::thread([&]{q10.runQuery(g, persons[0], (TranxList[9]), LkManager);}));
    threads.push_back(std::thread([&]{q11.runQuery(g, persons[0], (TranxList[10]), LkManager);}));
    threads.push_back(std::thread([&]{q12.runQuery(g, persons[0], (TranxList[11]), LkManager);}));
    threads.push_back(std::thread([&]{q13.runQuery(g, persons[0], persons[100], (TranxList[12]), LkManager);}));
    threads.push_back(std::thread([&]{q14.runQuery(g, persons[0], persons[100], (TranxList[13]), LkManager);}));

    for_each(threads.begin(), threads.end(), std::mem_fn(&thread::join)); 
  }

  else if (run == 3) {

    q21.runQuery(g, persons[0], (TranxList[0]), LkManager, Index);

    q15.runQuery(g, persons[0], (TranxList[0]), LkManager, Index);
  
    q16.runQuery(g, persons[0], (TranxList[0]), LkManager, Index);

//    q17.runQuery(g, persons[0], (TranxList[0]), LkManager, Index);
//  
//    q18.runQuery(g, persons[0], (TranxList[0]), LkManager, Index);

//    q19.runQuery(g, persons[0], (TranxList[0]), LkManager, Index);
  
//    q20.runQuery(g, persons[0], (TranxList[0]), LkManager, Index);


 //   q22.runQuery(g, persons[0], (TranxList[0]), LkManager, Index);
  }

  else {
    std::cout << "Error: Not single thread or multithread\n";
    exit(0);
  }
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

