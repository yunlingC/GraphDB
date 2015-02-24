//===-- traversals/CustomVisitor.h - Customed visitor class ----------*- C++ -*-===//
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

#include "Visitor.h"
#include "Utilities.h"

using namespace std;

/// visitor to deal with query of selection : query1 and query 4
class Q1Visitor : public Visitor {
public:
  typedef pair<FixedString, bool> ReturnValueType;
  typedef GraphType::VertexPointer VertexPointer;
  typedef std::vector<VertexPointer> VertexTarget;
public:
  Q1Visitor() { }

  virtual void setFilter(Filter & f) {
    _filter = f;
  }

  Filter & getFilter() { 
    return _filter;
  }

  VertexTarget & getVertexTargetList() {
    return _VertexTargetList;
  }

  virtual bool visitVertex(VertexPointer vp) {
    bool VertexMatch = checkProperty<ReturnValueType>(vp, getFilter());
    if(VertexMatch == true) 
      _VertexTargetList.push_back(vp);
//    cout <<"VertexMatch = " << VertexMatch << endl;
    return TerminateAtVertex(1, _VertexTargetList);
  }

  virtual bool  scheduleEdge(EdgePointer ep ) {
    return checkType(ep, getFilter() );
  }


  virtual void dumpTarget() {
    dumpVertexTarget(_VertexTargetList);
  }

private:

  VertexTarget _VertexTargetList;
  Filter _filter;
};

/// visitor to deal with adjacency query : from query 2 to query 3
class Q2Visitor : public Visitor {
public:
  typedef GraphType::VertexPointer VertexPointer;
  typedef std::vector<VertexPointer> VertexTarget;
public:
  Q2Visitor() { }

  void setFilter(Filter & f) {
    _filter = f;
  }

  Filter & getFilter() {
    return _filter;
  }

  VertexTarget & getVertexTargetList() {
    return _VertexTargetList;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    if(_depthList.find(vertex) != _depthList.end())
      if(_depthList[vertex] >= 1)
        return true;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    return checkType(edge, _filter);
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    bool TypeMatch = checkType(edge, _filter);
    if(TypeMatch == true) {
      _VertexTargetList.push_back(second);
    }
    computeDepth(first, edge, second, _depthList);
    return TerminateAtDepth(1, _depthList);
  }

private:
  Filter _filter;
  DepthList _depthList;
  VertexTarget _VertexTargetList;
};


/// visitor to deal with multiple typefilters: from query 5 to 7 
class Q5Visitor : public Visitor {
public:
  typedef GraphType::VertexPointer VertexPointer;
  typedef std::vector<VertexPointer> VertexTarget;
  typedef std::vector<VertexPointer> VertexPath;
  typedef std::queue<VertexPath>  PathQueue;
public:
  Q5Visitor() { }

  void setFilter(Filter & f) {
    _filterList.push_back(f);
  }

  void setDepth(unsigned int depth) {
    _depthSetting = depth;
  }

  VertexTarget & getVertexTargetList() {
    return _vertexTargetList;
  }

  virtual bool visitStartVertex(VertexPointer startVertex ) {
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
      _vertexTargetList.push_back(_prevPath.at(_depthSetting));
      while( !_pathQueue.empty()) {
        auto path = _pathQueue.front(); _pathQueue.pop();
        bool unique = true;
        for (auto it = _vertexTargetList.begin() ; it != _vertexTargetList.end(); ++ it) {/// do not store repeated veretx
          if ( *it == path.at(_depthSetting)) {
            unique = false;
            break;
          }
        }
        if(unique == true) 
        _vertexTargetList.push_back(path.at(_depthSetting));
      }
      return true;
    }
  }

  virtual bool visitDirection(VertexPointer target, EdgePointer edge) {
    return _direcMatch;

  }

  virtual bool scheduleEdge(EdgePointer edge) {
    return _typeMatch;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
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
    _direcMatch = checkDirection(second, edge, filter);
    if(_typeMatch == true)
    if(_typeMatch && _direcMatch) {
      VertexPath newPath = _prevPath;
      newPath.push_back(second);
      _pathQueue.push(newPath);
    }
    return false;
  }

private:
  unsigned int  _typeMatch;
  unsigned int  _direcMatch;
  unsigned int  _depthSetting;
  Filter _directionFilter;
  std::vector<Filter> _filterList;
  VertexTarget _vertexTargetList;
  VertexPath   _prevPath;
  PathQueue  _pathQueue;
};


class Q8Visitor : public Visitor {
public:
  typedef GraphType::VertexPointer VertexPointer;
  typedef GraphType::VertexDescriptor VertexDescriptor;
  typedef std::vector<VertexPointer> VertexTarget;
  typedef std::vector<VertexPointer> VertexPath;
  typedef std::queue<VertexPath>  PathQueue;
public:
  Q8Visitor() { }

  void setEndVertex(VertexDescriptor vertex) {
    _endVertex =  vertex;
  }

  VertexTarget & getVertexTargetList() {
    return _vertexTargetList;
  }

