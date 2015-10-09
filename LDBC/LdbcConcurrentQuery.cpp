//===-- LDBC/LdbcQueryDescription.cpp - Query class ----------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the query class for LDBC derived from LDBCConcurrnetQuery.
///
//===----------------------------------------------------------------------===//
#include <vector>
#include <string>
#include <time.h>
#include <unordered_map>
#ifdef _PRINTLOG_
  #include <iostream>
#endif

#include "LdbcConcurrentQuery.h"

class LdbcQuery1 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex, 
      LockManagerType & LockManager) {
//    LdbcFile << "===============Query 1================\n";
    getStartTime();  
    Filter tmpFilter;
    Filter NameFilter;
    filtProperty(ParamPair.first, ParamPair.second, NameFilter); 
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter);
    LimitedDepthVisitor  v1;
    for (auto i = 0; i < 3; i++) {
      v1.setFilter(tmpFilter);
    }
    v1.setNameFilter(NameFilter);
    v1.setDepth(3);
    ConcurrentBFS Cbfs; 
    Cbfs.breadthFirstSearch(graph, startVertex, v1, LockManager);

#ifdef _PRINTLOG_
    auto target = v1.getVertexTargetList(); 
    LdbcFile << startVertex << " is connected with " << target.size() << " people with " << ParamPair.first <<": " << ParamPair.second<< endl;
    for(auto it = target.begin(); it != target.end(); ++it) {
      LdbcFile << endl;

    Cbfs.releaseAll(LockManager);
    getExecTime();
    LdbcFile.close();
  }
};

class LdbcQuery2 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex, 
      LockManagerType & LockManager) {
//    LdbcFile << "===============Query 2================\n";
    getStartTime();  
    Filter tmpFilter[3];
    traverseThroughMultiRelType("KNOWS", tmpFilter[0]); 
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR", tmpFilter[1]); 
//  tmpFilter[2].setValueRange("creationDate", "", "2012-07-16T10:00:00.255"); 
    tmpFilter[2].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second); 
   
    MultiRelTypeVisitor v2;
    v2.setFilter(tmpFilter[0]);
    v2.setFilter(tmpFilter[1]);
    v2.setRangeFilter(tmpFilter[2]);
    v2.setDepth(2);
    v2.setDepthToCheckRange(2);
    v2.setPropToCheck(2); //check date
    ConcurrentBFS Cbfs;
    Cbfs.breadthFirstSearch(graph, startVertex, v2, LockManager);

#ifdef _PRINTLOG_
    auto target = v2.getVertexTargetList();
    auto targets = v2.getTargetsMap();
//    LdbcFile << startVertex, LockManagerType startVertex  LockManager<< " has friends made " << target.size() << " comments and posts \n";
    for(auto it = targets.begin(); it != targets.end(); ++it) {
      LdbcFile <<"person " << (*it).second->getPropertyValue("id").first << "\t" <<"comments/posts " << (*it).first->getPropertyValue("id").first << "\t" << (*it).first->getPropertyValue("creationDate").first ;
      LdbcFile << endl;
        }
#endif

    Cbfs.releaseAll(LockManager);
    getExecTime();  
    LdbcFile.close();
  }
};


class LdbcQuery3 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef map<VertexPointer, vector<VertexPointer>> PersonListMapType;
  typedef pair<VertexPointer, vector<VertexPointer>> PersonListPair;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex, 
      LockManagerType & LockManager) {
//    LdbcFile << "===============Query 3================\n";
    getStartTime();  
    Filter tmpFilter;
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter);

    SingleRelTypeVisitor v3;
    v3.setFilter(tmpFilter);
    ///cannot be deleted
    v3.setFilter(tmpFilter);
    v3.setDepth(2);
    ConcurrentBFS Cbfs;
    Cbfs.breadthFirstSearch(graph, startVertex, v3, LockManager);
    auto target = v3.getVertexTargetList();
#ifdef _PRINTLOG_
    LdbcFile << startVertex << " is connected with " << target.size() << " friends and friends of friends" << endl;
