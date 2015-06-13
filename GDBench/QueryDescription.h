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
#include <fstream>

//#include "util.h"
#include "FixedString.h"
#include "BreadthFirstSearch.h"
#include "DepthFirstSearch.h"
#include "CustomVisitor.h"

ofstream myfile("gd_execution.log", ios_base::out | ios_base::app);

class Query {
public:
  typedef std::string KeyType;
  typedef std::string ValueType;
  typedef GraphType Graph;
  typedef unsigned int  TMSwitch; //traversalMethodSwitch: 1: bfs 2: dfs
  typedef GraphType::VertexDescriptor VertexDescriptor;
public:
  Query() { }
  
  virtual void runQuery(Graph & graph, TMSwitch c) { }

  void setPersonProperty(const KeyType & key, const ValueType & value) {
    _Key = key;
    _Value = value;
  }

  void setPersonId (VertexDescriptor Id) {
    _PersonId = Id; 
  }

  void setEndPoints(const VertexDescriptor & Id1, const VertexDescriptor & Id2) {
    _PersonId1 = Id1;
    _PersonId2 = Id2;
  }

  void setWebId(VertexDescriptor webId) {
    _WebId = webId;
  }

protected:
  KeyType   _Key;
  ValueType _Value;
  VertexDescriptor _WebId;
  VertexDescriptor _PersonId;
  VertexDescriptor _PersonId1;
  VertexDescriptor _PersonId2;
};

class Query14 : public Query {
public:
 virtual void runQuery(Graph & graph, TMSwitch c) {
    Visitor v1;
    switch(c) {
      case 1:
//        SimMarker(1, 27);
        filtProperty(_Key, _Value, v1.getFilter());
        breadthFirstSearch(graph, 0, v1 );
//        SimMarker(2, 27);
        break;
      case 2:
//        SimMarker(1, 28);
        filtProperty(_Key, _Value, v1.getFilter());
        depthFirstSearch(graph, 0, v1);
//        SimMarker(2, 28);
        break;
    }

    myfile << "===============================\n";
    myfile << "Query 14\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    myfile << "Traversal from vertex 0." << endl;
    myfile << endl;
 }

};


class Query1 : public Query {
public:
 virtual void runQuery(Graph & graph, TMSwitch c) {
    SelectionVisitor v1;
    switch(c) {
      case 1:
//        SimMarker(1, 1);
        filtProperty(_Key, _Value, v1.getFilter());
        traverseThroughType("FRIENDS", v1.getFilter());
        breadthFirstSearch(graph, 0, v1 );
//        SimMarker(2, 1);
        break;
      case 2:
//        SimMarker(1, 2);
        filtProperty(_Key, _Value, v1.getFilter());
        traverseThroughType("FRIENDS", v1.getFilter());
        depthFirstSearch(graph, 0, v1);
//        SimMarker(2, 2);
        break;
    }

    myfile << "===============================\n";
    myfile << "Query 1\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    myfile <<"People with " <<_Key << " = " << _Value <<" is(are) as below\n";
    auto target = v1.getVertexTargetList();
    for(auto it = target.begin(); it != target.end(); ++it) {
      myfile << "Vertex " << (*it)->getId();
      myfile << endl;
    }
    myfile << endl;
 }

};


class Query2 : public Query {
public:
 virtual void runQuery(Graph & graph, TMSwitch c) {
    AdjacencyVisitor v2; 
    switch(c) {
      case 1:
//        SimMarker(1, 3);
        traverseThroughTypeAndDirection("LIKES","in", v2.getFilter());
        breadthFirstSearch(graph, _WebId, v2);
//        SimMarker(2, 3);
        break;
      case 2:
//        SimMarker(1, 4);
        traverseThroughTypeAndDirection("LIKES","in", v2.getFilter());
        depthFirstSearch(graph, _WebId, v2);
//        SimMarker(2, 4);
        break;
    }

    myfile << "===============================\n";
    myfile << "Query 2\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    myfile << "People who likes webpage id = " << _WebId  << " are as below\n";
    auto target= v2.getVertexTargetList();
    for(auto it = target.begin(); it != target.end(); ++it) {
      FixedString key("name");
      myfile << "Vertex " << (*it)->getId() <<"\t" << (*it)->getPropertyValue(key).first;
      myfile << endl;
    }
    myfile << endl;
 }
};


