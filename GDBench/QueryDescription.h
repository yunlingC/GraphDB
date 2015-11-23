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

#include "BreadthFirstSearch.h"
#include "DepthFirstSearch.h"
#include "CustomVisitor.h"

#include <fstream>

#ifdef _PRINTGDB_
std::ofstream myfile("gd_execution.log", std::ios_base::out 
                                       | std::ios_base::app);
#endif

class Query {
public:
  typedef std::string KeyType;
  typedef std::string ValueType;
  typedef GraphType Graph;
  /// TraversalMethodSwitch: 1: bfs 2: dfs
  typedef unsigned int  TraversalType; 
  typedef GraphType::VertexDescriptor VertexDescriptor;
public:
  Query() { }
  
  virtual void runQuery(Graph & graph, TraversalType c) { }

  void setPersonProperty(const KeyType & key, const ValueType & value) {
    Key = key;
    Value = value;
  }

  void setPersonId (VertexDescriptor Id) {
    PersonId = Id; 
  }

  void setEndPoints(const VertexDescriptor & Id1, const VertexDescriptor & Id2) {
    PersonId1 = Id1;
    PersonId2 = Id2;
  }

  void setWebId(VertexDescriptor webId) {
    WebId = webId;
  }
protected:
  KeyType   Key;
  ValueType Value;
  VertexDescriptor WebId;
  VertexDescriptor PersonId;
  VertexDescriptor PersonId1;
  VertexDescriptor PersonId2;
};

class Query14 : public Query {
public:
 virtual void runQuery(Graph & graph, TraversalType c) {
    Visitor v1;
    switch(c) {
      case 1:
        filtProperty(Key, Value, v1.getFilter());
        breadthFirstSearch(graph, 0, v1 );
        break;
      case 2:
        filtProperty(Key, Value, v1.getFilter());
        depthFirstSearch(graph, 0, v1);
        break;
    }

#ifdef _PRINTGDB_
    myfile << "Query 14\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    myfile << "Traversal from vertex 0.\n";
#endif
 }

};


class Query1 : public Query {
public:
 virtual void runQuery(Graph & graph, TraversalType c) {
    SelectionVisitor v1;
    switch(c) {
      case 1:
        filtProperty(Key, Value, v1.getFilter());
        traverseThroughType("FRIENDS", v1.getFilter());
        breadthFirstSearch(graph, 0, v1 );
        break;
      case 2:
        filtProperty(Key, Value, v1.getFilter());
        traverseThroughType("FRIENDS", v1.getFilter());
        depthFirstSearch(graph, 0, v1);
        break;
    }

#ifdef _PRINTGDB_
    myfile << "Query 1\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    myfile <<"People with " << Key << " = " 
           << Value <<" is(are) as below\n";
    auto target = v1.getVertexList();
    for(auto it = target.begin(); it != target.end(); ++it) {
      myfile << "Vertex " << (*it)->getId();
      myfile << "\n";
    }
    myfile << "\n";;
#endif
 }

};


class Query2 : public Query {
public:
 virtual void runQuery(Graph & graph, TraversalType c) {
    AdjacencyVisitor v2; 
    switch(c) {
      case 1:
        traverseThroughTypeAndDirection("LIKES","in", v2.getFilter());
        breadthFirstSearch(graph, WebId, v2);
        break;
      case 2:
        traverseThroughTypeAndDirection("LIKES","in", v2.getFilter());
        depthFirstSearch(graph, WebId, v2);
        break;
    }

#ifdef _PRINTGDB_
    myfile << "Query 2\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    myfile << "People who likes webpage id = " 
           << WebId  << " are as below\n";
    auto target= v2.getVertexList();
    for(auto it = target.begin(); it != target.end(); ++it) {
      std::string key("name");
      myfile << "Vertex " << (*it)->getId() <<"\t" 
             << (*it)->getPropertyValue(key).first;
      myfile << "\n";
    }
    myfile << "\n";
#endif
 }
};