#endif

    Filter FilterSet[4];
    traverseThroughMultiRelType("PERSON_IS_LOCATED_IN", FilterSet[0]); 
    traverseThroughMultiRelType("IS_PART_OF", FilterSet[1]); 
    auto country1 = PropRange.second.first;
    auto country2 = PropRange.second.second;
    FilterSet[2].setProperty("name", country1);
    FilterSet[3].setProperty("name", country2);
    
    auto it = target.begin(); 
    while(it != target.end()) {
      VertexPropertyRangeVisitor vp3;
      vp3.setFilter(FilterSet[0]);
      vp3.setFilter(FilterSet[1]);
      vp3.setVertexFilters(FilterSet[2]);
      vp3.setVertexFilters(FilterSet[3]);
      vp3.setDepth(2);
      vp3.setDepthToCheckVertexProp(2);
      auto startVertex = (*it)->getId();
      Cbfs.breadthFirstSearch(graph, startVertex, vp3, LockManager);
      if(vp3.getIncludeState() == false) {
        target.erase(it);
      }
      else {
        it++;
      }
    }

    Filter Filters[5];
    PersonListMapType PersonListMap;
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR",Filters[0]); 
    Filters[1].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second); 
    traverseThroughMultiRelType("POST_IS_LOCATED_IN+COMMENT_IS_LOCATED_IN",Filters[2]); 
    Filters[3].setProperty("name", country1); 
    Filters[4].setProperty("name", country2); 
    
    for (auto it = target.begin(); it != target.end(); it++) {
      VertexPropertyRangeVisitor vpr3;
      vpr3.setFilter(Filters[0]);
      vpr3.setFilter(Filters[2]);
      vpr3.setRangeFilter(Filters[1]);
      vpr3.setVertexFilters(Filters[4]);
      vpr3.setVertexFilters(Filters[3]);
      vpr3.setDepth(2);
      vpr3.setDepthToCheckRange(2);
      vpr3.setPropToCheck(2);
      vpr3.setDepthToCheckVertexProp(2);
      auto startVertex = (*it)->getId();
      Cbfs.breadthFirstSearch(graph, startVertex, vpr3, LockManager);
      auto targetList = vpr3.getVertexTargetList();
      PersonListMap.insert(PersonListPair((*it), targetList));
    }

#ifdef _PRINTLOG_
    for(auto it = PersonListMap.begin(); it != PersonListMap.end(); ++it) {
      LdbcFile <<"friend " << (*it).first->getPropertyValue("firstName").first  << " has " <<(*it).second.size() << " commmets/posts made in " << country1 << " or " << country2 << endl; 
    }
#endif

    Cbfs.releaseAll(LockManager);
    getExecTime();  
    LdbcFile.close();
  }
};

class LdbcQuery4 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex, 
      LockManagerType & LockManager) {
//    LdbcFile << "===============Query 4================\n";
    getStartTime();  
    Filter tmpFilter[4];
    traverseThroughMultiRelType("KNOWS", tmpFilter[0]); 
    traverseThroughMultiRelType("POST_HAS_CREATOR", tmpFilter[1]); 
    traverseThroughMultiRelType("POST_HAS_TAG", tmpFilter[2]); 
    tmpFilter[3].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second); 
   
    MultiResultVisitor v4;
    v4.setFilter(tmpFilter[0]);
    v4.setFilter(tmpFilter[1]);
    v4.setFilter(tmpFilter[2]);
    v4.setRangeFilter(tmpFilter[3]);
    v4.setDepth(3);
    v4.setDepthToCheckRange(2);
    v4.setPropToCheck(1); //check time
    ConcurrentBFS Cbfs;
    Cbfs.breadthFirstSearch(graph, startVertex, v4, LockManager);

#ifdef _PRINTLOG_
    auto targets = v4.getReturnResultMap();
    LdbcFile << startVertex << " has friends made posts of " << targets.size() << " tags\n";
    for(auto it = targets.begin(); it != targets.end(); ++it) {
      LdbcFile <<"tags " << (*it).first->getPropertyValue("id").first << "\t" <<"num of posts " << (*it).second <<  endl;
    }
#endif

    Cbfs.releaseAll(LockManager);
    getExecTime();  
    LdbcFile.close();
  }
};

