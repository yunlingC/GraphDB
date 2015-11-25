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

#ifndef _LDBCCUSTOMVISITOR_H_
#define _LDBCCUSTOMVISITOR_H_

#include "CustomVisitor.h"
#include "Utils.h"

class LimitedDepthVisitor : public Visitor {
public:
  typedef std::pair<FixedString, bool> ReturnValueType;
  typedef std::queue<VertexPath>  PathQueueType;
public:
  auto setNameFilter (Filter & filter) 
    -> void {
    _NameFilter = filter;
  }
   
  virtual void setFilter(Filter & filter) {
    FilterList.push_back(filter);
  }

  virtual void setDepth(unsigned int Depth) {
    DepthSetting = Depth;
  }


  virtual void visitStartVertex(VertexPointer StartVertex ) {
    VertexPath newPath;
    newPath.push_back(StartVertex);
    PathQueue.push(newPath);
  }

  virtual bool visitVertex(VertexPointer Vertex) {
    std::cout << "==>visit vertex " << Vertex->getId() << "\n";
    PrevPath  = PathQueue.front();
    PathQueue.pop();
    if (PrevPath.size() > DepthSetting)
      return true;
    auto NameMatch = checkProperty<ReturnValueType>(Vertex, _NameFilter); 
    if (NameMatch == true) {
      VertexList.push_back(Vertex);
      std::cout << "path size: " << PrevPath.size() << "\n";
      for (auto it = PrevPath.begin(); it != PrevPath.end(); ++it)
        std::cout << (*it)->getId() << "\t";
      std::cout << "\nVertex " << Vertex->getId() << " is found\n";
    }
    return false; 
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    return TypeMatch;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
//    std::cout << "-- schedule edge " << edge->getType() << "\n";
    unsigned int depthSecond = 0;
    if(PrevPath.back() == first) {
      depthSecond = PrevPath.size();
    }

    Filter filter;
    if(depthSecond > 0)
      filter = FilterList[depthSecond-1];
    else
      filter.setDefault();
   
    TypeMatch = checkType(edge, filter);
    return false;
  }
  
  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if(TypeMatch ) {
      VertexPath newPath = PrevPath;
      newPath.push_back(second);
      PathQueue.push(newPath);
      std::cout << "==>vid\t" << first->getPropertyValue("id").first << "\t" <<  second->getPropertyValue("id").first << "\t" << second->getPropertyValue("firstName").first << "\n";
    }
    return false;
  }

protected:
  unsigned int  TypeMatch;
  unsigned int  _direcMatch;
  unsigned int  DepthSetting;
  std::vector<Filter> FilterList;
  VertexPath   PrevPath;
  PathQueueType  PathQueue;
  Filter _NameFilter;
};

class SingleRelTypeVisitor : public LimitedDepthVisitor {
public:
  virtual bool visitVertex(VertexPointer Vertex) {
    std::cout << "==>visit vertex " << Vertex->getId() << "\n";
    PrevPath  = PathQueue.front();
    PathQueue.pop();
    if (PrevPath.size() > DepthSetting)
      return true;
    else 
      return false; 
  }

  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if(TypeMatch ) {
      VertexPath newPath = PrevPath;
      newPath.push_back(second);
      PathQueue.push(newPath);
      VertexList.push_back(second); //unique
      std::cout << "==>vid\t" << second->getPropertyValue("id").first << "\t" << second->getPropertyValue("firstName").first  << "\n";
    }
    return false;
  }
};
 
class MultiRelTypeVisitor: public ReachabilityVisitor {
public:
  typedef Filter &  FilterReference;
  typedef std::pair<FixedString, bool> ReturnValueType;
  typedef std::map<VertexPointer, VertexPointer> ReturnTargetsType;
  typedef std::map<VertexPointer, VertexPointer> ReturnTargetsTypeReference;
  typedef std::pair<VertexPointer, VertexPointer> TargetsPair;
public:
  virtual void setRangeFilter(Filter & filter) {
    _RangeFilter = filter;
  }