class Query3 : public Query {
public:
 virtual void runQuery(Graph & graph, TraversalType c ) {
    AdjacencyVisitor v3; 
    switch(c) {
      case 1:
        traverseThroughType("LIKES", v3.getFilter());
        breadthFirstSearch(graph, PersonId, v3);
        break;
      case 2:
        traverseThroughType("LIKES", v3.getFilter());
        depthFirstSearch(graph, PersonId, v3);
        break;
    }

#ifdef _PRINTGDB_
    myfile << "Query 3\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    myfile << "Person with vid = " << PersonId << " likes webpages:\n";
    auto target = v3.getVertexList();
    for(auto it = target.begin(); it != target.end(); ++it) {
      std::string key("wpurl");
      myfile <<"Vertex " << (*it)->getId() << "\t"  
             << (*it)->getPropertyValue(key).first;
      myfile << "\n";
    }
    myfile << "\n";
#endif
 }
};

class Query4 : public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType c) {
    SelectionVisitor  v4;
    switch(c) {
      case 1:
        filtProperty(Key, Value, v4.getFilter());
        traverseThroughType("FRIENDS", v4.getFilter());
        breadthFirstSearch(graph, 0, v4);
        break;
      case 2:
        filtProperty(Key, Value, v4.getFilter());
        traverseThroughType("FRIENDS", v4.getFilter());
        depthFirstSearch(graph, 0, v4);
        break;
    }

#ifdef _PRINTGDB_
    myfile << "Query 4\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    myfile <<"People with " << Key << " = " << Value <<" is(are) as below\n";
    auto target = v4.getVertexList();
    for(auto it = target.begin(); it != target.end(); ++it) {
      std::string key("name");
      myfile <<"Vertex " << (*it)->getId() << "\t" 
             << (*it)->getPropertyValue(key).first;
      myfile << "\n";
    }
#endif
 }
};

class Query5 : public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType c ) {

#ifdef _PRINTGDB_
    myfile << "Query 5\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    myfile << "The friends of Person with vid = " << PersonId << " has friends\n";
#endif
    Filter tmpFilter[2];
    std::string key("name");
    std::vector<VertexPointer> target;
    switch(c) {
      case 1: {
        traverseThroughTypeAndDirection("FRIENDS", "out",  tmpFilter[0]);
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[1]);
        ReachabilityVisitor  v5b;
        v5b.setFilter(tmpFilter[0]);
        v5b.setFilter(tmpFilter[1]);
        v5b.setDepth(2);
        breadthFirstSearch(graph, PersonId, v5b);
#ifdef _PRINTGDB_
        auto TargetSet = v5b.getTargetSet(); 
        for(auto it = TargetSet.begin(); it != TargetSet.end(); ++it) {
          myfile <<"Vertex " << (*it)->getId() << "\t" 
                 << (*it)->getPropertyValue(key).first << "\n";
        }
#endif
        break;
              }
      case 2: {
        traverseThroughTypeAndDirection("FRIENDS", "out",  tmpFilter[0]);
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[1]);
        DFSReachabilityVisitor v5d;
        v5d.setFilter(tmpFilter[0]);
        v5d.setFilter(tmpFilter[1]);
        v5d.setDepth(2);
        depthFirstSearch(graph, PersonId, v5d);
        auto target = v5d.getVertexTargetMap();

#ifdef _PRINTGDB_
        myfile << "---------------------DFS---------------------\n";
        for(auto it = target.begin(); it != target.end(); ++it) {
          myfile <<"Vertex " << (*it).first->getId() << "\t" 
                 << (*it).first->getPropertyValue(key).first;
        myfile << "\n";
        }
#endif
        break;
              }
    }

  }
};

