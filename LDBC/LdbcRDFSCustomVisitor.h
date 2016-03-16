//===-- LDBC/LdbcCustomVisitor.h - Customed visitor class -----*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the customed class for Graph visitors designed for LDBC.
///
//===----------------------------------------------------------------------===//

#ifndef _LDBCRDFSCUSTOMVISITOR_H_
#define _LDBCRDFSCUSTOMVISITOR_H_

#include "RecursiveDFSCustomVisitor.h"
#include "Utils.h"

#include <map>
#include <unordered_set>

class SingleShortestPathVisitor : public RecursiveDFSShortestPathVisitor {
public:
  virtual bool scheduleEdge(EdgePointer Edge) {
    return TypeMatch;
  }

  virtual bool scheduleBranch(VertexPointer FirstVertex
                            , EdgePointer Edge
                            , VertexPointer SecondVertex) {

    if (ReturnFlag) {
      /// Flip back so that recursive functions won't return forever.
      ReturnFlag = false;
      return true;;
    }

    FilterType Filter;
    traverseThroughType("FRIENDS", Filter);
    TypeMatch = checkType(Edge, Filter);

    /// This is to make sure the same vertex won't be revisited again 
    /// in the current path --> prevent infinite loop
    auto PrevPath = PathStack.back();
    for (auto it = PrevPath.begin(), it_end = PrevPath.end();
            it != it_end; it++) {
      if ((*it) == SecondVertex) {
        return true;
      }
    }

    auto CurrentPath = PrevPath;
    CurrentPath.push_back(SecondVertex);

    if (SecondVertex->getId() == EndVertex) {
      ShortestPath = CurrentPath;
      TmpShortestLength = CurrentPath.size() - 1 ;
      /// Set to true so that the previous level can exit without 
      /// visiting more vertices of the same depth
      ReturnFlag = true;
    } else {
      PathStack.push_back(CurrentPath);
    }
    return false;
  }

protected:
  bool TypeMatch;
};

class LikesVisitor : public RecursiveDFSReachabilityVisitor {
public:
  typedef std::pair<VertexPointer, unsigned int> LikesPair;
  typedef std::map<VertexPointer, unsigned int> LikesMapType;
public:
  LikesVisitor() {}

  LikesMapType & getLikesMap() {
    return LikesMap;
  }

  virtual bool discoverVertex(VertexPointer Vertex) {
    /// No need to revisit vertex.
    return false;
  }

  virtual bool visitDirection(VertexPointer Vertex, EdgePointer Edge) {
    /// In LDBC dataset, most relationships indicate a direction.
    /// No need to check the direction then.
    return true;
  }

  virtual bool scheduleBranch(VertexPointer first
                            , EdgePointer edge
                            , VertexPointer second) {

    TypeMatch = false;
    DirectionMatch = true;

    ///Don't pop out until the current branch won't be visited again
    auto PrevPath = PathStack.back();

    unsigned int FirstDepth = 1000;
    if (PrevPath.back() == first) {
      FirstDepth = PrevPath.size() - 1; 
    }

    FilterType Filter;
    if (FirstDepth >= 0 && FirstDepth < DepthSetting) {
      Filter = FilterList[FirstDepth];
    }
    else {
      Filter.setDefault();
    }

    if (second != StartVertex) {
      TypeMatch = checkMultiRelType(edge, Filter);
    }

//    std::cout << "first " << first->getPropertyValue("id").first
//              << " -- second " << second->getPropertyValue("id").first
//              << " depth " << PrevPath.size() 
//              << " typeMatch " << TypeMatch << "\n";
    return false;
  }

  virtual bool lastVisit(VertexPointer Vertex) { 
    auto LastPath = PathStack.back();
    if (LastPath[LastPath.size()-1] == Vertex) {
      PathStack.pop_back();    
    }
    if (LastPath.size() ==  DepthSetting + 1) {
      /// If not in the map, add the entry
      if (LikesMap.find(LastPath[1]) == LikesMap.end()) {
        LikesMap.insert(LikesPair(LastPath[1], 1));
      } else {
        LikesMap[LastPath[1]]++; 
      }
//      VertexSet.insert(LastPath[DepthSetting]);
    }
    return false;
  }
 
protected:
  LikesMapType LikesMap;
};

class RepliesVisitor : public LikesVisitor {
public:
  typedef std::vector<VertexPointer> CommentListType; 
  typedef std::unordered_map<VertexPointer, CommentListType> CommentMapType;
  typedef std::pair<VertexPointer, CommentListType> CommentPairType;
public:
  CommentMapType & getReplyMap(){
    return ReplyMap;
  }

