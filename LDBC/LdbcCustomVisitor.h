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

#include <queue>

#include "CustomVisitor.h"
#include "Utilities.h"

using namespace std;

class LimitedDepthVisitor : public Visitor {
public:
  typedef pair<FixedString, bool> ReturnValueType;
  typedef std::queue<VertexPath>  PathQueue;
public:
  auto setNameFilter (Filter & filter) 
    -> void {
    _NameFilter = filter;
  }
   
  virtual void setFilter(Filter & f) {
    _filterList.push_back(f);
  }

  virtual void setDepth(unsigned int depth) {
    _depthSetting = depth;
  }


  virtual void visitStartVertex(VertexPointer startVertex ) {
    VertexPath newPath;
    newPath.push_back(startVertex);
    _pathQueue.push(newPath);
  }

  virtual bool visitVertex(VertexPointer vertex) {
    cout << "==>visit vertex " << vertex->getId() << endl;
    _prevPath  = _pathQueue.front();
    _pathQueue.pop();
    if (_prevPath.size() > _depthSetting)
      return true;
    auto NameMatch = checkProperty<ReturnValueType>(vertex, _NameFilter); 
    if (NameMatch == true) {
      _VertexTargetList.push_back(vertex);
      cout << "path size: " << _prevPath.size() << endl;
      for (auto it = _prevPath.begin(); it != _prevPath.end(); ++it)
        cout << (*it)->getId() << "\t";
      cout << "\nVertex " << vertex->getId() << " is found\n";
    }
    return false; 
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    return _typeMatch;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
//    cout << "-- schedule edge " << edge->getType() << endl;
    unsigned int depthSecond;
    if(_prevPath.back() == first) {
      depthSecond = _prevPath.size();
    }

    Filter filter;
    if(depthSecond > 0)
      filter = _filterList[depthSecond-1];
    else
      filter.setDefault();
   
    _typeMatch = checkType(edge, filter);
    return false;
  }
  
  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if(_typeMatch ) {
      VertexPath newPath = _prevPath;
      newPath.push_back(second);
      _pathQueue.push(newPath);
      cout << "==>vid\t" << first->getPropertyValue("id").first << "\t" <<  second->getPropertyValue("id").first << "\t" << second->getPropertyValue("firstName").first << endl;
    }
    return false;
  }

protected:
  unsigned int  _typeMatch;
  unsigned int  _direcMatch;
  unsigned int  _depthSetting;
  std::vector<Filter> _filterList;
  VertexPath   _prevPath;
  PathQueue  _pathQueue;
  Filter _NameFilter;
};

class SingleRelTypeVisitor : public LimitedDepthVisitor {
public:
  virtual bool visitVertex(VertexPointer vertex) {
    cout << "==>visit vertex " << vertex->getId() << endl;
    _prevPath  = _pathQueue.front();
    _pathQueue.pop();
    if (_prevPath.size() > _depthSetting)
      return true;
    else 
      return false; 
  }

  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if(_typeMatch ) {
      VertexPath newPath = _prevPath;
      newPath.push_back(second);
      _pathQueue.push(newPath);
      _VertexTargetList.push_back(second); //unique
      cout << "==>vid\t" << second->getPropertyValue("id").first << "\t" << second->getPropertyValue("firstName").first  << endl;
    }
    return false;
  }
};
 