class Query6: public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType c) {

#ifdef _PRINTGDB_
    myfile << "Query 6\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
#endif
    std::string key("wpurl");
    Filter tmpFilter[2];
    switch(c) {
      case 1: {
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[0]);
        traverseThroughTypeAndDirection("LIKES", "out", tmpFilter[1]);
        ReachabilityVisitor  v6b;
        v6b.setFilter(tmpFilter[0]);
        v6b.setFilter(tmpFilter[1]);
        v6b.setDepth(2);
        breadthFirstSearch(graph, PersonId, v6b);

#ifdef _PRINTGDB_
        auto TargetSet = v6b.getTargetSet(); 
        myfile << "The friends of Person with vid = " << PersonId 
               << " like " << TargetSet.size() << " webpages\n";
        for(auto it = TargetSet.begin(); it != TargetSet.end(); ++it) {
          myfile <<"Vertex " << (*it)->getId() << "\t" 
                 << (*it)->getPropertyValue(key).first << "\n";
        }
#endif
        break;
              }
      case 2: {
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[0]);
        traverseThroughTypeAndDirection("LIKES", "out", tmpFilter[1]);
        DFSReachabilityVisitor v6d;
        v6d.setFilter(tmpFilter[0]);
        v6d.setFilter(tmpFilter[1]);
        v6d.setDepth(2);
        depthFirstSearch(graph, PersonId, v6d);

#ifdef _PRINTGDB_
        auto target = v6d.getVertexTargetMap();
        myfile << "The friends of Person with vid = " << PersonId 
               << " like " << target.size() << " webpages\n";
        for(auto it = target.begin(); it != target.end(); ++it) {
          myfile <<"Vertex " << (*it).first->getId() << "\t" 
                 << (*it).first->getPropertyValue(key).first;
        myfile << "\n";
        }
#endif
        break;
              }
    }
  }
};

class Query7: public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType c ) {

#ifdef _PRINTGDB_
    myfile << "Query 7\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
#endif
    Filter tmpFilter[2];
    std::string key("name");
    switch(c) {
      case 1: {
        traverseThroughType("LIKES", tmpFilter[0]);
        traverseThroughType("LIKES", tmpFilter[1]);
        ReachabilityVisitor  v7b;
        v7b.setFilter(tmpFilter[0]);
        v7b.setFilter(tmpFilter[1]);
        v7b.setDepth(2);
        breadthFirstSearch(graph, PersonId, v7b);

#ifdef _PRINTGDB_
        auto TargetSet = v7b.getTargetSet(); 
        myfile << "The webpages liked by person vid = " << PersonId 
               << " are liked by " << TargetSet.size() << " people: \n";
        for(auto it = TargetSet.begin(); it != TargetSet.end(); ++it) {
          myfile <<"Vertex " << (*it)->getId() << "\t" 
                 << (*it)->getPropertyValue(key).first << "\n";
        }
#endif
        break;
              }
      case 2: {
        traverseThroughType("LIKES", tmpFilter[0]);
        traverseThroughType("LIKES", tmpFilter[1]);
        DFSReachabilityVisitor v7d;
        v7d.setFilter(tmpFilter[0]);
        v7d.setFilter(tmpFilter[1]);
        v7d.setDepth(2);
        depthFirstSearch(graph, PersonId, v7d);

#ifdef _PRINTGDB_
        auto target = v7d.getVertexTargetMap();
        myfile << "The webpages liked by person vid = " << PersonId 
               << " are liked by  " << target.size() << " people\n";
        for(auto it = target.begin(); it != target.end(); ++it) {
          myfile <<"Vertex " << (*it).first->getId() << "\t" 
                 << (*it).first->getPropertyValue(key).first;
          myfile << "\n";
        }
#endif
        break;
              }
    }

  }
};

class Query8 : public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType c) {

#ifdef _PRINTGDB_
    myfile << "Query 8\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
#endif
    switch(c) {
      case 1: {
        PathVisitor v8b;
        v8b.setEndVertex(PersonId2);
        breadthFirstSearch(graph, PersonId1, v8b);
#ifdef _PRINTGDB_
        auto target = v8b.getVertexList();
        if(!target.empty())
          myfile << "There is path from " << PersonId1 << " to " <<  PersonId2 << "\n";
        else 
          myfile << PersonId1 << " and " <<  PersonId2 <<" are not connected" << "\n";
#endif
        break;
              }
      case 2: {
        DFSPathVisitor v8d;
        v8d.setEndVertex(PersonId2);
        depthFirstSearch(graph, PersonId1, v8d);
#ifdef _PRINTGDB_
        auto target = v8d.getPathList();
        if(!target.empty())
          myfile << "There is path from " << PersonId1 << " to " <<  PersonId2 << "\n";
        else 
          myfile << PersonId1 << " and " <<  PersonId2 <<" are not connected" << "\n";
#endif
        break;
              }
    }
  }
};

