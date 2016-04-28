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
*/
class LdbcRDFSQuery4 : public LdbcQuery {
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
    FilterType tmpFilter[4];
    traverseThroughType("KNOWS", tmpFilter[0]);
    traverseThroughType("POST_HAS_CREATOR", tmpFilter[1]); 
    traverseThroughType("POST_HAS_TAG", tmpFilter[2]); 
    tmpFilter[3].setValueRange(ValueRange.first, ValueRange.second.first
                             , ValueRange.second.second); 
   
    TagsVisitor v4;
    v4.setFilter(tmpFilter[0]);
    v4.setFilter(tmpFilter[1]);
    v4.setFilter(tmpFilter[2]);
    v4.setRangeFilter(tmpFilter[3]);
    v4.setDepth(3);
    recursiveDepthFirstSearch(graph, startVertex, v4);
#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 4================\n";
    auto targets = v4.getTargetSet();
    LDBCFile << startVertex << " has friends made posts of " 
             << targets.size() << " tags\n";
    for (auto it = targets.begin(), it_end = targets.end(); 
            it != it_end; ++it) {
      LDBCFile << "tags " << (*it)->getPropertyValue("id").first << "\t" 
              <<  "\n";
    }
      LDBCFile.close();
#endif
  }
};

/**
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

    RepliesVisitor v8;
    v8.setFilter(Filters[0]);
    v8.setFilter(Filters[1]);
    v8.setDepth(2);
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

class LdbcRDFSQuery9 : public LdbcQuery {
public:
  typedef std::unordered_set<VertexPointer> VertexSetType;
  typedef std::map<VertexPointer, VertexSetType> ReturnMapType;
  typedef std::pair<VertexPointer, VertexSetType> ReturnMapPair;
public:
  
  virtual VertexSetType getFriendsList(Graph & graph
                                        , VertexDescriptor startVertex
                                        , const bool & IsGettingAll
                                        , const unsigned int & DepthSetting) {
    FriendsVisitor v9;
    FilterType tmpFilter;
    traverseThroughTypeAndDirection("KNOWS", "out", tmpFilter);
    for (unsigned int i = 0; i < DepthSetting; i++) {
      v9.setFilter(tmpFilter);
    }
    v9.setDepth(DepthSetting);
    v9.isGettingAll(IsGettingAll);
    recursiveDepthFirstSearch(graph, startVertex, v9);
    return v9.getFriendList();
  }

  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
    auto target = getFriendsList(graph, startVertex, true, 2);

    FilterType Filters[2];
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR",Filters[0]); 
    Filters[1].setValueRange("creationDate", "", "2011-07-16T23:59:00.255"); 
    ReturnMapType VertexMap;
    for (auto it = target.begin(); it != target.end(); ++it) {
      PostsCommentsVisitor sv9;
      sv9.setFilter(Filters[0]);
      sv9.setRangeFilter(Filters[1]);
      unsigned int startVertex = (*it)->getId();
      recursiveDepthFirstSearch(graph, startVertex, sv9);
      VertexMap.insert(ReturnMapPair(*it, sv9.getVertexSet()));
   }
#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 9================\n";
    for (auto iter = VertexMap.begin(), iter_end = VertexMap.end(); 
            iter != iter_end; ++iter) {
      for (auto it = (*iter).second.begin(), it_end = (*iter).second.end();
              it != it_end; it++) {
        LDBCFile << "Friend " << (*iter).first->getPropertyValue("id").first 
              << "\t" <<  (*iter).first->getPropertyValue("firstName").first 
              << "\thas posts/comments on " 
              << (*it)->getPropertyValue("creationDate").first 
              << "\n";
      }
    }
    LDBCFile.close();
#endif
  }
};

class LdbcRDFSQuery2 : public LdbcRDFSQuery9 {
public:
  LdbcRDFSQuery2(){}

  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
    auto target = getFriendsList(graph, startVertex, false, 1);

#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 2================\n";
    LDBCFile << startVertex << " is connected with " 
              << target.size() << " friends and friends of friends" << "\n";
    for (auto it = target.begin(); it != target.end(); ++it) {
      LDBCFile << "friend " << (*it)->getId() << "\t" 
               << (*it)->getPropertyValue("id").first << "\t" 
               << (*it)->getPropertyValue("firstName").first  << "\n"; 
    }
#endif

    FilterType Filters[2];
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR",Filters[0]); 
    Filters[1].setValueRange("creationDate", "", "2011-07-16T23:59:00.255"); 
    ReturnMapType VertexMap;
    for (auto it = target.begin(); it != target.end(); ++it) {
      PostsCommentsVisitor sv9;
      sv9.setFilter(Filters[0]);
      sv9.setRangeFilter(Filters[1]);
      unsigned int startVertex = (*it)->getId();
      recursiveDepthFirstSearch(graph, startVertex, sv9);
      VertexMap.insert(ReturnMapPair(*it, sv9.getVertexSet()));
   }
#ifdef _PRINTLDBC_
    for (auto iter = VertexMap.begin(), iter_end = VertexMap.end(); 
            iter != iter_end; ++iter) {
      for (auto it = (*iter).second.begin(), it_end = (*iter).second.end();
              it != it_end; it++) {
        LDBCFile << "Friend " << (*iter).first->getPropertyValue("id").first 
              << "\t" <<  (*iter).first->getPropertyValue("firstName").first 
              << "\thas posts/comments on " 
              << (*it)->getPropertyValue("creationDate").first 
              << "\n";
      }
    }
    LDBCFile.close();
#endif
  }
};

class LdbcRDFSQuery1 : public LdbcRDFSQuery9 {
public:
  typedef std::pair<FixedString, bool> ReturnValueType;
public:
  LdbcRDFSQuery1(){}

  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
    auto target = getFriendsList(graph, startVertex, true, 3);

    std::vector<VertexPointer> FriendsList;
    FilterType NameFilter;
    filtProperty(ParamPair.first, ParamPair.second, NameFilter); 

    for (auto it = target.begin(); it != target.end(); ++it) {
      if (checkProperty<ReturnValueType>(*it, NameFilter)) {
        FriendsList.push_back(*it);
      }
    }
#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 1================\n";
    LDBCFile << startVertex << " is connected with " 
              << FriendsList.size() << " friends with given name" << "\n";
    for (auto it = FriendsList.begin(), it_end = FriendsList.end(); 
                it != it_end; ++it) {
      LDBCFile << "friend " << (*it)->getId() << "\t" 
               << (*it)->getPropertyValue("id").first << "\t" 
               << (*it)->getPropertyValue("firstName").first  << "\n"; 
    }
#endif
  }
};

class LdbcRDFSQuery5 : public LdbcRDFSQuery9 {
public:

  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
    auto target = getFriendsList(graph, startVertex, true, 2);

#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 5================\n";
    LDBCFile << startVertex << " is connected with " 
              << target.size() << " friends and friends of friends" << "\n";
    for (auto it = target.begin(); it != target.end(); ++it) {
      LDBCFile << "friend " << (*it)->getId() << "\t" 
               << (*it)->getPropertyValue("id").first << "\t" 
               << (*it)->getPropertyValue("firstName").first  << "\n"; 
    }
#endif

    FilterType Filters[2];
    traverseThroughMultiRelType("HAS_MEMBER", Filters[0]); 
    Filters[1].setValueRange("joinDate", "", "2011-07-16T23:59:00.255"); 
    ReturnMapType VertexMap;
    for (auto it = target.begin(); it != target.end(); ++it) {
      PostsCommentsVisitor sv9;
      sv9.setFilter(Filters[0]);
      sv9.setRangeFilter(Filters[1]);
      unsigned int startVertex = (*it)->getId();
      recursiveDepthFirstSearch(graph, startVertex, sv9);
      VertexMap.insert(ReturnMapPair(*it, sv9.getVertexSet()));
   }
#ifdef _PRINTLDBC_
    for (auto iter = VertexMap.begin(), iter_end = VertexMap.end(); 
            iter != iter_end; ++iter) {
      for (auto it = (*iter).second.begin(), it_end = (*iter).second.end();
              it != it_end; it++) {
        LDBCFile << "Friend " << (*iter).first->getPropertyValue("id").first 
              << "\t" <<  (*iter).first->getPropertyValue("firstName").first 
              << "\thas posts/comments on " 
              << (*it)->getPropertyValue("creationDate").first 
              << "\n";
      }
    }
    LDBCFile.close();
#endif
  }
};

class LdbcRDFSQuery6 : public LdbcRDFSQuery9 {
public:
  typedef std::map<VertexPointer, std::vector<VertexPointer>> PersonListMapType;
  typedef std::pair<VertexPointer, std::vector<VertexPointer>> PersonListPair;
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
    auto target = getFriendsList(graph, startVertex, true, 2);
#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 6================\n";
    LDBCFile << startVertex << " is connected with " 
            << target.size() << " friends and friends of friends" << "\n";
#endif

    FilterType Filters[3];
    traverseThroughType("POST_HAS_CREATOR", Filters[0]); 
    traverseThroughType("POST_HAS_TAG", Filters[1]); 
    Filters[2].setProperty("id", "62");
    
    std::unordered_map<VertexPointer, unsigned int> TagMap;
    TagCooccurrenceVisitor vpv6;
    vpv6.setFilter(Filters[0]);
    vpv6.setFilter(Filters[1]);
    vpv6.setPropertyFilter(Filters[2]);
    vpv6.setDepth(2);
    for (auto it = target.begin(); it != target.end(); it++) {
      unsigned int startVertex = (*it)->getId();
      recursiveDepthFirstSearch(graph, startVertex, vpv6);
    }
      TagMap = vpv6.getTargetMap(); 
#ifdef _PRINTLDBC_
    for (auto it = TagMap.begin(); it != TagMap.end(); it++)
      LDBCFile << "Tag " << (*it).first->getId() << " has " << (*it).second << " posts\n";

      LDBCFile.close();
#endif
  }
};

class LdbcRDFSQuery11 : public LdbcRDFSQuery9 {
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
    /// Get friends and friends of friends
    auto target = getFriendsList(graph, startVertex, true, 2);

    FilterType Filters[4];
    traverseThroughType("WORKS_AT",Filters[0]); 
    traverseThroughType("ORGANISATION_IS_LOCATED_IN",Filters[1]); 
    Filters[2].setValueRange(ValueRange.first
                           , ValueRange.second.first
                           , ValueRange.second.second); 
    Filters[3].setProperty(ParamPair.first, ParamPair.second);  

    JobsVisitor sv11;
    sv11.setFilter(Filters[0]);
    sv11.setFilter(Filters[1]);
    sv11.setRangeFilter(Filters[2]);
    sv11.setPropertyFilter(Filters[3]);
    sv11.setDepth(2);
    for (auto Person : target) {
      unsigned int startVertex = Person->getId();
      recursiveDepthFirstSearch(graph, startVertex, sv11);
    }

#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 11================\n";
    auto Targets = sv11.getTargetMap();
    for (auto PersonJob : Targets) {
      LDBCFile << PersonJob.first->getPropertyValue("firstName").first
               << " has jobs #" << PersonJob.second.size() << " \n";
      for (auto Job : PersonJob.second) {
        LDBCFile << " works at company " << Job.first->getPropertyValue("name").first 
                << " from " << Job.second
                << "\n";
      }
    }
    LDBCFile.close();
#endif
  }
};


class LdbcRDFSQuery3 : public LdbcRDFSQuery9 {
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
    /// Get friends and friends of friends
    auto target = getFriendsList(graph, startVertex, true, 2);

    FilterType FilterSet[4];
    traverseThroughType("PERSON_IS_LOCATED_IN", FilterSet[0]); 
    traverseThroughType("IS_PART_OF", FilterSet[1]); 
    auto country1 = PropRange.second.first;
    auto country2 = PropRange.second.second;
    FilterSet[2].setProperty("name", country1);
    FilterSet[3].setProperty("name", country2);
    
    CountriesVisitor vp3;
    vp3.setFilter(FilterSet[0]);
    vp3.setFilter(FilterSet[1]);
    vp3.setPropertyFilter(FilterSet[2]);
    vp3.setAnotherPropertyFilter(FilterSet[3]);
    vp3.setDepth(2);

    auto it = target.begin(); 
    auto it_prev = it;
    while (it != target.end()) {
      auto startVertex = (*it)->getId();
      recursiveDepthFirstSearch(graph, startVertex, vp3);
      it_prev = it;
      it++;
      if (vp3.isIncluded()) {
        target.erase(it_prev);
      }
      std::cout << target.size() << " friends left \n";
    }

//#ifdef _PRINTLDBC_
//    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
//    LDBCFile << "===============Query 3================\n";
//    for (auto Friend : target) {
//      LDBCFile << "friend " << Friend->getPropertyValue("firstName").first  
//              << " is not located in " << country1 
//              << " or " << country2 
//              << "\n"; 
//    }
//    LDBCFile << "\n";
//#endif


    FilterType Filters[5];
    traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR",Filters[0]); 
    Filters[1].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second); 
    traverseThroughMultiRelType("POST_IS_LOCATED_IN+COMMENT_IS_LOCATED_IN",Filters[2]); 
    Filters[3].setProperty("name", country1); 
    Filters[4].setProperty("name", country2); 
    
    TravellersVisitor vpr3;
    vpr3.setFilter(Filters[0]);
    vpr3.setFilter(Filters[2]);
    vpr3.setRangeFilter(Filters[1]);
    vpr3.setPropertyFilter(Filters[4]);
    vpr3.setAnotherPropertyFilter(Filters[3]);
    vpr3.setDepth(2);
    for (auto Friend : target) {
      auto startVertex = Friend->getId();
      recursiveDepthFirstSearch(graph, startVertex, vpr3);
    }

#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 3================\n";
    LDBCFile << startVertex 
      << " has friends travelling to foreign countries and make post/comment \n";
    auto TargetMap = vpr3.getTargetMap();
    LDBCFile << "friend firstName " << "\t" 
            << country1  << "\t"
            << country2 << "\n";
    for (auto PersonPC : TargetMap ) {
      LDBCFile << PersonPC.first->getPropertyValue("firstName").first  
              << "\t\t"
              << PersonPC.second.first
              << "\t\t"
              << PersonPC.second.second
              << "\n"; 
    }
      LDBCFile.close();
#endif
  }
};


class LdbcRDFSQuery10 : public LdbcRDFSQuery9 {
public:
  typedef std::unordered_map<VertexPointer, unsigned int> SimMapType;  
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex ) {
    /// Only get friends of friends not immediate friends
    FriendsBirthdayVisitor v9;
    FilterType tmpFilter;
    traverseThroughTypeAndDirection("KNOWS", "out", tmpFilter);
    for (unsigned int i = 0; i < 2; i++) {
      v9.setFilter(tmpFilter);
    }
    FilterType BDFilter;
//    BDFilter.setValueRange(ValueRange.first, ValueRange.second.first
//                          , ValueRange.second.second); 
    BDFilter.setValueRange("birthday", "1981-08-21T", "1981-09-22T"); 
    v9.setDepth(2);
    v9.setRangeFilter(BDFilter);
    recursiveDepthFirstSearch(graph, startVertex, v9);
    auto Friends = v9.getFriendList();

#ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 10================\n";
    LDBCFile << startVertex << " is connected with " 
             << Friends.size() << " friends \n";
    for (auto Friend : Friends) {
      LDBCFile << "Friend " << Friend->getId() 
                << " birthday " << Friend->getPropertyValue("birthday").first
                << "\n";
    }
#endif

    FilterType Filters[4];
    traverseThroughType("POST_HAS_CREATOR", Filters[0]); 
    traverseThroughType("POST_HAS_TAG", Filters[1]); 
    traverseThroughType("HAS_INTEREST", Filters[2]); 
    FriendsSimilarityVisitor  FSV;
    for (auto i = 0; i < 3; i++){
      FSV.setFilter(Filters[i]);
    }

    auto Value = graph.getVertexPointer(startVertex)->getPropertyValue("id").first.getString();
    Filters[3].setProperty("id", Value);
    FSV.setPropertyFilter(Filters[3]);
    FSV.setDepth(3);

    for (auto Friend : Friends) {
      recursiveDepthFirstSearch(graph, Friend->getId(), FSV);
    }

#ifdef _PRINTLDBC_
    auto SimilarityMap = FSV.getTargetMap();
    for(auto SimilarityPair: FSV.getTargetMap()) {
      LDBCFile << "Person " << SimilarityPair.first->getId() 
              << " similarity score " << SimilarityPair.second
              << "\n";
    }
#endif

  }
protected:
  SimMapType SimMap;
};


class LdbcRDFSQuery12 : public LdbcQuery {
public:
  typedef std::unordered_map<VertexPointer, unsigned int> SimMapType;  
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex ) {

    FilterType tmpFilter[7];
    traverseThroughTypeAndDirection("KNOWS", "out", tmpFilter[0]);
    traverseThroughType("COMMENT_HAS_CREATOR", tmpFilter[1]); 
    traverseThroughType("REPLY_OF_POST", tmpFilter[2]); 
    traverseThroughType("POST_HAS_TAG", tmpFilter[3]); 
    traverseThroughType("HAS_TYPE", tmpFilter[4]); 
    traverseThroughType("IS_SUBCLASS_OF", tmpFilter[5]); 
    tmpFilter[6].setProperty(ParamPair.first, ParamPair.second);


    ExpertSearchVisitor v10;
    v10.setDepth(6);
    v10.setPropertyFilter(tmpFilter[6]);

    for (unsigned int i = 0; i < 6; i++ ) {
      v10.setFilter(tmpFilter[i]);
    }

    recursiveDepthFirstSearch(graph, startVertex, v10);

  #ifdef _PRINTLDBC_
    LDBCFile.open("ldbc_rdfs.log", std::ios::out | std::ios::app);
    LDBCFile << "===============Query 12================\n";
    auto PersonMap = v10.getTargetMap();

    for (auto PersonPCPair: PersonMap) {
      LDBCFile << "person: " << PersonPCPair.first->getId() 
              << " expert post " << PersonPCPair.second 
              << "\n";
    } 

    LDBCFile.close();
  #endif
  }
};


class LdbcRDFSQuery13 : public LdbcQuery {
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex, VertexDescriptor endVertex ) {
    SingleShortestPathVisitor v13;
    v13.setEndVertex(endVertex);
    recursiveDepthFirstSearch(graph, startVertex, v13);
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

class LdbcRDFSQuery14 : public Query {
public:
  void runQuery(Graph & graph, VertexDescriptor startVertex, VertexDescriptor endVertex ) {
    SubGraphVisitor v13;
    v13.setEndVertex(endVertex);
    FilterType EdgeFilter;
    traverseThroughTypeAndDirection("KNOWS", "out",  EdgeFilter);
    v13.setEdgeFilter(EdgeFilter);
    recursiveDepthFirstSearch(graph, startVertex, v13);
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

#endif /*_LDBCRDFSQUERYDESCRIPTION_H_*/