  virtual void setDepthToCheckRange(unsigned int depth) {
      _DepthToCheckRange= depth;
  }

  virtual ReturnTargetsTypeReference getTargetsMap() {
    return _TargetsMap;
  }

  virtual void  setPropToCheck(unsigned int opt) {
    _PropToCheck = opt;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    std::cout << "==>vid " << vertex->getId() << "\t" <<  vertex->getPropertyValue("id").first << vertex->getPropertyValue("firstName").first <<  "\n";
    PrevPath = PathQueue.front();
    PathQueue.pop();
    if(PrevPath.size() > DepthSetting) {
      VertexList.push_back(PrevPath.at(DepthSetting));
      _TargetsMap.insert(TargetsPair(PrevPath.at(DepthSetting), PrevPath.at(DepthSetting-1) ));
      while( !PathQueue.empty()) {
        auto path = PathQueue.front(); PathQueue.pop();
        bool unique = true;
        for (auto it = VertexList.begin() ; it != VertexList.end(); ++ it) {/// do not store repeated veretx
          if ( *it == path.at(DepthSetting)) {
            unique = false;
            break;
          }
        }
        if(unique == true) { 
          VertexList.push_back(path.at(DepthSetting));
          _TargetsMap.insert(TargetsPair(path.at(DepthSetting), path.at(DepthSetting-1)));
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
    unsigned int depthSecond;
    if(PrevPath.back() == first) {
      depthSecond = PrevPath.size();
      _CurrentDepth = depthSecond;
    }

    Filter filter;
    if(depthSecond > 0) {
      filter = FilterList[depthSecond-1];
    }
    else 
      filter.setDefault();
    
    for(auto it = filter.getTypeList().begin() ; it != filter.getTypeList().end(); it++) {
      std::cout << "Filt Type " << (*it) << "\t" ;
    }
    std::cout << "\n";
    TypeMatch = checkMultiRelType(edge, filter);
    if(TypeMatch == true) {
      if(_CurrentDepth == _DepthToCheckRange) {
        auto equal = false;
        switch(_PropToCheck) {
          case 1:
            TypeMatch = checkTimeRange<ReturnValueType, VertexPointer>(second, _RangeFilter, equal);
            break;
          case 2:
            TypeMatch = checkDateRange<ReturnValueType, VertexPointer>(second, _RangeFilter, equal);
            break;
          case 0:
            break;
          default:
            break;
        }
      }
    }
    std::cout << "scheduleBranch typeMatch : " << TypeMatch << "\n";

    return false;
  }

  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if (TypeMatch) {
      VertexPath newPath = PrevPath;
      newPath.push_back(second);
      PathQueue.push(newPath);
      std::cout << "first " << first->getPropertyValue("id").first << "\t" << " second " << second->getPropertyValue("id").first << " is on path now\n";
    }
    return false;
  }

protected:
  unsigned int _DepthToCheckRange= 1000;  //by default, prop won't be checked;
  unsigned int _PropToCheck = 0;  /// 0 default 1 time 2 date 3 year
  unsigned int _CurrentDepth;
  ReturnTargetsType _TargetsMap;
  Filter  _RangeFilter;
};

class MultiResultVisitor : public MultiRelTypeVisitor {
public:
  typedef std::map<VertexPointer, unsigned int> ReturnResultMapType;
  typedef std::map<VertexPointer, unsigned int> &  ResultMapTypeReference;
  typedef std::pair<VertexPointer, unsigned int> ReturnMapPair;
public:

  virtual ResultMapTypeReference getReturnResultMap() {
    return _ReturnResultMap;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    std::cout << "==>vid " << vertex->getId() << "\t" <<  vertex->getPropertyValue("id").first << "\n";
    PrevPath = PathQueue.front();
    PathQueue.pop();
    if(PrevPath.size() > DepthSetting) {
      _TargetsMap.insert(TargetsPair(PrevPath.at(DepthSetting), PrevPath.at(DepthSetting) ));
      _ReturnResultMap.insert(ReturnMapPair(PrevPath.at(DepthSetting), 1));
      while( !PathQueue.empty()) {
        auto path = PathQueue.front(); PathQueue.pop();
        if(_ReturnResultMap.find(path.at(DepthSetting)) == _ReturnResultMap.end()) {
          _ReturnResultMap.insert(ReturnMapPair(path.at(DepthSetting), 1));
        }//end if
        else {
          auto counter = _ReturnResultMap[path.at(DepthSetting)] + 1;
          _ReturnResultMap.insert(ReturnMapPair(path.at(DepthSetting), counter));  
        }
      }
      return true;
    }
    return false;
  }

protected:
  ReturnResultMapType _ReturnResultMap;
};

class VertexPropertyVisitor : public MultiRelTypeVisitor {
public:
  typedef std::map<EdgePointer, VertexPointer> BranchMapType;
  typedef std::pair<EdgePointer, VertexPointer> BranchPair;
  typedef std::pair<VertexPointer, BranchPair> MapPair;
  typedef std::multimap<VertexPointer, BranchPair> PathMapType;

public:
  virtual void setDepthToCheckVertexProp ( unsigned int depth) {
    _DepthToCheckVertexProp = depth;
  }

  virtual void setVertexFilter (Filter & filter) {
    _VertexFilter = filter;
  }

  virtual PathMapType & getMatchMap() {
    return _PathMap;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int depthSecond;
    if(PrevPath.back() == first) {
      depthSecond = PrevPath.size();
      _CurrentDepth = depthSecond;
    }

    Filter filter;
    if(depthSecond > 0) {
      filter = FilterList[depthSecond-1];
    }
    else 
      filter.setDefault();
    
    for(auto it = filter.getTypeList().begin() ; it != filter.getTypeList().end(); it++) {
//      std::cout << "Filt Type " << (*it) << "\t" ;
    }
//    std::cout << "edge label " << edge->getType() << "\n";
    auto MultiTypeMatch = checkMultiRelType(edge, filter);
    auto RangeMatch = true;
    auto VertexMatch = true;
    auto equalFlag = false;
    if((_CurrentDepth == _DepthToCheckRange) && MultiTypeMatch) 
      RangeMatch = checkYearRange<EdgePointer>(edge, _RangeFilter, equalFlag);
    if((_CurrentDepth == _DepthToCheckVertexProp ) && MultiTypeMatch) {
      VertexMatch = checkProperty<ReturnValueType>(second, _VertexFilter); 
//      std::cout << "vertex place.id " << second->getPropertyValue("name").first << "\n";
    }
//    std::cout << "scheduleBranch:  MultiTypeMatch  " << MultiTypeMatch << " RangeMatch " << RangeMatch << " VertexMatch " << VertexMatch  << "\n";
    TypeMatch = (MultiTypeMatch && RangeMatch && VertexMatch);
    
    if(TypeMatch ) {
      if(_CurrentDepth != DepthSetting) {
        auto branchPair = std::make_pair(edge, second);
        _PathMap.insert(MapPair(first, branchPair));
      } 
    }
    if((_CurrentDepth == DepthSetting) && (VertexMatch == false)){
      for (auto it = _PathMap.begin(); it != _PathMap.end(); it++) {
//        std::cout << "++VertexMatch " << VertexMatch <<"\t"<< (*it).second.second->getId() <<"\t"<< first->getId() << "\n";
        if ((*it).second.second  == first) {
          _PathMap.erase(it);
//          std::cout << "vertex " << first->getPropertyValue("id").first << " is rm\n";
        }
      }
    } //else
    return false;
  }

protected:
  Filter _VertexFilter;
  unsigned int _DepthToCheckVertexProp;
  PathMapType _PathMap;
};

class VertexPropertyRangeVisitor : public VertexPropertyVisitor {
public:
  virtual void setVertexFilters(Filter & filter) {
    _VertexFilterList.push_back(filter);
  }

  virtual bool getIncludeState() {
    return _IncludeState;
  }
  
  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int depthSecond;
    if(PrevPath.back() == first) {
      depthSecond = PrevPath.size();
      _CurrentDepth = depthSecond;
    }

    Filter filter;
    if(depthSecond > 0) {
      filter = FilterList[depthSecond-1];
    }
    else 
      filter.setDefault();
    
    for(auto it = filter.getTypeList().begin() ; it != filter.getTypeList().end(); it++) {
      std::cout << "Filt Type " << (*it) << "\t" ;
    }
    std::cout << "edge label " << edge->getType() << "\n";
    auto MultiTypeMatch = checkMultiRelType(edge, filter);
    bool RangeMatch = true;
    bool VertexMatch = true;
    std::cout << "++Current Depth " << _CurrentDepth << "\n";
    if((_CurrentDepth == _DepthToCheckRange) && MultiTypeMatch)  {
      bool equalFlag;
      RangeMatch = checkRange<EdgePointer>(_PropToCheck, edge, _RangeFilter, equalFlag); 
    }
    if((_CurrentDepth == _DepthToCheckVertexProp ) && MultiTypeMatch) {
      std::cout << "vertex place.name " << second->getPropertyValue("name").first << "\n";
      VertexMatch = false;
      for(auto it = _VertexFilterList.begin(); it != _VertexFilterList.end(); it++) {
        std::cout << "vertexfilter list " << (*it).getValue() << "\n";
        if(checkProperty<ReturnValueType>(second, (*it)) == true) {
          VertexMatch = true;
          break;
        }
      }
    }
    std::cout << "scheduleBranch:  MultiTypeMatch  " << MultiTypeMatch << " RangeMatch " << RangeMatch << " VertexMatch " << VertexMatch  << "\n";

    TypeMatch = (MultiTypeMatch && RangeMatch && VertexMatch);
    
    if((_CurrentDepth == DepthSetting)){
      if(MultiTypeMatch && RangeMatch )
        if(VertexMatch == true) {
          _IncludeState= false; 
          std::cout <<"-->Include state " << _IncludeState << "\n";
        }
    } 
    return false;
  }

protected:
  bool _IncludeState= true;
  std::vector<Filter> _VertexFilterList;
};

class MultiPropertyVisitor: public VertexPropertyRangeVisitor {
public:
  typedef std::vector<Filter> FilterListType;
  typedef std::pair<VertexPointer, unsigned int> ResultPair;
  typedef std::map<VertexPointer, unsigned int> ResultMapType;
public:
  virtual void setVertexFilterlist(Filter & filter) {
    _VertexFilterList.push_back(filter);
  }