class LdbcQuery5 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef map<VertexPointer, vector<VertexPointer>> PersonListMapType;
  typedef pair<VertexPointer, vector<VertexPointer>> PersonListPair;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex, 
      LockManagerType & LockManager) {
//    LdbcFile << "===============Query 5================\n";
    getStartTime();  
    Filter tmpFilter[2];
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter[0]);
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter[1]);

    SingleRelTypeVisitor v5;
    v5.setFilter(tmpFilter[0]);
    v5.setFilter(tmpFilter[1]);
    v5.setDepth(2);
    ConcurrentBFS Cbfs;
    Cbfs.breadthFirstSearch(graph, startVertex, v5, LockManager);

    auto target = v5.getVertexTargetList();
#ifdef _PRINTLOG_
    LdbcFile << startVertex << " is connected with " << target.size() << " friends and friends of friends" << endl;
    for(auto it = targetMap.begin(); it != targetMap.end(); ++it) {
      if((*it).second != 0)
        LdbcFile <<"forum " << (*it).first->getPropertyValue("id").first  << " has " <<(*it).second << " posts made by friends"<< endl; 
    }
#endif

//    auto target = v5.getVertexTargetList();
//    LdbcFile << startVertex << " is connected with " << target.size() << " friends and friends of friends" << endl;
    Filter Filters[4];
    traverseThroughMultiRelType("HAS_MEMBER", Filters[0]); 
    Filters[1].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second); 
    traverseThroughMultiRelType("CONTAINER_OF",Filters[2]); 
    traverseThroughMultiRelType("POST_HAS_CREATOR",Filters[3]); 
    vector<Filter> VertexFilter;
    for(auto iter = target.begin(); iter != target.end(); iter++) {
      Filter newFilter;
      newFilter.setProperty("id", (*iter)->getPropertyValue("id").first.std_str());
      VertexFilter.push_back(newFilter);
    }

    map<VertexPointer, unsigned int> targetMap;
    for(auto it = target.begin(); it != target.end(); it++) {
      MultiPropertyVisitor mpv5;
      mpv5.setFilter(Filters[0]);
      mpv5.setFilter(Filters[2]);
      mpv5.setFilter(Filters[3]);
      mpv5.setRangeFilter(Filters[1]);
      mpv5.setVertexFilterList(VertexFilter);
      mpv5.setDepth(3);
      mpv5.setDepthToCheckVertexProp(3);
      mpv5.setDepthToCheckRange(1);
      mpv5.setPropToCheck(2); //check date 
      auto startVertex = (*it)->getId();
      Cbfs.breadthFirstSearch(graph, startVertex, mpv5, LockManager);
      targetMap.insert(mpv5.getResultMap().begin(), mpv5.getResultMap().end());
    }
 
    Cbfs.releaseAll(LockManager);
    getExecTime();  
    LdbcFile.close();
  }
};

class LdbcQuery6 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef map<VertexPointer, vector<VertexPointer>> PersonListMapType;
  typedef pair<VertexPointer, vector<VertexPointer>> PersonListPair;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex, 
      LockManagerType & LockManager) {
//    LdbcFile << "===============Query 6================\n";
    getStartTime();  
    Filter tmpFilter[2];
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter[0]);
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter[1]);

    SingleRelTypeVisitor v6;
    v6.setFilter(tmpFilter[0]);
    v6.setFilter(tmpFilter[1]);
    v6.setDepth(2);
    ConcurrentBFS Cbfs;
    Cbfs.breadthFirstSearch(graph, startVertex, v6, LockManager);

    auto target = v6.getVertexTargetList();
#ifdef _PRINTLOG_
    LdbcFile << startVertex << " is connected with " << target.size() << " friends and friends of friends" << endl;
#endif

