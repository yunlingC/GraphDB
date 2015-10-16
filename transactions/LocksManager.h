//===-- transaction/LocksManager.h - Lock class type -------------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the main class for managing locks .
///
//===----------------------------------------------------------------------===//

#ifndef _LOCKSMANAGER_H_
#define _LOCKSMANAGER_H_

///std=c++14
#include <shared_mutex>
#include <iostream>
#include <thread>
#include <map>

#include "GraphType.h"
#include "Lock.h"

enum MutexType { ID, Pp, LE, NE, FV, SV, FNE, FPE, SNE, SPE };
enum LockType { SH, EX };

class LocksManager {
public:
  typedef std::shared_timed_mutex Mutex;
  typedef std::shared_ptr<Mutex> MutexPointer;
  typedef std::map<unsigned int, VertexLock>  VLockMapType;
  typedef std::map<unsigned int, EdgeLock>    ELockMapType;
  typedef std::pair<unsigned int, VertexLock> VLockPair;
  typedef std::pair<unsigned int, EdgeLock>   ELockPair;
  typedef vector<unsigned int> LockIdListType;
  typedef vector<pair<unsigned int, pair<MutexType, LockType> > > LockListType; 
public:
  LocksManager() {}

  auto getVertexLock(unsigned int id, MutexType mt, LockType lt) 
    -> bool {
    if(VertexLockMap.find(id) == VertexLockMap.end()) {
      std::cerr  << "Error : No such vertex " << id <<" in map \n";
      ///TODO: shoould be exception here, need to be fixed.
      /**
      *exiting process would give out ownership of this mutex automatically,
      * no need to release it before exiting
      * but consistency state?
      * anyway, it won't happen if we don't delete any lock from manager
      */
      exit(0);
    }
    else {
      MutexPointer mp(nullptr);
      switch(mt) {
        case Pp:
          mp = VertexLockMap[id].getPpMutex();
          break;
        case LE:
          mp = VertexLockMap[id].getLEMutex();
          break;
        case NE:
          mp = VertexLockMap[id].getNEMutex();
          break;
        case ID:
          mp = VertexLockMap[id].getIdMutex();
          break;
        default:
          ///TODO should be exception here
          std::cerr << "ERROR: No such Mutex in VertexLock\n";
          exit(0);
      }//END_SWITCH
      switch (lt) {   
        ///Shared lock
        case SH:
          return mp->try_lock_shared();
        case EX:
          return mp->try_lock(); 
        default:
          ///TODO Should be exception here
          std::cerr << "ERROR: No such Mutex in VertexLock\n";
          exit(0);
      }
    } 
  }

  auto releaseVertexLock(unsigned int id, MutexType mt, LockType lt) 
    -> bool {
    if(VertexLockMap.find(id) == VertexLockMap.end()) {
      std::cerr << "Error : No such vertex " << id <<" in map \n";
      exit(0);
    }
    else {
      MutexPointer mp(nullptr);
      switch(mt) {
        case Pp:
          mp = VertexLockMap[id].getPpMutex();
          break;
        case LE:
          mp = VertexLockMap[id].getLEMutex();
          break;
        case NE:
          mp = VertexLockMap[id].getNEMutex();
          break;
        case ID:
          mp = VertexLockMap[id].getIdMutex();
          break;
        default:
          ///TODO should be exception here
          std::cerr << "ERROR: No such Mutex in VertexLock\n";
          exit(0);
      }//END_SWITCH

      switch (lt) {   
        ///Shared lock
        case SH:
          mp->unlock_shared();
          break;
        case EX:
          mp->unlock(); 
          break;
        default:
          ///TODO Should be exception here
          std::cerr << "ERROR: No such Mutex in VertexLock\n";
          exit(0);
      }
      return true;
    }
  }

  auto getEdgeLock(unsigned int id, MutexType mt, LockType lt) 
    -> bool {
    if (EdgeLockMap.find(id) == EdgeLockMap.end()) {
      std::cerr << "Error : No such edge" << id <<" in map \n";
      exit(0);
    }

    else {
      MutexPointer mp(nullptr);
      switch(mt) {
        case ID:
          mp = EdgeLockMap[id].getIdMutex();
          break;
        case Pp:
          mp = EdgeLockMap[id].getPpMutex();
          break;
        case FV:
          mp = EdgeLockMap[id].getFVMutex();
          break;
        case SV:
          mp = EdgeLockMap[id].getSVMutex();
          break;
        case FNE:
          mp = EdgeLockMap[id].getFNEMutex();
          break;
        case FPE:
          mp = EdgeLockMap[id].getFPEMutex();
          break;
        case SNE:
          mp = EdgeLockMap[id].getSNEMutex();
          break;
        case SPE:
          mp = EdgeLockMap[id].getSPEMutex();
          break;
        default:
          std::cerr << "ERROR: No such Mutex in EdgeLock\n";
          exit(0);
      }//END_SWITCH
      switch (lt) {   
        case SH:
          return mp->try_lock_shared();
        case EX:
          return mp->try_lock(); 
        default:
          std::cerr  << "ERROR: No such Mutex in EdgeLock\n";
          exit(0);
      }
    }
  }