  virtual bool lastVisit(VertexPointer Vertex) { 
    auto LastPath = PathStack.back();
    if (LastPath[LastPath.size()-1] == Vertex) {
      PathStack.pop_back();    
    }
    if (LastPath.size() ==  DepthSetting + 1) {
      /// If not in the map, add the entry
      if (ReplyMap.find(LastPath[1]) == ReplyMap.end()) {
        CommentListType NewCommentList;
        NewCommentList.push_back(LastPath[2]);
        ReplyMap.insert(CommentPairType(LastPath[1], NewCommentList));
      } else {
        ReplyMap[LastPath[1]].push_back(LastPath[2]); 
      }
    }
    return false;
  }
protected:
  CommentMapType ReplyMap;
};

class FriendsVisitor : public RecursiveDFSReachabilityVisitor {
public:
  typedef std::unordered_set<VertexPointer> FriendListType;
public:
  FriendsVisitor() {}

  FriendListType & getFriendList() {
    return FriendList;
  }

  /// True get all friends within depthsetting
  /// False only get friends of depthsetting
  void isGettingAll(bool Setting) {
    FriendsHopSetting = Setting;
  }

  virtual bool lastVisit(VertexPointer Vertex) {
    auto LastPath = PathStack.back();
    if (LastPath[LastPath.size()-1] == Vertex) {
      PathStack.pop_back();    
    }
    if (LastPath.size() ==  DepthSetting + 1) {
      switch(FriendsHopSetting) {
        case false: {
          /// Only get friends of at a certain hop-DepthSetting
          FriendList.insert(LastPath[DepthSetting]);
          break;
                    }
        case true: {
          /// get all the friends within "DepthSetting" 
          for (unsigned int i = 1; i <= DepthSetting; i++) {
            FriendList.insert(LastPath[i]);
          }
          break;
                   }
      }
    }
    return false;
  }

protected:
  bool FriendsHopSetting;
  FriendListType FriendList;
};

class PostsCommentsVisitor : public AdjacencyVisitor {
public:
  PostsCommentsVisitor(){}

  void setRangeFilter(FilterType & filter) {
    RangeFilter = filter;
  }
  
  virtual bool visitDirection(VertexPointer Target, EdgePointer Edge) {
    return true;
  }

  virtual bool scheduleEdge(EdgePointer EdgePtr) {
    return TypeMatch;
  }

  virtual bool scheduleBranch(VertexPointer First
                            , EdgePointer Edge
                            , VertexPointer Second) {
//    DirectionMatch = true;
//    TypeMatch = false;

    auto equal = false;
    TypeMatch = checkMultiRelType(Edge, Filter) & checkRange<VertexPointer>(2, Second, RangeFilter, equal); 

    if (TypeMatch) {
      VertexSet.insert(Second);
    }

    computeDepth(First, Edge, Second, DepthList);

    return false;
  }

protected:
  bool TypeMatch;
  FilterType RangeFilter;
};

class JobsVisitor : public PostsCommentsVisitor {
public:
  virtual bool scheduleBranch(VertexPointer First
                            , EdgePointer Edge
                            , VertexPointer Second) {

    auto equal = false;
    TypeMatch = checkMultiRelType(Edge, Filter) & checkRange<EdgePointer>(3, Edge, RangeFilter, equal); 

    if (TypeMatch) {
      VertexSet.insert(Second);
    }

    computeDepth(First, Edge, Second, DepthList);

    return false;
  }
};


class TagsVisitor : public RecursiveDFSReachabilityVisitor {
public:
public:
  TagsVisitor(){} 

  void setRangeFilter(FilterType & filter) {
    RangeFilter = filter;
  }

  virtual bool discoverVertex(VertexPointer Vertex) {
    /// No need to revisit since the edge Label indicates the direction
    return false;
  }

//  virtual bool visitDirection(VertexPointer Vertex){
//    /// Edge labels indicate the dirction.
//    return true;
//  }

