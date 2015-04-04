//===-- traversals/Visitor.h - Customed visitor class ----------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the base class for Graph visitors.
///
//===----------------------------------------------------------------------===//
#ifndef _VISITOR_H_
#define _VISITOR_H_

#include "GraphType.h"
#include "Filter.h"
#include "FixedString.h"

class Visitor {
public:
  typedef FixedString KeyType;
  typedef FixedString ValueType;
  typedef GraphType::VertexPointer VertexPointer;
  typedef GraphType::EdgePointer   EdgePointer;
  typedef GraphType::VertexPropertyList VertexPropertyList;
  typedef GraphType::EdgePropertyList EdgePropertyList;
  typedef GraphType::VertexDescriptor VertexDescriptor;
  typedef std::vector<VertexPointer> VertexTarget;
  typedef std::vector<VertexPointer> VertexPath;
public:
  virtual void setFilter(Filter & f) {
    _filter = f;
  }

  virtual Filter & getFilter() { 
    return _filter;
  }

  virtual VertexTarget & getVertexTargetList() {
    return _VertexTargetList;
  }

  virtual void  visitStartVertex(VertexPointer vp) {
    //return false;
  }

  virtual bool discoverVertex(VertexPointer vp) {
    return false;
  }

  virtual bool visitVertex(VertexPointer vp) {
    FixedString key("id");
    //std::cout << "==>vid " << vp->getId() << " ==>pid " << vp->getPropertyValue(key).first << endl;
    return false;
  }

  virtual bool scheduleVertex(VertexPointer vp, EdgePointer ep) {
    return false;
  }

  virtual  bool visitEdge(EdgePointer ep) {
    return false;
  }
  virtual bool scheduleEdge(EdgePointer ep) {
    return true; /// default vaue = true so that the vertex will be pushed into queue 
  }

  /// for bfs only
  virtual bool visitDirection(VertexPointer target, EdgePointer edge){
    return true; /// the same above
  }

  /// TODO delete this 
  /// for dfs only 0: all direction  1: outedges 2: incoming edges 
  virtual unsigned int visitDirection(VertexPointer) {
    return 0;
  }
  
  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second){
     return false;
  }

  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second) {
    return false;
  }

  virtual bool revisitVertex(VertexPointer vertex) {
    return false;
  }

  virtual bool finishVisit()
  {
    return false;
  }


protected:
  VertexTarget _VertexTargetList; 
  Filter _filter;

};

#endif /*_VISITORS_H_ */