class Query3 : public Query {
public:
 virtual void runQuery(Graph & graph, TMSwitch c ) {
    AdjacencyVisitor v3; 
    switch(c) {
      case 1:
//        SimMarker(1, 5);
        traverseThroughType("LIKES", v3.getFilter());
        breadthFirstSearch(graph, _PersonId, v3);
//        SimMarker(2, 5);
        break;
      case 2:
//        SimMarker(1, 6);
        traverseThroughType("LIKES", v3.getFilter());
        depthFirstSearch(graph, _PersonId, v3);
//        SimMarker(2, 6);
        break;
    }

    myfile << "===============================\n";
    myfile << "Query 3\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    myfile << "Person with vid = " << _PersonId << " likes webpages:\n";
    auto target = v3.getVertexTargetList();
    for(auto it = target.begin(); it != target.end(); ++it) {
      FixedString key("wpurl");
      myfile <<"Vertex " << (*it)->getId() << "\t"  << (*it)->getPropertyValue(key).first;
      myfile << endl;
    }
    myfile << endl;
 }
};

class Query4 : public Query {
public:
  virtual void runQuery(Graph & graph, TMSwitch c) {
    SelectionVisitor  v4;
    switch(c) {
      case 1:
//        SimMarker(1, 7);
        filtProperty(_Key, _Value, v4.getFilter());
        traverseThroughType("FRIENDS", v4.getFilter());
        breadthFirstSearch(graph, 0, v4);
//        SimMarker(2, 7);
        break;
      case 2:
//        SimMarker(1, 8);
        filtProperty(_Key, _Value, v4.getFilter());
        traverseThroughType("FRIENDS", v4.getFilter());
        depthFirstSearch(graph, 0, v4);
//        SimMarker(2, 8);
        break;
    }

    myfile << "===============================\n";
    myfile << "Query 4\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    myfile <<"People with " << _Key << " = " << _Value <<" is(are) as below\n";
    auto target = v4.getVertexTargetList();
    for(auto it = target.begin(); it != target.end(); ++it) {
      FixedString key("name");
      myfile <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue(key).first;
      myfile << endl;
    }
 }
};

class Query5 : public Query {
public:
  virtual void runQuery(Graph & graph, TMSwitch c ) {
    myfile << "===============================\n";
    myfile << "Query 5\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    myfile << "The friends of Person with vid = " << _PersonId << " has friends\n";
    Filter tmpFilter[2];
    FixedString key("name");
    std::vector<VertexPointer> target;
    switch(c) {
      case 1: {
//        SimMarker(1, 9);
        traverseThroughTypeAndDirection("FRIENDS", "out",  tmpFilter[0]);
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[1]);
        ReachabilityVisitor  v5b;
        v5b.setFilter(tmpFilter[0]);
        v5b.setFilter(tmpFilter[1]);
        v5b.setDepth(2);
        breadthFirstSearch(graph, _PersonId, v5b);
//        SimMarker(2, 9);
        auto TargetSet = v5b.getTargetSet(); 
        for(auto it = TargetSet.begin(); it != TargetSet.end(); ++it) {
          myfile <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue(key).first << endl;
        }
        break;
              }
      case 2: {
//        SimMarker(1, 10);
        traverseThroughTypeAndDirection("FRIENDS", "out",  tmpFilter[0]);
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[1]);
        DFSReachabilityVisitor v5d;
        v5d.setFilter(tmpFilter[0]);
        v5d.setFilter(tmpFilter[1]);
        v5d.setDepth(2);
        depthFirstSearch(graph, _PersonId, v5d);
//        SimMarker(2, 10);
        auto target = v5d.getVertexTargetMap();
        myfile << "---------------------DFS---------------------\n";
        for(auto it = target.begin(); it != target.end(); ++it) {
          myfile <<"Vertex " << (*it).first->getId() << "\t" << (*it).first->getPropertyValue(key).first;
        myfile << endl;
        }
        break;
              }
    }

  }
};