//    auto target = v6.getVertexTargetList();
//    LdbcFile << startVertex << " is connected with " << target.size() << " friends and friends of friends" << endl;

    Filter Filters[3];
    traverseThroughMultiRelType("POST_HAS_CREATOR", Filters[0]); 
    traverseThroughMultiRelType("POST_HAS_TAG", Filters[1]); 
    Filters[2].setProperty(ParamPair.first, ParamPair.second);
    
    map<string, unsigned int> TagMap; 
    for(auto it = target.begin(); it != target.end(); it++) {
      SinglePropertyVisitor  vpv6;
      vpv6.setFilter(Filters[0]);
      vpv6.setFilter(Filters[1]);
      vpv6.setVertexFilter(Filters[2]);
      vpv6.setDepthToCheckVertexProp(2);
      vpv6.setDepth(2);
      unsigned int startVertex = (*it)->getId();
      Cbfs.breadthFirstSearch(graph, startVertex, vpv6, LockManager);
      auto personMap = vpv6.getPersonMap(); 
      for(auto it = personMap.begin(); it != personMap.end(); it++) {
        if((*it).second ) {
          for(auto iter = vpv6.getResultTargetsMap()[(*it).first].begin(); 
              iter != vpv6.getResultTargetsMap()[(*it).first].end(); iter++) {
            // (*iter) --> Tag id (string)
            if(TagMap.find(*iter) == TagMap.end()) {
              TagMap.insert(pair<string, unsigned int>((*iter), 1));
            } else {
              TagMap[*iter]++;
//              LdbcFile << (*iter) << " with " << TagMap[*iter] << " posts \n";
            }
          }
        }
      }
    }
 
#ifdef _PRINTLOG_
    for (auto it = TagMap.begin(); it != TagMap.end(); it++) {
      LdbcFile << "Tag " << (*it).first << " has " << (*it).second << " posts\n";
    }
#endif

    Cbfs.releaseAll(LockManager);
    getExecTime();  
    LdbcFile.close();
  }
};

class LdbcQuery7 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef map<VertexPointer, vector<VertexPointer>> PersonListMapType;
  typedef pair<VertexPointer, vector<VertexPointer>> PersonListPair;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex, 
      LockManagerType & LockManager) {
//    LdbcFile << "===============Query 7================\n";
    getStartTime();  
    Filter tmpFilter[2];
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter[0]);

    SingleRelTypeVisitor v7;
    v7.setFilter(tmpFilter[0]);
    v7.setDepth(1);
    ConcurrentBFS Cbfs;
    Cbfs.breadthFirstSearch(graph, startVertex, v7, LockManager);
    auto target = v7.getVertexTargetList();
    vector<Filter> VertexFilter;
    for(auto iter = target.begin(); iter != target.end(); iter++) {
      Filter newFilter;
      newFilter.setProperty("id", (*iter)->getPropertyValue("id").first.std_str());
      VertexFilter.push_back(newFilter);
    }
#ifdef _PRINTLOG_
    LdbcFile << startVertex << " is connected with " << target.size() << " friends" << endl;
#endif
    Filter Filters[2];
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR", Filters[0]); 
    traverseThroughMultiRelType("LIKES_COMMENT+LIKES_POST", Filters[1]); 
    VertexMatchVisitor vmv7;
    vmv7.setFilter(Filters[0]);
    vmv7.setFilter(Filters[1]);
    vmv7.setDepth(2);
    vmv7.setVertexFilterList(VertexFilter);
    vmv7.setDepthToCheckVertexProp(2);
    vmv7.setDepthToCompareTime(2);
    Cbfs.breadthFirstSearch(graph, startVertex, vmv7, LockManager);
 
#ifdef _PRINTLOG_
    auto targetsMap = vmv7.getTimeMap();
    for(auto it = targetsMap.begin(); it != targetsMap.end(); it++) {
      LdbcFile << (*it).first->getPropertyValue("firstName").first << "\t" << (*it).first->getPropertyValue("id").first<< " likes comment/posts at " << (*it).second << endl; 
    }
    auto personMap = vmv7.getPersonMap();
    for(auto it = personMap.begin(); it != personMap.end(); it++)  {
      if((*it).second == true) 
        LdbcFile << "comment/post " << (*it).first->getPropertyValue("id").first << " are made by friend\n" ;
    }
#endif

    Cbfs.releaseAll(LockManager);
    getExecTime();  
    LdbcFile.close();
  }
};

