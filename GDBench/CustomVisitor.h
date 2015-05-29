//===-- traversals/CustomVisitor.h - Customed visitor class -----*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
/// / \file / \brief This is the customed class for Graph visitors.  /
//===----------------------------------------------------------------------===//

#ifndef _CUSTOMVISITOR_H_
#define _CUSTOMVISITOR_H_

#include <queue>
#include <set>

#include "Visitor.h"
#include "Utils.h"

using namespace std;

/// visitor to deal with query of selection : query1 and query 4
class SelectionVisitor: public Visitor {
public:
  typedef pair<FixedString, bool> ReturnValueType;
public:
  SelectionVisitor() { }

  virtual bool visitVertex(VertexPointer Vertex) {
    bool VertexMatch = checkProperty<ReturnValueType>(Vertex, getFilter());
    if(VertexMatch == true) 
      _VertexTargetList.push_back(Vertex);
    return terminateAtVertex(1, _VertexTargetList);
  }

  virtual bool  scheduleEdge(EdgePointer Edge ) {
    return checkType(Edge, getFilter() );
  }

  virtual void dumpTarget() {
    dumpVertexTarget(_VertexTargetList);
  }
};

/// visitor to deal with adjacency query : from query 2 to query 3
class AdjacencyVisitor: public Visitor {
public:
  AdjacencyVisitor() { }

  virtual bool visitVertex(VertexPointer Vertex) {
    if(_DepthList.find(Vertex) != _DepthList.end())
      if(_DepthList[Vertex] >= 1)
        return true;
      else 
        return false;
    else 
      return false;
  }

  virtual bool scheduleEdge(EdgePointer Edge) {
    return checkType(Edge, _Filter);
  }

  virtual bool visitDirection(VertexPointer Second, EdgePointer Edge) {
    return _DirecMatch;
  }

  virtual bool scheduleBranch(VertexPointer First, EdgePointer Edge, VertexPointer Second) {
    bool TypeMatch = checkType(Edge, _Filter);
    _DirecMatch = checkDirection(Second, Edge, _Filter);
    if(_DirecMatch && TypeMatch ) {
      _VertexTargetList.push_back(Second);
    }
    computeDepth(First, Edge, Second, _DepthList);
    return false;
  }

protected:
  DepthList _DepthList;
  bool _DirecMatch;
};


/// visitor to deal with multiple typefilters: from query 5 to 7 
class ReachabilityVisitor : public Visitor {
public:
  typedef std::queue<VertexPath>  PathQueue;
  typedef std::set<VertexPointer> VertexSetType;
public:
  ReachabilityVisitor () { }

  virtual void setFilter(Filter & f) {
    _FilterList.push_back(f);
  }

  virtual void setDepth(unsigned int depth) {
    _DepthSetting = depth;
  }

  virtual VertexSetType & getTargetSet() {
    return _TargetSet;
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    VertexPath NewPath;
    NewPath.push_back(startVertex);
    _PathQueue.push(NewPath);
  }

  virtual bool discoverVertex(VertexPointer vertex) {
    return true;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    _PrevPath = _PathQueue.front();
    _PathQueue.pop();
    if(_PrevPath.size() > _DepthSetting) {
      _VertexTargetList.push_back(_PrevPath.at(_DepthSetting));
      _TargetSet.insert(_PrevPath.at(_DepthSetting));
      while( !_PathQueue.empty()) {
        auto Path = _PathQueue.front(); _PathQueue.pop();
        _TargetSet.insert(Path.at(_DepthSetting));
      }
      return true;
    }
    return false;
  }


  virtual bool visitDirection(VertexPointer Second, EdgePointer Edge) {
    return _DirecMatch;
  }

  virtual bool scheduleEdge(EdgePointer Edge) {
    return _TypeMatch;
  }

  virtual bool scheduleBranch(VertexPointer First, EdgePointer Edge, VertexPointer Second) {
    unsigned int depthSecond = 0;
    if(_PrevPath.back() == First) {
      depthSecond = _PrevPath.size();
    }

    Filter filter;
    if(depthSecond > 0)
      filter = _FilterList[depthSecond-1];
    else
      filter.setDefault();
   
    _TypeMatch = checkType(Edge, filter);
    _DirecMatch = checkDirection(Second, Edge, filter);
    if(_TypeMatch && _DirecMatch) {
      VertexPath NewPath = _PrevPath;
      NewPath.push_back(Second);
      _PathQueue.push(NewPath);
    }
    return false;
  }

protected:
  unsigned int  _TypeMatch;
  unsigned int  _DirecMatch;
  unsigned int  _DepthSetting;
  std::vector<Filter> _FilterList;
  VertexPath   _PrevPath;
  PathQueue  _PathQueue;
  VertexSetType _TargetSet;
};


