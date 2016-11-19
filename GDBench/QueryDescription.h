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
#include "RecursiveDFSCustomVisitor.h"
#include "RecursiveDepthFirstSearch.h"

//#define _PRINTGDB_  true

#if _TIME_QUERY_
#define CLOCK_ID  CLOCK_THREAD_CPUTIME_ID
#define NANO 1000000000 
#define MILLION 1000000
#define SCALE 1000
#endif

#ifdef _PRINTGDB_
#include <fstream>
std::ofstream GDFile;
#endif

class Query {
public:
  typedef std::string KeyType;
  typedef std::string ValueType;
  typedef GraphType Graph;
  /// TraversalMethodSwitch: 1: bfs 2: dfs 3: recursive dfs
  typedef unsigned int  TraversalType; 
  typedef GraphType::VertexDescriptor VertexDescriptor;
public:
  Query() { }
  Query(unsigned int Id) : QueryId(Id) { }
  
  virtual void runQuery(Graph & graph, TraversalType Traversal) { }

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

#if _TIME_QUERY_
  void getExecTime() {
    End = (struct timespec){ 0 };
    if ( clock_gettime( CLOCK_ID, &End) == -1) {
      std::cout << "Query\t" << QueryId << "\tCould NOT get exec time\n"; 
      exit(0);
    }

    uint64_t execTime = ( (End.tv_sec - Start.tv_sec) * NANO
                  + ( End.tv_nsec - Start.tv_nsec) )/ SCALE;

    std::cout << "Query\t" << QueryId << "\t" << execTime << "\n"; 
  }

  void getStartTime() { 
    Start = (struct timespec){ 0 };
    if ( clock_gettime( CLOCK_ID, &Start ) == -1 ) {
      std::cout  << "Fail to get start time\n";
      exit(0);
    } 
  }
#endif 
  void getQueryId(unsigned int QId) {
    QueryId = QId;
  }

protected:
  KeyType   Key;
  ValueType Value;
  VertexDescriptor WebId;
  VertexDescriptor PersonId;
  VertexDescriptor PersonId1;
  VertexDescriptor PersonId2;
  unsigned int QueryId;
#if _TIME_QUERY_
  struct timespec Start;
  struct timespec End;
#endif 
};

class Query14 : public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType Traversal) {
    getQueryId(14);
#if _TIME_QUERY_
    getStartTime();
#endif
    Visitor v1;
    switch(Traversal) {
      case 1:
        filtProperty(Key, Value, v1.getFilter());
        breadthFirstSearch(graph, 0, v1 );
        break;
      case 2:
        filtProperty(Key, Value, v1.getFilter());
        depthFirstSearch(graph, 0, v1);
        break;
      case 3:
        filtProperty(Key, Value, v1.getFilter());
        recursiveDepthFirstSearch(graph, 0, v1);
        break;
    }
#if _TIME_QUERY_
    getExecTime();
#endif

#ifdef _PRINTGDB_
  GDFile.open("gd_execution_"+std::to_string(QueryId)+".log", std::ios_base::out | std::ios_base::app);
  GDFile << "Query 14\n";
  if(Traversal == 1)
      GDFile << "---------------------BFS---------------------\n";
   else if (Traversal == 2)
       GDFile << "---------------------DFS---------------------\n";
   else
       GDFile << "----------------recursiveDFS-----------------\n";
  GDFile << "Traversal from vertex 0.\n";
  GDFile.close();
#endif
 }

};


class Query1 : public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType Traversal) {
    getQueryId(1);
#if _TIME_QUERY_
    getStartTime();
#endif
    SelectionVisitor v1;
    filtProperty(Key, Value, v1.getFilter());
    traverseThroughType("FRIENDS", v1.getFilter());
    switch(Traversal) {
      case 1:
        breadthFirstSearch(graph, 0, v1 );
        break;
      case 2:
        depthFirstSearch(graph, 0, v1);
        break;
      case 3:
        recursiveDepthFirstSearch(graph, 0, v1);
    }

#if _TIME_QUERY_
    getExecTime();
#endif

#ifdef _PRINTGDB_
    GDFile.open("gd_execution_"+std::to_string(QueryId)+".log", std::ios_base::out | std::ios_base::app);
    GDFile << "Query 1\n";
    if(Traversal == 1)
        GDFile << "---------------------BFS---------------------\n";
    else if (Traversal == 2)
        GDFile << "---------------------DFS---------------------\n";
    else 
        GDFile << "----------------recursiveDFS-----------------\n";
    GDFile <<"People with " << Key << " = " 
           << Value <<" is(are) as below\n";
    auto target = v1.getVertexList();
    for(auto it = target.begin(); it != target.end(); ++it) {
      GDFile << "Vertex " << (*it)->getId();
      GDFile << "\n";
    }
    GDFile << "\n";;
    GDFile.close();
#endif
 }

};


