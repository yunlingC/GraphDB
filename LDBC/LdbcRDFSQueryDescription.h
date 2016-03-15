//===-- LDBC/LdbcRDFSQueryDescription.h - Query class ----------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the class for LDBC queries with RecursiveDepthFirstSearch
///        as the basic traversal.
///
//===----------------------------------------------------------------------===//

#ifndef _LDBCRDFSQUERYDESCRIPTION_H_
#define _LDBCRDFSQUERYDESCRIPTION_H_

#include "LdbcRDFSCustomVisitor.h"
#include "QueryDescription.h"
#include "LdbcQueryDescription.h"

#define _PRINTLDBC_ 1
#ifdef _PRINTLDBC_
#include <fstream>
std::ofstream LDBCFile;
#endif

/**
class LdbcQuery : public Query {
public:
  typedef std::pair<std::string, std::string> ParamPairType;
  typedef std::pair<std::string, std::pair<std::string, std::string> > RangePairType;
public:
  virtual void runQuery(Graph & graph, VertexDescriptor startVertex ) { }

  void setParam(const std::string & Key, const std::string & Value) {
    ParamPair.first = Key;
    ParamPair.second = Value;
  }
  
  void setParam(ParamPairType & param) {
    ParamPair = param;
  }

  void setRange(std::string Key, std::string Min, std::string Max) {
    ValueRange.first = Key;
    ValueRange.second.first = Min;
    ValueRange.second.second = Max;
  }

  void setPropertyRange(std::string Key, std::string A, std::string B) {
    PropRange.first = Key;
    PropRange.second.first = A;
    PropRange.second.second = B;
  }

protected:
  ParamPairType ParamPair;
  RangePairType ValueRange;
  RangePairType PropRange;
};

class LdbcQuery1 : public LdbcQuery {
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex) {
    FilterType tmpFilter;
    FilterType NameFilter;
    filtProperty(ParamPair.first, ParamPair.second, NameFilter); 
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter);
    LimitedDepthVisitor  v1;
    for (auto i = 0; i < 3; i++) {
      v1.setFilter(tmpFilter);
    }
    v1.setNameFilter(NameFilter);
    v1.setDepth(3);
    breadthFirstSearch(graph, startVertex, v1);
#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 1================\n";
    auto target = v1.getVertexList(); 
    LDBCFile << startVertex << " is connected with " 
             << target.size() << " people with " 
             << ParamPair.first <<": " 
             << ParamPair.second<< "\n";
      for (auto it = target.begin(); it != target.end(); ++it) {
        LDBCFile << "Vertex " << (*it)->getId() << "\t" 
                << (*it)->getPropertyValue("id").first << "\n"
                << (*it)->getPropertyValue("firstName").first <<"\t" 
                << (*it)->getPropertyValue("lastName").first;
        LDBCFile << "\n";
    }
      LDBCFile.close();
#endif
  }
};

class LdbcQuery2 : public LdbcQuery {
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
    FilterType tmpFilter[3];
    traverseThroughMultiRelType("KNOWS", tmpFilter[0]); 
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR", tmpFilter[1]); 
    tmpFilter[2].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second); 
   
    MultiRelTypeVisitor v2;
    v2.setFilter(tmpFilter[0]);
    v2.setFilter(tmpFilter[1]);
    v2.setRangeFilter(tmpFilter[2]);
    v2.setDepth(2);
    v2.setDepthToCheckRange(2);
    v2.setPropToCheck(2); //check date
    breadthFirstSearch(graph, startVertex, v2);
#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 2================\n";
    auto target = v2.getVertexList();
    auto targets = v2.getTargetsMap();
    for (auto it = targets.begin(); it != targets.end(); ++it) {
      LDBCFile << "person " << (*it).second->getPropertyValue("id").first << "\t" 
              << "comments/posts " << (*it).first->getPropertyValue("id").first << "\t" 
              << (*it).first->getPropertyValue("creationDate").first ;
      LDBCFile << "\n";
    }
      LDBCFile.close();
#endif
  }
};


class LdbcQuery3 : public LdbcQuery {
public:
  typedef std::map<VertexPointer, std::vector<VertexPointer>> PersonListMapType;
  typedef std::pair<VertexPointer, std::vector<VertexPointer>> PersonListPair;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
    FilterType tmpFilter;
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter);

    SingleRelTypeVisitor v3;
    v3.setFilter(tmpFilter);
    ///cannot be deleted
    v3.setFilter(tmpFilter);
    v3.setDepth(2);
    breadthFirstSearch(graph, startVertex, v3);
    auto target = v3.getVertexList();
#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 3================\n";
    LDBCFile << startVertex << " is connected with " << target.size() 
            << " friends and friends of friends" << "\n";
#endif
    FilterType FilterSet[4];
    traverseThroughMultiRelType("PERSON_IS_LOCATED_IN", FilterSet[0]); 
    traverseThroughMultiRelType("IS_PART_OF", FilterSet[1]); 
    auto country1 = PropRange.second.first;
    auto country2 = PropRange.second.second;
    FilterSet[2].setProperty("name", country1);
    FilterSet[3].setProperty("name", country2);
    
    auto it = target.begin(); 
    while (it != target.end()) {
      VertexPropertyRangeVisitor vp3;
      vp3.setFilter(FilterSet[0]);
      vp3.setFilter(FilterSet[1]);
      vp3.setVertexFilters(FilterSet[2]);
      vp3.setVertexFilters(FilterSet[3]);
      vp3.setDepth(2);
      vp3.setDepthToCheckVertexProp(2);
      auto startVertex = (*it)->getId();
      breadthFirstSearch(graph, startVertex, vp3);
      if (vp3.getIncludeState() == false) {
        target.erase(it);
      }
      else {
        it++;
      }
    }

    FilterType Filters[5];
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
      breadthFirstSearch(graph, startVertex, vpr3);
      auto targetList = vpr3.getVertexList();
      PersonListMap.insert(PersonListPair((*it), targetList));
    }

#ifdef _PRINTLDBC_
    for (auto it = PersonListMap.begin(); it != PersonListMap.end(); ++it) {
      LDBCFile << "friend " << (*it).first->getPropertyValue("firstName").first  
              << " has " <<(*it).second.size() 
              << " commmets/posts made in " << country1 
              << " or " << country2 << "\n"; 
    }
      LDBCFile.close();
#endif
  }
};

class LdbcQuery4 : public LdbcQuery {
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
    FilterType tmpFilter[4];
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
    breadthFirstSearch(graph, startVertex, v4);
#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 4================\n";
    auto targets = v4.getReturnResultMap();
    LDBCFile << startVertex << " has friends made posts of " 
             << targets.size() << " tags\n";
    for (auto it = targets.begin(); it != targets.end(); ++it) {
      LDBCFile << "tags " << (*it).first->getPropertyValue("id").first << "\t" 
              << "num of posts " << (*it).second <<  "\n";
    }
      LDBCFile.close();
#endif
  }
};

class LdbcQuery5 : public LdbcQuery {
public:
  typedef std::map<VertexPointer, std::vector<VertexPointer>> PersonListMapType;
  typedef std::pair<VertexPointer, std::vector<VertexPointer>> PersonListPair;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
    FilterType tmpFilter[2];
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter[0]);
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter[1]);

    SingleRelTypeVisitor v5;
    v5.setFilter(tmpFilter[0]);
    v5.setFilter(tmpFilter[1]);
    v5.setDepth(2);
    breadthFirstSearch(graph, startVertex, v5);
    auto target = v5.getVertexList();
#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 5================\n";
    LDBCFile << startVertex << " is connected with " 
             << target.size() << " friends and friends of friends" << "\n";
#endif
    FilterType Filters[4];
    traverseThroughMultiRelType("HAS_MEMBER", Filters[0]); 
    Filters[1].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second); 
    traverseThroughMultiRelType("CONTAINER_OF",Filters[2]); 
    traverseThroughMultiRelType("POST_HAS_CREATOR",Filters[3]); 
    std::vector<FilterType> VertexFilter;
    for (auto iter = target.begin(); iter != target.end(); iter++) {
      FilterType newFilter;
      newFilter.setProperty("id", (*iter)->getPropertyValue("id").first.std_str());
      VertexFilter.push_back(newFilter);
    }

    std::map<VertexPointer, unsigned int> targetMap;
    for (auto it = target.begin(); it != target.end(); it++) {
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
      breadthFirstSearch(graph, startVertex, mpv5);
      targetMap.insert(mpv5.getResultMap().begin(), mpv5.getResultMap().end());
    }
#ifdef _PRINTLDBC_
    for (auto it = targetMap.begin(); it != targetMap.end(); ++it) {
      if ((*it).second != 0)
        LDBCFile << "forum " << (*it).first->getPropertyValue("id").first  
                << " has " <<(*it).second << " posts made by friends"<< "\n"; 
    }
      LDBCFile.close();
#endif
  }
};

class LdbcQuery6 : public LdbcQuery {
public:
  typedef std::map<VertexPointer, std::vector<VertexPointer>> PersonListMapType;
  typedef std::pair<VertexPointer, std::vector<VertexPointer>> PersonListPair;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
    FilterType tmpFilter[2];
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter[0]);
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter[1]);

    SingleRelTypeVisitor v6;
    v6.setFilter(tmpFilter[0]);
    v6.setFilter(tmpFilter[1]);
    v6.setDepth(2);
    breadthFirstSearch(graph, startVertex, v6);
    auto target = v6.getVertexList();
#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 6================\n";
    LDBCFile << startVertex << " is connected with " 
            << target.size() << " friends and friends of friends" << "\n";
#endif

    FilterType Filters[3];
    traverseThroughMultiRelType("POST_HAS_CREATOR", Filters[0]); 
    traverseThroughMultiRelType("POST_HAS_TAG", Filters[1]); 
    Filters[2].setProperty(ParamPair.first, ParamPair.second);
    
    std::map<std::string, unsigned int> TagMap; 
    for (auto it = target.begin(); it != target.end(); it++) {
      SinglePropertyVisitor  vpv6;
      vpv6.setFilter(Filters[0]);
      vpv6.setFilter(Filters[1]);
      vpv6.setVertexFilter(Filters[2]);
      vpv6.setDepthToCheckVertexProp(2);
      vpv6.setDepth(2);
      unsigned int startVertex = (*it)->getId();
      breadthFirstSearch(graph, startVertex, vpv6);
      auto personMap = vpv6.getPersonMap(); 
      for (auto it = personMap.begin(); it != personMap.end(); it++) {
        if ((*it).second ) {
          for (auto iter = vpv6.getResultTargetsMap()[(*it).first].begin(); 
              iter != vpv6.getResultTargetsMap()[(*it).first].end(); iter++) {
            // (*iter) --> Tag id (string)
            if (TagMap.find(*iter) == TagMap.end()) {
              TagMap.insert(std::pair<std::string, unsigned int>((*iter), 1));
            } else {
              TagMap[*iter]++;
            }
          }
        }
      }
    }
#ifdef _PRINTLDBC_
    for (auto it = TagMap.begin(); it != TagMap.end(); it++)
      LDBCFile << "Tag " << (*it).first << " has " << (*it).second << " posts\n";

      LDBCFile.close();
#endif
  }
};

*/
class LdbcRDFSQuery7 : public LdbcQuery {
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
    FilterType Filters[2];
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR", Filters[0]); 
    traverseThroughMultiRelType("LIKES_COMMENT+LIKES_POST", Filters[1]); 
    LikesVisitor v7;
    v7.setFilter(Filters[0]);
    v7.setFilter(Filters[1]);
    v7.setDepth(2);
    recursiveDepthFirstSearch(graph, startVertex, v7);
#ifdef _PRINTLDBC_
    auto targetsMap = v7.getLikesMap();
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 7================\n";
    for (auto it = targetsMap.begin(); it != targetsMap.end(); it++) {
      LDBCFile << (*it).first->getPropertyValue("id").first << "\t" 
                << " get likes #" << (*it).second << "\n"; 
    }
    LDBCFile.close();
#endif
  }
};

