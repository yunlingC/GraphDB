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
  static void *runQueries(void * ptr) {
    QueryHelper *Helper = (QueryHelper *) ptr;
    Helper->Query.runQuery((Helper->Graph), 
        (Helper->StartVertex), 
        (Helper->LockManager) );
    return 0;
  }

  void runQuery(GraphType & Graph, VertexDescriptor StartVertex, 
      LockManagerType & LockManager) {
    getStartTime();  
    Filter TmpFilter;
    Filter NameFilter;
    filtProperty(ParamPair.first, ParamPair.second, NameFilter); 
    traverseThroughTypeAndDirection("KNOWS", "", TmpFilter);
    LimitedDepthVisitor  DepthVisitor;
    for (auto i = 0; i < 3; i++) {
      DepthVisitor.setFilter(TmpFilter);
    }
    DepthVisitor.setNameFilter(NameFilter);
    DepthVisitor.setDepth(3);
    ConcurrentBFS CcBfs; 
    CcBfs.breadthFirstSearch(Graph, StartVertex, DepthVisitor, LockManager);

#ifdef _PRINTLOG_
    auto target = DepthVisitor.getVertexTargetList(); 
    LdbcFile << StartVertex << " is connected with " << target.size() << " people with " << ParamPair.first <<": " << ParamPair.second<< endl;
#endif
    CcBfs.releaseAll(LockManager);
    getExecTime();
  }
};

class LdbcQuery2 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  static void *runQueries(void * ptr) {
    QueryHelper *Helper = (QueryHelper *) ptr;
    Helper->Query.runQuery((Helper->Graph), 
        (Helper->StartVertex), 
        (Helper->LockManager) );
    return 0;
  }

  void runQuery(GraphType & Graph, VertexDescriptor StartVertex, 
      LockManagerType & LockManager) {
    getStartTime();  
    Filter TmpFilter[3];
    traverseThroughMultiRelType("KNOWS", TmpFilter[0]); 
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR", TmpFilter[1]); 
    TmpFilter[2].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second); 
   
    MultiRelTypeVisitor TypeVisitor;
    TypeVisitor.setFilter(TmpFilter[0]);
    TypeVisitor.setFilter(TmpFilter[1]);
    TypeVisitor.setRangeFilter(TmpFilter[2]);
    TypeVisitor.setDepth(2);
    TypeVisitor.setDepthToCheckRange(2);
    //check date
    TypeVisitor.setPropToCheck(2); 
    ConcurrentBFS CcBfs;
    CcBfs.breadthFirstSearch(Graph, StartVertex, TypeVisitor, LockManager);

#ifdef _PRINTLOG_
    auto target = TypeVisitor.getVertexTargetList();
    auto targets = TypeVisitor.getTargetsMap();
    LdbcFile << StartVertex << " has friends made " << target.size() << " comments and posts \n";
    for(auto it = targets.begin(); it != targets.end(); ++it) {
      LdbcFile <<"person " << (*it).first->getId() << " " << (*it).second->getPropertyValue("id").first << "\t" <<"comments/posts " << (*it).first->getPropertyValue("id").first << "\t" << (*it).first->getPropertyValue("creationDate").first ;
      LdbcFile << endl;
    }
#endif

    CcBfs.releaseAll(LockManager);
    getExecTime();  
  }
};

class LdbcQuery3 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef map<VertexPointer, vector<VertexPointer>> PersonListMapType;
  typedef pair<VertexPointer, vector<VertexPointer>> PersonListPair;
