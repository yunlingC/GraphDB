//===-- traversals/UpdateVisitor.h - Customed visitor class -----*- C++ -*-===//
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

#ifndef _LDBCUPDATEVISITOR_H_
#define _LDBCUPDATEVISITOR_H_

#include "ConcurrentVisitor.h"
#include "Utils.h"

#include <queue>
#include <set>

/// to support delete
class AddVisitor : public ConcurrentVisitor {
public:
  typedef std::pair<FixedString, bool> ReturnValueType;
  typedef PropertyList<FixedString, FixedString> PropertyListType;
  typedef std::pair<bool, std::pair<std::string, GraphType::PropertyListType> > ReturnBranchType;
protected:
  LockManagerType & LockManager;
  GraphType & Graph;
protected:
  unsigned int VertexId;
  PropertyListType VertexProperty;
public:
  AddVisitor (LockManagerType & LockManager, GraphType & graph) :
                LockManager(LockManager), Graph (graph) {}

  void setVertexProperty(PropertyListType & PropertyList) {
    VertexProperty = PropertyList;
  }

  virtual void visitStartVertex(VertexPointer Vertex) {
    VertexId = Graph.addVertex(VertexProperty);
    LockManager.addToVertexLockMap(VertexId);
  }

  virtual bool visitVertex(VertexPointer first) {
      if (first->getId() == VertexId)
        return false;
    auto SourcePtr = first;
    auto VertexPtr = Graph.getVertexPointer(VertexId);
    auto VertexIdLock = false;

    while (!VertexIdLock ) {
      VertexIdLock = LockManager.getVertexLock(SourcePtr, ID, SH);
    }

    ReturnBranchType VertexMatch = checkBranch<ReturnValueType>(first, getFilter());
    if (!VertexMatch.first)  {
      LockManager.releaseVertexLock(SourcePtr, ID, SH);
      return false;
    }

    ///TODO should be moved to graphType.h

    auto VLELock = LockManager.getVertexLock(VertexPtr, LE, EX);
    auto VNELock = LockManager.getVertexLock(VertexPtr, NE, EX);
    auto SLELock = LockManager.getVertexLock(SourcePtr, LE, EX);
    auto SNELock = LockManager.getVertexLock(SourcePtr, NE, EX);

    while (!VLELock || !VNELock || !SLELock || !SNELock) {
      if (VLELock) 
        LockManager.releaseVertexLock(VertexPtr, LE, EX);
      if (VNELock)
        LockManager.releaseVertexLock(VertexPtr, NE, EX);
      if (SLELock) 
        LockManager.releaseVertexLock(SourcePtr, LE, EX);
      if (SNELock)
        LockManager.releaseVertexLock(SourcePtr, NE, EX);

      std::this_thread::yield();

      VLELock = LockManager.getVertexLock(VertexPtr, LE, EX);
      VNELock = LockManager.getVertexLock(VertexPtr, NE, EX);
      SLELock = LockManager.getVertexLock(SourcePtr, LE, EX);
      SNELock = LockManager.getVertexLock(SourcePtr, NE, EX);
    }

    auto EdgePtr = first->getLastEdge();
    auto EdgeLock = false;
    
    unsigned int WhoseLock = 0;
    while (!EdgeLock ) { 
      if (EdgePtr->getFirstVertexPtr()->getId() == first->getId()) {
        EdgeLock = LockManager.getEdgeLock(EdgePtr, FNE, SH); 
        WhoseLock = 1;
      }
      else {
        EdgeLock = LockManager.getEdgeLock(EdgePtr, SNE, SH); 
        WhoseLock = 2;
      }
    }

    unsigned int NewEdgeId = 0;
    ///if PropertyList is null for this edge, don't add; otherwise, add propertylist
    if (VertexMatch.second.second.get("null").second != true) {
      NewEdgeId = Graph.addEdge(VertexId, SourcePtr->getId(),
        VertexMatch.second.first, VertexMatch.second.second);  
    } else {
      NewEdgeId = Graph.addEdge(VertexId, SourcePtr->getId(), 
        VertexMatch.second.first);  
    }

    LockManager.addToEdgeLockMap(NewEdgeId);

    LockManager.releaseVertexLock(VertexPtr, LE, EX);
    LockManager.releaseVertexLock(VertexPtr, NE, EX);
    LockManager.releaseVertexLock(SourcePtr, LE, EX);
    LockManager.releaseVertexLock(SourcePtr, NE, EX);
    LockManager.releaseVertexLock(SourcePtr, ID, SH);
    if (WhoseLock == 2){
      LockManager.releaseEdgeLock(EdgePtr, SNE, SH);
    } else if (WhoseLock == 1){
      LockManager.releaseEdgeLock(EdgePtr, FNE, SH);
    }
    return false;
  }
};


#endif /*_LDBCUPDATEVISTIOR_H_*/
