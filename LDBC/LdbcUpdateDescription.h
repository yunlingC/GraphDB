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

#ifndef _LDBCUPDATEDESCRIPTION_H_
#define _LDBCUPDATEDESCRIPTION_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

///new test
#include "QueryDescription.h"
#include "LdbcUpdateVisitor.h"

ofstream CCFile("ldbc_concurrent.log", ios_base::out | ios_base::app);
///insert a new vertex
class LDBCQuery : public Query {
public:
  typedef PropertyList< KeyType, ValueType > PropertyListType;
  typedef PropertyListType& PropertyListTypeReference;

  auto setPropertyList(PropertyListTypeReference Pl) 
    -> void {
    PropertyList = pl; 
  }
protected:
  PropertyListType PropertyList;
}

class Query16 : public LDBCQuery { 
public:
  virtual void runQuery(Graph & graph, TransactionManager & TransM, LockManagerType & LockManager, TMSwitch c) {
    CCFile << "===============================\n";
    CCFile << "Query 16\n";
    //a new vertex vs 
    InsertVisitor v1(LockManager, graph);
    //v1.setSleepTime(_SleepTime);
    filtProperty(_Key, _Value, v1.getFilter());
    traverseThroughType("KNOWS", v1.getFilter());
    breadthFirstSearch(graph, 0, v1 );
    CCFile << "Add one more friends to existing person with " << _Key << " = " << _Value << "\n";
 }
};

class Query17 : public LDBCQuery { 
public:
  typedef std::string ValueType;
  typedef std::vector<ValueType> ValueListType;
public:
  void setValueList(ValueListType & vl) {
    ValueList = vl;
  }

  virtual void runQuery(Graph & graph, TransactionManager & TransM, LockManagerType & LockManager, TMSwitch c) {
    CCFile << "===============================\n";
    CCFile << "Query 17\n";
    //a new vertex vs 
    AddVisitor v1(LockManager, graph);
//    v1.getFilter().setPropertyMap;
    traverseThroughType("KNOWS", v1.getFilter());
    breadthFirstSearch(graph, 0, v1 );
    CCFile << "Add one more friends to existing person with " << _Key << " = " << _Value << "\n";
 }
protected:
  ValueListType ValueList;
};


class Query18: public Query {
public:

  virtual void runQuery(Graph & graph, TransactionManager & TransM, LockManagerType & LockManager,  TMSwitch c ) {
    vector<VertexPointer>  target;
    AdjacencyExplorer v12b(LockManager); 
    traverseThroughTypeAndDirection("KNOWS", "out", v12b.getFilter());
    breadthFirstSearch(graph, _PersonId, v12b);
    target = v12b.getVertexTargetList();
    CCFile << "===============================\n";
    CCFile << "Query 18 \n";
    if(c == 1)
        CCFile << "---------------------BFS---------------------\n";
    else
        CCFile << "---------------------DFS---------------------\n";
    FixedString key("firstName");
    CCFile << "Person with vid = " << _PersonId << " has name: " << graph.getVertexPointer(_PersonId)->getPropertyValue(key).first << " and " << target.size() << " friends\n";
    for (auto it = target.begin(); it != target.end(); it++)
      CCFile << "Vertex " << (*it)->getId() << endl;
 }
};


#endif /*_LDBCUPDATEDESCRIPTION_H_*/