class PathVisitor : public Visitor {
public:
  typedef std::queue<VertexPath>  PathQueue;
public:
  PathVisitor () { }

  void setEndVertex(VertexDescriptor vertex) {
    _EndVertex =  vertex;
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    _StartVertex = startVertex->getId();
    VertexPath NewPath;
    NewPath.push_back(startVertex);
    _PathQueue.push(NewPath);
  }

  virtual bool visitVertex(VertexPointer vertex) {
    _PrevPath = _PathQueue.front();
    _PathQueue.pop();
    return false;
  }

  virtual bool scheduleTree(VertexPointer First, EdgePointer Edge, VertexPointer Second) {
      VertexPath NewPath = _PrevPath;
      NewPath.push_back(Second);
      _PathQueue.push(NewPath);
      if(Second->getId() == _EndVertex) {
        _VertexTargetList = NewPath;
        return true;
      }
    return false;
  }
  
protected:
  VertexDescriptor _EndVertex;
  VertexDescriptor _StartVertex;
  VertexPath   _PrevPath;
  PathQueue  _PathQueue;
};

class PatternVisitor : public Visitor {
public:
  typedef std::queue<VertexPath>  PathQueue;
public:
  PatternVisitor () { }

  void setFilter(Filter & f) {
    _FilterList.push_back(f);
  }

  void setDepth(unsigned int depth) {
    _DepthSetting = depth;
  }
  
  void setEndVertex(VertexDescriptor vertex) {
    _EndVertex =  vertex;
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    VertexPath NewPath;
    NewPath.push_back(startVertex);
    _PathQueue.push(NewPath);
  }

  virtual bool visitVertex(VertexPointer vertex) {
    _PrevPath = _PathQueue.front();
    _PathQueue.pop();
    if(_PrevPath.size() > _DepthSetting) {
      if(_PrevPath.at(_DepthSetting)->getId() == _EndVertex) 
        _VertexTargetList.push_back(_PrevPath.at(1));
      while( !_PathQueue.empty()) {
        auto path = _PathQueue.front(); _PathQueue.pop();
        if( path.at(_DepthSetting)->getId() == _EndVertex) {
          bool unique = true;
          for (auto it = _VertexTargetList.begin() ; it != _VertexTargetList.end(); ++ it) {/// do not store repeated veretx
            if ( *it == path.at(1)) {
              unique = false;
              break;
            }
          }
          if(unique == true) 
          _VertexTargetList.push_back(path.at(1));
        }
      }
      return true;
    }
    return false;
  }

  virtual bool visitDirection(VertexPointer target, EdgePointer Edge) {
    return _DirecMatch;

  }

  virtual bool scheduleEdge(EdgePointer Edge) {
    return _TypeMatch;
  }

  virtual bool discoverVertex(VertexPointer vertex) {
    return true; 
  }

  virtual bool scheduleBranch(VertexPointer First, EdgePointer Edge, VertexPointer Second) {
    unsigned int depthSecond = 0;
    if(_PrevPath.back() == First) {
      depthSecond = _PrevPath.size();
    }

    Filter filter;
      if(depthSecond > 0)
        filter = _FilterList[depthSecond-1];
    else
      filter.setDefault();
   
    _TypeMatch = checkType(Edge, filter);
    _DirecMatch = checkDirection(Second, Edge, filter);
    return false;
  }

  virtual bool scheduleTree(VertexPointer First, EdgePointer Edge, VertexPointer Second) {
    if(_TypeMatch && _DirecMatch) {
      VertexPath NewPath = _PrevPath;
      NewPath.push_back(Second);
      _PathQueue.push(NewPath);
    }
    return false;
  }


protected:
  bool _TypeMatch;
  bool _DirecMatch;
  unsigned int  _DepthSetting;
  VertexDescriptor _EndVertex;
  std::vector<Filter> _FilterList;
  VertexPath   _PrevPath;
  PathQueue  _PathQueue;
};


class DFSReachabilityVisitor: public ReachabilityVisitor {
public:
  typedef std::map<VertexPointer,  unsigned int> VertexTarget;
  typedef std::pair<VertexPointer, unsigned int> VertexTargetPair;
public:

  virtual void setFilter(Filter & f) {
    _FilterList.push_back(f);
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    _StartVertex = startVertex;
    _DepthList.insert(VertexTargetPair(startVertex, 0));
  }


  VertexTarget & getVertexTargetMap() {
    return _VertexTargetList;
  }

