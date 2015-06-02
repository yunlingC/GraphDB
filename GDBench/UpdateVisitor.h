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

#ifndef _UPDATEVISITOR_H_
#define _UPDATEVISITOR_H_

#include <queue>
#include <set>

#include "CVisitor.h"
#include "Utils.h"

using namespace std;

class AdjacencyExplorer: public Visitor {
public:
  AdjacencyExplorer(LockManagerType & lm) : _LockManager(lm) { }

  virtual bool visitVertex(VertexPointer vertex) {
    auto ReturnValue = false;
    auto Lock = false; 
    while (!Lock) {
      Lock = _LockManager.getVertexSharedLock(vertex->getId());
    }
    if( _DepthList.find(vertex) != _DepthList.end() ) {
      if( _DepthList[vertex] >= 1 ) {
        ReturnValue = true;
      }
    }

   _LockManager.releaseVertexSharedLock(vertex->getId());
    return ReturnValue;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    auto ReturnValue = false;
    auto Lock = false; 
    while(!Lock) {
      Lock = _LockManager.getEdgeSharedLock(edge->getId());
    }

    ReturnValue = checkType(edge, _Filter);
    _LockManager.releaseEdgeSharedLock(edge->getId());
    return ReturnValue;
  }

  virtual bool visitDirection(VertexPointer target, EdgePointer edge) {
    return _DirecMatch;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    _DirecMatch = false;
    auto LockFirst = false; 
    auto LockSecond = false; 
    auto LockEdge = false; 

    while(!LockEdge || !LockFirst || !LockSecond) {
      if( LockEdge ) {
        LockEdge = _LockManager.releaseEdgeSharedLock(edge->getId());
      }

      if( LockFirst ) {
        LockFirst = _LockManager.releaseVertexSharedLock(first->getId());
      }

      if( LockSecond ) {
        LockSecond = _LockManager.releaseVertexSharedLock(second->getId());
      }

      LockEdge = _LockManager.getEdgeSharedLock(edge->getId());
      LockFirst = _LockManager.getVertexSharedLock(first->getId());
      LockSecond = _LockManager.getVertexSharedLock(second->getId());
    }

    bool TypeMatch = checkType(edge, _Filter);
    _DirecMatch = checkDirection(second, edge, _Filter);
    if( _DirecMatch && TypeMatch ) {
      _VertexTargetList.push_back(second);
    }

    computeDepth(first, edge, second, _DepthList);
    _LockManager.releaseVertexSharedLock(first->getId());
    _LockManager.releaseVertexSharedLock(second->getId());
    _LockManager.releaseEdgeSharedLock(edge->getId());

    return false;
  }

protected:
  DepthList _DepthList;
  LockManagerType & _LockManager;
  bool _DirecMatch;
};

///update should be limited to property only
class UpdateVisitor : public Visitor {
  typedef pair<FixedString, bool> ReturnValueType;
  typedef pair<VertexPropertyList, VertexPropertyList> VertexPropPair;
  typedef PropertyList<FixedString, FixedString> PropertyListType;
public:
  UpdateVisitor (LockManagerType & lm) : _LockManager(lm) {}
  virtual bool visitVertex(VertexPointer vp) {
    bool VertexMatch = checkProperty<ReturnValueType>(vp, getFilter());
    if( !VertexMatch ) 
      return false;

    _VertexTargetList.push_back(vp);
    auto Lock = false;
    while( !Lock ) {
      Lock = _LockManager.getVertexExLock(vp->getId());
    }

    _LogRecord->setOperationType(UPDATE);
    ValueType oldV = vp->getPropertyValue("age").first;
    PropertyListType oldProperty, newProperty; 
    oldProperty.set("age", oldV.std_str());
    newProperty.set("age", "20");
    _LogRecord->setVertexPropertyPair(VertexPropPair(oldProperty, newProperty));
    vp->updateProperty("age", "20");

    _LogRecord->setCommit();
    _LockManager.releaseVertexExLock(vp->getId());
    cout << "Update: Success\n";
    
    return true;
  }

  virtual bool scheduleEdge( EdgePointer edge ) {
    auto ReturnValue = false;
    auto Lock = false; 
    while(!Lock) {
      Lock = _LockManager.getEdgeSharedLock(edge->getId());
    }

    ReturnValue = checkType(edge, _Filter);
    _LockManager.releaseEdgeSharedLock(edge->getId());
    return ReturnValue;
  }

  virtual void dumpTarget() {
    dumpVertexTarget(_VertexTargetList);
  }

protected:
  LockManagerType & _LockManager;
};