  virtual void setVertexFilterList (FilterListType & filterList) {
    _VertexFilterList = filterList;
  }

  virtual ResultMapType & getResultMap() {
    return _ResultMap;
  }
  
  virtual bool discoverVertex(VertexPointer vertex) {
    return true; 
  }
 
  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int depthSecond;
    if(PrevPath.back() == first) {
      depthSecond = PrevPath.size();
      _CurrentDepth = depthSecond;
    }

    Filter filter;
    if(depthSecond > 0) {
      filter = FilterList[depthSecond-1];
    }
    else 
      filter.setDefault();
    
    auto MultiTypeMatch = checkMultiRelType(edge, filter);
    bool RangeMatch = true;
    bool VertexMatch = true;
    if((_CurrentDepth == _DepthToCheckRange) && MultiTypeMatch)  {
      bool equalFlag;
      RangeMatch = checkRange<EdgePointer>(_PropToCheck, edge, _RangeFilter, equalFlag); 
    }
    if((_CurrentDepth == _DepthToCheckVertexProp ) && MultiTypeMatch) {
      VertexMatch = false;
      for(auto it = _VertexFilterList.begin(); it != _VertexFilterList.end(); it++) {
        if(checkProperty<ReturnValueType>(second, (*it)) == true) {
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
      if(_CurrentDepth == 1) {
        _ResultMap.insert(ResultPair(second, 0 ));
      }
      if (_CurrentDepth == DepthSetting) {
        if(_ResultMap.find(newPath[1]) != _ResultMap.end()) {
          std::cout << _ResultMap[newPath[1]] << " posts are collected now\n";
          _ResultMap[newPath[1]]++;
        } else {
          std::cout << "Failed to find "<< newPath[1]->getPropertyValue("id").first << "\n";
        }
      }
    }
    return false;
  }

protected:
  ResultMapType _ResultMap;
};

class TimeCompareVisitor : public MultiPropertyVisitor {
public:
  typedef std::map<VertexPointer, VertexPointer> VertexMapType;
  typedef std::pair<VertexPointer, VertexPointer> VertexPair;
public:
  virtual void setDepthToCompareTime(unsigned int depth) {
    _DepthToCompareTime = depth;
  }