public:
  static void *runQueries(void * ptr) {
    QueryHelper *Helper = (QueryHelper *) ptr;
    Helper->Query.runQuery((Helper->Graph), 
        (Helper->StartVertex), 
        (Helper->LockManager) );
    return 0;
  }

  void runQuery(GraphType & Graph, VertexDescriptor StartVertex, 
      LockManagerType & LockManager) {
    getStartTime();  
    Filter TmpFilter;
    traverseThroughTypeAndDirection("KNOWS", "", TmpFilter);

    SingleRelTypeVisitor SingleVisitor;
    SingleVisitor.setFilter(TmpFilter);
    ///Do NOT delete
    SingleVisitor.setFilter(TmpFilter);
    SingleVisitor.setDepth(2);
    ConcurrentBFS CcBfs;
    CcBfs.breadthFirstSearch(Graph, StartVertex, SingleVisitor, LockManager);
    auto target = SingleVisitor.getVertexTargetList();

#ifdef _PRINTLOG_
    LdbcFile << StartVertex << " is connected with " << target.size() << " friends and friends of friends" << endl;
#endif

    Filter FilterSet[4];
    traverseThroughMultiRelType("PERSON_IS_LOCATED_IN", FilterSet[0]); 
    traverseThroughMultiRelType("IS_PART_OF", FilterSet[1]); 
    auto country1 = PropRange.second.first;
    auto country2 = PropRange.second.second;
    FilterSet[2].setProperty("name", country1);
    FilterSet[3].setProperty("name", country2);
    
    auto it = target.begin(); 
    while (it != target.end()) {
      VertexPropertyRangeVisitor RangeVisitor;
      RangeVisitor.setFilter(FilterSet[0]);
      RangeVisitor.setFilter(FilterSet[1]);
      RangeVisitor.setVertexFilters(FilterSet[2]);
      RangeVisitor.setVertexFilters(FilterSet[3]);
      RangeVisitor.setDepth(2);
      RangeVisitor.setDepthToCheckVertexProp(2);
      auto StartVertex = (*it)->getId();
      CcBfs.breadthFirstSearch(Graph, StartVertex, RangeVisitor, LockManager);
      if (RangeVisitor.getIncludeState() == false) {
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
    
    for (auto it = target.begin(), it_end = target.end(); 
        it != it_end; it++) {
      VertexPropertyRangeVisitor RangeVisitor;
      RangeVisitor.setFilter(Filters[0]);
      RangeVisitor.setFilter(Filters[2]);
      RangeVisitor.setRangeFilter(Filters[1]);
      RangeVisitor.setVertexFilters(Filters[4]);
      RangeVisitor.setVertexFilters(Filters[3]);
      RangeVisitor.setDepth(2);
      RangeVisitor.setDepthToCheckRange(2);
      RangeVisitor.setPropToCheck(2);
      RangeVisitor.setDepthToCheckVertexProp(2);
      auto StartVertex = (*it)->getId();
      CcBfs.breadthFirstSearch(Graph, StartVertex, RangeVisitor, LockManager);
      auto targetList = RangeVisitor.getVertexTargetList();
      PersonListMap.insert(PersonListPair((*it), targetList));
    }

#ifdef _PRINTLOG_
    for(auto it = PersonListMap.begin(), it_end = PersonListMap.begin(); 
        it != it_end; ++it) {
      LdbcFile << "friend " << (*it).first->getPropertyValue("firstName").first  << " has " <<(*it).second.size() << " commmets/posts made in " << country1 << " or " << country2 << endl; 
    }
#endif

    CcBfs.releaseAll(LockManager);
    getExecTime();  
  }
};

class LdbcQuery4 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  static void *runQueries(void * ptr) {
    QueryHelper *Helper = (QueryHelper *) ptr;
    Helper->Query.runQuery((Helper->Graph), 
        (Helper->StartVertex), 
        (Helper->LockManager) );
    return 0;
  }

  void runQuery(GraphType & Graph, VertexDescriptor StartVertex, 
      LockManagerType & LockManager) {
    getStartTime();  
    Filter TmpFilter[4];
    traverseThroughMultiRelType("KNOWS", TmpFilter[0]); 
    traverseThroughMultiRelType("POST_HAS_CREATOR", TmpFilter[1]); 
    traverseThroughMultiRelType("POST_HAS_TAG", TmpFilter[2]); 
    TmpFilter[3].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second); 
   
    MultiResultVisitor ResultsVisitor;
    ResultsVisitor.setFilter(TmpFilter[0]);
    ResultsVisitor.setFilter(TmpFilter[1]);
    ResultsVisitor.setFilter(TmpFilter[2]);
    ResultsVisitor.setRangeFilter(TmpFilter[3]);
    ResultsVisitor.setDepth(3);
    ResultsVisitor.setDepthToCheckRange(2);
    ResultsVisitor.setPropToCheck(1); //check time
    ConcurrentBFS CcBfs;
    CcBfs.breadthFirstSearch(Graph, StartVertex, ResultsVisitor, LockManager);

#ifdef _PRINTLOG_
    auto targets = ResultsVisitor.getReturnResultMap();
    LdbcFile << StartVertex << " has friends made posts of " << targets.size() << " tags\n";
    for(auto it = targets.begin(), it_end = targets.end(); 
        it != it_end; ++it) {
      LdbcFile <<"tags " << (*it).first->getPropertyValue("id").first << "\t" <<"num of posts " << (*it).second <<  endl;
    }
#endif

    CcBfs.releaseAll(LockManager);
    getExecTime();  
  }
};

class LdbcQuery5 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef map<VertexPointer, vector<VertexPointer>> PersonListMapType;
  typedef pair<VertexPointer, vector<VertexPointer>> PersonListPair;
