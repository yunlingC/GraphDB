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

#include "Visitor.h"
//#include "Filter.h"
#include "Utilities.h"

using namespace std;

class Q1Visitor : public Visitor {
public:
  typedef pair<FixedString, bool> ReturnValueType;
  typedef GraphType::VertexPointer VertexPointer;
  typedef std::vector<VertexPointer> VertexTarget;
public:
  Q1Visitor() { }
  void setFilter(Filter & f) {
    _filter = f;
  }

  Filter & getFilter() { 
    return _filter;
  }

  VertexTarget & getVertexTargetList() {
    return _VertexTargetList;
  }

  bool visitVertex(VertexPointer vp) {
    bool VertexMatch = checkProperty<ReturnValueType>(vp, getFilter());
    if(VertexMatch == true) 
      _VertexTargetList.push_back(vp);
//    cout <<"VertexMatch = " << VertexMatch << endl;
    return TerminateAtVertex(1, _VertexTargetList);
  }

  bool  scheduleEdge(EdgePointer ep ) {
    return checkType(ep, getFilter() );
  }


  void dumpTarget() {
    dumpVertexTarget(_VertexTargetList);
  }

private:

  VertexTarget _VertexTargetList;
  Filter _filter;
};

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

  bool visitVertex(VertexPointer vertex) {
    if(_depthList.find(vertex) != _depthList.end())
      if(_depthList[vertex] >= 1)
        return true;
  }

  bool scheduleEdge(EdgePointer edge) {
    return checkType(edge, _filter);
  }

  bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    bool TypeMatch = checkType(edge, _filter);
    if(TypeMatch == true) {
//      cout << first->getId() << "\t" << edge->getId() << "\t"  << second->getId() << "\t" << edge->getType() << endl;
      _VertexTargetList.push_back(second);
    }
    computeDepth(first, edge, second, _depthList);
    return TerminateAtDepth(1, _depthList);
  }
/*
  void dumpTarget() {
    for (auto it = _VertexTargetList.begin(); it != _VertexTargetList.end(); ++it) {
      cout << "Vertex: " << (*it)->getId() << "Name: " << (*it)->getProperty("name");
    }
  }
*/

private:
  Filter _filter;
  DepthList _depthList;
  VertexTarget _VertexTargetList;
};
#endif /**_CUSOTMVISITOR_H_*/