  virtual VertexMapType & getVertexMap() {
    return _ReturnMap;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    std::cout << "==>visit vertex " << vertex->getId() << "\t" << vertex->getPropertyValue("id").first << "\n";
    PrevPath  = PathQueue.front();
    PathQueue.pop();
    _RangeFilter.setValueRange("creationDate", "", "");
    if (PrevPath.size() > DepthSetting)
      return true;
    else 
      return false; 
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int depthSecond;
    if(PrevPath.back() == first) {
      depthSecond = PrevPath.size();
      _CurrentDepth = depthSecond;
    }
    Filter filter;
    if(depthSecond > 0) 
      filter = FilterList[depthSecond-1];
    else 
      filter.setDefault();
    
    auto MultiTypeMatch = true;
    if (_CurrentDepth == 2) {
      if(first->getType().std_str() == "COMMENT") {
        filter.setType("REPLY_OF_COMMENT");
        MultiTypeMatch = checkType(edge, filter);
      } else 
      if(first->getType().std_str() == "POST") {
        filter.setType("REPLY_OF_COMMENT");
        MultiTypeMatch = checkType(edge, filter);
      }
    }
    else
      MultiTypeMatch = checkMultiRelType(edge, filter);
    bool RangeMatch = true;
    bool VertexMatch = true;
    if((_CurrentDepth == _DepthToCompareTime) && MultiTypeMatch)  {
      bool DateEqualFlag = false; bool TimeEqualFlag = false;
      auto DateMatch = checkRange<VertexPointer>(2, second, _RangeFilter, DateEqualFlag);
      auto TimeMatch = checkRange<VertexPointer>(1, second, _RangeFilter, TimeEqualFlag);
        if((DateEqualFlag && TimeMatch) || (DateMatch && (!DateEqualFlag))) {
          _RangeFilter.setValueRange("creationDate", second->getPropertyValue("creationDate").first.std_str(), ""); 
          if(_MatchMap.find(PrevPath[1]) == _MatchMap.end()) {
          _MatchMap.insert(std::pair<VertexPointer, VertexPointer>(PrevPath[1], second));
          } else {
          _MatchMap[PrevPath[1]] = second;
         }
       }
    }
    if((_CurrentDepth == _DepthToCompareTime + 1) && MultiTypeMatch) {
      std::map<VertexPointer, VertexPointer>::iterator it;
      for(it = _MatchMap.begin(); it != _MatchMap.end(); it++) {
        if ((*it).second == first)
          break;
      }
      if(it == _MatchMap.end()) {
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
      if (_CurrentDepth == DepthSetting) {
        _ReturnMap.insert(VertexPair(first, second));
        }
    }
    return false;
  }
protected:
  unsigned int _DepthToCompareTime;
  VertexMapType _MatchMap;
  VertexMapType _ReturnMap; 
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
    return _TimeMatchMap;
  }
  virtual PersonMatchMapType & getPersonMap() {
    return _PersonMatchMap;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    std::cout << "==>visit vertex " << vertex->getId() << "\t" << vertex->getPropertyValue("id").first << "\n";
    PrevPath  = PathQueue.front();
    PathQueue.pop();
    _PersonMatchMap.insert(PersonMatchPair(vertex, false));
    return  ((PrevPath.size() > DepthSetting) ? true : false);
  }


  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int depthSecond;
    if(PrevPath.back() == first) {
      depthSecond = PrevPath.size();
      _CurrentDepth = depthSecond;
    }