class LdbcQuery8 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex, 
      LockManagerType & LockManager) {
//    LdbcFile << "===============Query 8================\n";
    getStartTime();  
    Filter Filters[3];
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR", Filters[0]); 
    traverseThroughMultiRelType("REPLY_OF_COMMENT+REPLY_OF_POST", Filters[1]); 
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR", Filters[2]); 

    TimeCompareVisitor v8;
    v8.setFilter(Filters[0]);
    v8.setFilter(Filters[1]);
    v8.setFilter(Filters[2]);
    v8.setDepth(3);
    v8.setDepthToCompareTime(2);
    ConcurrentBFS Cbfs;
    Cbfs.breadthFirstSearch(graph, startVertex, v8, LockManager);
 
#ifdef _PRINTLOG_
    auto vertexMap = v8.getVertexMap();
    for(auto it = vertexMap.begin(); it != vertexMap.end(); it++) {
      LdbcFile << "Person " << (*it).second->getPropertyValue("firstName").first << " made replies " << (*it).first->getPropertyValue("id").first << " at " << (*it).first->getPropertyValue("creationDate").first << " to startperson's posts/comments\n";
    }
#endif

    Cbfs.releaseAll(LockManager);
    getExecTime();  
    LdbcFile.close();
  }
};

class LdbcQuery9 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef map<VertexPointer, VertexPointer> ReturnMapType;
  typedef pair<VertexPointer, VertexPointer> ReturnMapPair;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex, 
      LockManagerType & LockManager) {
//    LdbcFile << "===============Query 9================\n";
    getStartTime();  
    Filter tmpFilter[2];
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter[0]);
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter[1]);
    SingleRelTypeVisitor v9;
    v9.setFilter(tmpFilter[0]);
    v9.setFilter(tmpFilter[1]);
    v9.setDepth(2);
    ConcurrentBFS Cbfs;
    Cbfs.breadthFirstSearch(graph, startVertex, v9, LockManager);

    auto target = v9.getVertexTargetList();
#ifdef _PRINTLOG_
    LdbcFile << startVertex << " is connected with " << target.size() << " friends and friends of friends" << endl;
#endif

    Filter Filters[2];
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR",Filters[0]); 
    Filters[1].setValueRange("creationDate", "", "2011-07-16T23:59:00.255"); 
          
    ReturnMapType TargetsMap; 
    for(auto it = target.begin(); it != target.end(); ++it) {
#ifdef _PRINTLOG_
      LdbcFile <<"friend " << (*it)->getId() << "\t" << (*it)->getPropertyValue("id").first << "\t" << (*it)->getPropertyValue("firstName").first  << endl; 
#endif
      MultiRelTypeVisitor sv9;
      sv9.setFilter(Filters[0]);
      sv9.setRangeFilter(Filters[1]);
      sv9.setDepth(1);
      sv9.setDepthToCheckRange(1);
      sv9.setPropToCheck(2); //check date 
      unsigned int startVertex = (*it)->getId();
      Cbfs.breadthFirstSearch(graph, startVertex, sv9, LockManager);
      auto targets = sv9.getTargetsMap();
      TargetsMap.insert(targets.begin(), targets.end());
   }
 
#ifdef _PRINTLOG_
    for(auto iter = TargetsMap.begin(); iter != TargetsMap.end(); ++iter) {
      LdbcFile << "posts/comments " << (*iter).first->getPropertyValue("id").first << "\t" << (*iter).first->getPropertyValue("creationDate").first << " made by person " << (*iter).second->getPropertyValue("id").first << "\t" <<  (*iter).second->getPropertyValue("firstName").first << endl;
    }
#endif

    Cbfs.releaseAll(LockManager);
    getExecTime();  
    LdbcFile.close();
  }
};