class Query2 : public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType Traversal) {
    getQueryId(2);
#if _TIME_QUERY_
    getStartTime();
#endif
    AdjacencyVisitor v2; 
    traverseThroughTypeAndDirection("LIKES","in", v2.getFilter());
    switch(Traversal) {
      case 1:
        breadthFirstSearch(graph, WebId, v2);
        break;
      case 2:
        depthFirstSearch(graph, WebId, v2);
        break;
      case 3:
        recursiveDepthFirstSearch(graph,WebId, v2);
        break;
    }

#if _TIME_QUERY_
    getExecTime();
#endif

#ifdef _PRINTGDB_
    GDFile.open("gd_execution_"+std::to_string(QueryId)+".log", std::ios_base::out | std::ios_base::app);
    GDFile << "Query 2\n";
    if(Traversal == 1)
        GDFile << "---------------------BFS---------------------\n";
    else if (Traversal == 2)
        GDFile << "---------------------DFS---------------------\n";
    else 
        GDFile << "----------------recursiveDFS-----------------\n";

    GDFile << "People who likes webpage id = " 
           << WebId  << " are as below\n";
    auto target= v2.getVertexSet();
    for(auto it = target.begin(); it != target.end(); ++it) {
      std::string key("name");
      GDFile << "Vertex " << (*it)->getId() <<"\t" 
             << (*it)->getPropertyValue(key).first;
      GDFile << "\n";
    }
    GDFile << "\n";
    GDFile.close();
#endif
 }
};


class Query3 : public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType Traversal ) {
    getQueryId(3);
#if _TIME_QUERY_
    getStartTime();
#endif

    AdjacencyVisitor v3; 
    traverseThroughType("LIKES", v3.getFilter());
    switch(Traversal) {
      case 1:
        breadthFirstSearch(graph, PersonId, v3);
        break;
      case 2:
        depthFirstSearch(graph, PersonId, v3);
        break;
      case 3:
        recursiveDepthFirstSearch(graph, PersonId, v3);
        break;
    }
#if _TIME_QUERY_
    getExecTime();
#endif

#ifdef _PRINTGDB_
    GDFile.open("gd_execution_"+std::to_string(QueryId)+".log", std::ios_base::out | std::ios_base::app);
    GDFile << "Query 3\n";
    if (Traversal == 1)
        GDFile << "---------------------BFS---------------------\n";
    else if (Traversal == 2)
        GDFile << "---------------------DFS---------------------\n";
    else
        GDFile << "----------------recursiveDFS-----------------\n";

    auto target = v3.getVertexSet();
    GDFile << "Person with vid = " << PersonId << " likes webpages: " 
           << target.size() <<" in totoal \n";
    for(auto it = target.begin(); it != target.end(); ++it) {
      std::string key("URL");
      GDFile <<"Vertex " << (*it)->getId() << "\t"  
             << (*it)->getPropertyValue(key).first;
      GDFile << "\n";
    }
    GDFile << "\n";
    GDFile.close();
#endif
 }
};

