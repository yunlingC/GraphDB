//===-- traversals/QueryDescription.h - Query class ----------*- C++ -*-===//
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

#ifndef _QUERYDESCRIPTION_H_
#define _QUERYDESCRIPTION_H_

#include <vector>
#include <string>
#include <iostream>

#include "FixedString.h"
#include "breadth_first_search.h"
#include "CustomVisitor.h"

class Query {
public:
  typedef std::string KeyType;
  typedef std::string ValueType;
  typedef GraphType Graph;
  typedef GraphType::VertexDescriptor Vertex;
public:
  Query() { }

private:
  KeyType   _key;
  ValueType _value; 
    
};

class Query1 : public Query {
public:
 void runQuery(Graph & graph ) {
    cout << "===============================\n";
    cout << "Query 1\n";
    Q1Visitor v1;
    filtProperty(_key, _value, v1.getFilter());
    traverseThroughType("FRIENDS", v1.getFilter());
    breadth_first_search(graph, 0, v1 );
    cout <<"People with " << _key << " = " << _value <<" is(are) as below\n";
    v1.dumpTarget();
    cout << endl;
 }
  void setPersonProperty(KeyType key, ValueType value)
  {
    _key = key;
    _value = value;
  }

private:
 KeyType     _key;
 ValueType   _value;
};


class Query2 : public Query {
public:
  typedef GraphType::VertexDescriptor Vertex;
public:
 void runQuery(Graph & graph, Vertex startVertex ) {
    cout << "===============================\n";
    cout << "Query 2 \n";
    Q2Visitor v2; 
    traverseThroughType("LIKES", v2.getFilter());
    breadth_first_search(graph, startVertex, v2);
    cout << "People who likes webpage id = " << startVertex   << " are as below\n";

    auto targetName = v2.getVertexTargetList();
    for(auto it = targetName.begin(); it != targetName.end(); ++it) {
      FixedString key("name");
      cout << "Vertex " << (*it)->getId() << (*it)->getPropertyValue(key).first;
      cout << endl;
    }
    cout << endl;
 }
  void setPersonProperty(KeyType key, ValueType value)
  {
    _key = key;
    _value = value;
  }

private:
 KeyType     _key;
 ValueType   _value;

};


class Query3 : public Query {
public:
  typedef GraphType::VertexDescriptor Vertex;
public:
 void runQuery(Graph & graph, Vertex startVertex ) {
    cout << "===============================\n";
    cout << "Query 3\n";
    Q2Visitor v3; 
    traverseThroughType("LIKES", v3.getFilter());
    breadth_first_search(graph, startVertex, v3);
    cout << "Person with vid = " << startVertex << " likes webpages:\n";

    auto targetName = v3.getVertexTargetList();
    for(auto it = targetName.begin(); it != targetName.end(); ++it) {
      FixedString key("wpurl");
      cout <<"Vertex " << (*it)->getId() << "\t"  << (*it)->getPropertyValue(key).first;
      cout << endl;
    }
    cout << endl;
 }
  void setPersonProperty(KeyType key, ValueType value)
  {
    _key = key;
    _value = value;
  }

private:
 KeyType     _key;
 ValueType   _value;

};

class Query4 : public Query {
public:
 void runQuery(Graph & graph ) {
    cout << "===============================\n";
    cout << "Query 4 \n";
    Q1Visitor v4;
    filtProperty(_key, _value, v4.getFilter());
    traverseThroughType("FRIENDS", v4.getFilter());
    breadth_first_search(graph, 0, v4 );
    cout <<"People with " << _key << " = " << _value <<" is(are) as below\n";

    auto targetName = v4.getVertexTargetList();
    for(auto it = targetName.begin(); it != targetName.end(); ++it) {
      FixedString key("name");
      cout <<"Vertex " << (*it)->getId() << "\t"  << (*it)->getPropertyValue(key).first;
      cout << endl;
    }
    

    cout << endl;
 }
  void setPersonProperty(KeyType key, ValueType value)
  {
    _key = key;
    _value = value;
  }

private:
 KeyType     _key;
 ValueType   _value;
};


class Query12 : public Query {
public:
  typedef GraphType::VertexDescriptor Vertex;
public:
 void runQuery(Graph & graph, Vertex startVertex ) {
    cout << "===============================\n";
    cout << "Query 12 \n";
    Q2Visitor v12; 
    traverseThroughType("FRIENDS", v12.getFilter());
    breadth_first_search(graph, startVertex, v12);
    FixedString key("name");
    cout << "Person with vid = " << startVertex << " has name: " << graph.getVertexPointer(startVertex)->getPropertyValue(key).first <<" and  " << v12.getVertexTargetList().size() << " friends\n";
 }

  void setPersonProperty(KeyType key, ValueType value)
  {
    _key = key;
    _value = value;
  }

private:
 KeyType     _key;
 ValueType   _value;

};




#endif 

