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
#include "DepthFirstSearch.h"
#include "CustomVisitor.h"

class Query {
public:
  typedef std::string KeyType;
  typedef std::string ValueType;
  typedef GraphType Graph;
  typedef unsigned int  TMSwitch; //traversalMethodSwitch: 1: bfs 2: dfs
  typedef GraphType::VertexDescriptor Vertex;
public:
  Query() { }

};

class Query1 : public Query {
public:
 void runQuery(Graph & graph, const KeyType & key, const ValueType & value, TMSwitch c) {
    cout << "===============================\n";
    cout << "Query 1\n";
    SelectionVisitor v1;
    filtProperty(key, value, v1.getFilter());
    traverseThroughType("FRIENDS", v1.getFilter());
    switch(c) {
      case 1:
        cout << "---------------------BFS---------------------\n";
        breadth_first_search(graph, 0, v1 );
        break;
      case 2:
        cout << "---------------------DFS---------------------\n";
        depthFirstSearch(graph, 0, v1);
        break;
      default:
        cout << "Traverse Method: 1. BFS  2. DFS\n"; 
    }

    cout <<"People with " << key << " = " << value <<" is(are) as below\n";
    v1.getVertexTargetList();
    v1.dumpTarget();
    cout << endl;
 }

};


class Query2 : public Query {
public:
 void runQuery(Graph & graph, const Vertex & startVertex, TMSwitch c) {
    cout << "===============================\n";
    cout << "Query 2 \n";
    AdjacencyVisitor v2; 
    traverseThroughType("LIKES", v2.getFilter());
    switch(c) {
      case 1:
        cout << "---------------------BFS---------------------\n";
        breadth_first_search(graph, startVertex, v2);
        break;
      case 2:
        cout << "---------------------DFS---------------------\n";
        depthFirstSearch(graph, startVertex, v2);
        break;
      default:
        cout << "Traverse Method: 1. BFS  2. DFS\n"; 
    }

    cout << "People who likes webpage id = " << startVertex   << " are as below\n";
    auto target= v2.getVertexTargetList();
    for(auto it = target.begin(); it != target.end(); ++it) {
      FixedString key("name");
      cout << "Vertex " << (*it)->getId() <<"\t" << (*it)->getPropertyValue(key).first;
      cout << endl;
    }
    cout << endl;
 }
};


class Query3 : public Query {
public:
 void runQuery(Graph & graph, const Vertex &  startVertex, TMSwitch c ) {
    cout << "===============================\n";
    cout << "Query 3\n";
    AdjacencyVisitor v3; 
    traverseThroughType("LIKES", v3.getFilter());
    switch(c) {
      case 1:
        cout << "---------------------BFS---------------------\n";
        breadth_first_search(graph, startVertex, v3);
        break;
      case 2:
        cout << "---------------------DFS---------------------\n";
        depthFirstSearch(graph, startVertex, v3);
        break;
      default:
        cout << "Traverse Method: 1. BFS  2. DFS\n"; 
    }

    cout << "Person with vid = " << startVertex << " likes webpages:\n";
    auto target = v3.getVertexTargetList();
    for(auto it = target.begin(); it != target.end(); ++it) {
      FixedString key("wpurl");
      cout <<"Vertex " << (*it)->getId() << "\t"  << (*it)->getPropertyValue(key).first;
      cout << endl;
    }
    cout << endl;
 }
};

class Query4 : public Query {
public:
  void runQuery(Graph & graph, KeyType key, ValueType value, TMSwitch c) {
    cout << "===============================\n";
    cout << "Query 4 \n";
    SelectionVisitor  v4;
    filtProperty(key, value, v4.getFilter());
    traverseThroughType("FRIENDS", v4.getFilter());
    switch(c) {
      case 1:
        cout << "---------------------BFS---------------------\n";
        breadth_first_search(graph, 0, v4);
        break;
      case 2:
        cout << "---------------------DFS---------------------\n";
        depthFirstSearch(graph, 0, v4);
        break;
      default:
        cout << "Traverse Method: 1. BFS  2. DFS\n"; 
    }

    cout <<"People with " << key << " = " << value <<" is(are) as below\n";
    auto target = v4.getVertexTargetList();
    for(auto it = target.begin(); it != target.end(); ++it) {
      FixedString key("name");
      cout <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue(key).first;
      cout << endl;
    }
 }
};