class Query4 : public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType Traversal) {
    getQueryId(4);
#if _TIME_QUERY_
    getStartTime();
#endif
    SelectionVisitor  v4;
    filtProperty(Key, Value, v4.getFilter());
    traverseThroughType("FRIENDS", v4.getFilter());
    switch(Traversal) {
      case 1:
        breadthFirstSearch(graph, 0, v4);
        break;
      case 2:
        depthFirstSearch(graph, 0, v4);
        break;
      case 3:
        recursiveDepthFirstSearch(graph, 0, v4);
        break;
    }
#if _TIME_QUERY_
    getExecTime();
#endif

#ifdef _PRINTGDB_
    GDFile.open("gd_execution_"+std::to_string(QueryId)+".log", std::ios_base::out | std::ios_base::app);
    GDFile << "Query 4\n";
    if(Traversal == 1)
        GDFile << "---------------------BFS---------------------\n";
    else if (Traversal == 2)
        GDFile << "---------------------DFS---------------------\n";
    else 
        GDFile << "----------------recursiveDFS-----------------\n";
    GDFile <<"People with " << Key << " = " << Value <<" is(are) as below\n";
    auto target = v4.getVertexSet();
    for(auto it = target.begin(); it != target.end(); ++it) {
      std::string key("name");
      GDFile <<"Vertex " << (*it)->getId() << "\t" 
             << (*it)->getPropertyValue(key).first;
      GDFile << "\n";
    }
    GDFile.close();
#endif
 }
};

class Query5 : public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType Traversal ) {
    getQueryId(5);
#ifdef _PRINTGDB_
    GDFile.open("gd_execution_"+std::to_string(QueryId)+".log", std::ios_base::out | std::ios_base::app);
    GDFile << "Query 5\n";
    if (Traversal == 1)
        GDFile << "---------------------BFS---------------------\n";
    else if (Traversal == 2)
       GDFile << "----------------------DFS---------------------\n";
    else
       GDFile << "-----------------recursiveDFS-----------------\n";
#endif

#if _TIME_QUERY_
    getStartTime();
#endif

    FilterType tmpFilter[2];
    std::string key("name");
    std::vector<VertexPointer> target;
    traverseThroughTypeAndDirection("FRIENDS", "out",  tmpFilter[0]);
    traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[1]);
    switch(Traversal) {
      case 1: {
        ReachabilityVisitor  v5b;
        v5b.setFilter(tmpFilter[0]);
        v5b.setFilter(tmpFilter[1]);
        v5b.setDepth(2);
        breadthFirstSearch(graph, PersonId, v5b);
#ifdef _PRINTGDB_
        auto TargetSet = v5b.getTargetSet(); 
        GDFile << "The friends of Person with vid = " << PersonId 
               << " has friends " << TargetSet.size() << " in total\n";
        for(auto it = TargetSet.begin(); it != TargetSet.end(); ++it) {
          GDFile <<"Vertex " << (*it)->getId() << "\t" 
                 << (*it)->getPropertyValue(key).first << "\n";
        }
        GDFile.close();
#endif
        break;
              }
      case 2: {
        DFSReachabilityVisitor v5d;
        v5d.setFilter(tmpFilter[0]);
        v5d.setFilter(tmpFilter[1]);
        v5d.setDepth(2);
        depthFirstSearch(graph, PersonId, v5d);
#ifdef _PRINTGDB_
        auto target = v5d.getVertexTargetMap();
        GDFile << "The friends of Person with vid = " << PersonId 
              << " has friends " << target.size() << " in total\n";
        for(auto it = target.begin(); it != target.end(); ++it) {
          GDFile <<"Vertex " << (*it).first->getId() << "\t" 
                 << (*it).first->getPropertyValue(key).first;
          GDFile << "\n";
        }
        GDFile.close();
#endif
        break;
              }
      case 3: {
        RecursiveDFSReachabilityVisitor v5r;
        v5r.setFilter(tmpFilter[0]);
        v5r.setFilter(tmpFilter[1]);
        v5r.setDepth(2);
        recursiveDepthFirstSearch(graph, PersonId, v5r);
#ifdef _PRINTGDB_
        auto target = v5r.getTargetSet();
        GDFile << "The friends of Person with vid = " << PersonId 
              << " has friends " << target.size() <<" in total\n";
        for(auto it = target.begin(); it != target.end(); ++it) {
          GDFile <<"Vertex " << (*it)->getId() << "\t" 
                 << (*it)->getPropertyValue(key).first;
          GDFile << "\n";
        }
        GDFile.close();
#endif
        break;
              }
    }

#if _TIME_QUERY_
    getExecTime();
#endif

  }
};

