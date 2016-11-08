#include "global.h"
#include "GDBReader.h"
#include "QueryDescription.h"
#include "QueryRandomizer.h"

#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <queue>
#include <thread>
#include <string>
#include <time.h>

#define SCALE 1000
#define NANO 1000000000

using namespace std;

uint64_t get_clock()  {
  timespec tp = (struct timespec) {0};
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp);
  uint64_t ret = tp.tv_sec * NANO + tp.tv_nsec;
  return ret;
}

uint64_t diff_clock(uint64_t  start, uint64_t end) {
  return (end - start) / SCALE;
}

int main(int argc, char *argv[]) {
  
  typedef GraphType Graph;
  typedef Graph::VertexDescriptor VertexDescriptor;

  Graph g;
  cout << "Begin testing\n";

  unsigned int InputSize;
  if ( argc < 2) {
    std::cout <<"Need one more arg: 1000 or 5000\n";
    exit(1);
  } else {
    InputSize = std::stoi(argv[1]);
//    if ( (InputSize != 1000) && (InputSize != 5000) ) {
//      std::cout <<"Arg error : should be: 1000 or 5000\n";
//      exit(1);
//    }
  }
  
#ifdef _FIXALLOC_
  g.allocVertexMemory(InputSize*1.1);
  g.allocEdgeMemory(InputSize*20);
#endif /* _FIXALLOC */
 
  GDBReader reader(g);
  std::string FileName = "../tests/gd/sndata" +std::to_string(InputSize)+".gd";
  std::cout <<"Read file name " << FileName <<"\n";
  reader.readFile(FileName);

  cout << "Finish reading\n";

  QueryRandomizer rander(reader);

  string name, pid;
  VertexDescriptor webId, personId1, personId2, personId3;

  int traversal = 1;

  auto id = 1;
//    name = rander.getName(id);
//    pid  = rander.getPid(id);
//    webId = rander.getAWebpageIndex(id);
//    personId1 = rander.getAPersonIndex(id);
//    personId2 = rander.getAPersonIndex(id);
//    personId3 = 0;

    Query1 Q1;
    Query2 Q2;
    Query3 Q3;
    Query4 Q4;
    Query5 Q5;
    Query6 Q6;
    Query7 Q7;
    Query8 Q8;
    Query9 Q9;
    Query10 Q10;
    Query11 Q11;
    Query12 Q12;
    Query13 Q13;
    Query14 Q14;

    
    int InputId[19] = {46, 0, 0, 28, 0, 
                        0, 0, 0, 54, 0, 
                        95, 0, 42, 0, 60, 
                        0, 0, 50, 11          //15-18
                      };


    id = InputId[0];
    name = rander.getName(id);
    Q1.setPersonProperty("name", name);

    id = InputId[1];
    webId = rander.getAWebpageIndex(id);
    Q2.setWebId(webId);

    id = InputId[2];
    personId1 = rander.getAPersonIndex(id);
    Q3.setPersonId(personId1);

    id = InputId[3];
    pid  = rander.getPid(id);
    Q4.setPersonProperty("pid", pid);

    id = InputId[4];
    personId1 = rander.getAPersonIndex(id);
    Q5.setPersonId(personId1);

    id = InputId[5];
    personId1 = rander.getAPersonIndex(id);
    Q6.setPersonId(personId1);

    id = InputId[6];
    personId1 = rander.getAPersonIndex(id);
    Q7.setPersonId(personId1);

    id = InputId[7];
    personId3 = rander.getAPersonIndex(id);
    id = InputId[8];
    personId2 = rander.getAPersonIndex(id);
    Q8.setEndPoints(personId2, personId3);

    id = InputId[9];
    personId2 = rander.getAPersonIndex(id);
    id = InputId[10];
    personId3 = rander.getAPersonIndex(id);
    Q9.setEndPoints(personId2, personId3);

    id = InputId[11];
    personId2 = rander.getAPersonIndex(id);
    id = InputId[12];
    personId3 = rander.getAPersonIndex(id);
    Q10.setEndPoints(personId2, personId3);

    id = InputId[13];
    personId2 = rander.getAPersonIndex(id);
    id = InputId[14];
    personId3 = rander.getAPersonIndex(id);
    Q11.setEndPoints(personId2, personId3);

    id = InputId[15];
    personId1 = rander.getAPersonIndex(id);
    Q12.setPersonId(personId1);

    id = InputId[16];
    personId1 = rander.getAPersonIndex(id);
    Q13.setPersonId(personId1);

    id = InputId[17];
    name = rander.getName(id);
    Q14.setPersonProperty("name", name);

    auto startTime = get_clock();

//    Q1.runQuery(g, 1);
//    Q2.runQuery(g, 1);
//    Q3.runQuery(g, 1);
//    Q4.runQuery(g, 1);
//    Q5.runQuery(g, 1);
//    Q6.runQuery(g, 1);
//    Q7.runQuery(g, 1);
//    Q8.runQuery(g, 1);
//    Q9.runQuery(g, 1);
//    Q10.runQuery(g, 1);
//    Q11.runQuery(g, 1);
//    Q12.runQuery(g, 1);
//    Q13.runQuery(g, 1);
//    Q14.runQuery(g, 1);

  vector<thread> threads;

  threads.push_back( thread([&]{Q1.runQuery(g, traversal );}));
  threads.push_back( thread([&]{Q2.runQuery(g, traversal );}));
  threads.push_back( thread([&]{Q3.runQuery(g, traversal );}));
  threads.push_back( thread([&]{Q4.runQuery(g, traversal );}));
  threads.push_back( thread([&]{Q5.runQuery(g, traversal );}));
  threads.push_back( thread([&]{Q6.runQuery(g, traversal );}));
  threads.push_back( thread([&]{Q7.runQuery(g, traversal );}));
  threads.push_back( thread([&]{Q8.runQuery(g, traversal );}));
  threads.push_back( thread([&]{Q9.runQuery(g, traversal );}));
  threads.push_back( thread([&]{Q10.runQuery(g, traversal);}));
  threads.push_back( thread([&]{Q11.runQuery(g, traversal);}));
  threads.push_back( thread([&]{Q12.runQuery(g, traversal);}));
  threads.push_back( thread([&]{Q13.runQuery(g, traversal);}));
  threads.push_back( thread([&]{Q14.runQuery(g, traversal);}));

  for_each(threads.begin(), threads.end(),
           std::mem_fn(&thread::join));

  auto endTime = get_clock();

  cout << "Processing time " << diff_clock(startTime, endTime) << "\n";

  cout << "finish testing\n";
///  myfile.close();
  return 0;
}
