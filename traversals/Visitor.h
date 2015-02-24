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

class Visitor {
public:
  typedef FixedString KeyType;
  typedef FixedString ValueType;
  typedef GraphType::VertexPointer VertexPointer;
  typedef GraphType::EdgePointer   EdgePointer;
  typedef GraphType::VertexPropertyList VertexPropertyList;
  typedef GraphType::EdgePropertyList EdgePropertyList;
public:
  virtual  void setFilter(Filter & filter) {
    _filter = filter;
  }

  virtual bool visitStartVertex(VertexPointer vp) {
    return false;
  }
  virtual bool discoverVertex(VertexPointer vp) {
    return false;
  }

  virtual bool visitVertex(VertexPointer vp) {
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

  virtual bool visitDirection(VertexPointer target, EdgePointer edge){
    return true; /// the same above
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



private:
  Filter _filter;

};

#endif /*_VISITORS_H_ */
