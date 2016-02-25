#include "LDBCReader.h"
#include "LdbcConcurrentQuery.cpp"
#include "LDBCRandomizer.h"

#include <algorithm>
#include <iostream>
#include <thread>

using namespace std;

int main( int argc, char * argv[]) {

  typedef GraphType Graph;

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
 

  Graph g[14];
  
#ifdef _FIXALLOC_
  /// Allocate space for all the graphs

  char* NodeMemory = new char[sizeof(Vertex)*40000*14];
  char* EdgeMemory = new char[sizeof(Edge)*80000*14];

  std::cout << "Start of node block " << reinterpret_cast<int*>(NodeMemory)
            << " , Ending address " << reinterpret_cast<int*>(NodeMemory + sizeof(Vertex)*40000*14)
            << "\n";

  std::cout << "Start of edge block " << reinterpret_cast<int*>(EdgeMemory)
            << " , Ending address " << reinterpret_cast<int*>(EdgeMemory + sizeof(Edge)*80000*14)
            << "\n";

#endif

  for (auto i = 0; i < 14; i++) {

  LDBCReader reader(g[i]);

#ifdef _FIXALLOC_
  g[i].allocVertexMemory(NodeMemory, 40000);
  g[i].allocEdgeMemory(EdgeMemory, 80000);

  NodeMemory += sizeof(Vertex)*40000;
  EdgeMemory += sizeof(Edge)*80000;

#endif /* _FIXALLOC */

  reader.readDirectory("../tests/ldbc/social_network_100/New");

  }
  std::cout << "Finish reading \n";
/**
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


  cout <<"Begin testing \n";

  std::vector<LdbcQuery *> Queries;

  LdbcQuery1 q1(1);
  q1.setParam("firstName", "Hermann");
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

  switch (opt){ 
    case 1:  
    {
      for (unsigned int i = 0; i < 12; i++) {
        Queries[i]->runQuery(g[i], Input[i]);
      }
      q13.runQuery(g[12], Input[12], Input[0]);
      q14.runQuery(g[13], Input[13], Input[1]);

      break;
    }

    case 2:
    {
      vector<thread> threads;
        threads.push_back(thread([&]{q1.runQuery(g[0], Input[0]);}));
        threads.push_back(thread([&]{q2.runQuery(g[1], Input[1]);}));
        threads.push_back(thread([&]{q3.runQuery(g[2], Input[2]);}));
        threads.push_back(thread([&]{q4.runQuery(g[3], Input[3]);}));
        threads.push_back(thread([&]{q5.runQuery(g[4], Input[4]);}));
        threads.push_back(thread([&]{q6.runQuery(g[5], Input[5]);}));
        threads.push_back(thread([&]{q7.runQuery(g[6], Input[6]);}));
        threads.push_back(thread([&]{q8.runQuery(g[7], Input[7]);}));
        threads.push_back(thread([&]{q9.runQuery(g[8], Input[8]);}));
        threads.push_back(thread([&]{q10.runQuery(g[9], Input[9]);}));
        threads.push_back(thread([&]{q11.runQuery(g[10], Input[10]);}));
        threads.push_back(thread([&]{q12.runQuery(g[11], Input[11]);}));
        threads.push_back(thread([&]{q13.runQuery(g[12], Input[12], Input[0]);}));
        threads.push_back(thread([&]{q14.runQuery(g[13], Input[13], Input[1]);}));

        for_each(threads.begin(), threads.end(),
               std::mem_fn(&thread::join));
 
        break;
    }

  }

  cout << "Finish testing\n";
*/

//#ifdef _FIXALLOC_
//  delete NodeMemory;
//  delete EdgeMemory;
//#endif
}
