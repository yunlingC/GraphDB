#include <algorithm>
#include <iostream>
#include <thread>

#include "LDBCReader.h"
#include "LocksManager.h"
#include "LdbcConcurrentQuery.cpp"
#include "LDBCRandomizer.h"

using namespace std;

int main( int argc, char * argv[]) {

  typedef GraphType Graph;
  typedef LocksManager LockManagerType;
//  typedef std::thread thread;
  typedef pthread_t thread;
  typedef PropertyList<FixedString, FixedString> PropertyListType;
  typedef std::pair<std::string, std::string> VertexPairType;
  typedef std::pair<std::string, PropertyListType> EdgePairType;
  typedef std::map<VertexPairType, EdgePairType> BranchMapType;
  typedef LdbcQuery* LdbcQueryPtr;

  unsigned int opt = 0;
  if ( (argc < 2) ) {
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

#ifdef _FIXALLOC_
  g.allocVertexMemory(40000);
  g.allocEdgeMemory(80000);
#endif /* _FIXALLOC */

  //reader.readDirectory("../tests/ldbc/social_network_100/New");
  reader.readDirectory("/home/y49cui/research/graph/ldbc-graphdb/graphdb/tests/ldbc/social_network_100/New");
  cout << "Finish reading \n";

  unsigned int interval = 14;
  if (argc > 2) {
    if ( std::stoi(argv[2]) > 14) {
      interval = std::stoi(argv[2]);
    }
  }

  LDBCRandomizer rand;
  std::vector<unsigned int> Input;
  rand.getFileName("../tests/ldbc/social_network_100/New/person_0.csv");
  rand.getIntervalPersonInput(interval, Input);

  LockManagerType LockManager(g);
  LockManager.buildLockMap(g);

  cout <<"Begin testing \n";

  std::vector<LdbcQueryPtr> Queries;

  LdbcQuery1 q1(1);
  q1.setParam("gender", "male");
  Queries.push_back(&q1);

  LdbcQuery2 q2(2);
  q2.setRange("creationDate", "", "2012-07-16T10:00:00.255"); 
  Queries.push_back(&q2);


  LdbcQuery3 q3(3);
  q3.setPropertyRange("name", "Austria", "Angola"); 
  q3.setRange("creationDate", "2010-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 
  Queries.push_back(&q3);

  LdbcQuery4 q4(4);
  q4.setRange("creationDate", "2010-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 
  Queries.push_back(&q4);

  LdbcQuery5 q5(5);
  q5.setRange("joinDate", "2012-01-00T00:01:00.255", "2013-07-16T23:59:00.255"); 
  Queries.push_back(&q5);

  LdbcQuery6 q6(6);
  q6.setParam("name", "Peter_Pan");
  Queries.push_back(&q6);

  LdbcQuery7 q7(7);
  Queries.push_back(&q7);

  LdbcQuery8 q8(8);
  Queries.push_back(&q8);

  LdbcQuery9 q9(9);
  q9.setRange("creationDate", "", "2011-07-16T23:59:00.255"); 
  Queries.push_back(&q9);

  LdbcQuery10 q10(10);
  q10.setRange("birthday", "0000-04-21", "0000-05-22");
  Queries.push_back(&q10);

  LdbcQuery11 q11(11);
  q11.setRange("workFrom", "", "2014");
  q11.setParam("name", "China");
  Queries.push_back(&q11);

  LdbcQuery12 q12(12);
  q12.setParam("name", "Person");
  Queries.push_back(&q12);

  LdbcQuery13 q13(13);
  Queries.push_back(&q13);

  LdbcQuery14 q14(14);
  Queries.push_back(&q14);

/////write operations 
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
  Queries.push_back(&Q15);


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
  Queries.push_back(&Q16);

  switch (opt){ 
    case 1:  
    {
        q1.runQuery(g, Input[0], LockManager);
        q2.runQuery(g, Input[0], LockManager);
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

        Q15.runQuery(g, LockManager);
        Q16.runQuery(g, LockManager);
      break;
    }

    case 2:
    {
      thread threads[16];
      QueryHelper *Helper[16];

      unsigned int ii = 0;

      for (auto it = Queries.begin(), it_end = Queries.end(); 
          (it != it_end)&&(ii <16); it++, ii++){
        Helper[ii] = new QueryHelper(g, Input[ii], Input[0], 
            LockManager, *(*it));
      }

      pthread_create(&threads[0], NULL, &LdbcQuery1::runQueries, Helper[0]);
      pthread_create(&threads[1], NULL, &LdbcQuery2::runQueries, Helper[1]);
      pthread_create(&threads[2], NULL, &LdbcQuery3::runQueries, Helper[2]);
      pthread_create(&threads[3], NULL, &LdbcQuery4::runQueries, Helper[3]);
      pthread_create(&threads[4], NULL, &LdbcQuery5::runQueries, Helper[4]);
      pthread_create(&threads[5], NULL, &LdbcQuery6::runQueries, Helper[5]);
      pthread_create(&threads[6], NULL, &LdbcQuery7::runQueries, Helper[6]);
      pthread_create(&threads[7], NULL, &LdbcQuery8::runQueries, Helper[7]);
      pthread_create(&threads[8], NULL, &LdbcQuery9::runQueries, Helper[8]);
      pthread_create(&threads[9], NULL, &LdbcQuery10::runQueries, Helper[9]);
      pthread_create(&threads[10], NULL, &LdbcQuery11::runQueries, Helper[10]);
      pthread_create(&threads[11], NULL, &LdbcQuery12::runQueries, Helper[11]);
      std::cout << "1-12 queriees done\n";
      pthread_create(&threads[12], NULL, &LdbcQuery13::runQueries, Helper[12]);
      pthread_create(&threads[13], NULL, &LdbcQuery14::runQueries, Helper[13]);
      std::cout << "13, 14 queriees done\n";
      pthread_create(&threads[14], NULL, &Query17::runQueries, Helper[13]);
      pthread_create(&threads[15], NULL, &Query17::runQueries, Helper[14]);
      std::cout << "15, 16 queriees done\n";
 
      
      for (auto i = 0; i < 16; i++) {
        pthread_join(threads[i], NULL);
      }
      std::cout << "pthread join done\n";

      for (auto i = 0; i < 16; i++) {
        delete Helper[i];
      }

      break;
    }

    default:
      std::cout << "wrong arg\n";
      exit(0);
  }

  cout << "Finish testing\n";

}