class LdbcRDFSQuery8 : public LdbcQuery {
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex) {
    FilterType Filters[3];
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR", Filters[0]); 
    traverseThroughMultiRelType("REPLY_OF_COMMENT+REPLY_OF_POST", Filters[1]); 
  //  traverseThroughMultiRelType("COMMENT_HAS_CREATOR", Filters[2]); 

    RepliesVisitor v8;
    v8.setFilter(Filters[0]);
    v8.setFilter(Filters[1]);
//    v8.setFilter(Filters[2]);
//    v8.setDepth(3);
    v8.setDepth(2);
//    v8.setDepthToCompareTime(2);
    recursiveDepthFirstSearch(graph, startVertex, v8);
#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 8================\n";
    auto vertexMap = v8.getReplyMap();
    for (auto it = vertexMap.begin(); it != vertexMap.end(); it++) {
      LDBCFile << "Post/Comment " << (*it).first->getPropertyValue("id").first 
                << " got replies " << (*it).second.size()
                << "\n";
    }
    LDBCFile.close();
#endif
  }
};

/**
class LdbcQuery9 : public LdbcQuery {
public:
  typedef std::map<VertexPointer, VertexPointer> ReturnMapType;
  typedef std::pair<VertexPointer, VertexPointer> ReturnMapPair;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
    FilterType tmpFilter[2];
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter[0]);
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter[1]);
    SingleRelTypeVisitor v9;
    v9.setFilter(tmpFilter[0]);
    v9.setFilter(tmpFilter[1]);
    v9.setDepth(2);
    breadthFirstSearch(graph, startVertex, v9);
    auto target = v9.getVertexList();
#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 9================\n";
    LDBCFile << startVertex << " is connected with " 
              << target.size() << " friends and friends of friends" << "\n";
#endif
    FilterType Filters[2];
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR",Filters[0]); 
    Filters[1].setValueRange("creationDate", "", "2011-07-16T23:59:00.255"); 
          
    ReturnMapType TargetsMap; 
    for (auto it = target.begin(); it != target.end(); ++it) {
#ifdef _PRINTLDBC_
      LDBCFile << "friend " << (*it)->getId() << "\t" 
               << (*it)->getPropertyValue("id").first << "\t" 
               << (*it)->getPropertyValue("firstName").first  << "\n"; 
#endif
      MultiRelTypeVisitor sv9;
      sv9.setFilter(Filters[0]);
      sv9.setRangeFilter(Filters[1]);
      sv9.setDepth(1);
      sv9.setDepthToCheckRange(1);
      sv9.setPropToCheck(2); //check date 
      unsigned int startVertex = (*it)->getId();
      breadthFirstSearch(graph, startVertex, sv9);
      auto targets = sv9.getTargetsMap();
      TargetsMap.insert(targets.begin(), targets.end());
   }
#ifdef _PRINTLDBC_
    for (auto iter = TargetsMap.begin(); iter != TargetsMap.end(); ++iter) {
      LDBCFile << "posts/comments " << (*iter).first->getPropertyValue("id").first 
              << "\t" << (*iter).first->getPropertyValue("creationDate").first 
              << " made by person " << (*iter).second->getPropertyValue("id").first 
              << "\t" <<  (*iter).second->getPropertyValue("firstName").first << "\n";
    }
    LDBCFile.close();
#endif
  }
};

class LdbcQuery10 : public LdbcQuery {
public:
  typedef std::unordered_map<VertexPointer, unsigned int> SimMapType;  
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 10================\n";
#endif
    ///first find start person's friends
    AdjacencyVisitor av1; 
    traverseThroughTypeAndDirection("KNOWS", "out", av1.getFilter());
    breadthFirstSearch(graph, startVertex, av1);
    auto startId = graph.getVertexPointer(startVertex)->getPropertyValue("id").first.std_str();

    ///find friends of friends of start person
    std::vector<VertexPointer> targets;
    FilterType BDFilter;
    BDFilter.setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second); 
    for (VertexPointer StartVertex : av1.getVertexList()) {
      auto EqualFlag = false; 
      if (checkRange<VertexPointer>(4, StartVertex, BDFilter, EqualFlag) == true ) {
        SimMap[StartVertex] = 0;
        AdjacencyVisitor av2; 
        traverseThroughTypeAndDirection("KNOWS", "out", av2.getFilter());
        breadthFirstSearch(graph, StartVertex->getId(), av2);
        ///concatenate two lists
        targets.insert(targets.end(), av2.getVertexList().begin(), 
          av2.getVertexList().end());
      }

    }

    for (VertexPointer Vertex : targets) {
      auto EqualFlag = false; 
      if (checkRange<VertexPointer>(4, Vertex, BDFilter, EqualFlag) == true ) {
        SimMap[Vertex] = 0;
      }
    }

    ///iterate over the friends who satisfy the condition
    auto itend  = SimMap.end();
    for (auto it = SimMap.begin(); it != itend; it++ ) {
      FilterType tmpFilter[4];
      traverseThroughMultiRelType("POST_HAS_CREATOR", tmpFilter[0]); 
      traverseThroughMultiRelType("POST_HAS_TAG", tmpFilter[1]); 
      traverseThroughMultiRelType("HAS_INTEREST", tmpFilter[2]); 
      tmpFilter[3].setProperty("id", startId);

      SimilarityVisitor v10;
      v10.setDepth(3);
      v10.setDepthToCheckVertexProp(3);
      v10.setVertexFilter(tmpFilter[3]);
      for (unsigned int i = 0; i < 3; i++ ) {
        v10.setFilter(tmpFilter[i]);
      }
      breadthFirstSearch(graph, (*it).first->getId(), v10);
      auto iterend = v10.getPostMap().end();
      unsigned int PostItrd = 0;
      for (auto iter = v10.getPostMap().begin(); iter != iterend; iter++ ) {
        if ((*iter).second ) { PostItrd++; }
      }

      SimMap[(*it).first] = 2*PostItrd - v10.getPostMap().size(); 
#ifdef _PRINTLDBC_
      LDBCFile << "person: " << (*it).first->getId() 
              << " similarity score " << (*it).second << "\n";

      LDBCFile.close();
#endif
    }
  }
protected:
  SimMapType SimMap;
};

class LdbcQuery11 : public LdbcQuery {
public:
  typedef std::pair<EdgePointer, VertexPointer> MapPair;
  typedef std::multimap<VertexPointer, MapPair> MatchMapType; 
  typedef std::pair<VertexPointer, VertexPointer> ReturnMapPair;
  typedef std::vector<MatchMapType> ReturnTargetsType;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
    FilterType tmpFilter;
    traverseThroughTypeAndDirection("KNOWS", "", tmpFilter);

    SingleRelTypeVisitor v11;
    v11.setFilter(tmpFilter);
    v11.setFilter(tmpFilter);
    v11.setDepth(2);
    breadthFirstSearch(graph, startVertex, v11);
    auto target = v11.getVertexList();
#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 11================\n";
    LDBCFile << startVertex << " is connected with " 
             << target.size() << " friends and friends of friends" << "\n";
#endif
    FilterType Filters[4];
    traverseThroughMultiRelType("WORKS_AT",Filters[0]); 
    traverseThroughMultiRelType("ORGANISATION_IS_LOCATED_IN",Filters[1]); 
    Filters[2].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second); 
    Filters[3].setProperty(ParamPair.first, ParamPair.second);  

    MatchMapType TargetsMap;
    for (auto it = target.begin(); it != target.end(); ++it) {
#ifdef _PRINTLDBC_
      LDBCFile <<"friend " << (*it)->getId() << "\t" 
              << (*it)->getPropertyValue("id").first << "\t" 
              << (*it)->getPropertyValue("firstName").first  << "\n"; 
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
    breadthFirstSearch(graph, startVertex, sv11);
    auto targets = sv11.getMatchMap();
    TargetsMap.insert(targets.begin(), targets.end());
  }
#ifdef _PRINTLDBC_
    FixedString key("workFrom");
    for (auto iter = TargetsMap.begin(); iter != TargetsMap.end(); ++iter) {
      LDBCFile << (*iter).first->getPropertyValue("firstName").first 
              << " works at "  << (*iter).second.second->getPropertyValue("id").first 
              << " from " << (*iter).second.first->getPropertyValue(key).first << "\n";
      }
      LDBCFile.close();
#endif
  }
};

class LdbcQuery12 : public LdbcQuery {
public:
  typedef std::unordered_map<VertexPointer, unsigned int> SimMapType;  
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 12================\n";
#endif

    ///first find start person's friends
    AdjacencyVisitor av1; 
    traverseThroughTypeAndDirection("KNOWS", "out", av1.getFilter());
    breadthFirstSearch(graph, startVertex, av1);

    ///find friends of friends of start person
    std::vector<VertexPointer> targets;
      FilterType tmpFilter[6];
      traverseThroughMultiRelType("COMMENT_HAS_CREATOR", tmpFilter[0]); 
      traverseThroughMultiRelType("REPLY_OF_POST", tmpFilter[1]); 
      traverseThroughMultiRelType("POST_HAS_TAG", tmpFilter[2]); 
      traverseThroughMultiRelType("HAS_TYPE", tmpFilter[3]); 
      traverseThroughMultiRelType("IS_SUBCLASS_OF", tmpFilter[4]); 
      tmpFilter[5].setProperty(ParamPair.first, ParamPair.second);


    for (VertexPointer StartVertex : av1.getVertexList()) {
      SimMap[StartVertex] = 0;

      ExpertVisitor v10;
      v10.setDepth(5);
      v10.setDepthToCheckVertexProp(4);
      v10.setVertexFilter(tmpFilter[5]);
      for (unsigned int i = 0; i < 5; i++ ) {
        v10.setFilter(tmpFilter[i]);
      }
      breadthFirstSearch(graph, StartVertex->getId(), v10);
      auto iterend = v10.getPostMap().end();
      auto PostNum = 0;
      for (auto iter = v10.getPostMap().begin(); iter != iterend; iter++ ) {
        if ((*iter).second ) { PostNum++; }
      }

      SimMap[StartVertex] = PostNum; 
#ifdef _PRINTLDBC_
      LDBCFile << "person: " << StartVertex->getId() 
              << " expert post " << PostNum << "\n";
  
      LDBCFile.close();
#endif
    }
  }
protected:
  SimMapType SimMap;
};

*/