public:
  static void *runQueries(void * ptr) {
    QueryHelper *Helper = (QueryHelper *) ptr;
    Helper->Query.runQuery((Helper->Graph), 
        (Helper->StartVertex), 
        (Helper->LockManager) );
    return 0;
  }

  void runQuery(GraphType & Graph, VertexDescriptor StartVertex, 
      LockManagerType & LockManager) {
    getStartTime();  
    Filter TmpFilter[2];
    traverseThroughTypeAndDirection("KNOWS", "", TmpFilter[0]);
    traverseThroughTypeAndDirection("KNOWS", "", TmpFilter[1]);

    SingleRelTypeVisitor SingleVisitor;
    SingleVisitor.setFilter(TmpFilter[0]);
    SingleVisitor.setFilter(TmpFilter[1]);
    SingleVisitor.setDepth(2);
    ConcurrentBFS CcBfs;
    CcBfs.breadthFirstSearch(Graph, StartVertex, SingleVisitor, LockManager);

    auto target = SingleVisitor.getVertexTargetList();
#ifdef _PRINTLOG_
    LdbcFile << StartVertex << " is connected with " << target.size() << " friends and friends of friends" << endl;
#endif

    Filter Filters[4];
    traverseThroughMultiRelType("HAS_MEMBER", Filters[0]); 
    Filters[1].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second); 
    traverseThroughMultiRelType("CONTAINER_OF",Filters[2]); 
    traverseThroughMultiRelType("POST_HAS_CREATOR",Filters[3]); 
    vector<Filter> VertexFilter;
    for (auto iter = target.begin(), iter_end = target.end(); 
        iter != iter_end; iter++) {
      Filter newFilter;
      newFilter.setProperty("id", (*iter)->getPropertyValue("id").first.std_str());
      VertexFilter.push_back(newFilter);
    }

    map<VertexPointer, unsigned int> targetMap;
    for (auto it = target.begin(), it_end = target.end(); 
        it != it_end; it++) {
      MultiPropertyVisitor PropertiesVisitor;
      PropertiesVisitor.setFilter(Filters[0]);
      PropertiesVisitor.setFilter(Filters[2]);
      PropertiesVisitor.setFilter(Filters[3]);
      PropertiesVisitor.setRangeFilter(Filters[1]);
      PropertiesVisitor.setVertexFilterList(VertexFilter);
      PropertiesVisitor.setDepth(3);
      PropertiesVisitor.setDepthToCheckVertexProp(3);
      PropertiesVisitor.setDepthToCheckRange(1);
      PropertiesVisitor.setPropToCheck(2); //check date 
      auto StartVertex = (*it)->getId();
      CcBfs.breadthFirstSearch(Graph, StartVertex, PropertiesVisitor, LockManager);
      targetMap.insert(PropertiesVisitor.getResultMap().begin(), 
          PropertiesVisitor.getResultMap().end());
    }

#ifdef _PRINTLOG_
     for (auto it = targetMap.begin(), it_end = targetMap.end(); 
         it != it_end; ++it) {
      if (!(*it).second)
        LdbcFile <<"forum " << (*it).first->getPropertyValue("id").first  << " has " <<(*it).second << " posts made by friends"<< endl; 
    }
#endif
    CcBfs.releaseAll(LockManager);
    getExecTime();  
  }
};

class LdbcQuery6 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef map<VertexPointer, vector<VertexPointer>> PersonListMapType;
  typedef pair<VertexPointer, vector<VertexPointer>> PersonListPair;
public:
  static void *runQueries(void * ptr) {
    QueryHelper *Helper = (QueryHelper *) ptr;
    Helper->Query.runQuery((Helper->Graph), 
        (Helper->StartVertex), 
        (Helper->LockManager) );
    return 0;
  }

  void runQuery(GraphType & Graph, VertexDescriptor StartVertex, 
      LockManagerType & LockManager) {
    getStartTime();  
    Filter TmpFilter[2];
    traverseThroughTypeAndDirection("KNOWS", "", TmpFilter[0]);
    traverseThroughTypeAndDirection("KNOWS", "", TmpFilter[1]);

    SingleRelTypeVisitor SingleVisitor;
    SingleVisitor.setFilter(TmpFilter[0]);
    SingleVisitor.setFilter(TmpFilter[1]);
    SingleVisitor.setDepth(2);
    ConcurrentBFS CcBfs;
    CcBfs.breadthFirstSearch(Graph, StartVertex, SingleVisitor, LockManager);

    auto target = SingleVisitor.getVertexTargetList();

#ifdef _PRINTLOG_
    LdbcFile << StartVertex << " is connected with " << target.size() << " friends and friends of friends" << endl;
#endif

    Filter Filters[3];
    traverseThroughMultiRelType("POST_HAS_CREATOR", Filters[0]); 
    traverseThroughMultiRelType("POST_HAS_TAG", Filters[1]); 
    Filters[2].setProperty(ParamPair.first, ParamPair.second);
    
    map<string, unsigned int> TagMap; 
    for(auto it = target.begin(); it != target.end(); it++) {
      SinglePropertyVisitor  SPVisitor;
      SPVisitor.setFilter(Filters[0]);
      SPVisitor.setFilter(Filters[1]);
      SPVisitor.setVertexFilter(Filters[2]);
      SPVisitor.setDepthToCheckVertexProp(2);
      SPVisitor.setDepth(2);
      unsigned int StartVertex = (*it)->getId();
      CcBfs.breadthFirstSearch(Graph, StartVertex, SPVisitor, LockManager);
      auto personMap = SPVisitor.getPersonMap(); 

      for (auto it = personMap.begin(), it_end = personMap.end(); 
          it != it_end; it++) {
        if ((*it).second ) {
          for (auto iter = SPVisitor.getResultTargetsMap()[(*it).first].begin();  
              iter != SPVisitor.getResultTargetsMap()[(*it).first].end(); iter++) {
            // (*iter) --> Tag id (string)
            if (TagMap.find(*iter) == TagMap.end()) {
              TagMap.insert(pair<string, unsigned int>((*iter), 1));
            } else {
              TagMap[*iter]++;
            }
          }
        }
      }
    }
 
#ifdef _PRINTLOG_
    for (auto it = TagMap.begin(), it_end = TagMap.end(); 
        it != it_end; it++) {
      LdbcFile << "Tag " << (*it).first << " has " << (*it).second << " posts\n";
    }
#endif

    CcBfs.releaseAll(LockManager);
    getExecTime();  
  }
};