  virtual bool visitStartVertex(VertexPointer startVertex ) {
    _startVertex = startVertex->getId();
    VertexPath newPath;
    newPath.push_back(startVertex);
    _pathQueue.push(newPath);
  }

  virtual bool visitVertex(VertexPointer vertex) {
    cout << "visit " << vertex->getId() << endl;
    _prevPath = _pathQueue.front();
    _pathQueue.pop();
    return false;
  }

  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
      VertexPath newPath = _prevPath;
      newPath.push_back(second);
      _pathQueue.push(newPath);
      cout << second->getId() << " is on path now\n";
      if(second->getId() == _endVertex) {
        _vertexTargetList = newPath;
        return true;
      }
    return false;
  }

  virtual bool finishVisit() {
    cout << "There is no path from " << _startVertex << " to " << _endVertex << endl;
  }

  
privateBranch:
  VertexDescriptor _endVertex;
  VertexDescriptor _startVertex;
  VertexTarget _vertexTargetList;
  VertexPath   _prevPath;
  PathQueue  _pathQueue;
};




class Q10Visitor: public Visitor {
public:
  typedef GraphType::VertexPointer VertexPointer;
  typedef GraphType::VertexDescriptor VertexDescriptor;
  typedef std::vector<VertexPointer> VertexTarget;
  typedef std::vector<VertexPointer> VertexPath;
  typedef std::queue<VertexPath>  PathQueue;
public:
  Q10Visitor() { }

  void setFilter(Filter & f) {
    _filterList.push_back(f);
  }

  void setDepth(unsigned int depth) {
    _depthSetting = depth;
  }
  
  void setEndVertex(VertexDescriptor vertex) {
    _endVertex =  vertex;
  }

  Filter & getFilter() {
    return _directionFilter;
  }

  VertexTarget & getVertexTargetList() {
    return _vertexTargetList;
  }

  virtual bool visitStartVertex(VertexPointer startVertex ) {
    VertexPath newPath;
    newPath.push_back(startVertex);
    _pathQueue.push(newPath);
  }

  virtual bool visitVertex(VertexPointer vertex) {
//    cout << "visitVertex: " << vertex->getId() << endl;
    _prevPath = _pathQueue.front();
    _pathQueue.pop();
    if(_prevPath.size() > _depthSetting) {
//      cout << "vertex " << _prevPath.at(_depthSetting)->getId() << endl;
      if(_prevPath.at(_depthSetting)->getId() == _endVertex) 
        _vertexTargetList.push_back(_prevPath.at(1));
      while( !_pathQueue.empty()) {
        auto path = _pathQueue.front(); _pathQueue.pop();
        if( path.at(_depthSetting)->getId() == _endVertex) {
          bool unique = true;
          for (auto it = _vertexTargetList.begin() ; it != _vertexTargetList.end(); ++ it) {/// do not store repeated veretx
            if ( *it == path.at(1)) {
              unique = false;
              break;
            }
          }
          if(unique == true) 
          _vertexTargetList.push_back(path.at(1));
//        cout << "vertex " << path.at(_depthSetting)->getId() << " is popped out\n";
        }
      }
      return true;
    }
  }

  virtual bool visitDirection(VertexPointer target, EdgePointer edge) {
//    _direcMatch = checkDirection(target, edge, _directionFilter);
//    return _direcMatch ;
    return _direcMatch;

  }

  virtual bool scheduleEdge(EdgePointer edge) {
    return _typeMatch;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    unsigned int depthSecond;
    if(_prevPath.back() == first) {
      depthSecond = _prevPath.size();
//      cout << "second" << second->getId() << " depth " << depthSecond << endl;
    }

    Filter filter;
//    unsigned int depthSecond = computeDepth(first, edge, second, _depthList);
///    cout << "vertex: " << second->getId() << "\tdepth " << depthSecond << endl;
 //   if( _filterList.size() >= depthSecond) {
      if(depthSecond > 0)
        filter = _filterList[depthSecond-1];
//    }
    else
      filter.setDefault();
   
    _typeMatch = checkType(edge, filter);
    _direcMatch = checkDirection(second, edge, filter);
//    cout << "second " << second->getId() << " typematch " << _typeMatch << " _direcMatch " << _direcMatch << endl;
    if(_typeMatch && _direcMatch )
//      cout << "=============================>edge: " << edge->getId() << " type: " << filter.getType()<< endl;
    if(_typeMatch && _direcMatch) {
      VertexPath newPath = _prevPath;
      newPath.push_back(second);
      _pathQueue.push(newPath);
//      cout << "second " << second->getId() << " in path now\n";
    }
    return false;
  }

private:
  unsigned int  _typeMatch;
  unsigned int  _direcMatch;
  unsigned int  _depthSetting;
  Filter _directionFilter;
  VertexDescriptor _endVertex;
  std::vector<Filter> _filterList;
  VertexTarget _vertexTargetList;
  VertexPath   _prevPath;
  PathQueue  _pathQueue;
};



#endif /**_CUSOTMVISITOR_H_*/
