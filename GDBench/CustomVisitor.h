//===-- traversals/CustomVisitor.h - Customed visitor class -----*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the customed class for Graph visitors.
///
//===----------------------------------------------------------------------===//

#ifndef _CUSTOMVISITOR_H_
#define _CUSTOMVISITOR_H_

#include <queue>
#include <set>

#include "Visitor.h"
#include "Utilities.h"

using namespace std;

/// just for address distribution analysis
class AddressVisitor : public Visitor {
public:
  AddressVisitor() {}

  bool visitVertex(VertexPointer Vertex) {
//    std::cout << "Visit " << Vertex->getId()
//              << "\n"; 

    int  diff = reinterpret_cast<int *>(Vertex) - reinterpret_cast<int *>(PrevVertexAddr); 
    VertexAddrDis.push_back(diff);

//    std::cout 
// << "Vertex:  Prev " << reinterpret_cast<int *>(PrevVertexAddr) << "\t"
//              << "Curr " << reinterpret_cast<int *>(Vertex) 
//              << diff << "\t";;


    PrevVertexAddr = Vertex;
    return false;
  }

  bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    int diff = reinterpret_cast<int *>(edge) - reinterpret_cast<int *>(PrevEdgeAddr);
    EdgeAddrDis.push_back(diff);

//    std::cout 
//      << "Edge:    Prev " << reinterpret_cast<int *>(PrevEdgeAddr) << "\t"
//              << "Curr " << reinterpret_cast<int *>(edge) 
//              << diff << "\t";

    PrevEdgeAddr = edge;

//    std::cout << first->getId() << " ---- " 
//              << edge->getId() << " ---- " 
//              << second->getId() << "\n";
    return false;
  }

protected:
  VertexPointer PrevVertexAddr;
  EdgePointer PrevEdgeAddr;
public:
  std::vector<int > VertexAddrDis;
  std::vector<int > EdgeAddrDis;
};

/// visitor to deal with query of selection : query1 and query 4
class SelectionVisitor: public Visitor {
public:
  typedef pair<FixedString, bool> ReturnValueType;
public:
  SelectionVisitor() { }

  virtual bool visitVertex(VertexPointer vp) {
    bool VertexMatch = checkProperty<ReturnValueType>(vp, getFilter());
    if(VertexMatch == true) 
      _VertexTargetList.push_back(vp);
    return TerminateAtVertex(1, _VertexTargetList);
  }

  virtual bool  scheduleEdge(EdgePointer ep ) {
    return checkType(ep, getFilter() );
  }


  virtual void dumpTarget() {
    dumpVertexTarget(_VertexTargetList);
  }
};

/// visitor to deal with adjacency query : from query 2 to query 3
class AdjacencyVisitor: public Visitor {
public:
  AdjacencyVisitor() { }

  virtual bool visitVertex(VertexPointer vertex) {
    if(_depthList.find(vertex) != _depthList.end())
      if(_depthList[vertex] >= 1)
        return true;
      else 
        return false;
    else 
      return false;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    return checkType(edge, _filter);
  }


  virtual bool visitDirection(VertexPointer target, EdgePointer edge) {
    return _direcMatch;

  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    bool TypeMatch = checkType(edge, _filter);
    _direcMatch = checkDirection(second, edge, _filter);
    if(_direcMatch && TypeMatch ) {
      _VertexTargetList.push_back(second);
    }
    computeDepth(first, edge, second, _depthList);
    return false;
  }

protected:
  DepthList _depthList;
  bool _direcMatch;
};


/// visitor to deal with multiple typefilters: from query 5 to 7 
class ReachabilityVisitor : public Visitor {
public:
  typedef std::queue<VertexPath>  PathQueue;
  typedef std::set<VertexPointer> VertexSetType;
public:
  ReachabilityVisitor () { }

  virtual void setFilter(Filter & f) {
    _filterList.push_back(f);
  }

  virtual void setDepth(unsigned int depth) {
    _depthSetting = depth;
  }

  virtual VertexSetType & getTargetSet() {
    return _TargetSet;
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    VertexPath newPath;
    newPath.push_back(startVertex);
    _pathQueue.push(newPath);
  }