    Filter filter;
    if(depthSecond > 0) {
      filter = FilterList[depthSecond-1];
    }
    else 
      filter.setDefault();
    
    TypeMatch = checkMultiRelType(edge, filter);
    auto VertexMatch = false;
    if((_CurrentDepth == _DepthToCheckVertexProp ) && TypeMatch) {
      for(auto it = _VertexFilterList.begin(); it != _VertexFilterList.end(); it++) {
        if(checkProperty<ReturnValueType>(second, (*it)) == true) {
          VertexMatch = true;
          break;
        }
      }
      if(VertexMatch) {
        _PersonMatchMap[first] = true;
      }
    }
     if((_CurrentDepth == _DepthToCompareTime) && TypeMatch)  {
       FixedString key("creationDate");
       if(_TimeMatchMap.find(second) == _TimeMatchMap.end()) {
         _TimeMatchMap.insert(TimeMatchPair(second, edge->getPropertyValue(key).first.std_str()));
       } else {
        _RangeFilter.setValueRange("creationDate", _TimeMatchMap[second], "");
        bool DateEqualFlag = false; bool TimeEqualFlag = false;
        auto DateMatch = checkRange<EdgePointer>(2, edge, _RangeFilter, DateEqualFlag);
        auto TimeMatch = checkRange<EdgePointer>(1, edge, _RangeFilter, TimeEqualFlag);
        if((DateEqualFlag && TimeMatch) || (DateMatch && (!DateEqualFlag))) {
          _TimeMatchMap[second] = edge->getPropertyValue(key).first.std_str(); 
         }
       }
   }
    return false;
  }


  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if(TypeMatch ) {
      VertexPath newPath = PrevPath;
      newPath.push_back(second);
      PathQueue.push(newPath);
    }
    return false;
  }

protected:
  PersonMatchMapType _PersonMatchMap;
  TimeMatchMapType  _TimeMatchMap;
};

