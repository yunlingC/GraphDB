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

#include "QueryDescription.h"
#include "LdbcUpdateVisitor.h"

//ofstream CCFile("ldbc_concurrent.log", ios_base::out | ios_base::app);
///insert a new vertex
class LDBCQuery : public Query {
public:
  typedef FixedString KeyType;
  typedef FixedString ValueType;
  typedef PropertyList< KeyType, ValueType > PropertyListType;
  typedef PropertyListType & PropertyListTypeReference;

  auto setPropertyList(PropertyListTypeReference pl) 
    -> void {
    VertexPropertyList = pl; 
  }
protected:
  PropertyListType VertexPropertyList;
};

class Query16 : public LDBCQuery { 
public:
  virtual void runQuery(Graph & graph, TransactionManager & TransM, LockManagerType & LockManager, TMSwitch c) {
    std::cout << "===============================\n";
    std::cout << "Query 16\n";
    //a new vertex vs 
    InsertVisitor v1(LockManager, graph);
    filtProperty(_Key, _Value, v1.getFilter());
    traverseThroughType("KNOWS", v1.getFilter());
    breadthFirstSearch(graph, 0, v1 );
    std::cout << "Add one more friends to existing person with " << _Key << " = " << _Value << "\n";
 }
};

///Add one vertex to current graph
class Query17 : public LDBCQuery { 
public:
  typedef std::string KeyType;
  typedef std::string ValueType;
  typedef std::vector<ValueType> ValueListType;
  typedef std::pair<std::string, GraphType::PropertyListType> EdgePairType;
  typedef std::map<std::pair<std::string, std::string>, EdgePairType> BranchMapType;
public:
  void setValueList(ValueListType & vl) {
    ValueList = vl;
  }

  void setBranchMap(BranchMapType & bm) {
    BranchMap = bm;
  }

  virtual void runQuery(Graph & graph, TransactionManager & TransM, LockManagerType & LockManager, TMSwitch c) {
    std::cout << "===============================\n";
    std::cout << "Query 17\n";
    //a new vertex vs 
    //need new propertylist for vertex and branchmap (criteria) for searching neighbor 
    AddVisitor v1(LockManager, graph);
    v1.setVertexProperty(VertexPropertyList);
    v1.getFilter().setBranchMap(BranchMap);
    breadthFirstSearch(graph, 0, v1);
    std::cout << "Add one more person into network \n";
 }
protected:
  BranchMapType BranchMap;
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
    std::cout << "===============================\n";
    std::cout << "Query 18 \n";
    if(c == 1)
        std::cout << "---------------------BFS---------------------\n";
    else
        std::cout << "---------------------DFS---------------------\n";
    FixedString key("firstName");
    std::cout << "Person with vid = " << _PersonId << " has name: " << graph.getVertexPointer(_PersonId)->getPropertyValue(key).first << " and " << target.size() << " friends\n";
    for (auto it = target.begin(); it != target.end(); it++)
      std::cout << "Vertex " << (*it)->getId() << endl;
 }
};


#endif /*_LDBCUPDATEDESCRIPTION_H_*/

