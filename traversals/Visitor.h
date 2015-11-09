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
  typedef GraphType::VertexDescriptor VertexDescriptor;
  typedef std::vector<VertexPointer> VertexTarget;
  typedef std::vector<VertexPointer> VertexPath;
public:
  virtual void setFilter(Filter & filter) {
    _Filter = filter;
  }

  virtual Filter & getFilter() { 
    return _Filter;
  }

  virtual VertexTarget & getVertexTargetList() {
    return _VertexTargetList;
  }

  virtual void  visitStartVertex(VertexPointer VertexPtr) { }

  virtual bool discoverVertex(VertexPointer VertexPtr) {
    return false;
  }

  virtual bool visitVertex(VertexPointer VertexPtr) {
    return false;
  }

  virtual bool scheduleVertex(VertexPointer VertexPtr, EdgePointer EdgePtr) {
    return false;
  }

  virtual  bool visitEdge(EdgePointer EdgePtr) {
    return false;
  }
  virtual bool scheduleEdge(EdgePointer EdgePtr) {
    /// default value = true so that the vertex will be pushed into queue 
    return true; 
  }

  /// for bfs only
  virtual bool visitDirection(VertexPointer TargetVertex, EdgePointer EdgePtr){
    /// the same above
    return true; 
  }

  virtual bool scheduleBranch(VertexPointer FirstVertex, 
                              EdgePointer EdgePtr, 
                              VertexPointer SecondVertex){
     return false;
  }

  virtual bool scheduleTree(VertexPointer FirstVertex, 
                            EdgePointer EdgePtr, 
                            VertexPointer SecondVertex) {
    return false;
  }

  virtual bool revisitVertex(VertexPointer VertexPtr) {
    return false;
  }

  virtual bool finishVisit()
  {
    return false;
  }


protected:
  VertexTarget _VertexTargetList; 
  Filter _Filter;

};

#endif /*_VISITORS_H_ */