class LdbcQuery10 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef std::unordered_map<VertexPointer, unsigned int> SimMapType;  
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex,
      LockManagerType & LockManager) {
//    LdbcFile << "===============Query 10================\n";
    getStartTime();  
    ///first find start person's friends
    AdjacencyVisitor av1; 
    traverseThroughTypeAndDirection("KNOWS", "out", av1.getFilter());
    ConcurrentBFS Cbfs;
    Cbfs.breadthFirstSearch(graph, startVertex, av1, LockManager);
    auto startId = graph.getVertexPointer(startVertex)->getPropertyValue("id").first.std_str();
#ifdef _PRINTLOG_
//    std::cout << "start person id and name " << graph.getVertexPointer(startVertex)->getId() << graph.getVertexPointer(startVertex)->getPropertyValue("firstName").first << "\n";
//    std::cout << "start person has " << av1.getVertexTargetList().size() << " friends\n";
#endif

    ///find friends of friends of start person
    std::vector<VertexPointer> targets;
    Filter BDFilter;
    BDFilter.setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second); 
    for (VertexPointer StartVertex : av1.getVertexTargetList()) {
      auto EqualFlag = false; 
//      std::cout << "frined " << StartVertex->getId() << "\n";
      if ( checkRange<VertexPointer>(4, StartVertex, BDFilter, EqualFlag) == true ) {
        SimMap[StartVertex] = 0;
        AdjacencyVisitor av2; 
        traverseThroughTypeAndDirection("KNOWS", "out", av2.getFilter());
        Cbfs.breadthFirstSearch(graph, StartVertex->getId(), av2, LockManager);
        ///concatenate two lists
        targets.insert(targets.end(), av2.getVertexTargetList().begin(), 
          av2.getVertexTargetList().end());
//      std::cout << "friends " << StartVertex->getId() << " has friend " << av2.getVertexTargetList().size() << "\n";
      }

    }

    for ( VertexPointer Vertex : targets) {
      auto EqualFlag = false; 
      if ( checkRange<VertexPointer>(4, Vertex, BDFilter, EqualFlag) == true ) {
        SimMap[Vertex] = 0;
      }
    }

    ///iterate over the friends who satisfy the condition
    auto itend  = SimMap.end();
    for ( auto it = SimMap.begin(); it != itend; it++ ) {
      Filter tmpFilter[4];
      traverseThroughMultiRelType("POST_HAS_CREATOR", tmpFilter[0]); 
      traverseThroughMultiRelType("POST_HAS_TAG", tmpFilter[1]); 
      traverseThroughMultiRelType("HAS_INTEREST", tmpFilter[2]); 
      tmpFilter[3].setProperty("id", startId);

      SimilarityVisitor v10;
      v10.setDepth(3);
      v10.setDepthToCheckVertexProp(3);
      v10.setVertexFilter(tmpFilter[3]);
      for ( unsigned int i = 0; i < 3; i++ ) {
        v10.setFilter(tmpFilter[i]);
      }
      Cbfs.breadthFirstSearch(graph, (*it).first->getId(), v10, LockManager);
      auto iterend = v10.getPostMap().end();
      unsigned int PostItrd = 0;
      for ( auto iter = v10.getPostMap().begin(); iter != iterend; iter++ ) {
        if ((*iter).second ) { PostItrd++; }
//        std::cout << "post " << (*iter).first << "\t" << (*iter).second << "\n";
      }

      SimMap[(*it).first] = 2*PostItrd - v10.getPostMap().size(); 
#ifdef _PRINTLOG_
      LdbcFile << "person: " << (*it).first->getId() << " similarity score " << (*it).second << "\n";
#endif
    }

    Cbfs.releaseAll(LockManager);
    getExecTime();  
    LdbcFile.close();
  }
protected:
  SimMapType SimMap;
};

class LdbcQuery11 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef pair<EdgePointer, VertexPointer> MapPair;
  typedef multimap<VertexPointer, MapPair> MatchMapType; 
  typedef pair<VertexPointer, VertexPointer> ReturnMapPair;
  typedef vector<MatchMapType> ReturnTargetsType;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex, 
      LockManagerType & LockManager) {
//    LdbcFile << "===============Query 11================\n";
    getStartTime();  
    Filter tmpFilter;
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter);

    SingleRelTypeVisitor v11;
    v11.setFilter(tmpFilter);
    v11.setFilter(tmpFilter);
    v11.setDepth(2);
    ConcurrentBFS Cbfs;
    Cbfs.breadthFirstSearch(graph, startVertex, v11, LockManager);

    auto target = v11.getVertexTargetList();
#ifdef _PRINTLOG_
    LdbcFile << startVertex << " is connected with " << target.size() << " friends and friends of friends" << endl;
