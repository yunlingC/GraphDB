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

#include <queue>
#include <set>

#include "ConcurrentVisitor.h"
#include "Utils.h"

using namespace std;

class InsertVisitor : public ConcurrentVisitor {
public:
  typedef pair<FixedString, bool> ReturnValueType;
//  typedef LocksManager & LockManagerType;
protected:
  LockManagerType & LockManager;
  GraphType & graph;
public:

  InsertVisitor (LockManagerType & lm, GraphType & graph) :
                LockManager(lm), graph (graph) {}

  virtual bool visitVertex(VertexPointer vp) {
//    bool GetLock = false;
//    while ( !GetLock ) {
//      GetLock = getVertexLock(vp->getId(), Pp, SH);
//    }
    bool VertexMatch = checkProperty<ReturnValueType>(vp, getFilter());
//    releaseVertexLock(vp->getId(), Pp, SH);
    ///TODO bug here. not consistent
    //there is nothing like upgrade lock from sh to ex directly without deadlock from C++
    //what should we do? Getting the ex at the beginning? too slow...
    //Give up on lock of pp because this is id. No one changes id.

    if( !VertexMatch )  {
      return false;
    }

    _VertexTargetList.push_back(vp);
   
    auto VertexId = graph.addVertex();
    LockManager.addToVertexLockMap(VertexId);
    std::cout << "add one vertex " << VertexId << "\n";

    auto SourceId = vp->getId();
    ///TODO should be moved to graphType.h
    auto VLELock = false;
    auto VNELock = false;
    auto SLELock = false;
    auto SNELock = false;

    while( !VLELock || !VNELock || !SLELock || !SNELock) {
      if( VLELock ) 
        LockManager.releaseVertexLock(VertexId, LE, EX);
      if( VNELock )
        LockManager.releaseVertexLock(VertexId, NE, EX);
      if( SLELock ) 
        LockManager.releaseVertexLock(SourceId, LE, EX);
      if( SNELock )
        LockManager.releaseVertexLock(SourceId, NE, EX);


      ///TODO should there be sleep or wait time here?
      VLELock = LockManager.getVertexLock(VertexId, LE, EX);
      VNELock = LockManager.getVertexLock(VertexId, NE, EX);
      SLELock = LockManager.getVertexLock(SourceId, LE, EX);
      SNELock = LockManager.getVertexLock(SourceId, NE, EX);
    }

    auto EdgeId = graph.addEdge(SourceId, VertexId, "KNOWS");
    LockManager.addToEdgeLockMap(EdgeId);
    cout << "Insert: Success\n";

    LockManager.releaseVertexLock(VertexId, LE, EX);
    LockManager.releaseVertexLock(VertexId, NE, EX);
    LockManager.releaseVertexLock(SourceId, LE, EX);
    LockManager.releaseVertexLock(SourceId, NE, EX);
    return true;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    auto ReturnValue = false;
    auto Lock = false; 
//    cout << "check locks number " <<  LockManager.getVertexLockMap().size() << "\n";
    while( !Lock ) {
      Lock = LockManager.getEdgeLock(edge->getId(), Pp, SH);
    }

    ReturnValue = checkType(edge, _Filter);
    LockManager.releaseEdgeLock( edge->getId(), Pp, SH);

    return ReturnValue;
  }

  virtual void dumpTarget() {
    dumpVertexTarget( _VertexTargetList );
  }
};

class AddVisitor : public ConcurrentVisitor {
public:
  typedef pair<FixedString, bool> ReturnValueType;
//  typedef LocksManager & LockManagerType;
protected:
  unsigned int VertexId;
  LockManagerType & LockManager;
  GraphType & graph;
public:

  AddVisitor (LockManagerType & lm, GraphType & graph) :
                LockManager(lm), graph (graph) {}

  virtual bool visitStartVertex() {
    VertexId = graph.addVertex();
    LockManager.addToVertexLockMap(VertexId);
    std::cout << "add one vertex " << VertexId << "\n";
  }

  virtual bool visitVertex( )
  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    //LOCK 1
    auto EdgeIdLock = false;
    auto VertexIdLock = false;
    while ( !EdgeIdLock ) {
      EdgeIdLock = LockManager.getEdgeLock(edge->getId(), ID, SH):
    }

    bool TypeMatch = checkTypes(edge, _Filter);
    if( !TypeMatch )  {
      LockManager.releaseEdgeLock(edge->getId(), ID, SH);
      return false;
    }

    auto SourceId = first->getId();
    auto VertexIdLock = false;
    while ( !VertexIdLock ) {
      VertexIdLock = LockManager.getVertexLock(SourceId, ID, SH);
    }