class Query5 : public Query {
public:
 typedef GraphType::VertexDescriptor vertex;
 
public:
  void runQuery(Graph & graph, Vertex startVertex, TMSwitch c ) {
    cout << "===============================\n";
    cout << "Query 5\n";
    Filter tmpFilter[2];
    traverseThroughTypeAndDirection("FRIENDS", "out",  tmpFilter[0]);
    traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[1]);
    FixedString key("name");
    cout << "The friends of Person with vid = " << startVertex << " has friends\n";
    switch(c) {
      case 1: {
        cout << "---------------------BFS---------------------\n";
        ReachabilityVisitor  v5b;
        v5b.setFilter(tmpFilter[0]);
        v5b.setFilter(tmpFilter[1]);
        v5b.setDepth(2);
        breadth_first_search(graph, startVertex, v5b);
        auto target = v5b.getVertexTargetList();
        for(auto it = target.begin(); it != target.end(); ++it) {
          cout <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue(key).first;
        cout << endl;
        }
        break;
              }
      case 2: {
        cout << "---------------------DFS---------------------\n";
        DFSReachabilityVisitor v5d;
        v5d.setFilter(tmpFilter[0]);
        v5d.setFilter(tmpFilter[1]);
        v5d.setDepth(2);
        depthFirstSearch(graph, startVertex, v5d);
        auto target = v5d.getVertexTargetMap();
        for(auto it = target.begin(); it != target.end(); ++it) {
          cout <<"Vertex " << (*it).first->getId() << "\t" << (*it).first->getPropertyValue(key).first;
        cout << endl;
        }
        break;
              }
      default:
        cout << "Traverse Method: 1. BFS  2. DFS\n"; 
    }

  }
};

class Query6: public Query {
public:
  void runQuery(Graph & graph, Vertex startVertex, TMSwitch c) {
    cout << "===============================\n";
    cout << "Query 6\n";
    FixedString key("wpurl");
    Filter tmpFilter[2];
    traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[0]);
    traverseThroughTypeAndDirection("LIKES", "out", tmpFilter[1]);
    switch(c) {
      case 1: {
        cout << "---------------------BFS---------------------\n";
        ReachabilityVisitor  v6b;
        v6b.setFilter(tmpFilter[0]);
        v6b.setFilter(tmpFilter[1]);
        v6b.setDepth(2);
        breadth_first_search(graph, startVertex, v6b);
        auto target = v6b.getVertexTargetList();
        cout << "The friends of Person with vid = " << startVertex << " like " << target.size() << " webpages\n";
        for(auto it = target.begin(); it != target.end(); ++it) {
          cout <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue(key).first;
        cout << endl;
        }
        break;
              }
      case 2: {
        cout << "---------------------DFS---------------------\n";
        DFSReachabilityVisitor v6d;
        v6d.setFilter(tmpFilter[0]);
        v6d.setFilter(tmpFilter[1]);
        v6d.setDepth(2);
        depthFirstSearch(graph, startVertex, v6d);
        auto target = v6d.getVertexTargetMap();
        cout << "The friends of Person with vid = " << startVertex << " like " << target.size() << " webpages\n";
        for(auto it = target.begin(); it != target.end(); ++it) {
          cout <<"Vertex " << (*it).first->getId() << "\t" << (*it).first->getPropertyValue(key).first;
        cout << endl;
        }
        break;
              }
      default:
        cout << "Traverse Method: 1. BFS  2. DFS\n"; 
    }
  }
};

class Query7: public Query {
public:
  void runQuery(Graph & graph, Vertex startVertex, TMSwitch c ) {
    cout << "===============================\n";
    cout << "Query 7\n";
    Filter tmpFilter[2];
    traverseThroughType("LIKES", tmpFilter[0]);
    traverseThroughType("LIKES", tmpFilter[1]);
    FixedString key("name");
    switch(c) {
      case 1: {
        cout << "---------------------BFS---------------------\n";
        ReachabilityVisitor  v7b;
        v7b.setFilter(tmpFilter[0]);
        v7b.setFilter(tmpFilter[1]);
        v7b.setDepth(2);
        breadth_first_search(graph, startVertex, v7b);
        auto target = v7b.getVertexTargetList();
        cout << "The webpages liked by person vid = " << startVertex << " are liked by " << target.size() << " people: \n";
        for(auto it = target.begin(); it != target.end(); ++it) {
          cout <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue(key).first;
        cout << endl;
        }
        break;
              }
      case 2: {
        cout << "---------------------DFS---------------------\n";
        DFSReachabilityVisitor v7d;
        v7d.setFilter(tmpFilter[0]);
        v7d.setFilter(tmpFilter[1]);
        v7d.setDepth(2);
        depthFirstSearch(graph, startVertex, v7d);
        auto target = v7d.getVertexTargetMap();
        cout << "The friends of Person with vid = " << startVertex << " like " << target.size() << " webpages\n";
        for(auto it = target.begin(); it != target.end(); ++it) {
          cout <<"Vertex " << (*it).first->getId() << "\t" << (*it).first->getPropertyValue(key).first;
        cout << endl;
        }
        break;
              }
      default:
        cout << "Traverse Method: 1. BFS  2. DFS\n"; 
    }

  }
};