#endif

    Filter Filters[4];
    traverseThroughMultiRelType("WORKS_AT",Filters[0]); 
    traverseThroughMultiRelType("ORGANISATION_IS_LOCATED_IN",Filters[1]); 
    Filters[2].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second); 
    Filters[3].setProperty(ParamPair.first, ParamPair.second);  

    MatchMapType TargetsMap;
    for(auto it = target.begin(); it != target.end(); ++it) {
#ifdef _PRINTLOG_
      LdbcFile <<"friend " << (*it)->getId() << "\t" << (*it)->getPropertyValue("id").first << "\t" << (*it)->getPropertyValue("firstName").first  << endl; 
#endif
   
    VertexPropertyVisitor sv11;
    sv11.setFilter(Filters[0]);
    sv11.setFilter(Filters[1]);
    sv11.setRangeFilter(Filters[2]);
    sv11.setVertexFilter(Filters[3]);
    sv11.setDepth(2);
    sv11.setDepthToCheckRange(1);
    sv11.setDepthToCheckVertexProp(2);
    unsigned int startVertex = (*it)->getId();
    Cbfs.breadthFirstSearch(graph, startVertex, sv11, LockManager);
    auto targets = sv11.getMatchMap();
    TargetsMap.insert(targets.begin(), targets.end());
  }
#ifdef _PRINTLOG_
    FixedString key("workFrom");
    for(auto iter = TargetsMap.begin(); iter != TargetsMap.end(); ++iter) {
      LdbcFile << (*iter).first->getPropertyValue("firstName").first << " works at "  << (*iter).second.second->getPropertyValue("id").first << " from " << (*iter).second.first->getPropertyValue(key).first << endl;
      }
#endif
    Cbfs.releaseAll(LockManager);
    getExecTime();  
    LdbcFile.close();
  }
};

class LdbcQuery12 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef std::unordered_map<VertexPointer, unsigned int> SimMapType;  
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex, 
      LockManagerType & LockManager) {
//    LdbcFile << "===============Query 12================\n";
    getStartTime();  
    ///first find start person's friends
    AdjacencyVisitor av1; 
    traverseThroughTypeAndDirection("KNOWS", "out", av1.getFilter());
    ConcurrentBFS Cbfs;
    Cbfs.breadthFirstSearch(graph, startVertex, av1, LockManager);

    ///find friends of friends of start person
    std::vector<VertexPointer> targets;
      Filter tmpFilter[6];
      traverseThroughMultiRelType("COMMENT_HAS_CREATOR", tmpFilter[0]); 
      traverseThroughMultiRelType("REPLY_OF_POST", tmpFilter[1]); 
      traverseThroughMultiRelType("POST_HAS_TAG", tmpFilter[2]); 
      traverseThroughMultiRelType("HAS_TYPE", tmpFilter[3]); 
      traverseThroughMultiRelType("IS_SUBCLASS_OF", tmpFilter[4]); 
      tmpFilter[5].setProperty(ParamPair.first, ParamPair.second);


    for (VertexPointer StartVertex : av1.getVertexTargetList()) {
//      std::cout << "friend " << StartVertex->getId() << "\n";
      SimMap[StartVertex] = 0;

      ExpertVisitor v10;
      v10.setDepth(5);
      v10.setDepthToCheckVertexProp(4);
      v10.setVertexFilter(tmpFilter[5]);
      for ( unsigned int i = 0; i < 5; i++ ) {
        v10.setFilter(tmpFilter[i]);
      }
      Cbfs.breadthFirstSearch(graph, StartVertex->getId(), v10, LockManager);
      auto iterend = v10.getPostMap().end();
      auto PostNum = 0;
      for ( auto iter = v10.getPostMap().begin(); iter != iterend; iter++ ) {
        if ((*iter).second ) { PostNum++; }
//          std::cout << "post " << (*iter).first << "\t" << (*iter).second << "\n";
      }

#ifdef _PRINTLOG_
      SimMap[StartVertex] = PostNum; 
      LdbcFile << "person: " << StartVertex->getId() << " expert post " << PostNum << "\n";
#endif
  
    }
    Cbfs.releaseAll(LockManager);
    getExecTime();  
    LdbcFile.close();
  }
protected:
  SimMapType SimMap;
};


class LdbcQuery13 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex, 
      VertexDescriptor endVertex, LockManagerType & LockManager) {
//    LdbcFile << "===============Query 13================\n";
    getStartTime();  
    PathVisitor v13;
    v13.setEndVertex(endVertex);
    ConcurrentBFS Cbfs;
    Cbfs.breadthFirstSearch(graph, startVertex, v13, LockManager);

#ifdef _PRINTLOG_
    auto target = v13.getVertexTargetList();
    if(target.empty())
      LdbcFile << startVertex << " and " <<  endVertex <<" are not connected" << endl;
    else {
      LdbcFile << "There are  shortest paths of length " << target.size() << "  from " << startVertex << " to " << endVertex << endl;
      for(auto it = target.begin(); it != target.end(); ++it) {
        LdbcFile <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue("id").first << (*it)->getPropertyValue("firstName").first<< endl;
      }
    }
#endif

    Cbfs.releaseAll(LockManager);
    getExecTime();  
    LdbcFile.close();
  }

};

