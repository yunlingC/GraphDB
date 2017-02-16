#include "LinkedList.h"
#include "QueryDescription.h"
#include "QueryRandomizer.h"
#include "GraphType.h"
#include "NeoReader.h"
#include "helper.h"

#include <iostream>
#include <string>
#include <time.h>

using namespace std;

int main(int argc, char *argv[]) {

  /// argv[0] ./main-gdbench
  /// argv[1] query number 1:14
  /// argv[2] param1 0:499
  /// argv[3] param2 0:499 for shortes path and path only(Q8-Q12)
 
  typedef GraphType Graph;

  Graph g;
//  cout << "Begin testing\n";

#ifdef _FIXALLOC_
  g.allocVertexMemory(5000);
  g.allocEdgeMemory(40000);
#endif /* _FIXALLOC */

  std::cout << "Begin testing\n";

  GDBReader reader(g);
  reader.readFile("/home/y49cui/research/graph/gromit/gdbench/graphdb/tests/gd/sndata.gd");

  cout << "Done reading graph\n";

  NeoReader input;
  input.readFile("/home/y49cui/research/graph/gromit/gdbench/graphdb/readers/GDBenchQueryInput.csv");

  cout << "Done reading input\n";
//  cout << "begin randomizing\n";
  QueryRandomizer rander(reader);

//  string name[5], pid[5] ;
//  VertexDescriptor webId[5], personId1[5], personId2[5], personId3[5];
 
  ///not random now 
  /**
  for(auto i = 0; i < 1; i++) {
    name[i] = rander.getName();
    pid[i]  = rander.getPid();
    webId[i] = rander.getAWebpageIndex();
    personId1[i] = rander.getAPersonIndex();
    personId2[i] = rander.getAPersonIndex();
    personId3[i] = personId2[i];

    ///TODO maybe infinite loop !!
    while( personId3[i] == personId2[i])     //for query of connection with p1 and p2
    personId3[i] = rander.getAPersonIndex();
//    cout << name[i] << endl << pid[i] << endl << webId[i] << endl << personId1[i] << endl << personId2[i] << endl << personId3[i] << endl;
  }
*/


  if(argc < 1) {
    cout <<"Error: At least 1 arguments\n";
    return 1;
  } 

    string name, pid;
    GraphType::VertexDescriptor webId, personId1, personId2, personId3;

    auto id = 0; //atoi(argv[2]);
    if ((id < 0) || (id > 499)) {
      cout <<"Error: Out of range(0:499)\n";
      return 0;
    }

    name = rander.getName(id);
    pid  = rander.getPid(id);
    webId = rander.getAWebpageIndex(id);
    personId1 = rander.getAPersonIndex(id);
    personId2 = rander.getAPersonIndex(id);
    personId3 = 0;
    if(argc > 3) {
      personId3 = 10; //atoi(argv[3]);
    }

//    cout << "name\t" << name << endl 
//         << "pid\t" << pid << endl 
//         << "webId\t" << webId << endl 
//         << "personId1\t" <<  personId1 << endl 
//         << "personId\t" <<  personId2 << endl 
//         << "personId3\t" << personId3 << endl;

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

//  auto id = 0; 
/// only one set of input right now
    Q1.setPersonProperty("name", name);
    Q2.setWebId(webId);
    Q3.setPersonId(personId1);
    Q4.setPersonProperty("pid", pid);
    Q5.setPersonId(personId1);
    Q6.setPersonId(personId1);
    Q7.setPersonId(personId1);
    Q8.setEndPoints(personId2, personId3);
    Q9.setEndPoints(personId2, personId3);
    Q10.setEndPoints(personId2, personId3);
    Q11.setEndPoints(personId2, personId3);
    Q12.setPersonId(personId1);
    Q13.setPersonId(personId1);
    Q14.setPersonProperty("name", name);

    vector<Query*> QueryList;

    QueryList.push_back(&Q1);
    QueryList.push_back(&Q2);
    QueryList.push_back(&Q3);
    QueryList.push_back(&Q4);
    QueryList.push_back(&Q5);
    QueryList.push_back(&Q6);
    QueryList.push_back(&Q7);
    QueryList.push_back(&Q8);
    QueryList.push_back(&Q9);
    QueryList.push_back(&Q10);
    QueryList.push_back(&Q11);
    QueryList.push_back(&Q12);
    QueryList.push_back(&Q13);
    QueryList.push_back(&Q14);

    auto qid = 0; 
    if (argc > 1) {
      qid = atoi(argv[1]);
    }

    if((qid < 0) || (qid > 14)) {
      cout <<"Error: Out of range(1:14)\n";
      return 1;
    }

    if((qid > 7) && (qid < 12)) {
      if(personId3 < 0) {
        cout <<"Error: No end vertex id\n";
        return 1;
      }
    }

    cout <<"query number is " << qid << endl;

    auto query = QueryList[0];
    if (qid > 0) {
      query = QueryList[qid-1];
    }

    auto begin = get_clock();

    switch(qid) {
      case 1: 
        for (int i = 0; i < 100; i++) {
          name = input.getInput()[i];
          query->setPersonProperty("name", name);
          query->runQuery(g, 2);
        }
        break;

      case 2:
        for (int i = 100; i < 200; i++) {
          webId= std::stoi(input.getInput()[i]);
          query->setWebId(webId);
          query->runQuery(g, 2);
        }
        break;

      case 4:
        for (int i = 300; i < 400; i++) {
          pid= std::stoi(input.getInput()[i]);
          query->setPersonProperty("pid", pid);
          query->runQuery(g, 2);
        }
        break;

      case 3:
      case 5:
      case 6:
      case 7:
      case 12:
        for (int i = (qid-1)*100; i < qid*100; i++) {
          personId1 = std::stoi(input.getInput()[i]);
          query->setPersonId(personId1);
          query->runQuery(g, 2);
        }
        break;

      case 8:
      case 9:
      case 10:
      case 11:
  
      default:
        Q1.runQuery(g, 2);
    }

    auto end = get_clock();
    cout << "----processing time\t" << diff_clock(begin, end) << "\n";

    pthread_t threads[2];
    pthread_create(&threads[0], NULL, &runQuery, &g, 2);
    pthread_create(&threads[1], NULL, &runQuery, &g, 2);
    pthread_exit(NULL);
//  Q1.runQuery(g,  2);
//  Q2.runQuery(g,  1);
//  Q2.runQuery(g, 2);
//  Q3.runQuery(g, 1);
//  Q3.runQuery(g, 2);
//  Q4.runQuery(g, 1);
//  Q4.runQuery(g, 2);
//  Q5.runQuery(g, 1);
//  Q5.runQuery(g, 2);
//  Q6.runQuery(g, 1);
//  Q6.runQuery(g, 2);
//  Q7.runQuery(g, 1);
//  Q7.runQuery(g, 2);
//  Q8.runQuery(g, 1);
//  Q8.runQuery(g, 2);
//  Q9.runQuery(g, 1);
//  Q9.runQuery(g, 2);
//  Q10.runQuery(g, 1);
//  Q10.runQuery(g, 2);
//  Q11.runQuery(g, 1);
//  Q11.runQuery(g, 2);
//  Q12.runQuery(g, 1);
//  Q12.runQuery(g, 2);
//  Q13.runQuery(g, 1);
//  Q13.runQuery(g, 2);
//  Q14.runQuery(g, 1);
//  Q14.runQuery(g, 2);

  cout << "finish testing\n";
  return 0;
}