class MultiRelTypeVisitor: public ReachabilityVisitor {
public:
  typedef Filter &  FilterReference;
  typedef pair<FixedString, bool> ReturnValueType;
  typedef map<VertexPointer, VertexPointer> ReturnTargetsType;
  typedef map<VertexPointer, VertexPointer> ReturnTargetsTypeReference;
  typedef pair<VertexPointer, VertexPointer> TargetsPair;
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
    cout << "==>vid " << vertex->getId() << "\t" <<  vertex->getPropertyValue("id").first << vertex->getPropertyValue("firstName").first <<  endl;
    _prevPath = _pathQueue.front();
    _pathQueue.pop();
    if(_prevPath.size() > _depthSetting) {
      _VertexTargetList.push_back(_prevPath.at(_depthSetting));
      _TargetsMap.insert(TargetsPair(_prevPath.at(_depthSetting), _prevPath.at(_depthSetting-1) ));
      while( !_pathQueue.empty()) {
        auto path = _pathQueue.front(); _pathQueue.pop();
        bool unique = true;
        for (auto it = _VertexTargetList.begin() ; it != _VertexTargetList.end(); ++ it) {/// do not store repeated veretx
          if ( *it == path.at(_depthSetting)) {
            unique = false;
            break;
          }
        }
        if(unique == true) { 
          _VertexTargetList.push_back(path.at(_depthSetting));
          _TargetsMap.insert(TargetsPair(path.at(_depthSetting), path.at(_depthSetting-1)));
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
    if(_prevPath.back() == first) {
      depthSecond = _prevPath.size();
      _CurrentDepth = depthSecond;
    }

    Filter filter;
    if(depthSecond > 0) {
      filter = _filterList[depthSecond-1];
    }
    else 
      filter.setDefault();
    
    for(auto it = filter.getTypeList().begin() ; it != filter.getTypeList().end(); it++) {
      cout << "Filt Type " << (*it) << "\t" ;
    }
    cout << endl;
    _typeMatch = checkMultiRelType(edge, filter);
    if(_typeMatch == true) {
      if(_CurrentDepth == _DepthToCheckRange) {
        auto equal = false;
        switch(_PropToCheck) {
          case 1:
            _typeMatch = checkTimeRange<ReturnValueType, VertexPointer>(second, _RangeFilter, equal);
            break;
          case 2:
            _typeMatch = checkDateRange<ReturnValueType, VertexPointer>(second, _RangeFilter, equal);
            break;
          case 0:
            break;
          default:
            break;
        }
      }
    }
    cout << "scheduleBranch typeMatch : " << _typeMatch << endl;

    return false;
  }

  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if (_typeMatch) {
      VertexPath newPath = _prevPath;
      newPath.push_back(second);
      _pathQueue.push(newPath);
      cout << "first " << first->getPropertyValue("id").first << "\t" << " second " << second->getPropertyValue("id").first << " is on path now\n";
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
  typedef map<VertexPointer, unsigned int> ReturnResultMapType;
  typedef map<VertexPointer, unsigned int> &  ResultMapTypeReference;
  typedef pair<VertexPointer, unsigned int> ReturnMapPair;
public:

  virtual ResultMapTypeReference getReturnResultMap() {
    return _ReturnResultMap;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    cout << "==>vid " << vertex->getId() << "\t" <<  vertex->getPropertyValue("id").first << endl;
    _prevPath = _pathQueue.front();
    _pathQueue.pop();
    if(_prevPath.size() > _depthSetting) {
      _TargetsMap.insert(TargetsPair(_prevPath.at(_depthSetting), _prevPath.at(_depthSetting) ));
      _ReturnResultMap.insert(ReturnMapPair(_prevPath.at(_depthSetting), 1));
      while( !_pathQueue.empty()) {
        auto path = _pathQueue.front(); _pathQueue.pop();
        if(_ReturnResultMap.find(path.at(_depthSetting)) == _ReturnResultMap.end()) {
          _ReturnResultMap.insert(ReturnMapPair(path.at(_depthSetting), 1));
        }//end if
        else {
          auto counter = _ReturnResultMap[path.at(_depthSetting)] + 1;
          _ReturnResultMap.insert(ReturnMapPair(path.at(_depthSetting), counter));  
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
  typedef map<EdgePointer, VertexPointer> BranchMapType;
  typedef pair<EdgePointer, VertexPointer> BranchPair;
  typedef pair<VertexPointer, BranchPair> MapPair;
  typedef multimap<VertexPointer, BranchPair> PathMapType;

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
    if(_prevPath.back() == first) {
      depthSecond = _prevPath.size();
      _CurrentDepth = depthSecond;
    }

    Filter filter;
    if(depthSecond > 0) {
      filter = _filterList[depthSecond-1];
    }
    else 
      filter.setDefault();
    
    for(auto it = filter.getTypeList().begin() ; it != filter.getTypeList().end(); it++) {
//      cout << "Filt Type " << (*it) << "\t" ;
    }
//    cout << "edge label " << edge->getType() << endl;
    auto MultiTypeMatch = checkMultiRelType(edge, filter);
    auto RangeMatch = true;
    auto VertexMatch = true;
    auto equalFlag = false;
    if((_CurrentDepth == _DepthToCheckRange) && MultiTypeMatch) 
      RangeMatch = checkYearRange<EdgePointer>(edge, _RangeFilter, equalFlag);
    if((_CurrentDepth == _DepthToCheckVertexProp ) && MultiTypeMatch) {
      VertexMatch = checkProperty<ReturnValueType>(second, _VertexFilter); 
//      cout << "vertex place.id " << second->getPropertyValue("name").first << endl;
    }
//    cout << "scheduleBranch:  MultiTypeMatch  " << MultiTypeMatch << " RangeMatch " << RangeMatch << " VertexMatch " << VertexMatch  << endl;
    _typeMatch = (MultiTypeMatch && RangeMatch && VertexMatch);
    
    if(_typeMatch ) {
      if(_CurrentDepth != _depthSetting) {
        auto branchPair = make_pair(edge, second);
        _PathMap.insert(MapPair(first, branchPair));
      } 
    }
    if((_CurrentDepth == _depthSetting) && (VertexMatch == false)){
      for (auto it = _PathMap.begin(); it != _PathMap.end(); it++) {
//        cout << "++VertexMatch " << VertexMatch <<"\t"<< (*it).second.second->getId() <<"\t"<< first->getId() << endl;
        if ((*it).second.second  == first) {
          _PathMap.erase(it);
//          cout << "vertex " << first->getPropertyValue("id").first << " is rm\n";
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
    if(_prevPath.back() == first) {
      depthSecond = _prevPath.size();
      _CurrentDepth = depthSecond;
    }

    Filter filter;
    if(depthSecond > 0) {
      filter = _filterList[depthSecond-1];
    }
    else 
      filter.setDefault();
    
    for(auto it = filter.getTypeList().begin() ; it != filter.getTypeList().end(); it++) {
      cout << "Filt Type " << (*it) << "\t" ;
    }
    cout << "edge label " << edge->getType() << endl;
    auto MultiTypeMatch = checkMultiRelType(edge, filter);
    bool RangeMatch = true;
    bool VertexMatch = true;
    cout << "++Current Depth " << _CurrentDepth << endl;
    if((_CurrentDepth == _DepthToCheckRange) && MultiTypeMatch)  {
      bool equalFlag;
      RangeMatch = checkRange<EdgePointer>(_PropToCheck, edge, _RangeFilter, equalFlag); 
    }
    if((_CurrentDepth == _DepthToCheckVertexProp ) && MultiTypeMatch) {
      cout << "vertex place.name " << second->getPropertyValue("name").first << endl;
      VertexMatch = false;
      for(auto it = _VertexFilterList.begin(); it != _VertexFilterList.end(); it++) {
        cout << "vertexfilter list " << (*it).getValue() << endl;
        if(checkProperty<ReturnValueType>(second, (*it)) == true) {
          VertexMatch = true;
          break;
        }
      }
    }
    cout << "scheduleBranch:  MultiTypeMatch  " << MultiTypeMatch << " RangeMatch " << RangeMatch << " VertexMatch " << VertexMatch  << endl;

    _typeMatch = (MultiTypeMatch && RangeMatch && VertexMatch);
    
    if((_CurrentDepth == _depthSetting)){
      if(MultiTypeMatch && RangeMatch )
        if(VertexMatch == true) {
          _IncludeState= false; 
          cout <<"-->Include state " << _IncludeState << endl;
        }
    } 
    return false;
  }

protected:
  bool _IncludeState= true;
  vector<Filter> _VertexFilterList;
};

class MultiPropertyVisitor: public VertexPropertyRangeVisitor {
public:
  typedef vector<Filter> FilterListType;
  typedef pair<VertexPointer, unsigned int> ResultPair;
  typedef map<VertexPointer, unsigned int> ResultMapType;
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
    if(_prevPath.back() == first) {
      depthSecond = _prevPath.size();
      _CurrentDepth = depthSecond;
    }

    Filter filter;
    if(depthSecond > 0) {
      filter = _filterList[depthSecond-1];
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
    _typeMatch = (MultiTypeMatch && RangeMatch && VertexMatch);
    return false;
  }

  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if (_typeMatch) {
      VertexPath newPath = _prevPath;
      newPath.push_back(second);
      _pathQueue.push(newPath);
      if(_CurrentDepth == 1) {
        _ResultMap.insert(ResultPair(second, 0 ));
      }
      if (_CurrentDepth == _depthSetting) {
        if(_ResultMap.find(newPath[1]) != _ResultMap.end()) {
          cout << _ResultMap[newPath[1]] << " posts are collected now\n";
          _ResultMap[newPath[1]]++;
        } else {
          cout << "Failed to find "<< newPath[1]->getPropertyValue("id").first << endl;
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
  typedef map<VertexPointer, VertexPointer> VertexMapType;
  typedef pair<VertexPointer, VertexPointer> VertexPair;
public:
  virtual void setDepthToCompareTime(unsigned int depth) {
    _DepthToCompareTime = depth;
  }

  virtual VertexMapType & getVertexMap() {
    return _ReturnMap;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    cout << "==>visit vertex " << vertex->getId() << "\t" << vertex->getPropertyValue("id").first << endl;
    _prevPath  = _pathQueue.front();
    _pathQueue.pop();
    _RangeFilter.setValueRange("creationDate", "", "");
    if (_prevPath.size() > _depthSetting)
      return true;
    else 
      return false; 
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int depthSecond;
    if(_prevPath.back() == first) {
      depthSecond = _prevPath.size();
      _CurrentDepth = depthSecond;
    }
    Filter filter;
    if(depthSecond > 0) 
      filter = _filterList[depthSecond-1];
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
          if(_MatchMap.find(_prevPath[1]) == _MatchMap.end()) {
          _MatchMap.insert(pair<VertexPointer, VertexPointer>(_prevPath[1], second));
          } else {
          _MatchMap[_prevPath[1]] = second;
         }
       }
    }
    if((_CurrentDepth == _DepthToCompareTime + 1) && MultiTypeMatch) {
      map<VertexPointer, VertexPointer>::iterator it;
      for(it = _MatchMap.begin(); it != _MatchMap.end(); it++) {
        if ((*it).second == first)
          break;
      }
      if(it == _MatchMap.end()) {
        VertexMatch = false;
      }
    }
    _typeMatch = (MultiTypeMatch && RangeMatch && VertexMatch);
    return false;
  }

  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if (_typeMatch) {
      VertexPath newPath = _prevPath;
      newPath.push_back(second);
      _pathQueue.push(newPath);
      if (_CurrentDepth == _depthSetting) {
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
  typedef map<VertexPointer, bool> PersonMatchMapType;
  typedef pair<VertexPointer, bool> PersonMatchPair;
  typedef map<VertexPointer, VertexPointer> ReturnMapType; 
  typedef map<VertexPointer, string> TimeMatchMapType;
  typedef pair<VertexPointer, string> TimeMatchPair;
public:
  virtual TimeMatchMapType & getTimeMap() {
    return _TimeMatchMap;
  }
  virtual PersonMatchMapType & getPersonMap() {
    return _PersonMatchMap;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    cout << "==>visit vertex " << vertex->getId() << "\t" << vertex->getPropertyValue("id").first << endl;
    _prevPath  = _pathQueue.front();
    _pathQueue.pop();
    _PersonMatchMap.insert(PersonMatchPair(vertex, false));
    return  ((_prevPath.size() > _depthSetting) ? true : false);
  }


  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int depthSecond;
    if(_prevPath.back() == first) {
      depthSecond = _prevPath.size();
      _CurrentDepth = depthSecond;
    }

    Filter filter;
    if(depthSecond > 0) {
      filter = _filterList[depthSecond-1];
    }
    else 
      filter.setDefault();
    
    _typeMatch = checkMultiRelType(edge, filter);
    auto VertexMatch = false;
    if((_CurrentDepth == _DepthToCheckVertexProp ) && _typeMatch) {
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
     if((_CurrentDepth == _DepthToCompareTime) && _typeMatch)  {
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
    if(_typeMatch ) {
      VertexPath newPath = _prevPath;
      newPath.push_back(second);
      _pathQueue.push(newPath);
    }
    return false;
  }

protected:
  PersonMatchMapType _PersonMatchMap;
  TimeMatchMapType  _TimeMatchMap;
};

class SinglePropertyVisitor : public VertexMatchVisitor {
public:
  typedef pair<VertexPointer, bool> VertexPair;
  typedef vector<string> VertexListType;
  typedef map<VertexPointer, VertexListType> TargetsMapType;
public:
  virtual TargetsMapType & getResultTargetsMap() {
    return _ResultMap;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    cout << "==>visit vertex " << vertex->getId() << "\t" << vertex->getPropertyValue("id").first << endl;
    _prevPath  = _pathQueue.front(); _pathQueue.pop();
    VertexListType VertexList;
    _PersonMatchMap.insert(pair<VertexPointer, bool>(vertex, false));
    _ResultMap.insert(pair<VertexPointer, VertexListType>(vertex, VertexList));
    return  ((_prevPath.size() > _depthSetting) ? true : false);
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int depthSecond;
    if(_prevPath.back() == first) {
      depthSecond = _prevPath.size();
      _CurrentDepth = depthSecond;
    }

    Filter filter;
    if(depthSecond > 0) {
      filter = _filterList[depthSecond-1];
    }
    else 
      filter.setDefault();
    
    _typeMatch = checkMultiRelType(edge, filter);
    if((_CurrentDepth == _DepthToCheckVertexProp ) && _typeMatch) {
      cout << "++Tag id " << second->getPropertyValue("id").first << endl; 
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
//// till here!
// TODO unfinished
/*
class SimilarityVisitor : public VertexMatchVisitor {
public:
  typedef pair<VertexPointer, bool> VertexPair;
  typedef vector<string> VertexListType;
  typedef map<VertexPointer, int> ReturnTargetMapType;
public:
  virtual ReturnTargetMapType & getResultTargetsMap() {
    return _TargetMap;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    cout << "==>visit vertex " << vertex->getId() << "\t" << vertex->getPropertyValue("id").first << endl;
    _prevPath  = _pathQueue.front(); _pathQueue.pop();
    _CurrentDepth = _prevPath.size();
    if(_CurrentDepth == 3) {
      _TargetMap.insert(pair<VertexPointer, int>(vertex, 0));
      cout << vertex->getPropertyValue("firstName").first << " is recoreded\n";
    }
    if(_CurrentDepth == 4) {
      _TagFlag = false;
    }
    return  ((_prevPath.size() > _depthSetting) ? true : false);
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int depthSecond;
    if(_prevPath.back() == first) {
      depthSecond = _prevPath.size();
    }
    Filter filter;
    if(depthSecond > 0) {
      filter = _filterList[depthSecond-1];
    }
    else 
      filter.setDefault();
    
    _typeMatch = checkMultiRelType(edge, filter);
    if(_CurrentDepth == 3) {
      _TargetMap[first]++;
      cout << second->getType() << "\t" << second->getPropertyValue("id").first << endl;
    }
    if((_CurrentDepth == 5) && _typeMatch) {
      cout << "person id " << second->getPropertyValue("id").first << endl; 
      if(checkProperty<ReturnValueType>(second, _VertexFilter ) == true) {
      }
    }
    return false;
  }

protected:
  TargetsMapType _ResultMap;
  ReturnTargetMapType _TargetMap;
  MemoryMapType  _MemoryMap;
};

*/
#endif /*_LDBCCUSTOMVISITOR_H_*/