class LdbcQuery7 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef map<VertexPointer, vector<VertexPointer>> PersonListMapType;
  typedef pair<VertexPointer, vector<VertexPointer>> PersonListPair;
public:
  static void *runQueries(void * ptr) {
    QueryHelper *Helper = (QueryHelper *) ptr;
    Helper->Query.runQuery((Helper->Graph), 
        (Helper->StartVertex), 
        (Helper->LockManager) );
    return 0;
  }

  void runQuery(GraphType & Graph, VertexDescriptor StartVertex, 
      LockManagerType & LockManager) {
    getStartTime();  
    Filter TmpFilter[2];
    traverseThroughTypeAndDirection("KNOWS", "", TmpFilter[0]);

    SingleRelTypeVisitor RelVisitor;
    RelVisitor.setFilter(TmpFilter[0]);
    RelVisitor.setDepth(1);
    ConcurrentBFS CcBfs;
    CcBfs.breadthFirstSearch(Graph, StartVertex, RelVisitor, LockManager);
    auto target = RelVisitor.getVertexTargetList();
    vector<Filter> VertexFilter;
    for (auto iter = target.begin(); iter != target.end(); iter++) {
      Filter newFilter;
      newFilter.setProperty("id", (*iter)->getPropertyValue("id").first.std_str());
      VertexFilter.push_back(newFilter);
    }

#ifdef _PRINTLOG_
    LdbcFile << StartVertex << " is connected with " << target.size() << " friends" << endl;
#endif

    Filter Filters[2];
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR", Filters[0]); 
    traverseThroughMultiRelType("LIKES_COMMENT+LIKES_POST", Filters[1]); 
    VertexMatchVisitor MatchVisitor;
    MatchVisitor.setFilter(Filters[0]);
    MatchVisitor.setFilter(Filters[1]);
    MatchVisitor.setDepth(2);
    MatchVisitor.setVertexFilterList(VertexFilter);
    MatchVisitor.setDepthToCheckVertexProp(2);
    MatchVisitor.setDepthToCompareTime(2);
    CcBfs.breadthFirstSearch(Graph, StartVertex, MatchVisitor, LockManager);
 
#ifdef _PRINTLOG_
    auto targetsMap = MatchVisitor.getTimeMap();
    for (auto it = targetsMap.begin(), it_end = targetsMap.end(); 
        it != it_end; it++) {
      LdbcFile << (*it).first->getPropertyValue("firstName").first << "\t" << (*it).first->getPropertyValue("id").first<< " likes comment/posts at " << (*it).second << endl; 
    }
    auto personMap = MatchVisitor.getPersonMap();
    for (auto it = personMap.begin(), it_end = personMap.end(); 
        it != it_end; it++)  {
      if ((*it).second) 
        LdbcFile << "comment/post " << (*it).first->getPropertyValue("id").first << " are made by friend\n" ;
    }
#endif

    CcBfs.releaseAll(LockManager);
    getExecTime();  
  }
};

