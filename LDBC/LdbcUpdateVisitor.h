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
protected:
  LockManagerType & LockManager;
  GraphType & graph;
public:

  InsertVisitor (LockManagerType & lm, GraphType & graph) :
                LockManager(lm), graph (graph) {}

  virtual bool visitVertex(VertexPointer vp) {
    bool VertexMatch = checkProperty<ReturnValueType>(vp, getFilter());
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
//    std::cout << "add one vertex " << VertexId << "\n";

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
//    cout << "Insert: Success\n";

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

class AddNodeVisitor : public ConcurrentVisitor {
public:
  typedef pair<FixedString, bool> ReturnValueType;
  typedef PropertyList<FixedString, FixedString> PropertyListType;
  typedef std::pair<bool, std::pair<std::string, GraphType::PropertyListType> > ReturnBranchType;
protected:
  LockManagerType & LockManager;
  GraphType & graph;
protected:
  unsigned int VertexId;
  PropertyListType VertexProperty;
public:

  AddNodeVisitor (LockManagerType & lm, GraphType & graph) :
                LockManager(lm), graph (graph) {}

  void setVertexProperty(PropertyListType & pl) {
    VertexProperty = pl;
  }

  virtual void visitStartVertex(VertexPointer Vertex) {
    VertexId = graph.addVertex(VertexProperty);
    LockManager.addToVertexLockMap(VertexId);

    graph.getVertexPointer(VertexId);
  }

  virtual bool visitVertex(VertexPointer first) {
      if (first->getId() == VertexId )
        return false;
    auto SourceId = first->getId();
    auto VertexIdLock = false;

    while ( !VertexIdLock ) {
      VertexIdLock = LockManager.getVertexLock(SourceId, ID, SH);
    }

    ReturnBranchType VertexMatch = checkBranch<ReturnValueType>(first, getFilter());
    if( !VertexMatch.first )  {
      LockManager.releaseVertexLock(SourceId, ID, SH);
      return false;
    }

    ///TODO should be moved to graphType.h
    auto VLELock = false;
    auto VNELock = false;
    auto SLELock = false;
    auto SNELock = false;

    while( !VLELock || !VNELock || !SLELock || !SNELock) {
      if ( VLELock ) 
        LockManager.releaseVertexLock(VertexId, LE, EX);
      if ( VNELock )
        LockManager.releaseVertexLock(VertexId, NE, EX);
      if ( SLELock ) 
        LockManager.releaseVertexLock(SourceId, LE, EX);
      if ( SNELock )
        LockManager.releaseVertexLock(SourceId, NE, EX);

      ///TODO should there be sleep or wait time here?
//      std::this_thread::sleep_for(std::chrono::milliseconds(1));

      std::this_thread::yield();
      VLELock = LockManager.getVertexLock(VertexId, LE, EX);
      VNELock = LockManager.getVertexLock(VertexId, NE, EX);
      SLELock = LockManager.getVertexLock(SourceId, LE, EX);
      SNELock = LockManager.getVertexLock(SourceId, NE, EX);

    }

    auto EdgePtr = first->getLastEdge();
    auto EdgeId = EdgePtr->getId();
    auto EdgeLock = false;
    
    unsigned int WhoseLock = 0;
    while( !EdgeLock ) { 
      if ( EdgePtr->getFirstVertexPtr()->getId() == first->getId() ) {
        EdgeLock = LockManager.getEdgeLock(EdgeId, FNE, SH); 
        WhoseLock = 1;
      }
      else {
        EdgeLock = LockManager.getEdgeLock(EdgeId, SNE, SH); 
        WhoseLock = 2;
      }
    }

    unsigned int NewEdgeId = 0;
    ///if PropertyList is null for this edge, don't add; otherwise, add propertylist
    if(VertexMatch.second.second.get("null").second != true) {
      NewEdgeId = graph.addEdge(VertexId, SourceId,
        VertexMatch.second.first, VertexMatch.second.second);  
    } else {
      NewEdgeId = graph.addEdge(VertexId, SourceId, 
        VertexMatch.second.first);  
    }

    LockManager.addToEdgeLockMap(NewEdgeId);
//    cout << "Insert: Success\n";

    graph.getEdgePointer(NewEdgeId);

    LockManager.releaseVertexLock(VertexId, LE, EX);
    LockManager.releaseVertexLock(VertexId, NE, EX);
    LockManager.releaseVertexLock(SourceId, LE, EX);
    LockManager.releaseVertexLock(SourceId, NE, EX);
    LockManager.releaseVertexLock(SourceId, ID, SH);
    if ( WhoseLock == 2 ) {
      LockManager.releaseEdgeLock(EdgeId, SNE, SH);
    } else if ( WhoseLock == 1 ){
      LockManager.releaseEdgeLock(EdgeId, FNE, SH);
    }
    return false;
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

//    std::cout << "first " << first->getId() << " second " << second->getId() << " edge " << edge->getId() << "\n";

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

#ifdef _SKIP_
class SkipVisitor : public ConcurrentVisitor {
public:
  typedef pair<FixedString, bool> ReturnValueType;
  typedef PropertyList<FixedString, FixedString> PropertyListType;
  typedef std::pair<bool, std::pair<std::string, GraphType::PropertyListType> > ReturnBranchType;
//  typedef LocksManager & LockManagerType;
protected:
  LockManagerType & LockManager;
  GraphType & graph;
protected:
  unsigned int VertexId;
  PropertyListType VertexProperty;
public:

  SkipVisitor (LockManagerType & lm, GraphType & graph) :
                LockManager(lm), graph (graph) {}

  void setVertexProperty(PropertyListType & pl) {
    VertexProperty = pl;
  }

  virtual void visitStartVertex(VertexPointer Vertex) {
    VertexId = graph.addVertex(VertexProperty);
    LockManager.addToVertexLockMap(VertexId);

    graph.getVertexPointer(VertexId);
  }

  virtual bool visitVertex(VertexPointer first) {
    if (first->getId() == VertexId )
        return false;
    auto SourceId = first->getId();
    auto VertexIdLock = false;

    auto attempt1 = _Attempt;
    while ( (!VertexIdLock) && (attempt1 > 0) ) {
      VertexIdLock = LockManager.getVertexLock(SourceId, ID, SH);
      attempt1--;
    }

    ///if the lock cannot be acquired, just skip the data...
    
    if ( !VertexIdLock) {
      _IsSkip = true;
      return false;
    }

    ReturnBranchType VertexMatch = checkBranch<ReturnValueType>(first, getFilter());
    if( !VertexMatch.first )  {
      LockManager.releaseVertexLock(SourceId, ID, SH);
      return false;
    }

    ///TODO should be moved to graphType.h
    auto  VLELock = LockManager.getVertexLock(VertexId, LE, EX);
    auto  VNELock = LockManager.getVertexLock(VertexId, NE, EX);
    auto  SLELock = LockManager.getVertexLock(SourceId, LE, EX);
    auto  SNELock = LockManager.getVertexLock(SourceId, NE, EX);

    auto att1 = _Attempt;

    while( (!VLELock || !VNELock || !SLELock || !SNELock ) && (att1 > 0)) {
      att1--;
      if ( VLELock ) 
        LockManager.releaseVertexLock(VertexId, LE, EX);
      if ( VNELock )
        LockManager.releaseVertexLock(VertexId, NE, EX);
      if ( SLELock ) 
        LockManager.releaseVertexLock(SourceId, LE, EX);
      if ( SNELock )
        LockManager.releaseVertexLock(SourceId, NE, EX);

      ///TODO should there be sleep or wait time here?
//      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      std::this_thread::yield();
      VLELock = LockManager.getVertexLock(VertexId, LE, EX);
      VNELock = LockManager.getVertexLock(VertexId, NE, EX);
      SLELock = LockManager.getVertexLock(SourceId, LE, EX);
      SNELock = LockManager.getVertexLock(SourceId, NE, EX);
    }

    if (att1 == 0) {
      if ( VLELock ) 
        LockManager.releaseVertexLock(VertexId, LE, EX);
      if ( VNELock )
        LockManager.releaseVertexLock(VertexId, NE, EX);
      if ( SLELock ) 
        LockManager.releaseVertexLock(SourceId, LE, EX);
      if ( SNELock )
        LockManager.releaseVertexLock(SourceId, NE, EX);

      LockManager.releaseVertexLock(SourceId, ID, SH);

//      std::cout << "Attention: skip data on Vertex " << first->getId() << std::endl;
      _IsSkip = true;

      return false;
    }
 
    auto EdgePtr = first->getLastEdge();
    auto EdgeId = EdgePtr->getId();
    auto EdgeLock = false;
    
    unsigned int WhoseLock = 0;

    auto att2 = _Attempt;
    while( (!EdgeLock) && (att2 > 0) ) { 
      att2--;
      if ( EdgePtr->getFirstVertexPtr()->getId() == first->getId() ) {
        EdgeLock = LockManager.getEdgeLock(EdgeId, FNE, SH); 
        WhoseLock = 1;
      }
      else {
        EdgeLock = LockManager.getEdgeLock(EdgeId, SNE, SH); 
        WhoseLock = 2;
      }
    }

    if ( !EdgeLock) {
      LockManager.releaseVertexLock(VertexId, LE, EX);
      LockManager.releaseVertexLock(VertexId, NE, EX);
      LockManager.releaseVertexLock(SourceId, LE, EX);
      LockManager.releaseVertexLock(SourceId, NE, EX);
      LockManager.releaseVertexLock(SourceId, ID, SH);

//      std::cout << "Attention: skip data on Vertex " << first->getId() << std::endl;
      _IsSkip = true;
      return false;
    }

    unsigned int NewEdgeId = 0;
    ///if PropertyList is null for this edge, don't add; otherwise, add propertylist
    if(VertexMatch.second.second.get("null").second != true) {
      NewEdgeId = graph.addEdge(VertexId, SourceId,
        VertexMatch.second.first, VertexMatch.second.second);  
    } else {
      NewEdgeId = graph.addEdge(VertexId, SourceId, 
        VertexMatch.second.first);  
    }

    LockManager.addToEdgeLockMap(NewEdgeId);

    graph.getEdgePointer(NewEdgeId);

    LockManager.releaseVertexLock(VertexId, LE, EX);
    LockManager.releaseVertexLock(VertexId, NE, EX);
    LockManager.releaseVertexLock(SourceId, LE, EX);
    LockManager.releaseVertexLock(SourceId, NE, EX);
    LockManager.releaseVertexLock(SourceId, ID, SH);
    if ( WhoseLock == 2 ) {
      LockManager.releaseEdgeLock(EdgeId, SNE, SH);
    } else if ( WhoseLock == 1 ){
      LockManager.releaseEdgeLock(EdgeId, FNE, SH);
    }
    return false;
  }

  virtual bool SkipVertex(VertexPointer first) {
    return _IsSkip;
  }

private:
  unsigned int _Attempt = 1;
  bool _IsSkip = 0;
};

#endif 


#endif /*_LDBCUPDATEVISTIOR_H_*/