class Query6: public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType Traversal) {
    getQueryId(6);
#ifdef _PRINTGDB_
    GDFile.open("gd_execution_"+std::to_string(QueryId)+".log", std::ios_base::out | std::ios_base::app);
    GDFile << "Query 6\n";
    if(Traversal == 1)
      GDFile << "---------------------BFS---------------------\n";
    else if (Traversal == 2)
      GDFile << "---------------------DFS---------------------\n";
    else
      GDFile << "----------------recursiveDFS-----------------\n";
#endif

#if _TIME_QUERY_
    getStartTime();
#endif
    std::string key("URL");
    FilterType tmpFilter[2];
    traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[0]);
    traverseThroughTypeAndDirection("LIKES", "out", tmpFilter[1]);
    switch(Traversal) {
      case 1: {
        ReachabilityVisitor  v6b;
        v6b.setFilter(tmpFilter[0]);
        v6b.setFilter(tmpFilter[1]);
        v6b.setDepth(2);
        breadthFirstSearch(graph, PersonId, v6b);

#ifdef _PRINTGDB_
        auto TargetSet = v6b.getTargetSet(); 
        GDFile << "The friends of Person with vid = " << PersonId 
               << " like " << TargetSet.size() << " webpages\n";
        for(auto it = TargetSet.begin(); it != TargetSet.end(); ++it) {
          GDFile <<"Vertex " << (*it)->getId() << "\t" 
                 << (*it)->getPropertyValue(key).first << "\n";
        }
        GDFile.close();
#endif
        break;
              }
      case 2: {
        DFSReachabilityVisitor v6d;
        v6d.setFilter(tmpFilter[0]);
        v6d.setFilter(tmpFilter[1]);
        v6d.setDepth(2);
        depthFirstSearch(graph, PersonId, v6d);

#ifdef _PRINTGDB_
        auto target = v6d.getVertexTargetMap();
        GDFile << "The friends of Person with vid = " << PersonId 
               << " like " << target.size() << " webpages\n";
        for(auto it = target.begin(); it != target.end(); ++it) {
          GDFile <<"Vertex " << (*it).first->getId() << "\t" 
                 << (*it).first->getPropertyValue(key).first;
        GDFile << "\n";
        }
        GDFile.close();
#endif
        break;
              }

      case 3: {
        RecursiveDFSReachabilityVisitor v6r;
        v6r.setFilter(tmpFilter[0]);
        v6r.setFilter(tmpFilter[1]);
        v6r.setDepth(2);
        recursiveDepthFirstSearch(graph, PersonId, v6r);

#ifdef _PRINTGDB_
        auto target = v6r.getTargetSet();
        GDFile << "The friends of Person with vid = " << PersonId 
               << " like " << target.size() << " webpages\n";
        for(auto it = target.begin(); it != target.end(); ++it) {
          GDFile <<"Vertex " << (*it)->getId() << "\t" 
                 << (*it)->getPropertyValue(key).first;
        GDFile << "\n";
        }
        GDFile.close();
#endif
        break;
              }
    }
#if _TIME_QUERY_
    getExecTime();
#endif
  }
};

class Query7: public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType Traversal ) {
    getQueryId(7);
#ifdef _PRINTGDB_
    GDFile.open("gd_execution_"+std::to_string(QueryId)+".log", std::ios_base::out | std::ios_base::app);
    GDFile << "Query 7\n";
    if(Traversal == 1)
        GDFile << "---------------------BFS---------------------\n";
    else if (Traversal == 2)
       GDFile << "---------------------DFS---------------------\n";
    else
       GDFile << "----------------recursiveDFS-----------------\n";
#endif

#if _TIME_QUERY_
    getStartTime();
#endif