class LdbcQuery8 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  static void *runQueries(void * ptr) {
    QueryHelper *Helper = (QueryHelper *) ptr;
    Helper->Query.runQuery((Helper->Graph), 
        (Helper->StartVertex), 
        (Helper->LockManager) );
    return 0;
  }

  void runQuery(GraphType & Graph, VertexDescriptor StartVertex, 
      LockManagerType & LockManager) {
    getStartTime();  
    Filter Filters[3];
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR", Filters[0]); 
    traverseThroughMultiRelType("REPLY_OF_COMMENT+REPLY_OF_POST", Filters[1]); 
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR", Filters[2]); 

    TimeCompareVisitor TimeVisitor;
    TimeVisitor.setFilter(Filters[0]);
    TimeVisitor.setFilter(Filters[1]);
    TimeVisitor.setFilter(Filters[2]);
    TimeVisitor.setDepth(3);
    TimeVisitor.setDepthToCompareTime(2);
    ConcurrentBFS CcBfs;
    CcBfs.breadthFirstSearch(Graph, StartVertex, TimeVisitor, LockManager);
 
#ifdef _PRINTLOG_
    auto vertexMap = TimeVisitor.getVertexMap();
    for (auto it = vertexMap.begin(), it_end = vertexMap.end(); 
        it != it_end; it++) {
      LdbcFile << "Person " << (*it).second->getPropertyValue("firstName").first << " made replies " << (*it).first->getPropertyValue("id").first << " at " << (*it).first->getPropertyValue("creationDate").first << " to startperson's posts/comments\n";
    }
#endif

    CcBfs.releaseAll(LockManager);
    getExecTime();  
  }
};

class LdbcQuery9 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef map<VertexPointer, VertexPointer> ReturnMapType;
  typedef pair<VertexPointer, VertexPointer> ReturnMapPair;
public:
  static void *runQueries(void * ptr) {
    QueryHelper *Helper = (QueryHelper *) ptr;
    Helper->Query.runQuery((Helper->Graph), 
        (Helper->StartVertex), 
        (Helper->LockManager) );
    return 0;
  }

  void runQuery(GraphType & Graph, VertexDescriptor StartVertex, 
      LockManagerType & LockManager) {
    getStartTime();  
    Filter TmpFilter[2];
    traverseThroughTypeAndDirection("KNOWS", "", TmpFilter[0]);
    traverseThroughTypeAndDirection("KNOWS", "", TmpFilter[1]);
    SingleRelTypeVisitor RelVisitor;
    RelVisitor.setFilter(TmpFilter[0]);
    RelVisitor.setFilter(TmpFilter[1]);
    RelVisitor.setDepth(2);
    ConcurrentBFS CcBfs;
    CcBfs.breadthFirstSearch(Graph, StartVertex, RelVisitor, LockManager);

    auto target = RelVisitor.getVertexTargetList();

#ifdef _PRINTLOG_
    LdbcFile << StartVertex << " is connected with " << target.size() << " friends and friends of friends" << endl;
#endif

    Filter Filters[2];
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR",Filters[0]); 
    Filters[1].setValueRange("creationDate", "", "2011-07-16T23:59:00.255"); 
          
    ReturnMapType TargetsMap; 
    for (auto it = target.begin(), it_end = target.end(); 
        it != it_end; ++it) {

#ifdef _PRINTLOG_
      LdbcFile <<"friend " << (*it)->getId() << "\t" << (*it)->getPropertyValue("id").first << "\t" << (*it)->getPropertyValue("firstName").first  << endl; 
#endif

      MultiRelTypeVisitor RelVisitor;
      RelVisitor.setFilter(Filters[0]);
      RelVisitor.setRangeFilter(Filters[1]);
      RelVisitor.setDepth(1);
      RelVisitor.setDepthToCheckRange(1);
      RelVisitor.setPropToCheck(2); //check date 
      unsigned int StartVertex = (*it)->getId();
      CcBfs.breadthFirstSearch(Graph, StartVertex, RelVisitor, LockManager);
      auto targets = RelVisitor.getTargetsMap();
      TargetsMap.insert(targets.begin(), targets.end());
   }
 
#ifdef _PRINTLOG_
    for (auto iter = TargetsMap.begin(), iter_end = TargetsMap.end(); 
        iter != iter_end; ++iter) {
      LdbcFile << "posts/comments " << (*iter).first->getPropertyValue("id").first << "\t" << (*iter).first->getPropertyValue("creationDate").first << " made by person " << (*iter).second->getPropertyValue("id").first << "\t" <<  (*iter).second->getPropertyValue("firstName").first << endl;
    }
#endif

    CcBfs.releaseAll(LockManager);
    getExecTime();  
  }
};