class LdbcQuery14 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex, 
      VertexDescriptor endVertex, LockManagerType & LockManager ) {
//    LdbcFile << "===============Query 14================\n";
    getStartTime();  
    SubGraphVisitor v13;
    v13.setEndVertex(endVertex);
    Filter EdgeFilter;
    traverseThroughTypeAndDirection("KNOWS", "out",  EdgeFilter);
    v13.setEdgeFilter(EdgeFilter);
    ConcurrentBFS Cbfs;
    Cbfs.breadthFirstSearch(graph, startVertex, v13, LockManager);

    auto target = v13.getVertexTargetList();
#ifdef _PRINTLOG_
    if(target.empty())
      LdbcFile << startVertex << " and " <<  endVertex <<" are not connected" << endl;
    else {
      LdbcFile << "There are  shortest paths of length " << target.size() << "  from " << startVertex << " to " << endVertex << endl;
      for(auto it = target.begin(); it != target.end(); ++it) {
        LdbcFile <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue("id").first <<"\t" << (*it)->getPropertyValue("firstName").first<< endl;
      }//for
    }//else
#endif

  ///already found all the paths, calculate weights now  
  
  auto itend = target.end();
  float Weight = 0.0;
  for ( auto it = target.begin(); it != itend-1; it++ ) {  
 
    Filter tmpFilter[4];
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR", tmpFilter[0]); 
    traverseThroughMultiRelType("REPLY_OF_POST+REPLY_OF_COMMENT", tmpFilter[1]); 
    traverseThroughMultiRelType("POST_HAS_CREATOR+COMMENT_HAS_CREATOR", tmpFilter[2]); 
  
    WeightedPathVisitor v14;
    for ( unsigned int i = 0; i < 3; i++ ) {
      v14.setFilter(tmpFilter[i]);
    }
  
    auto it2 = it+1;
    tmpFilter[3].setProperty("id",(*it2)->getPropertyValue("id").first.std_str());
    v14.setVertexFilter(tmpFilter[3]);
    v14.setDepth(3);
    Cbfs.breadthFirstSearch(graph, (*it)->getId(), v14, LockManager);
    Weight += v14.getScore();
  } 

#ifdef _PRINTLOG_
  LdbcFile << "weight " << Weight <<"\n";
#endif
  Cbfs.releaseAll(LockManager);
  getExecTime();  
  LdbcFile.close();
 }//run
//private:
//  unsigned int _QueryId;
};

class Query17 : public LDBCQuery { 
  using LDBCQuery::LDBCQuery;
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
//    LdbcFile << "===============Update Query================\n";
    getStartTime();  
    //a new vertex vs 
    //need new propertylist for vertex and branchmap (criteria) for searching neighbor 
    AddNodeVisitor v1(LockManager, graph);
    v1.setVertexProperty(VertexPropertyList);
    v1.getFilter().setBranchMap(BranchMap);
    breadthFirstSearch(graph, 0, v1);

    getExecTime();  
    LdbcFile.close();
    //LdbcFile << "Add one more person into network \n";
  }
protected:
  BranchMapType BranchMap;
  ValueListType ValueList;
};

#if _SKIP_
class Query18 : public LDBCQuery { 
  using LDBCQuery::LDBCQuery;
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
//    LdbcFile << "===============Skip Data================\n";
    getStartTime();  
    //a new vertex vs 
    //need new propertylist for vertex and branchmap (criteria) for searching neighbor 
    SkipVisitor v1(LockManager, graph);
    v1.setVertexProperty(VertexPropertyList);
    v1.getFilter().setBranchMap(BranchMap);
    breadthFirstSearch(graph, 0, v1);

    getExecTime();  
    LdbcFile.close();

    //LdbcFile << "Add one more person into network \n";
  }
protected:
  BranchMapType BranchMap;
  ValueListType ValueList;
};
#endif
