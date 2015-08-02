#include <algorithm>
#include <iostream>
#include <thread>

#include "LDBCReader.h"
//#include "LinkedList.h"
//#include "LdbcQueryDescription.h"
#include "LocksManager.h"
//#include "LdbcUpdateDescription.h"
#include "LdbcConcurrentQuery.h"

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
  if ( (argc < 2) || (argc > 2)) {
    std::cout <<"Need one more args \n 1: single-thread\t 2: multithread \n";
    exit(0);
  } else {
    opt = std::stoi(argv[1]);
    if ( (opt != 1) && (opt!=2)) {
      std::cout <<"wrong arg. \n 1: single-thread\t 2: multithread \n";
      exit(0);
    }
  }

  Graph g;
  LDBCReader reader(g);
  reader.readDirectory("../tests/ldbc/social_network_700/New");
  cout << "Finish reading \n";

  LockManagerType LockManager;
  LockManager.buildLockMap(g);
  cout << "LockMap built\n";


  cout <<"Begin testing \n";

  LdbcQuery1 q1;
  q1.setParam("firstName", "Brian");
//  q1.runQuery(g, 157337, LockManager);

  LdbcQuery2 q2;
  q2.setRange("creationDate", "", "2012-07-16T10:00:00.255"); 
//  q2.runQuery(g, 157337, LockManager);

  LdbcQuery3 q3;
  q3.setPropertyRange("name", "Austria", "Angola"); 
  q3.setRange("creationDate", "2010-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 
//  q3.runQuery(g, 157337, LockManager);

  LdbcQuery4 q4;
  q4.setRange("creationDate", "2010-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 
//  q4.runQuery(g, 157337, LockManager);

  LdbcQuery5 q5;
  q5.setRange("joinDate", "2012-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 
//  q5.runQuery(g, 157337, LockManager);

  LdbcQuery6 q6;
  q6.setParam("name", "Peter_Pan");
//  q6.runQuery(g, 157337, LockManager);

  LdbcQuery7 q7;
//  q7.runQuery(g, 157337, LockManager);

  LdbcQuery8 q8;
//  q8.runQuery(g, 157337, LockManager);

  LdbcQuery9 q9;
  q9.setRange("creationDate", "", "2011-07-16T23:59:00.255"); 
//  q9.runQuery(g, 157337, LockManager);

  LdbcQuery10 q10;
  q10.setRange("birthday", "0000-04-21", "0000-05-22");
//  q10.runQuery(g, 157337, LockManager);

  LdbcQuery11 q11;
  q11.setRange("workFrom", "", "2014");
  q11.setParam("name", "China");
//  q11.runQuery(g, 157337, LockManager);

  LdbcQuery12 q12;
  q12.setParam("name", "Person");
//  q12.runQuery(g, 157337, LockManager);

  LdbcQuery13 q13;
//  q13.runQuery(g, 157337, 31366, LockManager);

  LdbcQuery14 q14;
//  q14.runQuery(g, 157337, 31408, LockManager);

///write operations 
  TransactionManager transManager;
  
  ///add a person to network
  //***************query 15*******************/
  Query17 Q15;
  {
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
  }
//  Q17.runQuery(g, transManager, LockManager, 1);

  ///add a comment to network
  ///*****************query 16**********************/
  Query17 Q16;
  {
  PropertyListType VertexProp; 
  VertexProp.set("id", "1234567");
  VertexProp.set("creationDate", "2012-05-31T01:34:31.599+0000");

  PropertyListType EdgeProp[4]; 
  EdgeProp[0].set("null", "null");
  EdgeProp[1].set("null", "null");
  EdgeProp[2].set("null", "null");
  EdgeProp[3].set("null", "null");

  EdgePairType NewEdge[4];
  NewEdge[0] = make_pair("COMMENT_HAS_CREATOR", EdgeProp[0]);
  NewEdge[1] = make_pair("COMMENT_IS_LOCATED_IN", EdgeProp[1]);
  NewEdge[2] = make_pair("REPLY_OF_COMMENT", EdgeProp[2]);
  NewEdge[3] = make_pair("REPLY_OF_POST", EdgeProp[3]);

  BranchMapType NewBranch= {
    {make_pair("PERSON", "420"),  NewEdge[1]},
    {make_pair("PLACE", "32"),  NewEdge[1]},
    {make_pair("COMMENT", "103079215143"), NewEdge[2]}, 
    {make_pair("POST", "119"), NewEdge[3]}
  };
  Q16.setPropertyList(VertexProp);
  Q16.setBranchMap(NewBranch);
  }

  switch (opt){ 
    case 1:  
    {
      q1.runQuery(g, 157337, LockManager);
      q2.runQuery(g, 157387, LockManager);
      q3.runQuery(g, 157437, LockManager);
      q4.runQuery(g, 157487, LockManager);
      q5.runQuery(g, 157537, LockManager);
      q6.runQuery(g, 157587, LockManager);
      q7.runQuery(g, 157637, LockManager);
      q8.runQuery(g, 157687, LockManager);
      q9.runQuery(g, 157737, LockManager);
      q10.runQuery(g, 157787, LockManager);
      q11.runQuery(g, 157837, LockManager);
      q12.runQuery(g, 157887, LockManager);
      q13.runQuery(g, 157937, 157366, LockManager);
      q14.runQuery(g, 157987, 157663, LockManager);
      Q15.runQuery(g, transManager, LockManager, 1);
      Q16.runQuery(g, transManager, LockManager, 1);
      break;
    }
    case 2:
    {
      vector<thread> threads;
      threads.push_back( thread([&]{q1.runQuery(g, 157337, LockManager);}));
      threads.push_back( thread([&]{q2.runQuery(g, 157387, LockManager);}));
      threads.push_back( thread([&]{q3.runQuery(g, 157437, LockManager);}));
      threads.push_back( thread([&]{q4.runQuery(g, 157487, LockManager);}));
      threads.push_back( thread([&]{q5.runQuery(g, 157537, LockManager);}));
      threads.push_back( thread([&]{q6.runQuery(g, 157587, LockManager);}));
      threads.push_back( thread([&]{q7.runQuery(g, 157637, LockManager);}));
      threads.push_back( thread([&]{q8.runQuery(g, 157687, LockManager);}));
      threads.push_back( thread([&]{q9.runQuery(g, 157737, LockManager);}));
      threads.push_back( thread([&]{q10.runQuery(g, 157787, LockManager);}));
      threads.push_back( thread([&]{q11.runQuery(g, 157837, LockManager);}));
      threads.push_back( thread([&]{q12.runQuery(g, 157887, LockManager);}));
      threads.push_back( thread([&]{q13.runQuery(g, 157937, 157366, LockManager);}));
      threads.push_back( thread([&]{q14.runQuery(g, 157987, 157663, LockManager);}));
      threads.push_back( thread([&]{Q15.runQuery(g, transManager, LockManager, 1);}));
      threads.push_back( thread([&]{Q16.runQuery(g, transManager, LockManager, 1);}));
    
      for_each(threads.begin(), threads.end(),
               std::mem_fn(&thread::join));
      break;
    }

    default:
      std::cout << "wrong arg\n";
      exit(0);
  }


  LdbcFile.close();

  cout << "Finish testing\n ";

}
