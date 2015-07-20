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
#include "LockManager.h"
#include "TransactionManager.h"

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
//  typedef LogRecord * LogRecordPointer;
//  typedef LockManager &  LockManagerType;
public:

//  void requestLogRecord(LogRecordPointer log) {
//    _LogRecord = log;
//  }
//
//  void setSleepTime(unsigned int time) {
//    _SleepTime = time;
//  }

  virtual void setFilter(Filter & filter) {
    _Filter = filter;
  }

  virtual Filter & getFilter() { 
    return _Filter;
  }

  virtual VertexTarget & getVertexTargetList() {
    return _VertexTargetList;
  }

  virtual void visitStartVertex(VertexPointer vertex) { }

  virtual bool discoverVertex(VertexPointer vertex) {
    return false;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    return false;
  }

  virtual bool scheduleVertex(VertexPointer veretx, EdgePointer edge) {
    return false;
  }

  virtual  bool visitEdge(EdgePointer edge) {
    return false;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    return true; /// default value = true so that the vertex will be pushed into queue 
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

  virtual void finishVisit()  { }

protected:
  unsigned int _SleepTime = 1;
  VertexTarget _VertexTargetList; 
  Filter _Filter;
  ///TODO need another variable to record 
  // whether lockManager is requested
//  LogRecordPointer _LogRecord;
};

#endif /*_VISITORS_H_ */
