//===-- traversals/UpdateDescription.h - Query class ----------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the class for GDBench queries.
///
//===----------------------------------------------------------------------===//

#ifndef _UPDATEDESCRIPTION_H_
#define _UPDATEDESCRIPTION_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

///new test
#include "QueryDescription.h"
#include "UpdateVisitor.h"


class Query18: public Query {
public:

  virtual void runQuery(Graph & graph, TransactionManager & TransM, LockManager & _LockManager,  TMSwitch c ) {
    vector<VertexPointer>  target;
    switch(c) {
      case 1: {
        SimMarker(1, 23);
        AdjacencyExplorer v12b(_LockManager); 
        traverseThroughTypeAndDirection("FRIENDS", "out", v12b.getFilter());
        breadthFirstSearch(graph, _PersonId, v12b);
        SimMarker(2, 23);
        target = v12b.getVertexTargetList();
        break;
              }
      case 2: {
        SimMarker(1, 24);
        AdjacencyExplorer v12d(_LockManager); 
        traverseThroughTypeAndDirection("FRIENDS", "out",  v12d.getFilter());
        depthFirstSearch(graph, _PersonId, v12d);
        SimMarker(2, 24);
        target = v12d.getVertexTargetList();
        break;
              }
    }
    MyFile << "===============================\n";
    MyFile << "Query 18 \n";
    if(c == 1)
        MyFile << "---------------------BFS---------------------\n";
    else
        MyFile << "---------------------DFS---------------------\n";
    FixedString key("name");
    MyFile << "Person with vid = " << _PersonId << " has name: " << graph.getVertexPointer(_PersonId)->getPropertyValue(key).first <<" and  age " << graph.getVertexPointer(_PersonId)->getPropertyValue("age").first << " and " << target.size() << " friends\n";
    for (auto it = target.begin(); it != target.end(); it++)
      MyFile << "Vertex " << (*it)->getId() << endl;
 }
};

//update (key, value)
class Query15 : public Query {
public:

  virtual void runQuery(Graph & graph, TransactionManager & TransM, LockManager & _LockManager, TMSwitch c) {
    UpdateVisitor v1(_LockManager);
    v1.requestLogRecord(TransM.addTransaction());
    v1.setSleepTime(_SleepTime);
    switch(c) {
      case 1:
        SimMarker(1, 1);
        filtProperty(_Key, _Value, v1.getFilter());
        traverseThroughType("FRIENDS", v1.getFilter());
        breadthFirstSearch(graph, 0, v1 );
        SimMarker(2, 1);
        break;
      case 2:
        SimMarker(1, 2);
        filtProperty(_Key, _Value, v1.getFilter());
        traverseThroughType("FRIENDS", v1.getFilter());
        depthFirstSearch(graph, 0, v1);
        SimMarker(2, 2);
        break;
    }

    MyFile << "===============================\n";
    MyFile << "Query 15\n";
    if(c == 1)
        MyFile << "---------------------BFS---------------------\n";
    else
        MyFile << "---------------------DFS---------------------\n";
    MyFile <<"People with " <<_Key << " = " << _Value <<" is(are) as below\n";
    auto target = v1.getVertexTargetList();
    for(auto it = target.begin(); it!= target.end(); ++it) {
      MyFile << "update: Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue("age").first << endl;
    }
 }
};

class Query16 : public Query { 
public:

  virtual void runQuery(Graph & graph, TransactionManager & TransM, LockManager & _LockManager, TMSwitch c) {
    MyFile << "===============================\n";
    MyFile << "Query 16\n";
    //a new vertex vs
    InsertVisitor v1(graph, _LockManager);
    v1.setSleepTime(_SleepTime);
    switch(c) {
      case 1:
        SimMarker(1, 1);
        filtProperty(_Key, _Value, v1.getFilter());
        traverseThroughType("FRIENDS", v1.getFilter());
        breadthFirstSearch(graph, 0, v1 );
        SimMarker(2, 1);
        break;
      case 2:
        SimMarker(1, 2);
        filtProperty(_Key, _Value, v1.getFilter());
        traverseThroughType("FRIENDS", v1.getFilter());
        depthFirstSearch(graph, 0, v1);
        SimMarker(2, 2);
        break;
    }
    MyFile << "Add one more friends to existing person with " << _Key << " = " << _Value << "\n";
 }
};

class Query17 : public Query { 
public:

  virtual void runQuery(Graph & graph, TransactionManager & TransM, LockManager & _LockManager, TMSwitch c) {
    MyFile << "===============================\n";
    MyFile << "Query 17\n";
    //a new vertex vs
    DeleteVisitor v1(graph, _LockManager);
    v1.setSleepTime(_SleepTime);
    v1.setEndVertex(9);
    switch(c) {
      case 1:
        SimMarker(1, 1);
        filtProperty(_Key, _Value, v1.getFilter());
        traverseThroughType("FRIENDS", v1.getFilter());
        breadthFirstSearch(graph, 0, v1 );
        SimMarker(2, 1);
        break;
      case 2:
        SimMarker(1, 2);
        filtProperty(_Key, _Value, v1.getFilter());
        traverseThroughType("FRIENDS", v1.getFilter());
        depthFirstSearch(graph, 0, v1);
        SimMarker(2, 2);
        break;
    }

    MyFile << "Delete one friend Vertex :10 " << "\n";
    cout << endl;
 }
};

#endif /*_UPDATEDESCRIPTION_H_*/

