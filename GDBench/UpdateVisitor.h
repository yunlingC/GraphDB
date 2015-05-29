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

#include "Visitor.h"
#include "Utils.h"

using namespace std;

class AdjacencyExplorer: public Visitor {
public:
  AdjacencyExplorer() { }

  virtual bool visitVertex(VertexPointer vertex) {
    auto ReturnValue = false;
    auto Attempt = 10;
    auto Lock = false; 
    while (!Lock) {
      Lock = _LockManager.getVertexSharedLock(vertex->getId());
      Attempt--;
      if (Attempt < 0) {
        break;
      }
    }
    if( Lock ) {
      cout <<"AdjacencyExplorer: \nget shared lock on : " << vertex->getId() << endl;
      if( _DepthList.find(vertex) != _depthList.end() ) {
        if( _DepthList[vertex] >= 1 ) {
          ReturnValue = true;
        }
        else {
          ReturnValue = false;
        }
      }
      else  {
        ReturnValue = false;
      }
    } //end_if
    else {
      ///TODO:skip data now......
      return false;
    }
    _LockManager.releaseVertexSharedLock(vertex->getId());
    return ReturnValue;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    auto ReturnValue = false;
    auto Attempt = 10;
    auto Lock = false; 
    while(!Lock) {
      Lock = _LockManager.getEdgeSharedLock(edge->getId());
      Attempt--;
      if( Attempt < 0 ) {
        break;
      }
    }

    if( Lock ) {
      ReturnValue = checkType(edge, _Filter);
    }
    else {
      ///skip current edge
      cout << "skip data on edge" << edge->getId() << "\n";
      return false;
    }
    _LockManager.releaseEdgeSharedLock(edge->getId());
    return ReturnValue;
  }

  virtual bool visitDirection(VertexPointer target, EdgePointer edge) {
    return _DirecMatch;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    _DirecMatch = false;
    auto AttemptFirst = 10;
    auto AttemptSecond = 10;
    auto AttemptEdge = 10;
    auto LockFirst = false; 
    auto LockSecond = false; 
    auto LockEdge = false; 

    while( !LockEdge ) {
      LockEdge = _LockManager.getEdgeSharedLock(edge->getId());
      AttemptEdge--;
      if( AttemptEdge < 0 ) {
        break;
      }
    }

    while( !LockFirst ) {
      LockFirst = _LockManager.getVertexSharedLock(first->getId());
      AttemptFirst--;
      if( AttemptFirst < 0 ) {
        break;
      }
    }

    while( !LockSecond ) {
      LockSecond = _LockManager.getVertexSharedLock(second->getId());
      AttemptSecond--;
      if( AttemptSecond < 0 ) {
        break;
      }
    }

    if( LockFirst && LockSecond && LockEdge ) {
      bool TypeMatch = checkType(edge, _Filter);
      _DirecMatch = checkDirection(second, edge, _Filter);
      if( _DirecMatch && TypeMatch ) {
        _VertexTargetList.push_back(second);
      }
      computeDepth(first, edge, second, _DepthList);
    } 

    if( LockFirst ) {
        _LockManager.releaseVertexSharedLock(first->getId());
    }
    if( LockSecond ) {
      _LockManager.releaseVertexSharedLock(second->getId());
    } 
    if( LockEdge ) {
      _LockManager.releaseEdgeSharedLock(edge->getId());
    } 
    return false;
  }

protected:
  DepthList _DepthList;
  bool _DirecMatch;
};

///update should be limited to property only
class UpdateVisitor : public Visitor {
  typedef pair<FixedString, bool> ReturnValueType;
  typedef pair<VertexPropertyList, VertexPropertyList> VertexPropPair;
  typedef PropertyList<FixedString, FixedString> PropertyListType;
public:
  virtual bool visitVertex(VertexPointer vp) {
    bool VertexMatch = checkProperty<ReturnValueType>(vp, getFilter());
    if( VertexMatch == true ) {
      _VertexTargetList.push_back(vp);
      std::shared_ptr<Mutex> mp(nullptr);
      if( _LockManager.getVertexLockMap().find(vp->getId()) != 
          _LockManager.getVertexLockMap().end() ) {
        mp = ( _LockManager.getVertexLockMap()[vp->getId()] );
        unsigned int  chance = 20;
        while( !mp ) {
          chance --;
          if( chance > 0 ) {
            cout << "Update: Pointer taken, try " << chance << " more times\n";
            mp = (_LockManager.getVertexLockMap()[vp->getId()]);
          } else  {
            break;
          }
        }
        if( mp ) {
          auto lock = mp->try_lock();
          unsigned int chances = 10;
          while( !lock ) {
            chances --;
            if(chances > 0) {
              cout << "Update: Lock taken, try " << chances << " more times\n";
              std::this_thread::sleep_for(std::chrono::seconds(1));
              lock = mp->try_lock();
            } else  {
              break;
            }
          }
          if( lock == true ) {
            _LogRecord->setOperationType(UPDATE);
            ValueType oldV = vp->getPropertyValue("age").first;
            PropertyListType oldProperty, newProperty; 
            oldProperty.set("age", oldV.std_str());
            newProperty.set("age", "20");
            _LogRecord->setVertexPropertyPair(VertexPropPair(oldProperty, newProperty));
            vp->updateProperty("age", "20");
            std::this_thread::sleep_for(std::chrono::seconds(_SleepTime));
            _LogRecord->setCommit();
            mp->unlock();
            cout << "Update: Success\n";
          } else {
            cout << "Update: Failed to get lock, exit\n";
          }
        } else {
          cout << "Update: Failed to get mutex, exit\n";
        }
      }
 
      return true;
    }
    return false;
  }