class SinglePropertyVisitor : public VertexMatchVisitor {
public:
  typedef std::pair<VertexPointer, bool> VertexPair;
  typedef std::vector<std::string> VertexListType;
  typedef std::map<VertexPointer, VertexListType> TargetsMapType;
public:
  virtual TargetsMapType & getResultTargetsMap() {
    return _ResultMap;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    std::cout << "==>visit vertex " << vertex->getId() << "\t" << vertex->getPropertyValue("id").first << "\n";
    PrevPath  = PathQueue.front(); PathQueue.pop();
    VertexListType VertexList;
    _PersonMatchMap.insert(std::pair<VertexPointer, bool>(vertex, false));
    _ResultMap.insert(std::pair<VertexPointer, VertexListType>(vertex, VertexList));
    return  ((PrevPath.size() > DepthSetting) ? true : false);
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int depthSecond;
    if(PrevPath.back() == first) {
      depthSecond = PrevPath.size();
      _CurrentDepth = depthSecond;
    }

    Filter filter;
    if(depthSecond > 0) {
      filter = FilterList[depthSecond-1];
    }
    else 
      filter.setDefault();
    
    TypeMatch = checkMultiRelType(edge, filter);
    if((_CurrentDepth == _DepthToCheckVertexProp ) && TypeMatch) {
      std::cout << "++Tag id " << second->getPropertyValue("id").first << "\n"; 
      if(checkProperty<ReturnValueType>(second, _VertexFilter ) == true) {
        _PersonMatchMap[first] = true;
      }
        _ResultMap[first].push_back(second->getPropertyValue("id").first.std_str()); 
        /// tag is not visited yet; otherwise put this statement to scheduleTreee()
    }
    return false;
  }

protected:
  TargetsMapType _ResultMap;
};

#endif /*_LDBCCUSTOMVISITOR_H_*/