class Query9: public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType c ) {
#ifdef _PRINTGDB_
    myfile << "Query 9\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
#endif
    //PathVisitor v9;
    //v9.setEndVertex(PersonId2);
    switch(c) {
      case 1: {
        PathVisitor v9b;
        v9b.setEndVertex(PersonId2);
        breadthFirstSearch(graph, PersonId1, v9b);
#ifdef _PRINTGDB_
        auto target = v9b.getVertexList();
        if(target.empty())
          myfile << PersonId1 << " and " <<  PersonId2 <<" are not connected" << "\n";
        else {
            myfile << "There are  shortest paths from " << PersonId1 
                   << " to " <<  PersonId2 << "\n";
          for(auto it = target.begin(); it != target.end(); ++it) {
            myfile <<"Vertex " << (*it)->getId() << "\n";
          }
        }
#endif
        break;
              }
      case 2: {
        DFSShortestPathVisitor v9d;
        v9d.setEndVertex(PersonId2);
        depthFirstSearch(graph, PersonId1, v9d);
#ifdef _PRINTGDB_
        auto target = v9d.getPathList();
        if(target.empty())
          myfile << PersonId1 << " and " <<  PersonId2 <<" are not connected" << "\n";
        else {
          myfile << "There are " << target.size() << " shortest paths from " 
                 << PersonId1 << " to " <<  PersonId2 << "\n";
          for(auto it = target.begin(); it != target.end(); ++it) {
            for (auto iter = (*it).second.begin(); iter != (*it).second.end(); ++iter)
              myfile <<"Vertex " << (*iter)->getId() << "\n";
          myfile << "\n";
         }
       }
#endif
        break;
              }
    }
  }
};

class Query10 : public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType c ) { 
    Filter tmpFilter[2];
    std::string key("name");
    std::vector<VertexPointer> target;
    switch(c) {
      case 1: {
        traverseThroughTypeAndDirection("FRIENDS", "out",  tmpFilter[0]);
        traverseThroughTypeAndDirection("FRIENDS", "in", tmpFilter[1]);
        PatternVisitor v10d;
        v10d.setFilter(tmpFilter[0]);
        v10d.setFilter(tmpFilter[1]);
        v10d.setDepth(2);
        v10d.setEndVertex(PersonId2);
        breadthFirstSearch(graph, PersonId1, v10d);
        target = v10d.getVertexList();
        break;
              }
      case 2: {
        traverseThroughTypeAndDirection("FRIENDS", "out",  tmpFilter[0]);
        traverseThroughTypeAndDirection("FRIENDS", "in", tmpFilter[1]);
        DFSPatternVisitor v10d;
        v10d.setFilter(tmpFilter[0]);
        v10d.setFilter(tmpFilter[1]);
        v10d.setDepth(2);
        v10d.setEndVertex(PersonId2);
        depthFirstSearch(graph, PersonId1, v10d);
        target = v10d.getVertexList();
        break;
              }
    }
#ifdef _PRINTGDB_
    myfile << "Query 10\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    myfile << "There are " << target.size() << " common friends between  " 
           << PersonId1 << " and " <<  PersonId2 << "\n";
    for(auto it = target.begin(); it != target.end(); ++it) {
      myfile << "Vertex " << (*it)->getId() << "\t" 
             << (*it)->getPropertyValue(key).first;
    myfile << "\n";
    }
#endif
  }
};