  virtual bool scheduleEdge( EdgePointer edge ) {
    auto ReturnValue = false;
    auto Attempt = 10;
    auto Lock = false; 
    while(!Lock) {
      Lock = _LockManager.getEdgeSharedLock(edge->getId());
      Attempt--;
      if( Attempt < 0 ) {
        break;
      }
    }

    if( Lock ) {
      ReturnValue = checkType(edge, _Filter);
    }
    else {
      ///skip current edge
      return false;
    }
    _LockManager.releaseEdgeSharedLock(edge->getId());
    return ReturnValue;
  }

  virtual void dumpTarget() {
    dumpVertexTarget(_VertexTargetList);
  }
};

class InsertVisitor : public Visitor {
public:
  typedef pair<FixedString, bool> ReturnValueType;
public:
  InsertVisitor(GraphType & graph): Graph (graph) { };

  virtual bool visitVertex(VertexPointer vp) {
    bool VertexMatch = checkProperty<ReturnValueType>(vp, getFilter());
    if( VertexMatch == true ) {
      _VertexTargetList.push_back(vp);
     
      auto VertexId = Graph.addVertex();
      _LockManager.addToVertexLockMap(VertexId);
//      cout << "new Vertex id " << VertexId << endl;
      MutexPointer mpo(nullptr); //mutex for existed vertex
      MutexPointer mpn(nullptr); //mutex for new-added vertex
      if( (_LockManager.getVertexLockMap().find(vp->getId()) != 
            _LockManager.getVertexLockMap().end() ) 
          && ( _LockManager.getVertexLockMap().find(VertexId) != 
            _LockManager.getVertexLockMap().end())) {
        mpo = (_LockManager.getVertexLockMap()[vp->getId()]);
        mpn = (_LockManager.getVertexLockMap()[VertexId]);
        unsigned int  chance = 20;
        while( !(mpo && mpn) ) {
          chance --;
          if( chance > 0 ) {
            cout << "Insert: Pointer taken, try " << chance << " more times\n";
            mpo = (_LockManager.getVertexLockMap()[vp->getId()]);
            mpn = (_LockManager.getVertexLockMap()[VertexId]);
          } else  {
            break;
          }
        }
        if( mpo && mpn ) {
          auto lo = mpo->try_lock(); auto ln = mpn->try_lock();
          unsigned int chances = 10;
          while( !lo  || !ln) {
            chances --;
            if( chances > 0 ) {
              cout << "Insert: Lock taken, try " << chances << " more times\n";
              if ( lo )
                mpo->unlock(); 
              if(ln)
                mpn->unlock();
              std::this_thread::sleep_for(std::chrono::seconds(1));
              lo = mpo->try_lock(); ln = mpn->try_lock();
            } else  {
              break;
            }
          }
          if( lo && ln ) {
            auto EdgeId = Graph.addEdge(vp->getId(), VertexId, "FRIENDS");
            _LockManager.addToEdgeLockMap(EdgeId);
            std::this_thread::sleep_for(std::chrono::seconds(_SleepTime));
            cout << "Insert: Success\n";

            mpo->unlock();
            mpn->unlock();
          } else {
            cout << "Insert: Failed to get lock, exit\n";
          }
        } else {
          cout << "Insert: Failed to get mutex, exit\n";
        }
      }
      return true;
    }
    return false;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    auto ReturnValue = false;
    auto Attempt = 10;
    auto Lock = false; 
    while( !Lock ) {
      Lock = _LockManager.getEdgeSharedLock(edge->getId());
      Attempt--;
      if( Attempt < 0 ) {
        break;
      }
    }

    if( Lock ) {
      ReturnValue = checkType(edge, _Filter);
    }
    else {
      ///skip current edge
      return false;
    }
    _LockManager.releaseEdgeSharedLock( edge->getId() );
    return ReturnValue;
  }