    FilterType tmpFilter[2];
    std::string key("name");
//    traverseThroughType("LIKES", tmpFilter[0]);
//    traverseThroughType("LIKES", tmpFilter[1]);
    traverseThroughTypeAndDirection("LIKES", "out", tmpFilter[0]);
    traverseThroughTypeAndDirection("LIKES", "in", tmpFilter[1]);
    switch(Traversal) {
      case 1: {
        ReachabilityVisitor  v7b;
        v7b.setFilter(tmpFilter[0]);
        v7b.setFilter(tmpFilter[1]);
        v7b.setDepth(2);
        breadthFirstSearch(graph, PersonId, v7b);

#ifdef _PRINTGDB_
        auto TargetSet = v7b.getTargetSet(); 
        GDFile << "The webpages liked by person vid = " << PersonId 
               << " are liked by " << TargetSet.size() << " people: \n";
        for(auto it = TargetSet.begin(); it != TargetSet.end(); ++it) {
          GDFile <<"Vertex " << (*it)->getId() << "\t" 
                 << (*it)->getPropertyValue(key).first << "\n";
        }
        GDFile.close();
#endif
        break;
              }
      case 2: {
        DFSReachabilityVisitor v7d;
        v7d.setFilter(tmpFilter[0]);
        v7d.setFilter(tmpFilter[1]);
        v7d.setDepth(2);
        depthFirstSearch(graph, PersonId, v7d);

#ifdef _PRINTGDB_
        auto target = v7d.getVertexTargetMap();
        GDFile << "The webpages liked by person vid = " << PersonId 
               << " are liked by  " << target.size() << " people\n";
        for(auto it = target.begin(); it != target.end(); ++it) {
          GDFile <<"Vertex " << (*it).first->getId() << "\t" 
                 << (*it).first->getPropertyValue(key).first;
          GDFile << "\n";
        }
        GDFile.close();
#endif
        break;
              }

      case 3: {
        RecursiveDFSReachabilityVisitor v7r;
        v7r.setFilter(tmpFilter[0]);
        v7r.setFilter(tmpFilter[1]);
        v7r.setDepth(2);
        recursiveDepthFirstSearch(graph, PersonId, v7r);

#ifdef _PRINTGDB_
        auto target = v7r.getTargetSet();
        GDFile << "The webpages liked by person vid = " << PersonId 
               << " are liked by  " << target.size() << " people\n";
        for (auto it = target.begin(); it != target.end(); ++it) {
          GDFile <<"Vertex " << (*it)->getId() << "\t" 
                 << (*it)->getPropertyValue(key).first;
          GDFile << "\n";
        }
        GDFile.close();
#endif
        break;
              }
    }

#if _TIME_QUERY_
    getExecTime();
#endif

  }
};

class Query8 : public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType Traversal) {
    getQueryId(8);
#ifdef _PRINTGDB_
    GDFile.open("gd_execution_"+std::to_string(QueryId)+".log", std::ios_base::out | std::ios_base::app);
    GDFile << "Query 8\n";
    if(Traversal == 1)
        GDFile << "---------------------BFS---------------------\n";
    else if (Traversal == 2)
        GDFile << "---------------------DFS---------------------\n";
    else
        GDFile << "----------------recursiveDFS-----------------\n";
#endif

#if _TIME_QUERY_
    getStartTime();
#endif

    switch(Traversal) {
      case 1: {
        PathVisitor v8b;
        v8b.setEndVertex(PersonId2);
        breadthFirstSearch(graph, PersonId1, v8b);
#ifdef _PRINTGDB_
        auto target = v8b.getVertexList();
        if(!target.empty())
          GDFile << "There is path from " << PersonId1 << " to " <<  PersonId2 << "\n";
        else 
          GDFile << PersonId1 << " and " <<  PersonId2 <<" are not connected" << "\n";

        GDFile.close();
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
          GDFile << "There is path from " << PersonId1 << " to " <<  PersonId2 << "\n";
        else 
          GDFile << PersonId1 << " and " <<  PersonId2 <<" are not connected" << "\n";

        GDFile.close();
#endif
        break;
              }
      case 3: {
        RecursiveDFSPathVisitor v8r;
        v8r.setEndVertex(PersonId2);
        recursiveDepthFirstSearch(graph, PersonId1, v8r);
#ifdef _PRINTGDB_
        auto target = v8r.getTargetPath();
        if(!target.empty())
          GDFile << "There is path from " << PersonId1 << " to " <<  PersonId2 << "\n";
        else 
          GDFile << PersonId1 << " and " <<  PersonId2 <<" are not connected" << "\n";

        GDFile.close();
#endif
        break;
              }
    }
#if _TIME_QUERY_
    getExecTime();
#endif
  }
};