    bool VertexMatch = checkProperty<ReturnValueType>(first, getFilter());
    if( !VertexMatch )  {
      LockManager.releaseVertexLock(SourceId, ID, SH);
      LockManager.releaseEdgeLock(edge->getId(), ID, SH);
      return false;
    }

    ///TODO should be moved to graphType.h
    auto VLELock = false;
    auto VNELock = false;
    auto SLELock = false;
    auto SNELock = false;

    while( !VLELock || !VNELock || !SLELock || !SNELock) {
      if( VLELock ) 
        LockManager.releaseVertexLock(VertexId, LE, EX);
      if( VNELock )
        LockManager.releaseVertexLock(VertexId, NE, EX);
      if( SLELock ) 
        LockManager.releaseVertexLock(SourceId, LE, EX);
      if( SNELock )
        LockManager.releaseVertexLock(SourceId, NE, EX);

      ///TODO should there be sleep or wait time here?
      VLELock = LockManager.getVertexLock(VertexId, LE, EX);
      VNELock = LockManager.getVertexLock(VertexId, NE, EX);
      SLELock = LockManager.getVertexLock(SourceId, LE, EX);
      SNELock = LockManager.getVertexLock(SourceId, NE, EX);
    }

    auto EdgePtr = first->getLastEdge();
    auto EdgeId = EdgePtr->getId();
    auto EdgeLock = false;
    
    ///change FirstNextEdge
    while( !EdgeLock ) { 
      if ( EdgePtr->getFirstVertexPtr()->getId() == first->getId() )
        LockManager.getEdgeLock(EdgeId, FNE, SH); 
      else
        LockManager.getEdgeLock(EdgeId, SNE, SH); 
    }
    auto EdgeId = graph.addEdge(SourceId, VertexId, "KNOWS");
    LockManager.addToEdgeLockMap(EdgeId);
    cout << "Insert: Success\n";

    LockManager.releaseVertexLock(VertexId, LE, EX);
    LockManager.releaseVertexLock(VertexId, NE, EX);
    LockManager.releaseVertexLock(SourceId, LE, EX);
    LockManager.releaseVertexLock(SourceId, NE, EX);
    LockManager.releaseVertexLock(SourceId, ID, SH);
    LockManager.releaseEdgeLock(EdgeId, SNE, SH);
    LockManager.releaseEdgeLock(edge->getId(), ID, SH);
    return true;
  }

};


class AdjacencyExplorer: public ConcurrentVisitor {
public:
  AdjacencyExplorer(LockManagerType & lm) : LockManager(lm) { }

  virtual bool visitVertex(VertexPointer vertex) {
    auto ReturnValue = false;
    auto Lock = false; 
    while (!Lock) {
      Lock = LockManager.getVertexLock(vertex->getId(), ID, SH);
    }

    if( _DepthList.find(vertex) != _DepthList.end() ) {
      if( _DepthList[vertex] >= 1 ) {
        ReturnValue = true;
      }
    }

    LockManager.releaseVertexLock(vertex->getId(), ID, SH);
    return ReturnValue;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    auto ReturnValue = false;
    auto Lock = false; 
    while(!Lock) {
      Lock = LockManager.getEdgeLock(edge->getId(), Pp, SH);
    }

    ReturnValue = checkType(edge, _Filter);
    LockManager.releaseEdgeLock(edge->getId(), Pp, SH);
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

    //Edge Id lock and Vertex id lock
    while(!LockEdge || !LockFirst || !LockSecond) {
      if( LockEdge ) {
        LockEdge = LockManager.releaseEdgeLock(edge->getId(), ID, SH);
      }

      if( LockFirst ) {
        LockFirst = LockManager.releaseVertexLock(first->getId(), ID, SH);
      }

      if( LockSecond ) {
        LockSecond = LockManager.releaseVertexLock(second->getId(), ID, SH);
      }

      LockEdge = LockManager.getEdgeLock(edge->getId(), ID, SH);
      LockFirst = LockManager.getVertexLock(first->getId(), ID, SH);
      LockSecond = LockManager.getVertexLock(second->getId(), ID, SH);
    }

    std::cout << "first " << first->getId() << " second " << second->getId() << " edge " << edge->getId() << "\n";

    bool TypeMatch = checkType(edge, _Filter);
    _DirecMatch = checkDirection(second, edge, _Filter);
    if( _DirecMatch && TypeMatch ) {
      _VertexTargetList.push_back(second);
    }

    computeDepth(first, edge, second, _DepthList);
    LockManager.releaseVertexLock(first->getId(), ID, SH);
    LockManager.releaseVertexLock(second->getId(), ID, SH);
    LockManager.releaseEdgeLock(edge->getId(), ID, SH);

    return false;
  }

protected:
  DepthList _DepthList;
  LockManagerType & LockManager;
  bool _DirecMatch;
};


#endif /*_LDBCUPDATEVISTIOR_H_*/