class LdbcQuery10 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef std::unordered_map<VertexPointer, unsigned int> SimMapType;  
public:
  static void *runQueries(void * ptr) {
    QueryHelper *Helper = (QueryHelper *) ptr;
    Helper->Query.runQuery((Helper->Graph), 
        (Helper->StartVertex), 
        (Helper->LockManager) );
    return 0;
  }

  void runQuery(GraphType & Graph, VertexDescriptor StartVertex,
      LockManagerType & LockManager) {
    getStartTime();  
    ///first find start person's friends
    AdjacencyVisitor AdjVisitor; 
    traverseThroughTypeAndDirection("KNOWS", "out", AdjVisitor.getFilter());
    ConcurrentBFS CcBfs;
    CcBfs.breadthFirstSearch(Graph, StartVertex, AdjVisitor, LockManager);
    auto startId = Graph.getVertexPointer(StartVertex)->getPropertyValue("id").first.std_str();

    ///find friends of friends of start person
    std::vector<VertexPointer> targets;
    Filter BDFilter;
    BDFilter.setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second); 
    for (VertexPointer StartVertex : AdjVisitor.getVertexTargetList()) {
      auto EqualFlag = false; 
      if (checkRange<VertexPointer>(4, StartVertex, BDFilter, EqualFlag)){
        SimMap[StartVertex] = 0;
        AdjacencyVisitor AdjVisitor; 
        traverseThroughTypeAndDirection("KNOWS", "out", AdjVisitor.getFilter());
        CcBfs.breadthFirstSearch(Graph, StartVertex->getId(), AdjVisitor, LockManager);
        ///concatenate two lists
        targets.insert(targets.end(),AdjVisitor.getVertexTargetList().begin(),          AdjVisitor.getVertexTargetList().end());
      }

    }

    for (VertexPointer Vertex : targets) {
      auto EqualFlag = false; 
      if (checkRange<VertexPointer>(4, Vertex, BDFilter, EqualFlag)) {
        SimMap[Vertex] = 0;
      }
    }

    ///iterate over the friends who satisfy the condition
    auto itend  = SimMap.end();
    for ( auto it = SimMap.begin(); it != itend; it++ ) {
      Filter TmpFilter[4];
      traverseThroughMultiRelType("POST_HAS_CREATOR", TmpFilter[0]); 
      traverseThroughMultiRelType("POST_HAS_TAG", TmpFilter[1]); 
      traverseThroughMultiRelType("HAS_INTEREST", TmpFilter[2]); 
      TmpFilter[3].setProperty("id", startId);

      SimilarityVisitor SimVisitor;
      SimVisitor.setDepth(3);
      SimVisitor.setDepthToCheckVertexProp(3);
      SimVisitor.setVertexFilter(TmpFilter[3]);
      for ( unsigned int i = 0; i < 3; i++ ) {
        SimVisitor.setFilter(TmpFilter[i]);
      }
      CcBfs.breadthFirstSearch(Graph, (*it).first->getId(), SimVisitor, LockManager);
      auto iterend = SimVisitor.getPostMap().end();
      unsigned int PostItrd = 0;
      for ( auto iter = SimVisitor.getPostMap().begin(); 
          iter != iterend; iter++ ) {
        if ((*iter).second ) { PostItrd++; }
      }
      SimMap[(*it).first] = 2*PostItrd - SimVisitor.getPostMap().size(); 

#ifdef _PRINTLOG_
      LdbcFile << "person: " << (*it).first->getId() << " similarity score " << (*it).second << "\n";
#endif
    }

    CcBfs.releaseAll(LockManager);
    getExecTime();  
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
  static void *runQueries(void * ptr) {
    QueryHelper *Helper = (QueryHelper *) ptr;
    Helper->Query.runQuery((Helper->Graph), 
        (Helper->StartVertex), 
        (Helper->LockManager) );
    return 0;
  }

  void runQuery(GraphType & Graph, VertexDescriptor StartVertex, 
      LockManagerType & LockManager) {
    getStartTime();  
    Filter TmpFilter;
    traverseThroughTypeAndDirection("KNOWS", "", TmpFilter);

    SingleRelTypeVisitor RelVisitor;
    RelVisitor.setFilter(TmpFilter);
    RelVisitor.setFilter(TmpFilter);
    RelVisitor.setDepth(2);
    ConcurrentBFS CcBfs;
    CcBfs.breadthFirstSearch(Graph, StartVertex, RelVisitor, LockManager);

    auto target = RelVisitor.getVertexTargetList();
#ifdef _PRINTLOG_
    LdbcFile << StartVertex << " is connected with " << target.size() << " friends and friends of friends" << endl;
#endif

    Filter Filters[4];
    traverseThroughMultiRelType("WORKS_AT",Filters[0]); 
    traverseThroughMultiRelType("ORGANISATION_IS_LOCATED_IN",Filters[1]); 
    Filters[2].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second); 
    Filters[3].setProperty(ParamPair.first, ParamPair.second);  

    MatchMapType TargetsMap;
    for(auto it = target.begin(), it_end = target.end(); 
        it != it_end; ++it) {

#ifdef _PRINTLOG_
      LdbcFile <<"friend " << (*it)->getId() << "\t" << (*it)->getPropertyValue("id").first << "\t" << (*it)->getPropertyValue("firstName").first  << endl; 
#endif
   
    VertexPropertyVisitor VPVisitor;
    VPVisitor.setFilter(Filters[0]);
    VPVisitor.setFilter(Filters[1]);
    VPVisitor.setRangeFilter(Filters[2]);
    VPVisitor.setVertexFilter(Filters[3]);
    VPVisitor.setDepth(2);
    VPVisitor.setDepthToCheckRange(1);
    VPVisitor.setDepthToCheckVertexProp(2);
    unsigned int StartVertex = (*it)->getId();
    CcBfs.breadthFirstSearch(Graph, StartVertex, VPVisitor, LockManager);
    auto targets = VPVisitor.getMatchMap();
    TargetsMap.insert(targets.begin(), targets.end());
  }