class Query11: public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType c) {
    Filter tmpFilter[2];
    std::string key("name");
    std::vector<VertexPointer> target;
    switch(c) {
      case 1: {
        traverseThroughTypeAndDirection("LIKES", "out",  tmpFilter[0]);
        traverseThroughTypeAndDirection("LIKES", "in",   tmpFilter[1]);
        PatternVisitor v11b;
        v11b.setFilter(tmpFilter[0]);
        v11b.setFilter(tmpFilter[1]);
        v11b.setDepth(2);
        v11b.setEndVertex(PersonId2);
        breadthFirstSearch(graph, PersonId1, v11b);
        target = v11b.getVertexList();
        break;
              }
      case 2: {
        traverseThroughTypeAndDirection("LIKES", "out",  tmpFilter[0]);
        traverseThroughTypeAndDirection("LIKES", "in",   tmpFilter[1]);
        DFSPatternVisitor v11d;
        v11d.setFilter(tmpFilter[0]);
        v11d.setFilter(tmpFilter[1]);
        v11d.setDepth(2);
        v11d.setEndVertex(PersonId2);
        depthFirstSearch(graph, PersonId1, v11d);
        target = v11d.getVertexList();
        break;
              }
    }
#ifdef _PRINTGDB_
    myfile << "Query 11\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    myfile << "There are " << target.size() 
           << " common webpages liked by both " 
           << PersonId1 << " and " <<  PersonId2 << "\n";
    for(auto it = target.begin(); it != target.end(); ++it) {
      std::string key("wpurl");
      myfile << "Vertex " << (*it)->getId() << "\t" 
             << (*it)->getPropertyValue(key).first;
      myfile << "\n";
    }
#endif

  }
};

class Query12 : public Query {
public:
 virtual void runQuery(Graph & graph, TraversalType c ) {
   std::vector<VertexPointer>  target;
    switch(c) {
      case 1: {
        AdjacencyVisitor v12b; 
        traverseThroughTypeAndDirection("FRIENDS", "out", v12b.getFilter());
        breadthFirstSearch(graph, PersonId, v12b);
        target = v12b.getVertexList();
        break;
              }
      case 2: {
        AdjacencyVisitor v12d; 
        traverseThroughTypeAndDirection("FRIENDS", "out",  v12d.getFilter());
        depthFirstSearch(graph, PersonId, v12d);
        target = v12d.getVertexList();
        break;
              }
    }
#ifdef _PRINTGDB_
    myfile << "Query 12\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
    std::string key("name");
    myfile << "Person with vid = " << PersonId << " has name: " 
           << graph.getVertexPointer(PersonId)->getPropertyValue(key).first 
           <<" and  " << target.size() << " friends\n";
    for (auto it = target.begin(); it != target.end(); it++)
      myfile << "Vertex " << (*it)->getId() << "\n";
#endif
 }
};

class Query13: public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType c ) {
#ifdef _PRINTGDB_
    myfile << "Query 13\n";
    if(c == 1)
        myfile << "---------------------BFS---------------------\n";
    else
        myfile << "---------------------DFS---------------------\n";
#endif
    Filter tmpFilter[3];
    std::string key("name");
    switch(c) {
      case 1: {
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[0]);
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[1]);
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[2]);
        ReachabilityVisitor  v13b;
        v13b.setFilter(tmpFilter[0]);
        v13b.setFilter(tmpFilter[1]);
        v13b.setFilter(tmpFilter[2]);
        v13b.setDepth(3);
        breadthFirstSearch(graph, PersonId, v13b);
#ifdef _PRINTGDB_
        auto TargetSet = v13b.getTargetSet(); 
        myfile << "The friends of friends of person vid = " << PersonId 
               << " has " << TargetSet.size() << " friends: \n";
        for(auto it = TargetSet.begin(); it != TargetSet.end(); ++it) {
          myfile <<"Vertex " << (*it)->getId() << "\t" 
                << (*it)->getPropertyValue(key).first << "\n";
        }
#endif
        break;
              }
      case 2: {
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[0]);
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[1]);
        traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[2]);
        DFSReachabilityVisitor v13d;
        v13d.setFilter(tmpFilter[0]);
        v13d.setFilter(tmpFilter[1]);
        v13d.setFilter(tmpFilter[2]);
        v13d.setDepth(3);
        depthFirstSearch(graph, PersonId, v13d);
#ifdef _PRINTGDB_
        auto target = v13d.getVertexTargetMap();
        myfile << "The friends of friends of person vid = " << PersonId 
               << " has " << target.size() << " friends: \n";
        for(auto it = target.begin(); it != target.end(); ++it) {
        if( (*it).first->getId() != PersonId ) {
        myfile << "Vertex " << (*it).first->getId() << "\t"  
               << (*it).first->getPropertyValue(key).first;
      myfile << "\n";
        }
      }
#endif
        break;
              }
    }
  }
};

#endif /**_QUERYDESCRIPTION_H_ */