class Query8 : public Query {
public:
  void runQuery(Graph & graph, Vertex startVertex, Vertex endVertex, TMSwitch c) {
    cout << "===============================\n";
    cout << "Query 8\n";
    switch(c) {
      case 1: {
        cout << "---------------------BFS---------------------\n";
        PathVisitor v8b;
        v8b.setEndVertex(endVertex);
        breadth_first_search(graph, startVertex, v8b);
        auto target = v8b.getVertexTargetList();
        if(!target.empty())
          cout << "There is path from " << startVertex << " to " <<  endVertex << endl;
        else 
          cout << startVertex << " and " <<  endVertex <<" are not connected" << endl;
        break;
              }
      case 2: {
        cout << "---------------------DFS---------------------\n";
        DFSPathVisitor v8d;
        v8d.setEndVertex(endVertex);
        depthFirstSearch(graph, startVertex, v8d);
        auto target = v8d.getPathList();
        if(!target.empty())
          cout << "There is path from " << startVertex << " to " <<  endVertex << endl;
        else 
          cout << startVertex << " and " <<  endVertex <<" are not connected" << endl;
        break;
              }
      default:
        cout << "Traverse Method: 1. BFS  2. DFS\n"; 
    }
  }
};

class Query9: public Query {
public:
  void runQuery(Graph & graph, Vertex startVertex, Vertex endVertex, TMSwitch c ) {
    cout << "===============================\n";
    cout << "Query 9\n";
    PathVisitor v9;
    v9.setEndVertex(endVertex);
    switch(c) {
      case 1: {
        cout << "---------------------BFS---------------------\n";
        PathVisitor v9b;
        v9b.setEndVertex(endVertex);
        breadth_first_search(graph, startVertex, v9b);
        auto target = v9b.getVertexTargetList();
        if(target.empty())
          cout << startVertex << " and " <<  endVertex <<" are not connected" << endl;
        else {
            cout << "There are  shortest paths from " << startVertex << " to " <<  endVertex << endl;
          for(auto it = target.begin(); it != target.end(); ++it) {
            cout <<"Vertex " << (*it)->getId() << endl;
          }
        }
        break;
              }
      case 2: {
        cout << "---------------------DFS---------------------\n";
        DFSPathVisitor v9d;
        v9d.setEndVertex(endVertex);
        depthFirstSearch(graph, startVertex, v9d);
        auto target = v9d.getPathList();
        if(target.empty())
          cout << startVertex << " and " <<  endVertex <<" are not connected" << endl;
        else {
        cout << "There are " << target.size() << " shortest paths from " << startVertex << " to " <<  endVertex << endl;
         for(auto it = target.begin(); it != target.end(); ++it) {
            for (auto iter = (*it).second.begin(); iter != (*it).second.end(); ++iter)
              cout <<"Vertex " << (*iter)->getId() << endl;
          cout << endl;
         }
       }
        break;
              }
      default:
        cout << "Traverse Method: 1. BFS  2. DFS\n"; 
    }
  }
};

class Query10 : public Query {
public:
  typedef GraphType::VertexDescriptor vertex; 
public: 
  void runQuery(Graph & graph, Vertex startVertex, Vertex endVertex, TMSwitch c ) { 
    cout << "===============================\n"; 
    cout << "Query 10\n"; 
    Filter tmpFilter[2];
    traverseThroughTypeAndDirection("FRIENDS", "out",  tmpFilter[0]);
    traverseThroughTypeAndDirection("FRIENDS", "in", tmpFilter[1]);
    FixedString key("name");
    vector<VertexPointer> target;
    switch(c) {
      case 1: {
        cout << "---------------------BFS---------------------\n";
        PatternVisitor v10d;
        v10d.setFilter(tmpFilter[0]);
        v10d.setFilter(tmpFilter[1]);
        v10d.setDepth(2);
        v10d.setEndVertex(endVertex);
        breadth_first_search(graph, startVertex, v10d);
        target = v10d.getVertexTargetList();
        break;
              }
      case 2: {
        cout << "---------------------DFS---------------------\n";
        DFSPathVisitor v10d;
        v10d.setFilter(tmpFilter[0]);
        v10d.setFilter(tmpFilter[1]);
        v10d.setDepth(2);
        depthFirstSearch(graph, startVertex, v10d);
        target = v10d.getVertexTargetList();
        break;
              }
      default:
        cout << "Traverse Method: 1. BFS  2. DFS\n"; 
    }
    cout << "There are " << target.size() << " common friends between  " << startVertex << " and " <<  endVertex << endl;
    for(auto it = target.begin(); it != target.end(); ++it) {
      cout <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue(key).first;
    cout << endl;
    }
  }
};