#ifdef _PRINTLOG_
    FixedString key("workFrom");
    for (auto iter = TargetsMap.begin(), iter_end = TargetsMap.end(); 
        iter != iter_end; ++iter) {
      LdbcFile << (*iter).first->getPropertyValue("firstName").first << " works at "  << (*iter).second.second->getPropertyValue("id").first << " from " << (*iter).second.first->getPropertyValue(key).first << endl;
      }
#endif

    CcBfs.releaseAll(LockManager);
    getExecTime();  
  }
};

class LdbcQuery12 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef std::unordered_map<VertexPointer, unsigned int> SimMapType;  
public:
  static void *runQueries(void * ptr) {
    QueryHelper *Helper = (QueryHelper *) ptr;
    Helper->Query.runQuery((Helper->Graph), 
        (Helper->StartVertex), 
        (Helper->LockManager) );
    return 0;
  }

  void runQuery(GraphType & Graph, VertexDescriptor StartVertex, 
      LockManagerType & LockManager) {
    getStartTime();  
    ///first find start person's friends
    AdjacencyVisitor AdjVisitor; 
    traverseThroughTypeAndDirection("KNOWS", "out", AdjVisitor.getFilter());
    ConcurrentBFS CcBfs;
    CcBfs.breadthFirstSearch(Graph, StartVertex, AdjVisitor, LockManager);

    ///find friends of friends of start person
    std::vector<VertexPointer> targets;
      Filter TmpFilter[6];
      traverseThroughMultiRelType("COMMENT_HAS_CREATOR", TmpFilter[0]); 
      traverseThroughMultiRelType("REPLY_OF_POST", TmpFilter[1]); 
      traverseThroughMultiRelType("POST_HAS_TAG", TmpFilter[2]); 
      traverseThroughMultiRelType("HAS_TYPE", TmpFilter[3]); 
      traverseThroughMultiRelType("IS_SUBCLASS_OF", TmpFilter[4]); 
      TmpFilter[5].setProperty(ParamPair.first, ParamPair.second);

    for (VertexPointer StartVertex : AdjVisitor.getVertexTargetList()) {
      SimMap[StartVertex] = 0;

      ExpertVisitor ExpertVisitor;
      ExpertVisitor.setDepth(5);
      ExpertVisitor.setDepthToCheckVertexProp(4);
      ExpertVisitor.setVertexFilter(TmpFilter[5]);

      for ( unsigned int i = 0; i < 5; i++ ) {
        ExpertVisitor.setFilter(TmpFilter[i]);
      }

      CcBfs.breadthFirstSearch(Graph, StartVertex->getId(), 
          ExpertVisitor, LockManager);
      auto iterend = ExpertVisitor.getPostMap().end();
      auto PostNum = 0;
      for ( auto iter = ExpertVisitor.getPostMap().begin(); iter != iterend; iter++ ) {
        if ((*iter).second ) { PostNum++; }
      }

#ifdef _PRINTLOG_
      SimMap[StartVertex] = PostNum; 
      LdbcFile << "person: " << StartVertex->getId() << " expert post " << PostNum << "\n";
#endif
  
    }
    CcBfs.releaseAll(LockManager);
    getExecTime();  
  }
protected:
  SimMapType SimMap;
};

class LdbcQuery13 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  static void *runQueries(void * ptr) {
    QueryHelper *Helper = (QueryHelper *) ptr;
    Helper->Query.runQuery((Helper->Graph), 
        (Helper->StartVertex), 
        (Helper->EndVertex), 
        (Helper->LockManager) );
    return 0;
  }

  void runQuery(GraphType & Graph, VertexDescriptor StartVertex, 
      VertexDescriptor endVertex, LockManagerType & LockManager) {

    getStartTime();  

    if ( StartVertex == endVertex ) {
      std::cerr << "StartVertex == endVertex \n";
      exit(0);
    }

    PathVisitor PVisitor;
    PVisitor.setEndVertex(endVertex);
    ConcurrentBFS CcBfs;
    CcBfs.breadthFirstSearch(Graph, StartVertex, PVisitor, LockManager);

#ifdef _PRINTLOG_
    auto target = PVisitor.getVertexTargetList();
    if (target.empty())
      LdbcFile << StartVertex << " and " <<  endVertex <<" are not connected" << endl;
    else {
      LdbcFile << "There are  shortest paths of length " << target.size() << "  from " << StartVertex << " to " << endVertex << endl;
      for(auto it = target.begin(); it != target.end(); ++it) {
        LdbcFile <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue("id").first << (*it)->getPropertyValue("firstName").first<< endl;
      }
    }
#endif

    CcBfs.releaseAll(LockManager);
    getExecTime();  
  }

};