class InsertVisitor : public Visitor {
public:
  typedef pair<FixedString, bool> ReturnValueType;
public:
  InsertVisitor( GraphType & graph, LockManagerType & lm ): Graph (graph), 
                                                          _LockManager(lm) { };

  virtual bool visitVertex(VertexPointer vp) {
    bool VertexMatch = checkProperty<ReturnValueType>(vp, getFilter());
    if( !VertexMatch ) 
      return false;

    _VertexTargetList.push_back(vp);
   
    auto VertexId = Graph.addVertex();
    _LockManager.addToVertexLockMap(VertexId);

    auto SourceId = vp->getId();
    auto VLock = false;
    auto SLock = false;

    while( !VLock || !SLock ) {
      if( VLock ) 
        _LockManager.releaseVertexExLock(VertexId);
      if( SLock )
        _LockManager.releaseVertexExLock(SourceId);

      VLock = _LockManager.getVertexExLock(VertexId);
      SLock = _LockManager.getVertexExLock(SourceId);
    }

    auto EdgeId = Graph.addEdge(vp->getId(), VertexId, "FRIENDS");
    _LockManager.addToEdgeLockMap(EdgeId);
    cout << "Insert: Success\n";

    _LockManager.releaseVertexExLock(VertexId);
    _LockManager.releaseVertexExLock(SourceId);

    return true;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    auto ReturnValue = false;
    auto Lock = false; 
//    cout << "check locks number " <<  _LockManager.getVertexLockMap().size() << "\n";
    while( !Lock ) {
      Lock = _LockManager.getEdgeSharedLock(edge->getId());
    }

    ReturnValue = checkType(edge, _Filter);
    _LockManager.releaseEdgeSharedLock( edge->getId() );

    return ReturnValue;
  }

  virtual void dumpTarget() {
    dumpVertexTarget( _VertexTargetList );
  }

protected:
  GraphType & Graph;
  LockManagerType &_LockManager;

};

class DeleteVisitor : public Visitor { 
public:
  typedef pair<FixedString, bool> ReturnValueType;
public:
  DeleteVisitor( GraphType & graph, LockManagerType & lm ): Graph (graph),
                                                          _LockManager (lm) { };

  void setEndVertex(VertexDescriptor vertex) {
    _EndVertex =  vertex;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    VertexMatchFlag = false;
    auto Lock = false; 
    while(!Lock) {
      Lock = _LockManager.getVertexSharedLock(vertex->getId());
    }

    bool VertexMatch = checkProperty<ReturnValueType>(vertex, getFilter());
    if(VertexMatch == true) {
      VertexMatchFlag = true;
      _VertexTargetList.push_back(vertex);
    }
    _LockManager.releaseVertexSharedLock(vertex->getId());
    return false;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    auto ReturnValue = false;
    auto Lock = false; 
    while(!Lock) {
      Lock = _LockManager.getEdgeSharedLock(edge->getId());
    }

    ReturnValue = checkType(edge, _Filter);
    _LockManager.releaseEdgeSharedLock(edge->getId());
    return ReturnValue;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if (VertexMatchFlag == false)
      return false;
    if (second->getId() != _EndVertex)
      return false;

    unsigned int FirstId = first->getId();
    unsigned int SecondId = second->getId();
    unsigned int EdgeId = edge->getId();

    auto FLock = false;
    auto ELock = false;
    auto SLock = false;

    while( !FLock || !ELock || !SLock ) {
      if( FLock ) 
        _LockManager.releaseVertexExLock(FirstId);

      if( SLock ) 
        _LockManager.releaseVertexExLock(SecondId);

      if( ELock) 
        _LockManager.releaseVertexExLock(EdgeId);

      FLock = _LockManager.getVertexExLock(FirstId);
      SLock = _LockManager.getVertexExLock(SecondId);
      ELock = _LockManager.getVertexExLock(EdgeId);
    }

    Graph.removeEdgeChain(edge);
    _LockManager.releaseVertexExLock(FirstId);
    _LockManager.releaseVertexExLock(SecondId);
    _LockManager.releaseVertexExLock(EdgeId);

    return true;
  }

  virtual void dumpTarget() {
    dumpVertexTarget(_VertexTargetList);
  }

protected:
  bool VertexMatchFlag = false;
  GraphType & Graph;
  VertexDescriptor _EndVertex;
  LockManagerType & _LockManager;
};

#endif /*_UPDATEVISTIOR_H_*/