class Query11: public Query {
public:
  void runQuery(Graph & graph, Vertex startVertex, Vertex endVertex, TMSwitch c) {
    cout << "===============================\n";
    cout << "Query 11\n";
    Filter tmpFilter[2];
    traverseThroughTypeAndDirection("LIKES", "out",  tmpFilter[0]);
    traverseThroughTypeAndDirection("LIKES", "in",   tmpFilter[1]);
    FixedString key("name");
    vector<VertexPointer> target;
    switch(c) {
      case 1: {
        cout << "---------------------BFS---------------------\n";
        PatternVisitor v11b;
        v11b.setFilter(tmpFilter[0]);
        v11b.setFilter(tmpFilter[1]);
        v11b.setDepth(2);
        v11b.setEndVertex(endVertex);
        breadth_first_search(graph, startVertex, v11b);
        target = v11b.getVertexTargetList();
        break;
              }
      case 2: {
        cout << "---------------------DFS---------------------\n";
        DFSPatternVisitor v11d;
        v11d.setFilter(tmpFilter[0]);
        v11d.setFilter(tmpFilter[1]);
        v11d.setDepth(2);
        depthFirstSearch(graph, startVertex, v11d);
        target = v11d.getVertexTargetList();
        break;
              }
      default:
        cout << "Traverse Method: 1. BFS  2. DFS\n"; 
    }
    cout << "There are " << target.size() << " common webpages liked by both " << startVertex << " and " <<  endVertex << endl;
    for(auto it = target.begin(); it != target.end(); ++it) {
      FixedString key("wpurl");
      cout <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue(key).first;
      cout << endl;
    }
  }

};

class Query12 : public Query {
public:
  typedef GraphType::VertexDescriptor Vertex;
public:
 void runQuery(Graph & graph, Vertex startVertex, TMSwitch c ) {
    cout << "===============================\n";
    cout << "Query 12 \n";
    AdjacencyVisitor v12; 
    traverseThroughType("FRIENDS", v12.getFilter());
    switch(c) {
      case 1:
        cout << "---------------------BFS---------------------\n";
        breadth_first_search(graph, startVertex, v12);
        break;
      case 2:
        cout << "---------------------DFS---------------------\n";
        depthFirstSearch(graph, startVertex, v12);
        break;
      default:
        cout << "Traverse Method: 1. BFS  2. DFS\n"; 
    }
    FixedString key("name");
    cout << "Person with vid = " << startVertex << " has name: " << graph.getVertexPointer(startVertex)->getPropertyValue(key).first <<" and  " << v12.getVertexTargetList().size() << " friends\n";
 }
};

class Query13: public Query {
public:
 typedef GraphType::VertexDescriptor vertex;
 
public:
  void runQuery(Graph & graph, Vertex startVertex, TMSwitch c ) {
    cout << "===============================\n";
    cout << "Query 13\n";
    Filter tmpFilter[3];
    traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[0]);
    traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[1]);
    traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[2]);
    FixedString key("name");
    switch(c) {
      case 1: {
        cout << "---------------------BFS---------------------\n";
        ReachabilityVisitor  v13b;
        v13b.setFilter(tmpFilter[0]);
        v13b.setFilter(tmpFilter[1]);
        v13b.setFilter(tmpFilter[2]);
        v13b.setDepth(3);
        breadth_first_search(graph, startVertex, v13b);
        auto target = v13b.getVertexTargetList();
        cout << "The friends of friends of person vid = " << startVertex << " has " << target.size() << " friends: \n";
        for(auto it = target.begin(); it != target.end(); ++it) {
          cout <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue(key).first;
        cout << endl;
    }

        break;
              }
      case 2: {
        cout << "---------------------DFS---------------------\n";
        DFSReachabilityVisitor v13d;
        v13d.setFilter(tmpFilter[0]);
        v13d.setFilter(tmpFilter[1]);
        v13d.setFilter(tmpFilter[2]);
        v13d.setDepth(3);
        depthFirstSearch(graph, startVertex, v13d);
        auto target = v13d.getVertexTargetMap();
        cout << "The friends of friends of person vid = " << startVertex << " has " << target.size() << " friends: \n";
        for(auto it = target.begin(); it != target.end(); ++it) {
        if( (*it).first->getId() != startVertex ) {
        cout <<"Vertex " << (*it).first->getId() << "\t"  << (*it).first->getPropertyValue(key).first;
      cout << endl;
      }
    }
        break;
              }
      default:
        cout << "Traverse Method: 1. BFS  2. DFS\n"; 
    }
  }
};

#endif 