  virtual bool discoverVertex(VertexPointer vertex) {
    return true;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    _prevPath = _pathQueue.front();
    _pathQueue.pop();
    if(_prevPath.size() > _depthSetting) {
      _VertexTargetList.push_back(_prevPath.at(_depthSetting));
      _TargetSet.insert(_prevPath.at(_depthSetting));
      while( !_pathQueue.empty()) {
        auto path = _pathQueue.front(); _pathQueue.pop();
        _TargetSet.insert(path.at(_depthSetting));
      }
      return true;
    }
    return false;
  }


  virtual bool visitDirection(VertexPointer target, EdgePointer edge) {
    return _direcMatch;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    return _typeMatch;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int depthSecond = 0;
    if(_prevPath.back() == first) {
      depthSecond = _prevPath.size();
    }

    Filter filter;
    if(depthSecond > 0)
      filter = _filterList[depthSecond-1];
    else
      filter.setDefault();
   
    _typeMatch = checkType(edge, filter);
    _direcMatch = checkDirection(second, edge, filter);
    if(_typeMatch && _direcMatch) {
      VertexPath newPath = _prevPath;
      newPath.push_back(second);
      _pathQueue.push(newPath);
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
  VertexSetType _TargetSet;
};


class PathVisitor : public Visitor {
public:
  typedef std::queue<VertexPath>  PathQueue;
public:
  PathVisitor () { }

  void setEndVertex(VertexDescriptor vertex) {
    _endVertex =  vertex;
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    _startVertex = startVertex->getId();
    VertexPath newPath;
    newPath.push_back(startVertex);
    _pathQueue.push(newPath);
  }

  virtual bool visitVertex(VertexPointer vertex) {
    _prevPath = _pathQueue.front();
    _pathQueue.pop();
    return false;
  }

  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
      VertexPath newPath = _prevPath;
      newPath.push_back(second);
      _pathQueue.push(newPath);
      if(second->getId() == _endVertex) {
        _VertexTargetList = newPath;
        return true;
      }
    return false;
  }
  
protected:
  VertexDescriptor _endVertex;
  VertexDescriptor _startVertex;
  VertexPath   _prevPath;
  PathQueue  _pathQueue;
};

class PatternVisitor : public Visitor {
public:
  typedef std::queue<VertexPath>  PathQueue;
public:
  PatternVisitor () { }

  void setFilter(Filter & f) {
    _filterList.push_back(f);
  }

  void setDepth(unsigned int depth) {
    _depthSetting = depth;
  }
  
  void setEndVertex(VertexDescriptor vertex) {
    _endVertex =  vertex;
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    VertexPath newPath;
    newPath.push_back(startVertex);
    _pathQueue.push(newPath);
  }