  auto releaseEdgeAll(LockListType & EdgeLocks) 
    -> void {
    auto itend = EdgeLocks.end();
    for ( auto it = EdgeLocks.begin(); it != itend; ++it) {
      releaseEdgeLock((*it).first, 
          (*it).second.first, (*it).second.second);
    }
//    std::cout <<"Release Edge Locks num" << EdgeLocks.size() <<"\n";
  }

  auto releaseVertexAll(LockListType & VertexLocks) 
    -> void {
    auto itend = VertexLocks.end();
    for ( auto it = VertexLocks.begin(); it != itend; ++it) {
      releaseVertexLock((*it).first, 
          (*it).second.first, (*it).second.second);
    }
//    std::cout <<"Release Vertex Locks num" << VertexLocks.size() <<"\n";
  }

  auto releaseAll(LockListType & VertexLocks, LockListType & EdgeLocks) 
    -> void {
      releaseVertexAll(VertexLocks);
      releaseEdgeAll(EdgeLocks);
  }

  auto releaseEdgeLock(unsigned int id, MutexType mt, LockType lt) 
    -> bool {
    if (EdgeLockMap.find(id) == EdgeLockMap.end()) {
      exit(0);
    }
    else {
      MutexPointer mp(nullptr);
      switch(mt) {
        case ID:
          mp = EdgeLockMap[id].getIdMutex();
          break;
        case Pp:
          mp = EdgeLockMap[id].getPpMutex();
          break;
        case FV:
          mp = EdgeLockMap[id].getFVMutex();
          break;
        case SV:
          mp = EdgeLockMap[id].getSVMutex();
          break;
        case FNE:
          mp = EdgeLockMap[id].getFNEMutex();
          break;
        case FPE:
          mp = EdgeLockMap[id].getFPEMutex();
          break;
        case SNE:
          mp = EdgeLockMap[id].getSNEMutex();
          break;
        case SPE:
          mp = EdgeLockMap[id].getSPEMutex();
          break;
        default:
          std::cerr << "ERROR: No such Mutex in EdgeLock\n";
          exit(0);
      }//END_SWITCH

//      std::cout << "Lock Type " << lt << std::endl;
      switch (lt) {   
        case SH:
          mp->unlock_shared();
          break;
        case EX:
          mp->unlock(); 
          break;
        default:
          std::cerr << "ERROR: No such Mutex in EdgeLock\n";
          exit(0);
      }
      return true;
    }
  }

    /// TODO check map
  auto addToVertexLockMap(unsigned int id) 
    -> void  {
      VertexLock NewVertex;
      VertexLockMap.insert(VLockPair(id, NewVertex));
//      std::cout << "add " << id << " to VertexLock map\n";
//      std::cout << VertexLockMap.size() << " locks in total\n";
  }

  auto addToEdgeLockMap(unsigned int id) 
    -> void  {
      EdgeLock NewEdge;
      EdgeLockMap.insert(ELockPair(id, NewEdge)); 
//      std::cout << "add " << id << " to EdgeLock map\n";
//      std::cout << EdgeLockMap.size() << " locks in total\n";
  }
 
  auto buildLockMap(GraphType & graph) 
  -> void {
    typedef GraphType::VertexPointer VertexPointer;
    typedef GraphType::EdgePointer EdgePointer;
    std::map<unsigned int, VertexPointer> VertexMap;
    std::map<unsigned int, EdgePointer> EdgeMap;
    VertexMap = graph.getVertexMap();
    EdgeMap = graph.getEdgeMap();

    for(auto iter = VertexMap.begin();
        iter != VertexMap.end(); iter++) {
//      VertexLock NewVertex;
      VertexLock* NewVertexLock = new VertexLock();
      VertexLockMap.insert(VLockPair((*iter).first, *NewVertexLock));
#ifdef _LOCKING_
      (*iter).second->setVertexLock(NewVertexLock); 
#endif
    }

    for(auto it = EdgeMap.begin();
        it != EdgeMap.end(); it++) {
//      EdgeLock NewEdge;
      EdgeLock* NewEdgeLock = new EdgeLock();
      EdgeLockMap.insert(ELockPair((*it).first, *NewEdgeLock));
#ifdef _LOCKING_
      (*it).second->setEdgeLock(NewEdgeLock);
#endif
    }

//    cout << "after build maps, vertex lock num " << VertexLockMap.size() << " edge lock num " << EdgeLockMap.size() << endl;
  }
 

  auto getVertexLockMap() 
    -> VLockMapType {
      return VertexLockMap;
  }
   auto getEdgeLockMap() 
    -> ELockMapType {
      return EdgeLockMap;
  }

  /// no need to delete; 
  /**
  LockManager();

  ~LockManager() {
    for (auto it = VertexLockMap.begin(); 
          it != VertexLockMap.end(); it++) {
      delete (*it).second;
    }
    for (auto it = EdgeLockMap.begin(); 
          it != EdgeLockMap.end(); it++) {
      delete (*it).second;
    }
  }
  */
protected:
  VLockMapType VertexLockMap;
  ELockMapType EdgeLockMap;
  
};

#endif /*_LOCKSMANAGER_H_*/
