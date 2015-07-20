//#include "macros.h"
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <queue>
#include <thread>
#include <string>
#include <time.h>

//#include "GDBench/util.h"
//#include "GDReader.h"
#include "LinkedList.h"
#include "QueryDescription.h"
#include "QueryRandomizer.h"

int main(int argc, char *argv[]) {

  
  typedef GraphType Graph;
  typedef Graph::VertexDescriptor VertexDescriptor;

  Graph g;
  cout << "Begin testing\n";

#ifdef _FIXALLOC_
  g.allocVertexMemory(1200);
  g.allocEdgeMemory(20000);
#endif /* _FIXALLOC */
  cout << "Begin testing\n";


  GDReader reader(g);
  reader.setPrintInterval(10000);
  reader.readFile("../tests/gd/sndata1000.gd");

//  auto PeopleConnter = reader.getPeopleCounter();
//  auto WebCounter = reader.getWebpagesCounter();
//  auto FriendsCounter = getFriendsCounter();
//  auto LikesCounter = getLikesCounter();

  cout << "Finish reading\n";

  QueryRandomizer rander(reader);

    string name, pid;
    VertexDescriptor webId, personId1, personId2, personId3;

    auto id = 1;
    name = rander.getName(id);
    pid  = rander.getPid(id);
    webId = rander.getAWebpageIndex(id);
    personId1 = rander.getAPersonIndex(id);
    personId2 = rander.getAPersonIndex(id);
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

//    g.handleAddr();

    int InputId[19] = {46, 92, 42, 28, 59, 3, 98, 44, 54, 12, 95, 14, 42, 12, 60, 51, 99, 50, 11};
    int i = 0;
    id = InputId[i++];

    name = rander.getName(id);
    Q1.setPersonProperty("name", name);

    id = InputId[i++];
    webId = rander.getAWebpageIndex(id);
    Q2.setWebId(webId);

    id = InputId[i++];
    personId1 = rander.getAPersonIndex(id);
    Q3.setPersonId(personId1);

    id = InputId[i++];
    pid  = rander.getPid(id);
    Q4.setPersonProperty("pid", pid);

    id = InputId[i++];
    personId1 = rander.getAPersonIndex(id);
    Q5.setPersonId(personId1);

    id = InputId[i++];
    personId1 = rander.getAPersonIndex(id);
    Q6.setPersonId(personId1);

    id = InputId[i++];
    personId1 = rander.getAPersonIndex(id);
    Q7.setPersonId(personId1);

    id = InputId[i++];
    personId3 = rander.getAPersonIndex(id);
    id = InputId[i++];
    personId2 = rander.getAPersonIndex(id);
    Q8.setEndPoints(personId2, personId3);

    id = InputId[i++];
    personId2 = rander.getAPersonIndex(id);
    id = InputId[i++];
    personId3 = rander.getAPersonIndex(id);
    Q9.setEndPoints(personId2, personId3);

    id = InputId[i++];
    personId2 = rander.getAPersonIndex(id);
    id = InputId[i++];
    personId3 = rander.getAPersonIndex(id);
    Q10.setEndPoints(personId2, personId3);

    id = InputId[i++];
    personId2 = rander.getAPersonIndex(id);
    id = InputId[i++];
    personId3 = rander.getAPersonIndex(id);
    Q11.setEndPoints(personId2, personId3);

    id = InputId[i++];
    personId1 = rander.getAPersonIndex(id);
    Q12.setPersonId(personId1);

    id = InputId[i++];
    personId1 = rander.getAPersonIndex(id);
    Q13.setPersonId(personId1);

    id = InputId[i++];
    name = rander.getName(id);
    Q14.setPersonProperty("name", name);


//    SimRoiStart();
    Q1.runQuery(g, 1);
    Q2.runQuery(g, 1);
    Q3.runQuery(g, 1);
    Q4.runQuery(g, 1);
    Q5.runQuery(g, 1);
    Q6.runQuery(g, 1);
    Q7.runQuery(g, 1);
    Q8.runQuery(g, 1);
    Q9.runQuery(g, 1);
    Q10.runQuery(g, 1);
    Q11.runQuery(g, 1);
    Q12.runQuery(g, 1);
    Q13.runQuery(g, 1);
    Q14.runQuery(g, 1);
//    SimRoiEnd();

  cout << "finish testing\n";
  myfile.close();
  return 0;
}