  virtual void setDepth(unsigned int depth) {
    _DepthSetting = depth;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    if( checkMaxDepth(_DepthList) >= _DepthSetting) {
     for( auto it = _DepthList.begin(); it != _DepthList.end(); ++it ) {
       if( (*it).second == _DepthSetting) {
         _VertexTargetList.insert(VertexTargetPair((*it).first, true));
       }
     }
    }
    return false;
  }

  virtual bool scheduleBranch(VertexPointer First, EdgePointer Edge, VertexPointer Second) {
    _TypeMatch = false;
    _DirecMatch = false;
    for(auto it = _DepthList.equal_range(First).first; it != _DepthList.equal_range(First).second; ++it) {
      if((*it).first == First) {
        if((*it).second < _DepthSetting) {
          Filter filter = _FilterList[(*it).second];
          if(_DepthList.count(Second) <= _DepthSetting) {
          if(checkType(Edge, filter) == true)
            _TypeMatch = true;
          if(checkDirection(Second, Edge, filter) == true)
            _DirecMatch = true;
          if(_TypeMatch && _DirecMatch)
            recordDepth(First, Edge, Second, _DepthList);
          } //end if 
        }
      }
    }
   return false;
  }

  virtual bool visitDirection(VertexPointer target, EdgePointer Edge) {
    return _DirecMatch;
  }

  virtual bool scheduleEdge(EdgePointer Edge) {
    return _TypeMatch;
  }

protected:
  Filter _Filter;
  MultiDepthList   _DepthList;
  VertexPointer    _StartVertex;
  bool _TypeMatch;
  bool _DirecMatch;
  unsigned int     _DepthSetting;
  VertexTarget     _VertexTargetList;
  std::vector<Filter> _FilterList;
};

class DFSPatternVisitor : public Visitor {
public:
  typedef std::vector<VertexPath>  PathStack;
public:

  void setFilter(Filter & f) {
    _FilterList.push_back(f);
  }

  void setDepth(unsigned int depth) {
    _DepthSetting = depth;
  }
  
  void setEndVertex(VertexDescriptor vertex) {
    _EndVertex =  vertex;
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    _StartVertex = startVertex;
    VertexPath NewPath;
    NewPath.push_back(startVertex);
    _pathStack.push_back(NewPath);
  }

  virtual bool visitVertex(VertexPointer vertex) {
    _PrevPath = _pathStack.back();
    _pathStack.pop_back();
    if(_PrevPath.size() > _DepthSetting) {
      if(_PrevPath.at(_DepthSetting)->getId() == _EndVertex) {
        bool unique = true;
        for (auto it = _VertexTargetList.begin() ; it != _VertexTargetList.end(); ++ it) {/// do not store repeated veretx
          if ( *it == _PrevPath.at(1)) {
            unique = false;
            break;
          }
        }
        if(unique == true) {
          _VertexTargetList.push_back(_PrevPath.at(1));
        }
      }
    }
    return false;
  }

  virtual bool discoverVertex(VertexPointer vertex) {
    return true; 
  }

  virtual bool visitDirection(VertexPointer target, EdgePointer Edge) {
    return _DirecMatch;
  }

  virtual bool scheduleEdge(EdgePointer Edge) {
    return _TypeMatch;
  }

  virtual bool scheduleBranch(VertexPointer First, EdgePointer Edge, VertexPointer Second) {
    _TypeMatch = false;
    _DirecMatch = false;
    if(_PrevPath.size() > _DepthSetting) {
      return true;
    }

    unsigned int firstDepth = 1000; //default: get default filter later
    if(_PrevPath.back() == First) {
      firstDepth = _PrevPath.size() - 1;
    }

    Filter filter;
      if(firstDepth >= 0 && firstDepth < _DepthSetting)
        filter = _FilterList[firstDepth];
    else
      filter.setDefault();

   if(Second != _StartVertex) {
      _TypeMatch = checkType(Edge, filter);
      _DirecMatch = checkDirection(Second, Edge, filter);
   }
    return false;
  }

  virtual bool scheduleTree(VertexPointer First, EdgePointer Edge, VertexPointer Second) {
    if(_TypeMatch && _DirecMatch) {
      VertexPath NewPath = _PrevPath;
      NewPath.push_back(Second);
      _pathStack.push_back(NewPath);
    }
    return false;
  }

protected:
  bool _TypeMatch;
  bool _DirecMatch;
  unsigned int  _DepthSetting;
  VertexPointer _StartVertex;
  VertexDescriptor _EndVertex;
  std::vector<Filter> _FilterList;
  VertexPath   _PrevPath;
  PathStack _pathStack;
};