class Query6: public Query {
public:
  virtual void runQuery(Graph & graph, TMSwitch c) {
    myfile << "===============================\n";
    myfile << "Query 6\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    FixedString key("wpurl");
    Filter tmpFilter[2];
    switch(c) {
      case 1: {
//        SimMarker(1, 11);
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[0]);
        traverseThroughTypeAndDirection("LIKES", "out", tmpFilter[1]);
        ReachabilityVisitor  v6b;
        v6b.setFilter(tmpFilter[0]);
        v6b.setFilter(tmpFilter[1]);
        v6b.setDepth(2);
        breadthFirstSearch(graph, _PersonId, v6b);
//        SimMarker(2, 11);
        auto TargetSet = v6b.getTargetSet(); 
        myfile << "The friends of Person with vid = " << _PersonId<< " like " << TargetSet.size() << " webpages\n";
        for(auto it = TargetSet.begin(); it != TargetSet.end(); ++it) {
          myfile <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue(key).first << endl;
        }
        break;
              }
      case 2: {
//        SimMarker(1, 12);
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[0]);
        traverseThroughTypeAndDirection("LIKES", "out", tmpFilter[1]);
        DFSReachabilityVisitor v6d;
        v6d.setFilter(tmpFilter[0]);
        v6d.setFilter(tmpFilter[1]);
        v6d.setDepth(2);
        depthFirstSearch(graph, _PersonId, v6d);
//        SimMarker(2, 12);
        auto target = v6d.getVertexTargetMap();
        myfile << "The friends of Person with vid = " << _PersonId << " like " << target.size() << " webpages\n";
        for(auto it = target.begin(); it != target.end(); ++it) {
          myfile <<"Vertex " << (*it).first->getId() << "\t" << (*it).first->getPropertyValue(key).first;
        myfile << endl;
        }
        break;
              }
    }
  }
};

class Query7: public Query {
public:
  virtual void runQuery(Graph & graph, TMSwitch c ) {
    myfile << "===============================\n";
    myfile << "Query 7\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    Filter tmpFilter[2];
    FixedString key("name");
    switch(c) {
      case 1: {
//        SimMarker(1, 13);
        traverseThroughType("LIKES", tmpFilter[0]);
        traverseThroughType("LIKES", tmpFilter[1]);
        ReachabilityVisitor  v7b;
        v7b.setFilter(tmpFilter[0]);
        v7b.setFilter(tmpFilter[1]);
        v7b.setDepth(2);
        breadthFirstSearch(graph, _PersonId, v7b);
//        SimMarker(2, 13);
        auto TargetSet = v7b.getTargetSet(); 
        myfile << "The webpages liked by person vid = " << _PersonId<< " are liked by " << TargetSet.size() << " people: \n";
        for(auto it = TargetSet.begin(); it != TargetSet.end(); ++it) {
          myfile <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue(key).first << endl;
        }
 
        break;
              }
      case 2: {
//        SimMarker(1, 14);
        traverseThroughType("LIKES", tmpFilter[0]);
        traverseThroughType("LIKES", tmpFilter[1]);
        DFSReachabilityVisitor v7d;
        v7d.setFilter(tmpFilter[0]);
        v7d.setFilter(tmpFilter[1]);
        v7d.setDepth(2);
        depthFirstSearch(graph, _PersonId, v7d);
//        SimMarker(2, 14);
        auto target = v7d.getVertexTargetMap();
        myfile << "The webpages liked by person vid = " << _PersonId << " are liked by  " << target.size() << " people\n";
        for(auto it = target.begin(); it != target.end(); ++it) {
          myfile <<"Vertex " << (*it).first->getId() << "\t" << (*it).first->getPropertyValue(key).first;
        myfile << endl;
        }
        break;
              }
    }

  }
};

class Query8 : public Query {
public:
  virtual void runQuery(Graph & graph, TMSwitch c) {
    myfile << "===============================\n";
    myfile << "Query 8\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    switch(c) {
      case 1: {
//        SimMarker(1, 15);
        PathVisitor v8b;
        v8b.setEndVertex(_PersonId2);
        breadthFirstSearch(graph, _PersonId1, v8b);
//        SimMarker(2, 15);
        auto target = v8b.getVertexTargetList();
        if(!target.empty())
          myfile << "There is path from " << _PersonId1 << " to " <<  _PersonId2 << endl;
        else 
          myfile << _PersonId1 << " and " <<  _PersonId2 <<" are not connected" << endl;
        break;
              }
      case 2: {
//        SimMarker(1, 16);
        DFSPathVisitor v8d;
        v8d.setEndVertex(_PersonId2);
        depthFirstSearch(graph, _PersonId1, v8d);
//        SimMarker(2, 16);
        auto target = v8d.getPathList();
        if(!target.empty())
          myfile << "There is path from " << _PersonId1 << " to " <<  _PersonId2 << endl;
        else 
          myfile << _PersonId1 << " and " <<  _PersonId2 <<" are not connected" << endl;
        break;
              }
    }
  }
};

