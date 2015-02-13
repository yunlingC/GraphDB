#ifndef _QUERYDESCRIPTION_H_
#define _QUERYDESCRIPTION_H_

#include "FixedString.h"
#include "TraversalDescription.h"
#include <vector>
#include <string>
#include <iostream>

/*
struct QueryDescription {
  typedef std::string KeyType;
  typedef std::string ValueType;
public:
  QueryDescription() { }

  void getQueryProperty(KeyType key, ValueType value)
  {
    _key = key;
    _value = value;
  }

  virtual void QueryBuild( ) { }

private:
  KeyType   _key;
  ValueType _value; 
  TraversalDescription Q;
    
};

*/ 
typedef std::string KeyType;
typedef std::string ValueType;
typedef GraphType Graph;

struct Query1  {
public:
 
  Query1(KeyType key, ValueType value, Graph & g) {
    TraversalDescription Q1(g);
    Graph::VertexPtr vp0 = g.getVertexPointer(0);
    Q1.traverseFromVertex(vp0->getId());
    Q1.filtProperty(key, value);
    Q1.traverseThroughType("FRIENDS");
    Q1.terminateAtTargetNumber(1);
    Q1.breadth_first_search();
    Q1.dumpVertexTarget();
    Q1.dumpDepthList();
 }

};

struct Query2 {
public:

  Query2(KeyType key, ValueType value, Graph & g) {
    TraversalDescription Q2(g);
    Graph::VertexPtr vp0 = g.getVertexPointer(0);
    Q2.traverseFromVertex(vp0->getId());
    Q2.filtProperty(key, value);
//  Q2.filtProperty("wpid", "15");
//  Q2.traverseThroughType("LIKES");
//  Q2.filtProperty("name", "KIRA VERLATO");
//  Q2.traverseThroughType("FRIENDS");
    Q2.terminateAtTargetNumber(1);
    Q2.breadth_first_search();
//  Q2.startRecording();
    Q2.dumpVertexTarget();
  
    auto targets2 = Q2.getVertexTargetSet();
 
    for(auto it = targets2.begin(); it != targets2.end(); ++it) {
      TraversalDescription q(g);
      cout << "start from vertex: " << (*it)->getId() << endl;
      q.traverseFromVertex((*it)->getId());
      q.traverseThroughType("LIKES");
      q.filtType("LIKES");
//    q.traverseThroughDirection("in");
      q.terminateAtDepth(1);
      q.breadth_first_search();
      q.dumpVertexTarget();
    }

  }
};

struct Query3 {
public:

  Query3(KeyType key, ValueType value, Graph & g) {
    TraversalDescription Q3(g);
    Graph::VertexPtr vp0 = g.getVertexPointer(0);
    Q3.traverseFromVertex(vp0->getId());
    Q3.filtProperty(key, value);
    Q3.traverseThroughType("FRIENDS");
    Q3.terminateAtTargetNumber(1);
    Q3.breadth_first_search();
//  Q2.startRecording();
    Q3.dumpVertexTarget();
  
    auto targets3 = Q3.getVertexTargetSet();
 
    for(auto it = targets3.begin(); it != targets3.end(); ++it) {
      TraversalDescription q(g);
      cout << "start from vertex: " << (*it)->getId() << endl;
      q.traverseFromVertex((*it)->getId());
      q.traverseThroughType("LIKES");
      q.filtType("LIKES");
//    q.traverseThroughDirection("in");
      q.terminateAtDepth(1);
      q.breadth_first_search();
      q.dumpVertexTarget();
    }

  }
};

struct Query4 {
public:

  Query4(KeyType key, ValueType value, Graph & g) {
    TraversalDescription Q4(g);
    Graph::VertexPtr vp0 = g.getVertexPointer(0);
    Q4.traverseFromVertex(vp0->getId());
    Q4.filtProperty(key, value);
    Q4.traverseThroughType("FRIENDS");
    Q4.terminateAtTargetNumber(1);
    Q4.breadth_first_search();
    Q4.dumpVertexTarget();
  
    auto targets4 = Q4.getVertexTargetSet();
 
    for(auto it = targets4.begin(); it != targets4.end(); ++it) {
      FixedString key("name");
      cout << (*it)->getPropertyValue(key).first;
      cout << endl;
    }


  }
};



#endif 