class LdbcRDFSQuery13 : public LdbcQuery {
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex, VertexDescriptor endVertex ) {
    SingleShortestPathVisitor v13;
    v13.setEndVertex(endVertex);
    breadthFirstSearch(graph, startVertex, v13);
#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 13================\n";
    auto target = v13.getVertexList();
    if (target.empty())
      LDBCFile << startVertex << " and " <<  endVertex 
                << " are not connected" << "\n";
    else {
      LDBCFile << "There are  shortest paths of length " << target.size() 
              << "  from " << startVertex << " to " << endVertex << "\n";
      for (auto it = target.begin(); it != target.end(); ++it) {
        LDBCFile <<"Vertex " << (*it)->getId() << "\t" 
                << (*it)->getPropertyValue("id").first 
                << (*it)->getPropertyValue("firstName").first<< "\n";
      }
    }
    LDBCFile.close();
#endif
  }
};

/**
class LdbcQuery14 : public Query {
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex, VertexDescriptor endVertex ) {
    SubGraphVisitor v13;
    v13.setEndVertex(endVertex);
    FilterType EdgeFilter;
    traverseThroughTypeAndDirection("KNOWS", "out",  EdgeFilter);
    v13.setEdgeFilter(EdgeFilter);
    breadthFirstSearch(graph, startVertex, v13);
    auto target = v13.getVertexList();
#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 14================\n";
    if (target.empty())
      LDBCFile << startVertex << " and " <<  endVertex 
               << " are not connected" << "\n";
    else {
      LDBCFile << "There are  shortest paths of length " << target.size() 
              << "  from " << startVertex << " to " << endVertex << "\n";
      for (auto it = target.begin(); it != target.end(); ++it) {
        LDBCFile <<"Vertex " << (*it)->getId() << "\t" 
                  << (*it)->getPropertyValue("id").first <<"\t" 
                  << (*it)->getPropertyValue("firstName").first<< "\n";
      }
    }
    LDBCFile.close();
#endif

///already found all the paths, calculate weights now  
  auto itend = target.end();
  float Weight = 0.0;
  for (auto it = target.begin(); it != itend-1; it++ ) {  
 
  FilterType tmpFilter[4];
  traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR", tmpFilter[0]); 
  traverseThroughMultiRelType("REPLY_OF_POST+REPLY_OF_COMMENT", tmpFilter[1]); 
  traverseThroughMultiRelType("POST_HAS_CREATOR+COMMENT_HAS_CREATOR", tmpFilter[2]); 

  WeightedPathVisitor v14;
  for (unsigned int i = 0; i < 3; i++ ) {
    v14.setFilter(tmpFilter[i]);
  }

    auto it2 = it+1;
    tmpFilter[3].setProperty("id",(*it2)->getPropertyValue("id").first.std_str());
    v14.setVertexFilter(tmpFilter[3]);
    v14.setDepth(3);
    breadthFirstSearch(graph, (*it)->getId(), v14);
    Weight += v14.getScore();
 } 

#ifdef _PRINTLDBC_
  LDBCFile << "weight " << Weight <<"\n";
  LDBCFile.close();
#endif

 }
};
*/

#endif /*_LDBCRDFSQUERYDESCRIPTION_H_*/
