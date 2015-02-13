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
    cout <<"People with " << key << " = " << value <<" is(are) as below\n";
    Q1.dumpVertexTarget();
 }

};

struct Query2 {
public:

  Query2(unsigned int web_id, Graph & g) {
    TraversalDescription Q2(g);
    Graph::VertexPtr vp0 = g.getVertexPointer(web_id);
    Q2.traverseFromVertex(vp0->getId());
    Q2.traverseThroughType("LIKES");
    Q2.filtType("LIKES");
//    q.traverseThroughDirection("in");
    Q2.terminateAtDepth(1);
    Q2.breadth_first_search();
    cout << "People who likes webpage id = " << web_id  << " are as below\n";
//      q.dumpVertexTarget();
    auto targetName = Q2.getVertexTargetSet();
    for(auto it = targetName.begin(); it != targetName.end(); ++it) {
      FixedString key("name");
      cout << (*it)->getPropertyValue(key).first;
      cout << endl;
    }
  }

};

struct Query3 {
public:

  Query3(unsigned int person_id, Graph & g) {
    TraversalDescription Q3(g);
    Graph::VertexPtr vp0 = g.getVertexPointer(person_id);
    Q3.traverseFromVertex(vp0->getId());
//    Q3.filtProperty(key, value);
//    Q3.traverseThroughType("FRIENDS");
//    Q3.terminateAtTargetNumber(1);
//    Q3.breadth_first_search();
//    Q2.startRecording();
//    Q3.dumpVertexTarget();
  
//    auto targets3 = Q3.getVertexTargetSet();
 
//    for(auto it = targets3.begin(); it != targets3.end(); ++it) {
//      TraversalDescription q(g);
//      cout << "start from vertex: " << (*it)->getId() << endl;
//      q.traverseFromVertex((*it)->getId());
      Q3.traverseThroughType("LIKES");
      Q3.filtType("LIKES");
//    q.traverseThroughDirection("in");
      Q3.terminateAtDepth(1);
      Q3.breadth_first_search();
      Q3.dumpVertexTarget();
      cout << "Person with pid = " << person_id << " likes webpages:\n";
//      q.dumpVertexTarget();
      auto targetName = Q3.getVertexTargetSet();
      for(auto it = targetName.begin(); it != targetName.end(); ++it) {
        FixedString key("wpurl");
        cout << (*it)->getPropertyValue(key).first;
        cout << endl;
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
 
    cout << "People with " << key << " = " << value << " are as below\n";
    for(auto it = targets4.begin(); it != targets4.end(); ++it) {
      FixedString key("name");
      cout << (*it)->getPropertyValue(key).first;
      cout << endl;
    }

  }
};

struct Query12 {
public:

  Query12(unsigned int person_id, Graph & g) {
    TraversalDescription Q12(g);
    Graph::VertexPtr vp0 = g.getVertexPointer(person_id);
    Q12.traverseFromVertex(vp0->getId());
//    Q13.filtProperty(key, value);
//    Q13.traverseThroughType("FRIENDS");
//    Q13.terminateAtTargetNumber(1);
//    Q13.breadth_first_search();
//    Q13.dumpVertexTarget();
 
//    auto targets1 =  Q13.getVertexTargetSet();
//    for(auto it = targets1.begin(); it != targets1.end(); ++it) {
//      TraversalDescription q(g);
//      cout << "start from vertex: " << (*it)->getId() << endl;
//      q.traverseFromVertex((*it)->getId());
    Q12.traverseThroughType("FRIENDS");
    Q12.filtType("FRIENDS");
    Q12.terminateAtDepth(1);
    Q12.breadth_first_search();
//      q.dumpVertexTarget();
//    }

 
    auto targets =  Q12.getVertexTargetSet();
 
    cout << "Person with person id = " << person_id << " has " << targets.size() << " friends \n";
    for(auto it = targets.begin(); it != targets.end(); ++it) {
      FixedString key("name");
      cout << "vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue(key).first;
      cout << endl;
    }
  }
};






#endif 