  virtual bool visitVertex(VertexPointer vertex) {
    _prevPath = _pathQueue.front();
    _pathQueue.pop();
    if(_prevPath.size() > _depthSetting) {
      if(_prevPath.at(_depthSetting)->getId() == _endVertex) 
        _VertexTargetList.push_back(_prevPath.at(1));
      while( !_pathQueue.empty()) {
        auto path = _pathQueue.front(); _pathQueue.pop();
        if( path.at(_depthSetting)->getId() == _endVertex) {
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

  virtual bool visitDirection(VertexPointer target, EdgePointer edge) {
    return _direcMatch;

  }

  virtual bool scheduleEdge(EdgePointer edge) {
    return _typeMatch;
  }

  virtual bool discoverVertex(VertexPointer vertex) {
    return true; 
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int depthSecond = 0;
    if(_prevPath.back() == first) {
      depthSecond = _prevPath.size();
    }

    Filter filter;
      if(depthSecond > 0)
        filter = _filterList[depthSecond-1];
    else
      filter.setDefault();
   
    _typeMatch = checkType(edge, filter);
    _direcMatch = checkDirection(second, edge, filter);
    return false;
  }

  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if(_typeMatch && _direcMatch) {
      VertexPath newPath = _prevPath;
      newPath.push_back(second);
      _pathQueue.push(newPath);
    }
    return false;
  }


protected:
  bool _typeMatch;
  bool _direcMatch;
  unsigned int  _depthSetting;
  VertexDescriptor _endVertex;
  std::vector<Filter> _filterList;
  VertexPath   _prevPath;
  PathQueue  _pathQueue;
};


class DFSReachabilityVisitor: public ReachabilityVisitor {
public:
  typedef std::map<VertexPointer,  unsigned int> VertexTarget;
  typedef std::pair<VertexPointer, unsigned int> VertexTargetPair;
public:

  virtual void setFilter(Filter & f) {
    _filterList.push_back(f);
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    _startVertex = startVertex;
    _depthList.insert(VertexTargetPair(startVertex, 0));
  }


  VertexTarget & getVertexTargetMap() {
    return _VertexTargetList;
  }

  virtual void setDepth(unsigned int depth) {
    _depthSetting = depth;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    if( checkMaxDepth(_depthList) >= _depthSetting) {
     for( auto it = _depthList.begin(); it != _depthList.end(); ++it ) {
       if( (*it).second == _depthSetting) {
         _VertexTargetList.insert(VertexTargetPair((*it).first, true));
       }
     }
    }
    return false;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    _typeMatch = false;
    _direcMatch = false;
    for(auto it = _depthList.equal_range(first).first; it != _depthList.equal_range(first).second; ++it) {
      if((*it).first == first) {
        if((*it).second < _depthSetting) {
          Filter filter = _filterList[(*it).second];
          if(_depthList.count(second) <= _depthSetting) {
          if(checkType(edge, filter) == true)
            _typeMatch = true;
          if(checkDirection(second, edge, filter) == true)
            _direcMatch = true;
          if(_typeMatch && _direcMatch)
            recordDepth(first, edge, second, _depthList);
          } //end if 
        }
      }
    }
   return false;
  }

  virtual bool visitDirection(VertexPointer target, EdgePointer edge) {
    return _direcMatch;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    return _typeMatch;
  }

protected:
  Filter _filter;
  MultiDepthList   _depthList;
  VertexPointer    _startVertex;
  bool _typeMatch;
  bool _direcMatch;
  unsigned int     _depthSetting;
  VertexTarget     _VertexTargetList;
  std::vector<Filter> _filterList;
};

class DFSPatternVisitor : public Visitor {
public:
  typedef std::vector<VertexPath>  PathStack;
public:

  void setFilter(Filter & f) {
    _filterList.push_back(f);
  }

  void setDepth(unsigned int depth) {
    _depthSetting = depth;
  }
  
  void setEndVertex(VertexDescriptor vertex) {
    _endVertex =  vertex;
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    _startVertex = startVertex;
    VertexPath newPath;
    newPath.push_back(startVertex);
    _pathStack.push_back(newPath);
  }

  virtual bool visitVertex(VertexPointer vertex) {
    _prevPath = _pathStack.back();
    _pathStack.pop_back();
    if(_prevPath.size() > _depthSetting) {
      if(_prevPath.at(_depthSetting)->getId() == _endVertex) {
        bool unique = true;
        for (auto it = _VertexTargetList.begin() ; it != _VertexTargetList.end(); ++ it) {/// do not store repeated veretx
          if ( *it == _prevPath.at(1)) {
            unique = false;
            break;
          }
        }
        if(unique == true) {
          _VertexTargetList.push_back(_prevPath.at(1));
        }
      }
    }
    return false;
  }

  virtual bool discoverVertex(VertexPointer vertex) {
    return true; 
  }

  virtual bool visitDirection(VertexPointer target, EdgePointer edge) {
    return _direcMatch;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    return _typeMatch;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    _typeMatch = false;
    _direcMatch = false;
    if(_prevPath.size() > _depthSetting) {
      return true;
    }

    unsigned int firstDepth = 1000; //default: get default filter later
    if(_prevPath.back() == first) {
      firstDepth = _prevPath.size() - 1;
    }

    Filter filter;
      if(firstDepth >= 0 && firstDepth < _depthSetting)
        filter = _filterList[firstDepth];
    else
      filter.setDefault();

   if(second != _startVertex) {
      _typeMatch = checkType(edge, filter);
      _direcMatch = checkDirection(second, edge, filter);
   }
    return false;
  }

  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if(_typeMatch && _direcMatch) {
      VertexPath newPath = _prevPath;
      newPath.push_back(second);
      _pathStack.push_back(newPath);
    }
    return false;
  }

protected:
  bool _typeMatch;
  bool _direcMatch;
  unsigned int  _depthSetting;
  VertexPointer _startVertex;
  VertexDescriptor _endVertex;
  std::vector<Filter> _filterList;
  VertexPath   _prevPath;
  PathStack _pathStack;
};


class DFSShortestPathVisitor : public Visitor {
public:
  typedef std::vector<VertexPath>  PathStack;
  typedef std::multimap<unsigned int, VertexPath> PathMap;
  typedef std::pair<unsigned int, VertexPath> PathPair;
public:

  void setFilter(Filter & f) {
    _filterList.push_back(f);
  }

  void setDepth(unsigned int depth) {
    _depthSetting = depth;
  }
  
  void setEndVertex(VertexDescriptor vertex) {
    _endVertex =  vertex;
  }


  PathMap & getPathList() {
    return _pathMap;
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    _startVertex = startVertex;
    _tmpMinDepth = 500;
    _turnFlag = false;
    VertexPath newPath;
    newPath.push_back(startVertex);
    _pathStack.push_back(newPath);
  }

  virtual bool visitVertex(VertexPointer vertex) {
    _prevPath = _pathStack.back();
    _pathStack.pop_back();
    if(_prevPath.size() >=  _tmpMinDepth) {
      _turnFlag = true; //prune not to go on with this branch
    } else 
      _turnFlag = false;
    return false;
  }

  virtual bool discoverVertex(VertexPointer vertex) {
    return true; 
  }

  virtual bool visitDirection(VertexPointer target, EdgePointer edge) {
    return _direcMatch;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    return _typeMatch;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    _typeMatch = false;
    _direcMatch = false;
    /// if the depth of this branch already exceeds the shortest path, then prune
    if(_turnFlag == true) {
      return true;
    }

    VertexPath newPath = _prevPath;
    newPath.push_back(second);
    /// if find the endVertex, add to map and never visit this branch and following branches again
    if(second->getId() == _endVertex) {
      if (newPath.size() <= _tmpMinDepth) {
        if(newPath.size() < _tmpMinDepth)
          _pathMap.clear();
        _pathMap.insert(PathPair(newPath.size(), newPath));
        _tmpMinDepth = newPath.size();
      }
      return true;
    } else {
      _typeMatch = true;
      _direcMatch = true;
      return false;
    }
  }

  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second){

    if(_typeMatch && _direcMatch) {
      VertexPath newPath = _prevPath;
      newPath.push_back(second);
      _pathStack.push_back(newPath);
    }
    return false;
  }

protected:
  bool _turnFlag;
  unsigned int  _tmpMinDepth;
  unsigned int  _typeMatch;
  unsigned int  _direcMatch;
  unsigned int  _depthSetting;
  VertexPointer _startVertex;
  VertexDescriptor _endVertex;
  std::vector<Filter> _filterList;
  PathMap     _pathMap;
  VertexPath   _prevPath;
  PathStack _pathStack;
};

class DFSPathVisitor : public DFSShortestPathVisitor {
public:
  virtual bool visitVertex(VertexPointer vertex) {
    _prevPath = _pathStack.back();
    _pathStack.pop_back();
    if(_prevPath.size() >=  _tmpMinDepth) {
      _turnFlag = true; //prune not to go on with this branch
    } else 
      _turnFlag = false;
    return _TerminateFlag;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    _typeMatch = false;
    _direcMatch = false;
    /// if the depth of this branch already exceeds the shortest path, then prune
    if(_turnFlag == true) {
      return true;
    }

    VertexPath newPath = _prevPath;
    newPath.push_back(second);
    /// if find the endVertex, add to map and never visit this branch and following branches again
    if(second->getId() == _endVertex) {
//      if (newPath.size() <= _tmpMinDepth) {
//        if(newPath.size() < _tmpMinDepth)
//          _pathMap.clear();
        _pathMap.insert(PathPair(newPath.size(), newPath));
        _tmpMinDepth = newPath.size();
        _TerminateFlag = true;
//      }
      return true;
    } else {
      _typeMatch = true;
      _direcMatch = true;
      return false;
    }
  }

protected:
  bool _TerminateFlag = false;
};

#endif /**_CUSOTMVISITOR_H_*/