class Query9: public Query {
public:
  virtual void runQuery(Graph & graph, TMSwitch c ) {
    myfile << "===============================\n";
    myfile << "Query 9\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    //PathVisitor v9;
    //v9.setEndVertex(_PersonId2);
    switch(c) {
      case 1: {
//        SimMarker(1, 17);
        PathVisitor v9b;
        v9b.setEndVertex(_PersonId2);
        breadthFirstSearch(graph, _PersonId1, v9b);
//        SimMarker(2, 17);
        auto target = v9b.getVertexTargetList();
        if(target.empty())
          myfile << _PersonId1 << " and " <<  _PersonId2 <<" are not connected" << endl;
        else {
            myfile << "There are  shortest paths from " << _PersonId1 << " to " <<  _PersonId2 << endl;
          for(auto it = target.begin(); it != target.end(); ++it) {
            myfile <<"Vertex " << (*it)->getId() << endl;
          }
        }
        break;
              }
      case 2: {
//        SimMarker(1, 18);
        DFSShortestPathVisitor v9d;
        v9d.setEndVertex(_PersonId2);
        depthFirstSearch(graph, _PersonId1, v9d);
//        SimMarker(2, 18);
        auto target = v9d.getPathList();
        if(target.empty())
          myfile << _PersonId1 << " and " <<  _PersonId2 <<" are not connected" << endl;
        else {
          myfile << "There are " << target.size() << " shortest paths from " << _PersonId1 << " to " <<  _PersonId2 << endl;
          for(auto it = target.begin(); it != target.end(); ++it) {
            for (auto iter = (*it).second.begin(); iter != (*it).second.end(); ++iter)
              myfile <<"Vertex " << (*iter)->getId() << endl;
          myfile << endl;
         }
       }
        break;
              }
    }
  }
};

class Query10 : public Query {
public:
  virtual void runQuery(Graph & graph, TMSwitch c ) { 
    Filter tmpFilter[2];
    FixedString key("name");
    vector<VertexPointer> target;
    switch(c) {
      case 1: {
//        SimMarker(1, 19);
        traverseThroughTypeAndDirection("FRIENDS", "out",  tmpFilter[0]);
        traverseThroughTypeAndDirection("FRIENDS", "in", tmpFilter[1]);
        PatternVisitor v10d;
        v10d.setFilter(tmpFilter[0]);
        v10d.setFilter(tmpFilter[1]);
        v10d.setDepth(2);
        v10d.setEndVertex(_PersonId2);
        breadthFirstSearch(graph, _PersonId1, v10d);
//        SimMarker(2, 19);
        target = v10d.getVertexTargetList();
        break;
              }
      case 2: {
//        SimMarker(1, 20);
        traverseThroughTypeAndDirection("FRIENDS", "out",  tmpFilter[0]);
        traverseThroughTypeAndDirection("FRIENDS", "in", tmpFilter[1]);
        DFSPatternVisitor v10d;
        v10d.setFilter(tmpFilter[0]);
        v10d.setFilter(tmpFilter[1]);
        v10d.setDepth(2);
        v10d.setEndVertex(_PersonId2);
        depthFirstSearch(graph, _PersonId1, v10d);
//        SimMarker(2, 20);
        target = v10d.getVertexTargetList();
        break;
              }
    }
    myfile << "===============================\n";
    myfile << "Query 10\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    myfile << "There are " << target.size() << " common friends between  " << _PersonId1 << " and " <<  _PersonId2 << endl;
    for(auto it = target.begin(); it != target.end(); ++it) {
      myfile <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue(key).first;
    myfile << endl;
    }
  }
};