class Query9: public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType Traversal ) {
    getQueryId(9);

#ifdef _PRINTGDB_
    GDFile.open("gd_execution_"+std::to_string(QueryId)+".log", std::ios_base::out | std::ios_base::app);
    GDFile << "Query 9\n";
    if(Traversal == 1)
        GDFile << "---------------------BFS---------------------\n";
    else if (Traversal == 2)
        GDFile << "---------------------DFS---------------------\n";
    else
        GDFile << "----------------recursiveDFS-----------------\n";
#endif
    //PathVisitor v9;
    //v9.setEndVertex(PersonId2);
    //
#if _TIME_QUERY_
    getStartTime();
#endif
    switch(Traversal) {
      case 1: {
        PathVisitor v9b;
        v9b.setEndVertex(PersonId2);
        breadthFirstSearch(graph, PersonId1, v9b);
#ifdef _PRINTGDB_
        auto target = v9b.getVertexList();
        if(target.empty())
          GDFile << PersonId1 << " and " <<  PersonId2 <<" are not connected" << "\n";
        else {
            GDFile << "There are  shortest paths from " << PersonId1 
                   << " to " <<  PersonId2 << "\n";
          for(auto it = target.begin(); it != target.end(); ++it) {
            GDFile <<"Vertex " << (*it)->getId() << "\n";
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
          GDFile << PersonId1 << " and " <<  PersonId2 <<" are not connected" << "\n";
        else {
          GDFile << "There are " << target.size() << " shortest paths from " 
                 << PersonId1 << " to " <<  PersonId2 << "\n";
          for(auto it = target.begin(); it != target.end(); ++it) {
            for (auto iter = (*it).second.begin(); iter != (*it).second.end(); ++iter)
              GDFile <<"Vertex " << (*iter)->getId() << "\n";
          GDFile << "\n";
         }
       }
#endif
        break;
              }
      case 3: {
        RecursiveDFSShortestPathVisitor v9r;
        v9r.setEndVertex(PersonId2);
        recursiveDepthFirstSearch(graph, PersonId1, v9r);
#ifdef _PRINTGDB_
        auto Path = v9r.getShortestPath();
        if (Path.empty())
          GDFile << PersonId1 << " and " <<  PersonId2 <<" are not connected" << "\n";
        else {
          GDFile << "There is 1 shortest path from " 
                 << PersonId1 << " to " <<  PersonId2 << "\n";
          for (auto it = Path.begin(), it_end = Path.end(); 
                it != it_end; it++) {
            GDFile << "Vertex " << (*it)->getId() << "\n";
          }
        }
        GDFile.close();
#endif
        break;
              }
    }
#if _TIME_QUERY_
    getExecTime();
#endif
  }
};

class Query10 : public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType Traversal ) { 
    getQueryId(10);

#if _TIME_QUERY_
    getStartTime();
#endif
    FilterType tmpFilter[2];
    std::string key("name");
    std::unordered_set<VertexPointer> TargetSet;
    traverseThroughTypeAndDirection("FRIENDS", "out",  tmpFilter[0]);
    traverseThroughTypeAndDirection("FRIENDS", "in", tmpFilter[1]);
    switch(Traversal) {
      case 1: {
        PatternVisitor v10d;
        v10d.setFilter(tmpFilter[0]);
        v10d.setFilter(tmpFilter[1]);
        v10d.setDepth(2);
        v10d.setEndVertex(PersonId2);
        breadthFirstSearch(graph, PersonId1, v10d);
        TargetSet = v10d.getVertexSet();
        break;
              }
      case 2: {
        DFSPatternVisitor v10d;
        v10d.setFilter(tmpFilter[0]);
        v10d.setFilter(tmpFilter[1]);
        v10d.setDepth(2);
        v10d.setEndVertex(PersonId2);
        depthFirstSearch(graph, PersonId1, v10d);
        TargetSet = v10d.getVertexSet();
        break;
              }
      case 3: {
        RecursiveDFSPatternVisitor v10r;
        v10r.setFilter(tmpFilter[0]);
        v10r.setFilter(tmpFilter[1]);
        v10r.setEndVertex(PersonId2);
        recursiveDepthFirstSearch(graph, PersonId1, v10r);
        TargetSet = v10r.getVertexSet();
        break;
              }

    }

#if _TIME_QUERY_
    getExecTime();
#endif

#ifdef _PRINTGDB_
    GDFile.open("gd_execution_"+std::to_string(QueryId)+".log", std::ios_base::out | std::ios_base::app);
    GDFile << "Query 10\n";
    if(Traversal == 1)
        GDFile << "---------------------BFS---------------------\n";
    else if (Traversal == 2)
        GDFile << "---------------------DFS---------------------\n";
    else
        GDFile << "----------------recursiveDFS-----------------\n";