class LdbcQuery14 : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  static void *runQueries(void * ptr) {
    QueryHelper *Helper = (QueryHelper *) ptr;
    Helper->Query.runQuery((Helper->Graph), 
        (Helper->StartVertex), 
        (Helper->EndVertex), 
        (Helper->LockManager) );
    return 0;
  }

  void runQuery(GraphType & Graph, VertexDescriptor StartVertex, 
      VertexDescriptor endVertex, LockManagerType & LockManager ) {
    getStartTime();  
    SubGraphVisitor SubgVisitor;
    SubgVisitor.setEndVertex(endVertex);
    Filter EdgeFilter;
    traverseThroughTypeAndDirection("KNOWS", "out",  EdgeFilter);
    SubgVisitor.setEdgeFilter(EdgeFilter);
    ConcurrentBFS CcBfs;
    CcBfs.breadthFirstSearch(Graph, StartVertex, SubgVisitor, LockManager);

    auto target = SubgVisitor.getVertexTargetList();

#ifdef _PRINTLOG_
    if (target.empty())
      LdbcFile << StartVertex << " and " <<  endVertex <<" are not connected" << endl;
    else {
      LdbcFile << "There are  shortest paths of length " << target.size() << "  from " << StartVertex << " to " << endVertex << endl;
      for(auto it = target.begin(); it != target.end(); ++it) {
        LdbcFile <<"Vertex " << (*it)->getId() << "\t" << (*it)->getPropertyValue("id").first <<"\t" << (*it)->getPropertyValue("firstName").first<< endl;
      }//for
    }//else
#endif

  ///already found all the paths, calculate weights now  
  auto itend = target.end();
  float Weight = 0.0;
  for ( auto it = target.begin(); it != itend-1; it++ ) {  
 
    Filter TmpFilter[4];
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR", TmpFilter[0]); 
    traverseThroughMultiRelType("REPLY_OF_POST+REPLY_OF_COMMENT", TmpFilter[1]); 
    traverseThroughMultiRelType("POST_HAS_CREATOR+COMMENT_HAS_CREATOR", TmpFilter[2]); 
  
    WeightedPathVisitor WPathVisitor;
    for ( unsigned int i = 0; i < 3; i++ ) {
      WPathVisitor.setFilter(TmpFilter[i]);
    }
  
    auto it2 = it+1;
    TmpFilter[3].setProperty("id",(*it2)->getPropertyValue("id").first.std_str());
    WPathVisitor.setVertexFilter(TmpFilter[3]);
    WPathVisitor.setDepth(3);
    CcBfs.breadthFirstSearch(Graph, (*it)->getId(), WPathVisitor, LockManager);
    Weight += WPathVisitor.getScore();
  } 

#ifdef _PRINTLOG_
  LdbcFile << "weight " << Weight <<"\n";
#endif
  CcBfs.releaseAll(LockManager);
  getExecTime();  
 }//run
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
  static void *runQueries(void * ptr) {
    QueryHelper *Helper = (QueryHelper *) ptr;
    Helper->Query.runQuery((Helper->Graph),(Helper->LockManager) );
    return 0;
  }

  void setValueList(ValueListType & vl) {
    ValueList = vl;
  }

  void setBranchMap(BranchMapType & bm) {
    BranchMap = bm;
  }

  virtual void runQuery(GraphType & Graph, LockManagerType & LockManager) {
    getStartTime();  
    //a new vertex vs 
    //need new propertylist for vertex and branchmap (criteria) for searching neighbor 
    AddVisitor AdVisitor(LockManager, Graph);
    AdVisitor.setVertexProperty(VertexPropertyList);
    AdVisitor.getFilter().setBranchMap(BranchMap);
    breadthFirstSearch(Graph, 0, AdVisitor);

    getExecTime();  
    LdbcFile << "Add one more node into network \n";
  }
protected:
  BranchMapType BranchMap;
  ValueListType ValueList;
};

/**
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

  virtual void runQuery(GraphType & Graph, TransactionManager & TransM, LockManagerType & LockManager, TMSwitch c) {
//    LdbcFile << "===============Skip Data================\n";
    getStartTime();  
    //a new vertex vs 
    //need new propertylist for vertex and branchmap (criteria) for searching neighbor 
    SkipVisitor v1(LockManager, Graph);
    v1.setVertexProperty(VertexPropertyList);
    v1.getFilter().setBranchMap(BranchMap);
    breadthFirstSearch(Graph, 0, v1);

    getExecTime();  

    //LdbcFile << "Add one more person into network \n";
  }
protected:
  BranchMapType BranchMap;
  ValueListType ValueList;
};

#endif
*/