class Query11: public Query {
public:
  virtual void runQuery(Graph & graph, TMSwitch c) {
    Filter tmpFilter[2];
    FixedString key("name");
    vector<VertexPointer> target;
    switch(c) {
      case 1: {
//        SimMarker(1, 21);
        traverseThroughTypeAndDirection("LIKES", "out",  tmpFilter[0]);
        traverseThroughTypeAndDirection("LIKES", "in",   tmpFilter[1]);
        PatternVisitor v11b;
        v11b.setFilter(tmpFilter[0]);
        v11b.setFilter(tmpFilter[1]);
        v11b.setDepth(2);
        v11b.setEndVertex(_PersonId2);
        breadthFirstSearch(graph, _PersonId1, v11b);
//        SimMarker(2, 21);
        target = v11b.getVertexTargetList();
        break;
              }
      case 2: {
//        SimMarker(1, 22);
        traverseThroughTypeAndDirection("LIKES", "out",  tmpFilter[0]);
        traverseThroughTypeAndDirection("LIKES", "in",   tmpFilter[1]);
        DFSPatternVisitor v11d;
        v11d.setFilter(tmpFilter[0]);
        v11d.setFilter(tmpFilter[1]);
        v11d.setDepth(2);
        v11d.setEndVertex(_PersonId2);
        depthFirstSearch(graph, _PersonId1, v11d);
//        SimMarker(2, 22);
        target = v11d.getVertexTargetList();
        break;
              }
    }
    myfile << "===============================\n";
    myfile << "Query 11\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    myfile << "There are " << target.size() << " common webpages liked by both " << _PersonId1 << " and " <<  _PersonId2 << endl;
    for(auto it = target.begin(); it != target.end(); ++it) {
      FixedString key("wpurl");
      myfile <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue(key).first;
      myfile << endl;
    }
  }

};

class Query12 : public Query {
public:
 virtual void runQuery(Graph & graph, TMSwitch c ) {
    vector<VertexPointer>  target;
    switch(c) {
      case 1: {
//        SimMarker(1, 23);
        AdjacencyVisitor v12b; 
        traverseThroughTypeAndDirection("FRIENDS", "out", v12b.getFilter());
        breadthFirstSearch(graph, _PersonId, v12b);
//        SimMarker(2, 23);
        target = v12b.getVertexTargetList();
        break;
              }
      case 2: {
//        SimMarker(1, 24);
        AdjacencyVisitor v12d; 
        traverseThroughTypeAndDirection("FRIENDS", "out",  v12d.getFilter());
        depthFirstSearch(graph, _PersonId, v12d);
//        SimMarker(2, 24);
        target = v12d.getVertexTargetList();
        break;
              }
    }
    myfile << "===============================\n";
    myfile << "Query 12\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    FixedString key("name");
    myfile << "Person with vid = " << _PersonId << " has name: " << graph.getVertexPointer(_PersonId)->getPropertyValue(key).first <<" and  " << target.size() << " friends\n";
    for (auto it = target.begin(); it != target.end(); it++)
      myfile << "Vertex " << (*it)->getId() << endl;
 }
};

class Query13: public Query {
public:
  virtual void runQuery(Graph & graph, TMSwitch c ) {
    myfile << "===============================\n";
    myfile << "Query 13\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    Filter tmpFilter[3];
    FixedString key("name");
    switch(c) {
      case 1: {
//        SimMarker(1, 25);
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[0]);
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[1]);
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[2]);
        ReachabilityVisitor  v13b;
        v13b.setFilter(tmpFilter[0]);
        v13b.setFilter(tmpFilter[1]);
        v13b.setFilter(tmpFilter[2]);
        v13b.setDepth(3);
        breadthFirstSearch(graph, _PersonId, v13b);
//        SimMarker(2, 25);
        auto TargetSet = v13b.getTargetSet(); 
        myfile << "The friends of friends of person vid = " << _PersonId << " has " << TargetSet.size() << " friends: \n";
        for(auto it = TargetSet.begin(); it != TargetSet.end(); ++it) {
          myfile <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue(key).first << endl;
        }
        break;
              }
      case 2: {
//        SimMarker(1, 26);
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[0]);
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[1]);
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[2]);
        DFSReachabilityVisitor v13d;
        v13d.setFilter(tmpFilter[0]);
        v13d.setFilter(tmpFilter[1]);
        v13d.setFilter(tmpFilter[2]);
        v13d.setDepth(3);
        depthFirstSearch(graph, _PersonId, v13d);
//        SimMarker(2, 26);
        auto target = v13d.getVertexTargetMap();
        myfile << "The friends of friends of person vid = " << _PersonId << " has " << target.size() << " friends: \n";
        for(auto it = target.begin(); it != target.end(); ++it) {
        if( (*it).first->getId() != _PersonId ) {
        myfile <<"Vertex " << (*it).first->getId() << "\t"  << (*it).first->getPropertyValue(key).first;
      myfile << endl;
      }
    }
        break;
              }
    }
  }
};

#endif 

