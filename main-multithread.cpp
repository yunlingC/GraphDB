#include "global.h"
#include "GDBReader.h"
#include "QueryDescription.h"
#include "QueryRandomizer.h"
#include "helper.h"

#include <stdlib.h>
#include <algorithm>
#include <queue>
#include <thread>
#include <string>

#include <iostream>
//#include <time.h>

using namespace std;

int main(int argc, char *argv[]) {
  
  typedef GraphType Graph;
  typedef Graph::VertexDescriptor VertexDescriptor;

  Graph g;
//  cout << "Begin testing\n";

  ///arg: 0-main-multithread 1- filesize 2-traversal 3-Q1_startVertex 4-Q2_startVertex

  if ( argc < 7) {
    std::cout <<"Need one more arg: 1000 or 5000 or 10000 or 100000\n";
    exit(1);
  } 
//    if ( (InputSize != 1000) && (InputSize != 5000) ) {
//      std::cout <<"Arg error : should be: 1000 or 5000\n";
//      exit(1);
//    }
#ifdef _FIXALLOC_
  g.allocVertexMemory(InputSize*1.1);
  g.allocEdgeMemory(InputSize*20);
#endif /* _FIXALLOC */

  unsigned int InputSize = std::stoi(argv[1]);
 
  GDBReader reader(g);
  std::string FileName = "../tests/gd/sndata" +std::to_string(InputSize)+".gd";
//  std::cout <<"Read file name " << FileName <<"\n"; 
  reader.readFile(FileName);

//  cout << "Finish reading\n";

  QueryRandomizer rander(reader);

  string name, pid;
  VertexDescriptor webId, personId1, personId2, personId3;

//  std::cout <<"Arguments:\t" << argv[1] 
//            << "\t" << argv[2]
//            << "\t" << argv[3]
//            << "\t" << argv[4]
//            << "\t" << argv[5]
//            << "\t" << argv[6]
//            << "\t" << argv[7]
//            << "\n";

//  cout <<"number of vertices\n" << g.getAllVertices().size() << "\n";

  int traversal = std::stoi(argv[2]);
  
  int run = std::stoi(argv[3]);

  auto id = 1;
  auto webid = 1;
  auto pathid = 1;
  auto friendid = 1;
//  auto id4 = 1;
//  auto id5 = 1;
//    name = rander.getName(id);
//    pid  = rander.getPid(id);
//    webId = rander.getAWebpageIndex(id);
//    personId1 = rander.getAPersonIndex(id);
//    personId2 = rander.getAPersonIndex(id);
//    personId3 = 0;

    Query1 Q1(1);
    Query2 Q2(2);
    Query3 Q3(3);
    Query4 Q4(4);
    Query5 Q5(5);
    Query6 Q6(6);
    Query7 Q7(7);
    Query8 Q8(8);
    Query9 Q9(9);
    Query10 Q10(10);
    Query11 Q11(11);
    Query12 Q12(12);
    Query13 Q13(13);
    Query14 Q14(14);

    
//    int InputId[19] = {46, 0, 0, 28, 0, 
//                        0, 0, 0, 54, 0, 
//                        95, 0, 42, 0, 60, 
//                        0, 0, 50, 11          //15-18
//                      };

    id = std::stoi(argv[4]);
    webid = std::stoi(argv[5]);
    pathid = std::stoi(argv[6]);
    friendid = std::stoi(argv[7]);
//    for (int i = 0; i < 18; i++) {
//      InputId[i] = std::stoi(argv[i+2]);
//    }
//
    //	id = InputId[0];
    name = rander.getName(id);
    Q1.setPersonProperty("name", name);

    //	id = InputId[1];
    webId = rander.getAWebpageIndex(webid);
    Q2.setWebId(webId);

    //	id = InputId[2];
    personId1 = rander.getAPersonIndex(id);
    Q3.setPersonId(personId1);

    //	id = InputId[3];
    pid  = rander.getPid(id);
    Q4.setPersonProperty("pid", pid);

    //	id = InputId[4];
    personId1 = rander.getAPersonIndex(id);
    Q5.setPersonId(personId1);

    //	id = InputId[5];
    personId1 = rander.getAPersonIndex(id);
    Q6.setPersonId(personId1);

    //	id = InputId[6];
    personId1 = rander.getAPersonIndex(id);
    Q7.setPersonId(personId1);

    //	id = InputId[7];
    personId3 = rander.getAPersonIndex(id);
    //	id = InputId[8];
    personId2 = rander.getAPersonIndex(pathid);
    Q8.setEndPoints(personId2, personId3);

    //	id = InputId[9];
    personId2 = rander.getAPersonIndex(id);
    //	id = InputId[10];
    personId3 = rander.getAPersonIndex(pathid);
    Q9.setEndPoints(personId2, personId3);

    //	id = InputId[11];
    personId2 = rander.getAPersonIndex(id);
    //	id = InputId[12];
    personId3 = rander.getAPersonIndex(friendid);
    Q10.setEndPoints(personId2, personId3);

    //	id = InputId[13];
    personId2 = rander.getAPersonIndex(id);
    //	id = InputId[14];
    personId3 = rander.getAPersonIndex(friendid);
    Q11.setEndPoints(personId2, personId3);

    //	id = InputId[15];
    personId1 = rander.getAPersonIndex(id);
    Q12.setPersonId(personId1);

    //	id = InputId[16];
    personId1 = rander.getAPersonIndex(id);
    Q13.setPersonId(personId1);

    //	id = InputId[17];
    name = rander.getName(id);
    Q14.setPersonProperty("name", name);

    auto startTime = get_clock();

    for (int i = 0; i < 40; i++) {
    if (run == 1) {
      Q1.runQuery(g, traversal);
  
  //    auto Time1 = get_clock();
  //    cout << "Processing time Q1\t" << diff_clock(startTime, Time1) << "\n";
  
      Q2.runQuery(g, traversal);
  
  //    auto Time2 = get_clock();
  //    cout << "Processing time Q2\t" << diff_clock(Time1, Time2) << "\n";
  
      Q3.runQuery(g, traversal);
  
  //    auto Time3 = get_clock();
  //    cout << "Processing time Q3\t" << diff_clock(Time2, Time3) << "\n";
  
      Q4.runQuery(g, traversal);
  
  //    auto Time4 = get_clock();
  //    cout << "Processing time Q4\t" << diff_clock(Time3, Time4) << "\n";
  
      Q5.runQuery(g, traversal);
      Q6.runQuery(g, traversal);
  
  //    auto Time6 = get_clock();
  //    cout << "Processing time Q6\t" << diff_clock(Time4, Time6) << "\n";
  
      Q7.runQuery(g, traversal);
  
      Q8.runQuery(g, traversal);

      Q9.runQuery(g, traversal);
  
  //    auto Time8 = get_clock();
  //    cout << "Processing time Q8\t" << diff_clock(Time6, Time8) << "\n";
  
      Q10.runQuery(g, traversal);
      Q11.runQuery(g, traversal);
      Q12.runQuery(g, traversal);
  
  //    auto Time12 = get_clock();
  //    cout << "Processing time Q12\t" << diff_clock(Time8, Time12) << "\n";
  
      Q13.runQuery(g, traversal);
  
  //    auto Time13 = get_clock();
  //    cout << "Processing time Q13\t" << diff_clock(Time12, Time13) << "\n";
  
      Q14.runQuery(g, traversal);
  
  //    auto Time14 = get_clock();
  //    cout << "Processing time Q14\t" << diff_clock(Time8, Time14) << "\n";
  

    }
    else if (run == 2) {
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

    }
    else {
      cout << "Error: Not single thread or multithread\n";
      exit(0);
    }
    }

  auto endTime = get_clock();

  cout << "Processing time " << diff_clock(startTime, endTime) << "\n";

//  cout << "finish testing\n";
  return 0;
}