      GDFile << "There are " << TargetSet.size() << " common friends between  " 
             << PersonId1 << " and " <<  PersonId2 << "\n";
      for(auto it = TargetSet.begin(); it != TargetSet.end(); ++it) {
        GDFile << "Vertex " << (*it)->getId() << "\t" 
               << (*it)->getPropertyValue(key).first;
      GDFile << "\n";
      }
#endif
  }
};


class Query11: public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType Traversal) {
    getQueryId(11);
#if _TIME_QUERY_
    getStartTime();
#endif

    FilterType tmpFilter[2];
    std::string key("name");
    std::unordered_set<VertexPointer> target;
    traverseThroughTypeAndDirection("LIKES", "out",  tmpFilter[0]);
    traverseThroughTypeAndDirection("LIKES", "in",   tmpFilter[1]);
    switch(Traversal) {
      case 1: {
        PatternVisitor v11b;
        v11b.setFilter(tmpFilter[0]);
        v11b.setFilter(tmpFilter[1]);
        v11b.setDepth(2);
        v11b.setEndVertex(PersonId2);
        breadthFirstSearch(graph, PersonId1, v11b);
        target = v11b.getVertexSet();
        break;
              }
      case 2: {
        DFSPatternVisitor v11d;
        v11d.setFilter(tmpFilter[0]);
        v11d.setFilter(tmpFilter[1]);
        v11d.setDepth(2);
        v11d.setEndVertex(PersonId2);
        depthFirstSearch(graph, PersonId1, v11d);
        target = v11d.getVertexSet();
        break;
              }

      case 3: {
        RecursiveDFSPatternVisitor v11r;
        v11r.setFilter(tmpFilter[0]);
        v11r.setFilter(tmpFilter[1]);
        v11r.setEndVertex(PersonId2);
        recursiveDepthFirstSearch(graph, PersonId1, v11r);
        target = v11r.getVertexSet();
        break;
              }
    }

#if _TIME_QUERY_
    getExecTime();
#endif

#ifdef _PRINTGDB_
    GDFile.open("gd_execution_"+std::to_string(QueryId)+".log", std::ios_base::out | std::ios_base::app);
    GDFile << "Query 11\n";
    if(Traversal == 1)
        GDFile << "---------------------BFS---------------------\n";
    else if (Traversal == 2)
        GDFile << "---------------------DFS---------------------\n";
    else
        GDFile << "----------------recursiveDFS-----------------\n";
    GDFile << "There are " << target.size() 
           << " common webpages liked by both " 
           << PersonId1 << " and " <<  PersonId2 << "\n";
    for(auto it = target.begin(); it != target.end(); ++it) {
      std::string key("URL");
      GDFile << "Vertex " << (*it)->getId() << "\t" 
             << (*it)->getPropertyValue(key).first;
      GDFile << "\n";
    }
#endif

  }
};

class Query12 : public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType Traversal ) {

    getQueryId(12);
#if _TIME_QUERY_
    getStartTime();
#endif
    std::unordered_set<VertexPointer>  target;
    AdjacencyVisitor v12; 
    traverseThroughTypeAndDirection("FRIENDS", "out", v12.getFilter());
    switch(Traversal) {
      case 1: {
        breadthFirstSearch(graph, PersonId, v12);
        target = v12.getVertexSet();
        break;
              }
      case 2: {
        depthFirstSearch(graph, PersonId, v12);
        target = v12.getVertexSet();
        break;
              }
      case 3:
        recursiveDepthFirstSearch(graph, PersonId, v12);
        target = v12.getVertexSet();
        break;

    }

#if _TIME_QUERY_
    getExecTime();
#endif

#ifdef _PRINTGDB_
    GDFile.open("gd_execution_"+std::to_string(QueryId)+".log", std::ios_base::out | std::ios_base::app);
    GDFile << "Query 12\n";
    if (Traversal == 1)
        GDFile << "---------------------BFS---------------------\n";
    else if (Traversal == 2)
        GDFile << "---------------------DFS---------------------\n";
    else
        GDFile << "----------------recursiveDFS-----------------\n";

