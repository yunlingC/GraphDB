#include <algorithm>
#include <iostream>
#include <thread>

#include "LDBCReader.h"
#include "LocksManager.h"
#include "LdbcConcurrentQuery.cpp"
#include "LDBCRandomizer.h"

int main( int argc, char * argv[]) {

  typedef GraphType Graph;
  typedef LocksManager LockManagerType;
///
  typedef std::thread thread;
  typedef PropertyList<FixedString, FixedString> PropertyListType;
  typedef std::pair<std::string, std::string> VertexPairType;
  typedef std::pair<std::string, PropertyListType> EdgePairType;
  typedef std::map<VertexPairType, EdgePairType> BranchMapType;

  unsigned int opt = 0;
  if ( (argc < 2) ) {
    std::cout <<"Need one more args \n 1: single-thread\t 2: multithread \n";
    exit(0);
  } else {
    opt = std::stoi(argv[1]);
    if ( (opt != 1) && (opt!=2) && (opt != 3)) {
      std::cout <<"wrong arg. \n 1: single-thread\t 2: multithread\t 3: multithread with skipping \n";
      exit(0);
    }
  }

  Graph g;
  LDBCReader reader(g);

#ifdef _FIXALLOC_
  g.allocVertexMemory(400000);
  g.allocEdgeMemory(2000000);
#endif /* _FIXALLOC */

  reader.readDirectory("../tests/ldbc/social_network_1000/New");
  cout << "Finish reading \n";

  unsigned int interval = 14;
  if (argc > 2) {
    if ( std::stoi(argv[2]) > 14) {
      interval = std::stoi(argv[2]);
    }
  }
  std::cout <<"interval:  " << interval << std::endl;

  LDBCRandomizer rand;
  std::vector<unsigned int> Input;
  rand.getFileName("../tests/ldbc/social_network_1000/New/person_0.csv");
  rand.getIntervalPersonInput(interval, Input);

  LockManagerType LockManager(g);
  LockManager.buildLockMap(g);

  cout <<"Begin testing \n";

  LdbcQuery1 q1(1);
  q1.setParam("firstName", "Hermann");

  LdbcQuery2 q2(2);
  q2.setRange("creationDate", "", "2012-07-16T10:00:00.255"); 

  LdbcQuery3 q3(3);
  q3.setPropertyRange("name", "Austria", "Angola"); 
  q3.setRange("creationDate", "2010-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 

  LdbcQuery4 q4(4);
  q4.setRange("creationDate", "2010-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 

  LdbcQuery5 q5(5);
  q5.setRange("joinDate", "2012-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 

  LdbcQuery6 q6(6);
  q6.setParam("name", "Peter_Pan");

  LdbcQuery7 q7(7);

  LdbcQuery8 q8(8);

  LdbcQuery9 q9(9);
  q9.setRange("creationDate", "", "2011-07-16T23:59:00.255"); 

  LdbcQuery10 q10(10);
  q10.setRange("birthday", "0000-04-21", "0000-05-22");

  LdbcQuery11 q11(11);
  q11.setRange("workFrom", "", "2014");
  q11.setParam("name", "China");

  LdbcQuery12 q12(12);
  q12.setParam("name", "Person");

  LdbcQuery13 q13(13);

  LdbcQuery14 q14(14);

///write operations 

  TransactionManager transManager;
  
  Query17 Q15(15);
  PropertyListType VertexProp; 
  VertexProp.set("firstName", "A");
  VertexProp.set("id", "1234567");

  PropertyListType EdgeProp[4]; 
  EdgeProp[0].set("null", "null");
  EdgeProp[1].set("null", "null");
  EdgeProp[2].set("classYear", "2010");
  EdgeProp[3].set("workFrom", "2014");

  EdgePairType NewEdge[4];
  NewEdge[0] = make_pair("PERSON_IS_LOCATED_IN", EdgeProp[0]);
  NewEdge[1] = make_pair("HAS_INTEREST", EdgeProp[1]);
  NewEdge[2] = make_pair("STUDY_AT", EdgeProp[2]);
  NewEdge[3] = make_pair("WORKS_AT", EdgeProp[3]);

  BranchMapType NewBranch= {
    {make_pair("PLACE", "1351"),  NewEdge[0]},
    {make_pair("TAG", "10"), NewEdge[1]}, 
    {make_pair("ORGANISATION", "7878"), NewEdge[2]}, 
    {make_pair("ORGANISATION", "787"), NewEdge[3]}
  };
  Q15.setPropertyList(VertexProp);
  Q15.setBranchMap(NewBranch);


  ///add a comment to network
  ///*****************query 16**********************/
  Query17 Q16(16);

  PropertyListType VertexProp2; 
  VertexProp2.set("id", "1234567");
  VertexProp2.set("creationDate", "2012-05-31T01:34:31.599+0000");

  PropertyListType EdgeProp2[4]; 
  EdgeProp2[0].set("null", "null");
  EdgeProp2[1].set("null", "null");
  EdgeProp2[2].set("null", "null");
  EdgeProp2[3].set("null", "null");

  EdgePairType NewEdge2[4];
  NewEdge2[0] = make_pair("COMMENT_HAS_CREATOR", EdgeProp[0]);
  NewEdge2[1] = make_pair("COMMENT_IS_LOCATED_IN", EdgeProp[1]);
  NewEdge2[2] = make_pair("REPLY_OF_COMMENT", EdgeProp[2]);
  NewEdge2[3] = make_pair("REPLY_OF_POST", EdgeProp[3]);

  BranchMapType NewBranch2 = {
    {make_pair("PERSON", "420"),  NewEdge2[1]},
    {make_pair("PLACE", "32"),  NewEdge2[1]},
    {make_pair("COMMENT", "137438953481"), NewEdge2[2]}, 
    {make_pair("POST", "137438953524"), NewEdge2[3]}
  };
  Q16.setPropertyList(VertexProp2);
  Q16.setBranchMap(NewBranch2);


  ///add a person to network
  //***************query 15*******************/
#ifdef _SKIP_
  Query18 Q17(17);

  PropertyListType VertexProp3; 
  VertexProp3.set("firstName", "A");
  VertexProp3.set("id", "1234567");


  PropertyListType EdgeProp3[4]; 
  EdgeProp3[0].set("null", "null");
  EdgeProp3[1].set("null", "null");
  EdgeProp3[2].set("classYear", "2010");
  EdgeProp3[3].set("workFrom", "2014");

  EdgePairType NewEdge3[4];
  NewEdge3[0] = make_pair("PERSON_IS_LOCATED_IN", EdgeProp3[0]);
  NewEdge3[1] = make_pair("HAS_INTEREST", EdgeProp3[1]);
  NewEdge3[2] = make_pair("STUDY_AT", EdgeProp3[2]);
  NewEdge3[3] = make_pair("WORKS_AT", EdgeProp3[3]);

  BranchMapType NewBranch3 = {
    {make_pair("PLACE", "1351"),  NewEdge3[0]},
    {make_pair("TAG", "10"), NewEdge3[1]}, 
    {make_pair("ORGANISATION", "7878"), NewEdge3[2]}, 
    {make_pair("ORGANISATION", "787"), NewEdge3[3]}
  };
  Q17.setPropertyList(VertexProp3);
  Q17.setBranchMap(NewBranch3);

  ///add a comment to network
  ///*****************query 16**********************/
  Query18 Q18(18);

  PropertyListType VertexProp4; 
  VertexProp4.set("id", "1234567");
  VertexProp4.set("creationDate", "2012-05-31T01:34:31.599+0000");

  PropertyListType EdgeProp4[4]; 
  EdgeProp4[0].set("null", "null");
  EdgeProp4[1].set("null", "null");
  EdgeProp4[2].set("null", "null");
  EdgeProp4[3].set("null", "null");

  EdgePairType NewEdge4[4];
  NewEdge4[0] = make_pair("COMMENT_HAS_CREATOR", EdgeProp4[0]);
  NewEdge4[1] = make_pair("COMMENT_IS_LOCATED_IN", EdgeProp4[1]);
  NewEdge4[2] = make_pair("REPLY_OF_COMMENT", EdgeProp4[2]);
  NewEdge4[3] = make_pair("REPLY_OF_POST", EdgeProp4[3]);

  BranchMapType NewBranch4 = {
    {make_pair("PERSON", "420"),  NewEdge4[1]},
    {make_pair("PLACE", "32"),  NewEdge4[1]},
    {make_pair("COMMENT", "137438953481"), NewEdge4[2]}, 
    {make_pair("POST", "137438953524"), NewEdge4[3]}
  };

  Q18.setPropertyList(VertexProp4);
  Q18.setBranchMap(NewBranch4);
#endif

  switch (opt){ 
    case 1:  
    {
        q1.runQuery(g, Input[0], LockManager);
        q2.runQuery(g, Input[1], LockManager);
        q3.runQuery(g, Input[2], LockManager);
        q4.runQuery(g, Input[3], LockManager);
        q5.runQuery(g, Input[4], LockManager);
        q6.runQuery(g, Input[5], LockManager);
        q7.runQuery(g, Input[6], LockManager);
        q8.runQuery(g, Input[7], LockManager);
        q9.runQuery(g, Input[8], LockManager);
        q10.runQuery(g, Input[9], LockManager);
        q11.runQuery(g, Input[10], LockManager);
        q12.runQuery(g, Input[11], LockManager);
        q13.runQuery(g, Input[12], Input[0], LockManager);
        q14.runQuery(g, Input[13], Input[1], LockManager);

        Q15.runQuery(g, transManager, LockManager, 1);
        Q16.runQuery(g, transManager, LockManager, 1);
      break;
    }
    case 2:
    {
      vector<thread> threads;
        threads.push_back( thread([&]{q1.runQuery(g, Input[0], LockManager);}));
        threads.push_back( thread([&]{q2.runQuery(g, Input[1], LockManager);}));
        threads.push_back( thread([&]{q3.runQuery(g, Input[2], LockManager);}));
        threads.push_back( thread([&]{q4.runQuery(g, Input[3], LockManager);}));
        threads.push_back( thread([&]{q5.runQuery(g, Input[4], LockManager);}));
        threads.push_back( thread([&]{q6.runQuery(g, Input[5], LockManager);}));
        threads.push_back( thread([&]{q7.runQuery(g, Input[6], LockManager);}));
        threads.push_back( thread([&]{q8.runQuery(g, Input[7], LockManager);}));
        threads.push_back( thread([&]{q9.runQuery(g, Input[8], LockManager);}));
        threads.push_back( thread([&]{q10.runQuery(g, Input[9], LockManager);}));
        threads.push_back( thread([&]{q11.runQuery(g, Input[10], LockManager);}));
        threads.push_back( thread([&]{q12.runQuery(g, Input[11], LockManager);}));
        threads.push_back( thread([&]{q13.runQuery(g, Input[12], Input[0], LockManager);}));
        threads.push_back( thread([&]{q14.runQuery(g, Input[13], Input[1], LockManager);}));

        threads.push_back( thread([&]{Q15.runQuery(g, transManager, LockManager, 1);}));
        threads.push_back( thread([&]{Q16.runQuery(g, transManager, LockManager, 1);}));
   
      for_each(threads.begin(), threads.end(),
               std::mem_fn(&thread::join));

      break;
    }
 #ifdef _SKIP_
    case 3:
    {
      vector<thread> threads;
        threads.push_back( thread([&]{q1.runQuery(g, Input[0], LockManager);}));
        threads.push_back( thread([&]{q2.runQuery(g, Input[1], LockManager);}));
        threads.push_back( thread([&]{q3.runQuery(g, Input[2], LockManager);}));
        threads.push_back( thread([&]{q4.runQuery(g, Input[3], LockManager);}));
        threads.push_back( thread([&]{q5.runQuery(g, Input[4], LockManager);}));
        threads.push_back( thread([&]{q6.runQuery(g, Input[5], LockManager);}));
        threads.push_back( thread([&]{q7.runQuery(g, Input[6], LockManager);}));
        threads.push_back( thread([&]{q8.runQuery(g, Input[7], LockManager);}));
        threads.push_back( thread([&]{q9.runQuery(g, Input[8], LockManager);}));
        threads.push_back( thread([&]{q10.runQuery(g, Input[9], LockManager);}));
        threads.push_back( thread([&]{q11.runQuery(g, Input[10], LockManager);}));
        threads.push_back( thread([&]{q12.runQuery(g, Input[11], LockManager);}));
        threads.push_back( thread([&]{q13.runQuery(g, Input[12], Input[0], LockManager);}));
        threads.push_back( thread([&]{q14.runQuery(g, Input[13], Input[1], LockManager);}));

        threads.push_back( thread([&]{Q17.runQuery(g, transManager, LockManager, 1);}));
        threads.push_back( thread([&]{Q18.runQuery(g, transManager, LockManager, 1);}));
   
      for_each(threads.begin(), threads.end(),
               std::mem_fn(&thread::join));
      break;
    }
#endif

    default:
      std::cout << "wrong arg\n";
      exit(0);
  }

  cout << "Finish testing\n";

}