class DFSShortestPathVisitor : public Visitor {
public:
  typedef std::vector<VertexPath>  PathStack;
  typedef std::multimap<unsigned int, VertexPath> PathMap;
  typedef std::pair<unsigned int, VertexPath> PathPair;
public:

  void setFilter(Filter & f) {
    _FilterList.push_back(f);
  }

  void setDepth(unsigned int depth) {
    _DepthSetting = depth;
  }
  
  void setEndVertex(VertexDescriptor vertex) {
    _EndVertex =  vertex;
  }


  PathMap & getPathList() {
    return _PathMap;
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    _StartVertex = startVertex;
    _TmpMinDepth = 500;
    _TurnFlag = false;
    VertexPath NewPath;
    NewPath.push_back(startVertex);
    _pathStack.push_back(NewPath);
  }

  virtual bool visitVertex(VertexPointer vertex) {
    _PrevPath = _pathStack.back();
    _pathStack.pop_back();
    if(_PrevPath.size() >=  _TmpMinDepth) {
      _TurnFlag = true; //prune not to go on with this branch
    } else 
      _TurnFlag = false;
    return false;
  }

  virtual bool discoverVertex(VertexPointer vertex) {
    return true; 
  }

  virtual bool visitDirection(VertexPointer target, EdgePointer Edge) {
    return _DirecMatch;
  }

  virtual bool scheduleEdge(EdgePointer Edge) {
    return _TypeMatch;
  }

  virtual bool scheduleBranch(VertexPointer First, EdgePointer Edge, VertexPointer Second) {
    _TypeMatch = false;
    _DirecMatch = false;
    /// if the depth of this branch already exceeds the shortest path, then prune
    if(_TurnFlag == true) {
      return true;
    }

    VertexPath NewPath = _PrevPath;
    NewPath.push_back(Second);
    /// if find the endVertex, add to map and never visit this branch and following branches again
    if(Second->getId() == _EndVertex) {
      if (NewPath.size() <= _TmpMinDepth) {
        if(NewPath.size() < _TmpMinDepth)
          _PathMap.clear();
        _PathMap.insert(PathPair(NewPath.size(), NewPath));
        _TmpMinDepth = NewPath.size();
      }
      return true;
    } else {
      _TypeMatch = true;
      _DirecMatch = true;
      return false;
    }
  }

  virtual bool scheduleTree(VertexPointer First, EdgePointer Edge, VertexPointer Second){

    if(_TypeMatch && _DirecMatch) {
      VertexPath NewPath = _PrevPath;
      NewPath.push_back(Second);
      _pathStack.push_back(NewPath);
    }
    return false;
  }

protected:
  bool _TurnFlag;
  unsigned int  _TmpMinDepth;
  unsigned int  _TypeMatch;
  unsigned int  _DirecMatch;
  unsigned int  _DepthSetting;
  VertexPointer _StartVertex;
  VertexDescriptor _EndVertex;
  std::vector<Filter> _FilterList;
  PathMap     _PathMap;
  VertexPath   _PrevPath;
  PathStack _pathStack;
};

class DFSPathVisitor : public DFSShortestPathVisitor {
public:
  virtual bool visitVertex(VertexPointer vertex) {
    _PrevPath = _pathStack.back();
    _pathStack.pop_back();
    if(_PrevPath.size() >=  _TmpMinDepth) {
      _TurnFlag = true; //prune not to go on with this branch
    } else 
      _TurnFlag = false;
    return _TerminateFlag;
  }

  virtual bool scheduleBranch(VertexPointer First, EdgePointer Edge, VertexPointer Second) {
    _TypeMatch = false;
    _DirecMatch = false;
    /// if the depth of this branch already exceeds the shortest path, then prune
    if(_TurnFlag == true) {
      return true;
    }

    VertexPath NewPath = _PrevPath;
    NewPath.push_back(Second);
    /// if find the endVertex, add to map and never visit this branch and following branches again
    if(Second->getId() == _EndVertex) {
//      if (NewPath.size() <= _TmpMinDepth) {
//        if(NewPath.size() < _TmpMinDepth)
//          _PathMap.clear();
        _PathMap.insert(PathPair(NewPath.size(), NewPath));
        _TmpMinDepth = NewPath.size();
        _TerminateFlag = true;
//      }
      return true;
    } else {
      _TypeMatch = true;
      _DirecMatch = true;
      return false;
    }
  }

protected:
  bool _TerminateFlag = false;
};

#endif /**_CUSOTMVISITOR_H_*/