    std::string key("name");
    GDFile << "Person with vid = " << PersonId << " has name: " 
           << graph.getVertexPointer(PersonId)->getPropertyValue(key).first 
           <<" and  " << target.size() << " friends\n";
    for (auto it = target.begin(); it != target.end(); it++)
      GDFile << "Vertex " << (*it)->getId() << "\n";
    
    GDFile.close();
#endif
 }
};

class Query13: public Query {
public:
  virtual void runQuery(Graph & graph, TraversalType Traversal ) {
    getQueryId(13);
#ifdef _PRINTGDB_
    GDFile.open("gd_execution_"+std::to_string(QueryId)+".log", std::ios_base::out | std::ios_base::app);
    GDFile << "Query 13\n";
    if(Traversal == 1)
        GDFile << "---------------------BFS---------------------\n";
    else if (Traversal == 2)
        GDFile << "---------------------DFS---------------------\n";
    else
        GDFile << "----------------recursiveDFS-----------------\n";
#endif

#if _TIME_QUERY_
    getStartTime();
#endif

    FilterType tmpFilter[3];
    std::string key("name");
    traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[0]);
    traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[1]);
    traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter[2]);
    switch(Traversal) {
      case 1: {
        ReachabilityVisitor  v13b;
        v13b.setFilter(tmpFilter[0]);
        v13b.setFilter(tmpFilter[1]);
        v13b.setFilter(tmpFilter[2]);
        v13b.setDepth(3);
        breadthFirstSearch(graph, PersonId, v13b);

#ifdef _PRINTGDB_
        auto TargetSet = v13b.getTargetSet(); 
        GDFile << "The friends of friends of person vid = " << PersonId 
               << " has " << TargetSet.size() << " friends: \n";
        for(auto it = TargetSet.begin(); it != TargetSet.end(); ++it) {
          GDFile <<"Vertex " << (*it)->getId() << "\t" 
                << (*it)->getPropertyValue(key).first << "\n";
        }

        GDFile.close();
#endif
        break;
              }
      case 2: {
        DFSReachabilityVisitor v13d;
        v13d.setFilter(tmpFilter[0]);
        v13d.setFilter(tmpFilter[1]);
        v13d.setFilter(tmpFilter[2]);
        v13d.setDepth(3);
        depthFirstSearch(graph, PersonId, v13d);
#ifdef _PRINTGDB_
        auto target = v13d.getVertexTargetMap();
        GDFile << "The friends of friends of person vid = " << PersonId 
               << " has " << target.size() << " friends: \n";
        for(auto it = target.begin(); it != target.end(); ++it) {
        if( (*it).first->getId() != PersonId ) {
          GDFile << "Vertex " << (*it).first->getId() << "\t"  
               << (*it).first->getPropertyValue(key).first;
          GDFile << "\n";
          }
        }

        GDFile.close();
#endif
        break;
              }

      case 3: {
        RecursiveDFSReachabilityVisitor v13r;
        v13r.setFilter(tmpFilter[0]);
        v13r.setFilter(tmpFilter[1]);
        v13r.setFilter(tmpFilter[2]);
        v13r.setDepth(3);
        recursiveDepthFirstSearch(graph, PersonId, v13r);
#ifdef _PRINTGDB_
        auto target = v13r.getTargetSet();
        GDFile << "The friends of friends of person vid = " << PersonId 
               << " has " << target.size() << " friends: \n";
        for(auto it = target.begin(); it != target.end(); ++it) {
        if( (*it)->getId() != PersonId ) {
          GDFile << "Vertex " << (*it)->getId() << "\t"  
               << (*it)->getPropertyValue(key).first;
          GDFile << "\n";
          }
        }
        GDFile.close();
#endif
        break;
              }
    }

#if _TIME_QUERY_
    getExecTime();
#endif
  }
};

#endif /**_QUERYDESCRIPTION_H_ */