  virtual bool scheduleBranch(VertexPointer FirstVertex
                            , EdgePointer Edge
                            , VertexPointer SecondVertex ) {

    DirectionMatch = true;
    TypeMatch = false;

    auto PrevPath = PathStack.back();

    unsigned int FirstDepth = 1000;
    if (PrevPath.back() == FirstVertex) {
      FirstDepth = PrevPath.size() - 1; 
    }

    FilterType Filter;
    if (FirstDepth >= 0 && FirstDepth < DepthSetting) {
      Filter = FilterList[FirstDepth];
    }
    else {
      Filter.setDefault();
    }

    if (SecondVertex != StartVertex) {
      TypeMatch = checkType(Edge, Filter);
    }

    if (FirstDepth == 1) {
      /// 1 indicates checking time
      /// equal flag is not used here
      auto equal = false;
      TypeMatch = TypeMatch & \
                  checkRange<VertexPointer>(1, SecondVertex, RangeFilter, equal);
    }

    std::cout << "first " << FirstVertex->getId()
              << " -- second " << SecondVertex->getId()
              << " -- Type " << SecondVertex->getType()
              << " firstDepth " << FirstDepth
              << " depth " << PrevPath.size() 
              << " typeMatch " << TypeMatch << "\n";

    return false;
  }
  
protected:
  FilterType RangeFilter;
};
/**
class LimitedDepthVisitor : public Visitor {
public:
  typedef std::pair<FixedString, bool> ReturnValueType;
  typedef std::queue<VertexPath>  PathQueueType;
public:
  auto setNameFilter(FilterType & filter) 
    -> void {
    NameFilter = filter;
  }
   
  virtual void setFilter(FilterType & filter) {
    FilterList.push_back(filter);
  }

  virtual void setDepth(unsigned int depth) {
    DepthSetting = depth;
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    VertexPath newPath;
    newPath.push_back(startVertex);
    PathQueue.push(newPath);
  }

  virtual bool visitVertex(VertexPointer vertex) {
    PrevPath  = PathQueue.front();
    PathQueue.pop();
    if (PrevPath.size() > DepthSetting)
      return true;
    auto NameMatch = checkProperty<ReturnValueType>(vertex, NameFilter); 
    if (NameMatch == true) {
      VertexList.push_back(vertex);
    }
    return false; 
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    return TypeMatch;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, 
                              VertexPointer second) {
    unsigned int DepthSecond = 0;
    if (PrevPath.back() == first) {
      DepthSecond = PrevPath.size();
    }

    FilterType filter;
    if (DepthSecond > 0)
      filter = FilterList[DepthSecond-1];
    else
      filter.setDefault();
   
    TypeMatch = checkType(edge, filter);
    return false;
  }
  
  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if (TypeMatch ) {
      VertexPath newPath = PrevPath;
      newPath.push_back(second);
      PathQueue.push(newPath);
    }
    return false;
  }

protected:
  unsigned int  TypeMatch;
  unsigned int  DirecMatch;
  unsigned int  DepthSetting;
  std::vector<FilterType> FilterList;
  VertexPath   PrevPath;
  PathQueueType  PathQueue;
  FilterType NameFilter;
};

class SingleRelTypeVisitor : public LimitedDepthVisitor {
public:
  virtual bool visitVertex(VertexPointer vertex) {
    PrevPath  = PathQueue.front();
    PathQueue.pop();
    if (PrevPath.size() > DepthSetting)
      return true;
    else 
      return false; 
  }

  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if (TypeMatch) {
      VertexPath newPath = PrevPath;
      newPath.push_back(second);
      PathQueue.push(newPath);
      VertexList.push_back(second); //unique
    }
    return false;
  }
};
 
class MultiRelTypeVisitor: public ReachabilityVisitor {
public:
  typedef FilterType &  FilterReference;
  typedef std::pair<FixedString, bool> ReturnValueType;
  typedef std::map<VertexPointer, VertexPointer> ReturnTargetsType;
  typedef std::map<VertexPointer, VertexPointer> ReturnTargetsTypeReference;
  typedef std::pair<VertexPointer, VertexPointer> TargetsPair;
public:
  virtual void setRangeFilter(FilterType & filter) {
    RangeFilter = filter;
  }

  virtual void setDepthToCheckRange(unsigned int depth) {
      DepthToCheckRange= depth;
  }

  virtual ReturnTargetsTypeReference getTargetsMap() {
    return TargetsMap;
  }

  virtual void  setPropToCheck(unsigned int option) {
    PropToCheck = option;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    PrevPath = PathQueue.front();
    PathQueue.pop();
    if (PrevPath.size() > DepthSetting) {
      VertexList.push_back(PrevPath.at(DepthSetting));
      TargetsMap.insert(TargetsPair(PrevPath.at(DepthSetting), PrevPath.at(DepthSetting-1) ));
      while (!PathQueue.empty()) {
        auto path = PathQueue.front(); PathQueue.pop();
        bool unique = true;
        for (auto it = VertexList.begin() ; it != VertexList.end(); ++ it) {/// do not store repeated veretx
          if ( *it == path.at(DepthSetting)) {
            unique = false;
            break;
          }
        }
        if (unique == true) { 
          VertexList.push_back(path.at(DepthSetting));
          TargetsMap.insert(TargetsPair(path.at(DepthSetting), path.at(DepthSetting-1)));
        }//end if
      }
      return true;
    }
    return false;
  }

  virtual bool visitDirection(VertexPointer target, EdgePointer edge) {
    return true;
  }

  virtual bool discoverVertex(VertexPointer vertex) {
    return false;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int DepthSecond = 0;
    if (PrevPath.back() == first) {
      DepthSecond = PrevPath.size();
      CurrentDepth = DepthSecond;
    }

    FilterType filter;
    if (DepthSecond > 0) {
      filter = FilterList[DepthSecond-1];
    }
    else 
      filter.setDefault();
    
    TypeMatch = checkMultiRelType(edge, filter);
    if (TypeMatch == true) {
      if (CurrentDepth == DepthToCheckRange) {
        auto equal = false;
        TypeMatch = checkRange<VertexPointer>(PropToCheck, second, RangeFilter, equal);
      }
    }
    return false;
  }

  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if (TypeMatch) {
      VertexPath newPath = PrevPath;
      newPath.push_back(second);
      PathQueue.push(newPath);
    }
    return false;
  }

protected:
  unsigned int DepthToCheckRange = 1000;  //by default, prop won't be checked;
  unsigned int PropToCheck = 0;  /// 0 default 1 time 2 date 3 year
  unsigned int CurrentDepth;
  ReturnTargetsType TargetsMap;
  FilterType  RangeFilter;
};

class MultiResultVisitor : public MultiRelTypeVisitor {
public:
  typedef std::map<VertexPointer, unsigned int> ReturnResultMapType;
  typedef std::map<VertexPointer, unsigned int> &  ResultMapTypeReference;
  typedef std::pair<VertexPointer, unsigned int> ReturnMapPair;
public:

  virtual ResultMapTypeReference getReturnResultMap() {
    return ReturnResultMap;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    PrevPath = PathQueue.front();
    PathQueue.pop();
    if (PrevPath.size() > DepthSetting) {
      TargetsMap.insert(TargetsPair(PrevPath.at(DepthSetting), PrevPath.at(DepthSetting) ));
      ReturnResultMap.insert(ReturnMapPair(PrevPath.at(DepthSetting), 1));
      while (!PathQueue.empty()) {
        auto path = PathQueue.front(); PathQueue.pop();
        if (ReturnResultMap.find(path.at(DepthSetting)) == ReturnResultMap.end()) {
          ReturnResultMap.insert(ReturnMapPair(path.at(DepthSetting), 1));
        }//end if
        else {
          auto counter = ReturnResultMap[path.at(DepthSetting)] + 1;
          ReturnResultMap.insert(ReturnMapPair(path.at(DepthSetting), counter));  
        }
      }
      return true;
    }
    return false;
  }

protected:
  ReturnResultMapType ReturnResultMap;
};

class VertexPropertyVisitor : public MultiRelTypeVisitor {
public:
  typedef std::map<EdgePointer, VertexPointer> BranchMapType;
  typedef std::pair<EdgePointer, VertexPointer> BranchPair;
  typedef std::pair<VertexPointer, BranchPair> MapPair;
  typedef std::multimap<VertexPointer, BranchPair> PathMapType;

public:
  virtual void setDepthToCheckVertexProp ( unsigned int depth) {
    DepthToCheckVertexProp = depth;
  }

  virtual void setVertexFilter (FilterType & filter) {
    VertexFilter = filter;
  }

  virtual PathMapType & getMatchMap() {
    return PathMap;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int DepthSecond = 0;
    if (PrevPath.back() == first) {
      DepthSecond = PrevPath.size();
      CurrentDepth = DepthSecond;
    }

    FilterType filter;
    if (DepthSecond > 0) {
      filter = FilterList[DepthSecond-1];
    }
    else 
      filter.setDefault();
    
    auto MultiTypeMatch = checkMultiRelType(edge, filter);
    auto RangeMatch = true;
    auto VertexMatch = true;
    auto equalFlag = false;

    if ((CurrentDepth == DepthToCheckRange) && MultiTypeMatch) {
      RangeMatch = checkYearRange<EdgePointer>(edge, RangeFilter, equalFlag);
    }
    if ((CurrentDepth == DepthToCheckVertexProp ) && MultiTypeMatch) {
      VertexMatch = checkProperty<ReturnValueType>(second, VertexFilter); 
    }

    TypeMatch = (MultiTypeMatch && RangeMatch && VertexMatch);
    
    if (TypeMatch) {
      if (CurrentDepth != DepthSetting) {
        auto branchPair = std::make_pair(edge, second);
        PathMap.insert(MapPair(first, branchPair));
      } 
    }

    if ((CurrentDepth == DepthSetting) && (VertexMatch == false)){
      for (auto it = PathMap.begin(); it != PathMap.end(); it++) {
        if ((*it).second.second  == first) {
          PathMap.erase(it);
        }
      }
    } 
    return false;
  }

protected:
  FilterType VertexFilter;
  unsigned int DepthToCheckVertexProp;
  PathMapType PathMap;
};

class VertexPropertyRangeVisitor : public VertexPropertyVisitor {
public:
  virtual void setVertexFilters(FilterType & filter) {
    VertexFilterList.push_back(filter);
  }

  virtual bool getIncludeState() {
    return IncludeState;
  }
  
  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int DepthSecond = 0;
    if (PrevPath.back() == first) {
      DepthSecond = PrevPath.size();
      CurrentDepth = DepthSecond;
    }

    FilterType filter;
    if (DepthSecond > 0) {
      filter = FilterList[DepthSecond-1];
    }
    else 
      filter.setDefault();
    
    auto MultiTypeMatch = checkMultiRelType(edge, filter);
    bool RangeMatch = true;
    bool VertexMatch = true;
    if ((CurrentDepth == DepthToCheckRange) && MultiTypeMatch)  {
      bool equalFlag;
      RangeMatch = checkRange<EdgePointer>(PropToCheck, edge, RangeFilter, equalFlag); 
    }
    if ((CurrentDepth == DepthToCheckVertexProp ) && MultiTypeMatch) {
      VertexMatch = false;
      for (auto it = VertexFilterList.begin(); it != VertexFilterList.end(); it++) {
        if (checkProperty<ReturnValueType>(second, (*it)) == true) {
          VertexMatch = true;
          break;
        }
      }
    }

    TypeMatch = (MultiTypeMatch && RangeMatch && VertexMatch);
    
    if ((CurrentDepth == DepthSetting)){
      if (MultiTypeMatch && RangeMatch )
        if (VertexMatch == true) {
          IncludeState= false; 
        }
    } 
    return false;
  }

protected:
  bool IncludeState= true;
  std::vector<FilterType> VertexFilterList;
};

class MultiPropertyVisitor: public VertexPropertyRangeVisitor {
public:
  typedef std::vector<FilterType> FilterListType;
  typedef std::pair<VertexPointer, unsigned int> ResultPair;
  typedef std::map<VertexPointer, unsigned int> ResultMapType;
public:
  virtual void setVertexFilterlist(FilterType & filter) {
    VertexFilterList.push_back(filter);
  }

  virtual void setVertexFilterList (FilterListType & filterList) {
    VertexFilterList = filterList;
  }

  virtual ResultMapType & getResultMap() {
    return ResultMap;
  }
  
  virtual bool discoverVertex(VertexPointer vertex) {
    return true; 
  }
 
  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int DepthSecond = 0;
    if (PrevPath.back() == first) {
      DepthSecond = PrevPath.size();
      CurrentDepth = DepthSecond;
    }

    FilterType filter;
    if (DepthSecond > 0) {
      filter = FilterList[DepthSecond-1];
    }
    else 
      filter.setDefault();
    
    auto MultiTypeMatch = checkMultiRelType(edge, filter);
    bool RangeMatch = true;
    bool VertexMatch = true;
    if ((CurrentDepth == DepthToCheckRange) && MultiTypeMatch)  {
      bool equalFlag;
      RangeMatch = checkRange<EdgePointer>(PropToCheck, edge, RangeFilter, equalFlag); 
    }
    if ((CurrentDepth == DepthToCheckVertexProp ) && MultiTypeMatch) {
      VertexMatch = false;
      for (auto it = VertexFilterList.begin(); it != VertexFilterList.end(); it++) {
        if (checkProperty<ReturnValueType>(second, (*it)) == true) {
          VertexMatch = true;
          break;
        }
      }
    }
    TypeMatch = (MultiTypeMatch && RangeMatch && VertexMatch);
    return false;
  }

  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if (TypeMatch) {
      VertexPath newPath = PrevPath;
      newPath.push_back(second);
      PathQueue.push(newPath);
      if (CurrentDepth == 1) {
        ResultMap.insert(ResultPair(second, 0 ));
      }
      if (CurrentDepth == DepthSetting) {
        if (ResultMap.find(newPath[1]) != ResultMap.end()) {
          ResultMap[newPath[1]]++;
        } 
      }
    }
    return false;
  }

protected:
  ResultMapType ResultMap;
};

class TimeCompareVisitor : public MultiPropertyVisitor {
public:
  typedef std::map<VertexPointer, VertexPointer> VertexMapType;
  typedef std::pair<VertexPointer, VertexPointer> VertexPair;
public:
  virtual void setDepthToCompareTime(unsigned int depth) {
    DepthToCompareTime = depth;
  }

  virtual VertexMapType & getVertexMap() {
    return ReturnMap;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    PrevPath  = PathQueue.front();
    PathQueue.pop();
    RangeFilter.setValueRange("creationDate", "", "");
    if (PrevPath.size() > DepthSetting)
      return true;
    else 
      return false; 
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int DepthSecond = 0 ;
    if (PrevPath.back() == first) {
      DepthSecond = PrevPath.size();
      CurrentDepth = DepthSecond;
    }
    FilterType filter;
    if (DepthSecond > 0) 
      filter = FilterList[DepthSecond-1];
    else 
      filter.setDefault();
    
    auto MultiTypeMatch = true;
    if (CurrentDepth == 2) {
      if (first->getType().std_str() == "COMMENT") {
        filter.setType("REPLY_OF_COMMENT");
        MultiTypeMatch = checkType(edge, filter);
      } else 
      if (first->getType().std_str() == "POST") {
        filter.setType("REPLY_OF_COMMENT");
        MultiTypeMatch = checkType(edge, filter);
      }
    }
    else
      MultiTypeMatch = checkMultiRelType(edge, filter);
    bool RangeMatch = true;
    bool VertexMatch = true;
    if ((CurrentDepth == DepthToCompareTime) && MultiTypeMatch)  {
      bool DateEqualFlag = false; bool TimeEqualFlag = false;
      auto DateMatch = checkRange<VertexPointer>(2, second, RangeFilter, DateEqualFlag);
      auto TimeMatch = checkRange<VertexPointer>(1, second, RangeFilter, TimeEqualFlag);
        if ((DateEqualFlag && TimeMatch) || (DateMatch && (!DateEqualFlag))) {
          RangeFilter.setValueRange("creationDate", second->getPropertyValue("creationDate").first.std_str(), ""); 
          if (MatchMap.find(PrevPath[1]) == MatchMap.end()) {
          MatchMap.insert(std::pair<VertexPointer, VertexPointer>(PrevPath[1], second));
          } else {
          MatchMap[PrevPath[1]] = second;
         }
       }
    }
    if ((CurrentDepth == DepthToCompareTime + 1) && MultiTypeMatch) {
      std::map<VertexPointer, VertexPointer>::iterator it;
      for (it = MatchMap.begin(); it != MatchMap.end(); it++) {
        if ((*it).second == first)
          break;
      }
      if (it == MatchMap.end()) {
        VertexMatch = false;
      }
    }
    TypeMatch = (MultiTypeMatch && RangeMatch && VertexMatch);
    return false;
  }

  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if (TypeMatch) {
      VertexPath newPath = PrevPath;
      newPath.push_back(second);
      PathQueue.push(newPath);
      if (CurrentDepth == DepthSetting) {
        ReturnMap.insert(VertexPair(first, second));
        }
    }
    return false;
  }
protected:
  unsigned int DepthToCompareTime;
  VertexMapType MatchMap;
  VertexMapType ReturnMap; 
};


class VertexMatchVisitor: public TimeCompareVisitor {
public:
  typedef std::map<VertexPointer, bool> PersonMatchMapType;
  typedef std::pair<VertexPointer, bool> PersonMatchPair;
  typedef std::map<VertexPointer, VertexPointer> ReturnMapType; 
  typedef std::map<VertexPointer, std::string> TimeMatchMapType;
  typedef std::pair<VertexPointer, std::string> TimeMatchPair;
public:
  virtual TimeMatchMapType & getTimeMap() {
    return TimeMatchMap;
  }
  virtual PersonMatchMapType & getPersonMap() {
    return PersonMatchMap;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    PrevPath  = PathQueue.front();
    PathQueue.pop();
    PersonMatchMap.insert(PersonMatchPair(vertex, false));
    return  ((PrevPath.size() > DepthSetting) ? true : false);
  }


  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int DepthSecond = 0;
    if (PrevPath.back() == first) {
      DepthSecond = PrevPath.size();
      CurrentDepth = DepthSecond;
    }

    FilterType filter;
    if (DepthSecond > 0) {
      filter = FilterList[DepthSecond-1];
    }
    else 
      filter.setDefault();
    
    TypeMatch = checkMultiRelType(edge, filter);
    auto VertexMatch = false;
    if ((CurrentDepth == DepthToCheckVertexProp ) && TypeMatch) {
      auto it_end = VertexFilterList.end();
      for (auto it = VertexFilterList.begin(); it != it_end; it++) {
        if (checkProperty<ReturnValueType>(second, (*it)) == true) {
          VertexMatch = true;
          break;
        }
      }
      if (VertexMatch) {
        PersonMatchMap[first] = true;
      }
    }
     if ((CurrentDepth == DepthToCompareTime) && TypeMatch)  {
       FixedString key("creationDate");
       if (TimeMatchMap.find(second) == TimeMatchMap.end()) {
         TimeMatchMap.insert(TimeMatchPair(second, edge->getPropertyValue(key).first.std_str()));
       } else {
        RangeFilter.setValueRange("creationDate", TimeMatchMap[second], "");
        bool DateEqualFlag = false; bool TimeEqualFlag = false;
        auto DateMatch = checkRange<EdgePointer>(2, edge, RangeFilter, DateEqualFlag);
        auto TimeMatch = checkRange<EdgePointer>(1, edge, RangeFilter, TimeEqualFlag);
        if ((DateEqualFlag && TimeMatch) || (DateMatch && (!DateEqualFlag))) {
          TimeMatchMap[second] = edge->getPropertyValue(key).first.std_str(); 
         }
       }
   }
    return false;
  }


  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if (TypeMatch ) {
      VertexPath newPath = PrevPath;
      newPath.push_back(second);
      PathQueue.push(newPath);
    }
    return false;
  }

protected:
  PersonMatchMapType PersonMatchMap;
  TimeMatchMapType  TimeMatchMap;
};

class SinglePropertyVisitor : public VertexMatchVisitor {
public:
  typedef std::pair<VertexPointer, bool> VertexPair;
  typedef std::vector<std::string> VertexListType;
  typedef std::map<VertexPointer, VertexListType> TargetsMapType;
public:
  virtual TargetsMapType & getResultTargetsMap() {
    return ResultMap;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    PrevPath  = PathQueue.front(); PathQueue.pop();
    VertexListType VertexList;
    PersonMatchMap.insert(std::pair<VertexPointer, bool>(vertex, false));
    ResultMap.insert(std::pair<VertexPointer, VertexListType>(vertex, VertexList));
    return  ((PrevPath.size() > DepthSetting) ? true : false);
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int DepthSecond = 0;
    if (PrevPath.back() == first) {
      DepthSecond = PrevPath.size();
      CurrentDepth = DepthSecond;
    }

    FilterType filter;
    if (DepthSecond > 0) {
      filter = FilterList[DepthSecond-1];
    }
    else 
      filter.setDefault();
    
    TypeMatch = checkMultiRelType(edge, filter);
    if ((CurrentDepth == DepthToCheckVertexProp ) && TypeMatch) {
      if (checkProperty<ReturnValueType>(second, VertexFilter ) == true) {
        PersonMatchMap[first] = true;
      }
        ResultMap[first].push_back(second->getPropertyValue("id").first.std_str()); 
        /// tag is not visited yet; otherwise put this statement to scheduleTreee()
    }
    return false;
  }

protected:
  TargetsMapType ResultMap;
};

class SimilarityVisitor : public VertexMatchVisitor {
public:
  typedef std::pair<VertexPointer, bool> VertexPair;
  typedef std::vector<std::string> VertexListType;
  ///<post.id, post_interted_by_personx>
  typedef std::unordered_map<unsigned int, bool> PostMapType;
public:
  PostMapType & getPostMap() {
    return PostMap;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    PrevPath  = PathQueue.front(); PathQueue.pop();
    CurrentDepth = PrevPath.size();
    return  ((PrevPath.size() > DepthSetting ) ? true : false);
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int DepthSecond = 0 ;
    if (PrevPath.back() == first) {
      DepthSecond = PrevPath.size();
    }
    FilterType filter;
    if (DepthSecond > 0) {
      filter = FilterList[DepthSecond-1];
    }
    else 
      filter.setDefault();
    
    TypeMatch = checkMultiRelType(edge, filter);
    if ( (CurrentDepth == 1) && TypeMatch ) {
      PostMap.insert(std::pair<unsigned int, bool>(second->getId(), false));
    }

    if ((CurrentDepth == 3) && TypeMatch) {
      if (checkProperty<ReturnValueType>(second, VertexFilter)) {
        PostMap[PrevPath[1]->getId()] = true;
      }
    }
    return false;
  }

protected:
  PostMapType PostMap;
};

class ExpertVisitor : public SimilarityVisitor {
public:
  typedef std::pair<VertexPointer, bool> VertexPair;
  typedef std::vector<std::string> VertexListType;
  typedef std::unordered_map<unsigned int, bool> PostMapType;
public:
  PostMapType & getPostMap() {
    return PostMap;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    PrevPath  = PathQueue.front(); PathQueue.pop();
    CurrentDepth = PrevPath.size();
    return  ((PrevPath.size() > DepthSetting) ? true : false);
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {

    unsigned int DepthSecond = 0 ;
    if (PrevPath.back() == first) {
      DepthSecond = PrevPath.size();
    }
    FilterType filter;
    if (DepthSecond > 0) {
      filter = FilterList[DepthSecond-1];
    }
    else 
      filter.setDefault();
    
    TypeMatch = checkMultiRelType(edge, filter);


    if ( (CurrentDepth == 1) && TypeMatch ) {
      PostMap.insert(std::pair<unsigned int, bool>(second->getId(), false));
    }

    
    if ( (CurrentDepth == 4) && TypeMatch ) {
      if (checkProperty<ReturnValueType>(second, VertexFilter ) == true) {
        TypeMatch = false;
        PostMap[PrevPath[1]->getId()] = true;
      }
    }

    if ( (CurrentDepth == 5) && TypeMatch ) {
      if ( checkProperty<ReturnValueType>(second, VertexFilter ) == true) {
        PostMap[PrevPath[1]->getId()] = true;
      }
    }

    return false;
  }

protected:
  PostMapType PostMap;
};

class SubGraphVisitor : public PathVisitor {
public:
  typedef std::queue<VertexPath>  PathQueueType;
public:
  SubGraphVisitor (): TypeMatch(false), DirecMatch(false)  { }

  virtual void setEdgeFilter(FilterType & filter) {
    EdgeFilter = filter;
  }

  virtual bool visitDirection(VertexPointer target, EdgePointer Edge) {
    return DirecMatch;
  }

  virtual bool scheduleEdge(EdgePointer Edge) {
    return TypeMatch;
  }

  virtual bool scheduleBranch(VertexPointer First, EdgePointer Edge, VertexPointer Second) {
    TypeMatch = checkType(Edge, EdgeFilter);
    DirecMatch = checkDirection(Second, Edge, EdgeFilter);
    return false;
  }

  virtual bool scheduleTree(VertexPointer First, EdgePointer Edge, VertexPointer Second) {
    if ( TypeMatch && DirecMatch ) {
      VertexPath NewPath = PrevPath;
      NewPath.push_back(Second);
      PathQueue.push(NewPath);
      if (Second->getId() == EndVertex) {
        VertexList = NewPath;
        return true;
      }
    }
    return false;
  }

protected:
  FilterType EdgeFilter;
  bool TypeMatch;
  bool DirecMatch;
};


class WeightedPathVisitor : public SimilarityVisitor {
public:
  typedef std::pair<VertexPointer, bool> VertexPair;
  typedef std::vector<std::string> VertexListType;
public:

  WeightedPathVisitor ():Score(0.0) {}
  float getScore() {
    return Score;
  }

  virtual bool visitVertex(VertexPointer vertex) {  
    PrevPath  = PathQueue.front(); PathQueue.pop();
    CurrentDepth = PrevPath.size();
    return  ((PrevPath.size() > DepthSetting) ? true : false);
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {

    unsigned int DepthSecond = 0 ;
    if (PrevPath.back() == first) {
      DepthSecond = PrevPath.size();
    }
    FilterType filter;
    if (DepthSecond > 0) {
      filter = FilterList[DepthSecond-1];
    }
    else 
      filter.setDefault();
    
    TypeMatch = checkMultiRelType(edge, filter);

    if ( (CurrentDepth == 3) && TypeMatch ) {
      if ( checkProperty<ReturnValueType>(second, VertexFilter ) == true) {

        ///comment_replyOf_comment 0.5
        ///comment_replyOf_post    1.0
        auto Reply = PrevPath[1]->getType().std_str();
        auto ReplyOf = PrevPath[2]->getType().std_str();
        if ( "COMMENT" == Reply && "POST" == ReplyOf) {
          Score += 1; 
        } else if ( "COMMENT" == Reply && "COMMENT" == ReplyOf ) {
          Score += 0.5;
        }

      }
    }

///    if ( (CurrentDepth == 5) && _TypeMatch ) {
///      if ( checkProperty<ReturnValueType>(second, _VertexFilter ) == true) {
/////        std::cout << second->getPropertyValue("name").first.std_str() << "\n";
///        PostMap[PrevPath[1]->getId()] = true;
///      }
///    }

    return false;
  }

protected:
  float Score;
  ///  PostMapType PostMap;
};
*/

#endif /*_LDBCRDFSCUSTOMVISITOR_H_*/
