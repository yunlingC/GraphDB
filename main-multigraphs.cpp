#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <queue>
#include <thread>
#include <string>
#include <time.h>

#include "LinkedList.h"
#include "QueryDescription.h"
#include "QueryRandomizer.h"

int main(int argc, char *argv[]) {
  
  typedef GraphType Graph;
  typedef Graph::VertexDescriptor VertexDescriptor;

  unsigned int InputSize;
  if ( argc < 2) {
    std::cout <<"Need one more arg: 1000 or 5000\n";
    exit(1);
  } else {
    InputSize = std::stoi(argv[1]);
    if ( (InputSize != 1000) && (InputSize != 5000) ) {
      std::cout <<"Arg error : should be: 1000 or 5000\n";
      exit(1);
    }
  }
 
  Graph g[10];
  cout << "Begin testing\n";
  
#ifdef _FIXALLOC_
  for ( auto i = 0; i < 10; ++i ) {
    std::cout <<"Allocate space for graph " << i << "\n";
    g[i].allocVertexMemory(InputSize*0.11);
    g[i].allocEdgeMemory(InputSize);
  }
#endif /* _FIXALLOC */
 
  unsigned int size = InputSize/10;
  std::string FileName = "";
  for ( auto i = 0; i < 9; ++i ) {
    FileName = "../tests/gd/sndata" + std::to_string(size) + "_" + std::to_string(i+1) + ".gd";
    std::cout << "Read File " << FileName << "\n";
    GDReader reader(g[i]);
    reader.readFile(FileName.c_str());
  }

  std::cout <<"Allocate space for graph " << 10 << "\n";
  FileName = "../tests/gd/sndata" + std::to_string(size) + "_10.gd";
  GDReader reader(g[9]);
  reader.readFile(FileName.c_str());


  cout << "Finish reading\n";

  QueryRandomizer Rander(reader);

  string name, pid;
  VertexDescriptor webId, personId1, personId2, personId3;

  name = "";
  pid = "";
  webId = 0;
  personId1 = 0;
  personId2 = 0;
  personId3 = 0;

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

  int InputId[19] = {16, 0, 0, 28, 0, 
                        0, 0, 0, 54, 0, 
                        95, 0, 42, 0, 60, 
                        0, 0, 50, 11          //15-18
                      };




  name = Rander.getName(InputId[0]);
  Q1.setPersonProperty("name", name);

  webId = Rander.getAWebpageIndex(InputId[1]);
  Q2.setWebId(webId);

  personId1 = Rander.getAPersonIndex(InputId[2]);
  Q3.setPersonId(personId1);

  pid  = Rander.getPid(InputId[3]);
  Q4.setPersonProperty("pid", pid);

  personId1 = Rander.getAPersonIndex(InputId[4]);
  Q5.setPersonId(personId1);

  personId1 = Rander.getAPersonIndex(InputId[5]);
  Q6.setPersonId(personId1);

  personId1 = Rander.getAPersonIndex(InputId[6]);
  Q7.setPersonId(personId1);

  personId3 = Rander.getAPersonIndex(InputId[7]);
  personId2 = Rander.getAPersonIndex(InputId[8]);
  Q8.setEndPoints(personId2, personId3);

  personId2 = Rander.getAPersonIndex(InputId[9]);
  personId3 = Rander.getAPersonIndex(InputId[10]);
  Q9.setEndPoints(personId2, personId3);

  personId2 = Rander.getAPersonIndex(InputId[11]);
  personId3 = Rander.getAPersonIndex(InputId[12]);
  Q10.setEndPoints(personId2, personId3);

  personId2 = Rander.getAPersonIndex(InputId[13]);
  personId3 = Rander.getAPersonIndex(InputId[14]);
  Q11.setEndPoints(personId2, personId3);

  personId1 = Rander.getAPersonIndex(InputId[15]);
  Q12.setPersonId(personId1);

  personId1 = Rander.getAPersonIndex(InputId[16]);
  Q13.setPersonId(personId1);

  name = Rander.getName(InputId[17]);
  Q14.setPersonProperty("name", name);

  std::cout << "begin running\n";

  vector<thread> threads;

  threads.push_back( thread([&]{Q1.runQuery(g[0], 1);}));
  threads.push_back( thread([&]{Q2.runQuery(g[0], 1);}));
  threads.push_back( thread([&]{Q3.runQuery(g[1], 1);}));
  threads.push_back( thread([&]{Q4.runQuery(g[1], 1);}));
  threads.push_back( thread([&]{Q5.runQuery(g[2], 1);}));
  threads.push_back( thread([&]{Q6.runQuery(g[3], 1);}));
  threads.push_back( thread([&]{Q7.runQuery(g[4], 1);}));
  threads.push_back( thread([&]{Q8.runQuery(g[5], 1);}));
  threads.push_back( thread([&]{Q9.runQuery(g[6], 1);}));
  threads.push_back( thread([&]{Q10.runQuery(g[7], 1);}));
  threads.push_back( thread([&]{Q11.runQuery(g[8], 1);}));
  threads.push_back( thread([&]{Q12.runQuery(g[9], 1);}));
  threads.push_back( thread([&]{Q13.runQuery(g[5], 1);}));
  threads.push_back( thread([&]{Q14.runQuery(g[8], 1);}));

  for_each(threads.begin(), threads.end(),
           std::mem_fn(&thread::join));

  cout << "finish testing\n";
  myfile.close();
  return 0;
}