  virtual void dumpTarget() {
    dumpVertexTarget( _VertexTargetList );
  }

protected:
  GraphType & Graph;
};

class DeleteVisitor : public Visitor { 
public:
  typedef pair<FixedString, bool> ReturnValueType;
public:
  DeleteVisitor( GraphType & graph ): Graph (graph) { };

  void setEndVertex(VertexDescriptor vertex) {
    _EndVertex =  vertex;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    VertexMatchFlag = false;
    auto Attempt = 10;
    auto Lock = false; 
    while(!Lock) {
      Lock = _LockManager.getVertexSharedLock(vertex->getId());
      Attempt--;
      if(Attempt < 0) {
        break;
      }
    }
    if(Lock) {
      cout <<"AdjacencyExplorer: get shared lock on : " << vertex->getId() << endl;
      bool VertexMatch = checkProperty<ReturnValueType>(vertex, getFilter());
      if(VertexMatch == true) {
        VertexMatchFlag = true;
        _VertexTargetList.push_back(vertex);
      }
    _LockManager.releaseVertexSharedLock(vertex->getId());
    }
    return false;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    auto ReturnValue = false;
    auto Attempt = 10;
    auto Lock = false; 
    while(!Lock) {
      Lock = _LockManager.getEdgeSharedLock(edge->getId());
      Attempt--;
      if(Attempt < 0) {
        break;
      }
    }

    if(Lock) {
      ReturnValue = checkType(edge, _Filter);
    }
    else {
      ///skip current edge
      return false;
    }
    _LockManager.releaseEdgeSharedLock(edge->getId());
    return ReturnValue;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    if (VertexMatchFlag == false)
      return false;
    if (second->getId() != _EndVertex)
      return false;

    MutexPointer mpo(nullptr); //mutex for existed vertex
    MutexPointer mpn(nullptr); //mutex for new-added vertex
    MutexPointer mpe(nullptr); //mutex for edge between two 
      
    if( (_LockManager.getVertexLockMap().find(first->getId()) 
          != _LockManager.getVertexLockMap().end()) 
        && ( _LockManager.getVertexLockMap().find(second->getId()) != _LockManager.getVertexLockMap().end())
        && ( _LockManager.getEdgeLockMap().find(edge->getId()) != _LockManager.getEdgeLockMap().end())) {
        mpo = (_LockManager.getVertexLockMap()[first->getId()]);
        mpn = (_LockManager.getVertexLockMap()[_EndVertex]);
        mpe = (_LockManager.getEdgeLockMap()[edge->getId()]);
        unsigned int  chance = 20;
        while( !(mpo && mpn && mpe)) {
          chance --;
          if(chance > 0) {
            cout << "Delete: Pointer taken, try " << chance << " more times\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (!mpo) {
              mpo = (_LockManager.getVertexLockMap()[first->getId()]);
            }
            if(!mpn) {
              mpn = (_LockManager.getVertexLockMap()[_EndVertex]);
            }
            if (!mpe) {
              mpe = (_LockManager.getEdgeLockMap()[edge->getId()]);
            }
          } else  {
            break;
          }
        }
        if(mpo && mpn && mpe) {
          auto lo = mpo->try_lock(); auto ln = mpn->try_lock(); auto le = mpe->try_lock();
          unsigned int chances = 10;
          while( !lo  || !ln || !le) {
            chances --;
            if(chances > 0) {
              cout << "Delete: Lock taken, try " << chances << " more times\n";
              if (lo)
                mpo->unlock(); 
              if(ln)
                mpn->unlock();
              if(le)
                mpe->unlock();
              std::this_thread::sleep_for(std::chrono::seconds(_SleepTime));
              lo = mpo->try_lock(); ln = mpn->try_lock(); le = mpe->try_lock();
            } else  {
              break;
            }
          }
          if( lo && ln && le) {
            Graph.removeEdgeChain(edge);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            mpo->unlock();
            mpn->unlock();
            mpe->unlock();
            cout << "Delete: Success\n";
          } else {
            cout << "Delete: Failed to get lock, exit\n";
          }
        } else {
          cout << "Delete: Failed to get mutex, exit\n";
        }
      } else {
        cout << "Delete: Lock does not exist\n";
      }
      return true;
    }

  virtual void dumpTarget() {
    dumpVertexTarget(_VertexTargetList);
  }

protected:
  bool VertexMatchFlag = false;
  GraphType & Graph;
  VertexDescriptor _EndVertex;
};

#endif /*_UPDATEVISTIOR_H_*/
