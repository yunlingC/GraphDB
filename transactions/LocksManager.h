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

#include "GraphType.h"
#include "Lock.h"

///std=c++14 
#include <thread>
#include <map>
#include <iostream>

/// TODO full name
enum MutexType { ID, Pp, LE, NE, FV, SV, FNE, FPE, SNE, SPE, Lb};
enum LockType { SH, EX };

/// currently PLock is only supported in _LOCKING_
/// i.e. in the LockMap we are still use shared_mutex from C++ lib
/// TODO: support PLock with LockMap

class LocksManager {
public:
  typedef std::shared_timed_mutex Mutex;
  typedef VertexLock::MutexPointer  MutexPointer;
  typedef GraphType::VertexPointer VertexPtr;
  typedef GraphType::EdgePointer EdgePtr;
  typedef std::map<unsigned int, VertexLock>  VertexLockMapType;
  typedef std::map<unsigned int, EdgeLock>    EdgeLockMapType;
  typedef std::pair<unsigned int, VertexLock> VLockPair;
  typedef std::pair<unsigned int, EdgeLock>   ELockPair;
  typedef std::vector<std::pair<VertexPtr, std::pair<MutexType, LockType> > > VLockListType; 
  typedef std::vector<std::pair<EdgePtr, std::pair<MutexType, LockType> > > ELockListType; 
public:

#ifndef _LOCKING_
/// locks stored in a map
  LocksManager() {}

  auto getVertexLock(unsigned int VertexId, MutexType Mutex, LockType Lock) 
    -> bool {
    if (VertexLockMap.find(VertexId) == VertexLockMap.end()) {
      std::cerr  << "Error : No such vertex " << VertexId <<" in map \n";
      ///TODO: shoould be exception here and roll back, need to be fixed.
      /**
      *exiting process would give out ownership of this mutex automatically,
      * no need to release it before exiting
      * but consistency state?
      * anyway, it won't happen if we don't delete any lock from manager
      */
      exit(0);
    }
    else {
      MutexPointer MutexPtr = nullptr;
      switch (Mutex) {
        case Pp:
          MutexPtr = VertexLockMap[VertexId].getPpMutex();
          break;
        case LE:
          MutexPtr = VertexLockMap[VertexId].getLEMutex();
          break;
        case NE:
          MutexPtr = VertexLockMap[VertexId].getNEMutex();
          break;
        case ID:
          MutexPtr = VertexLockMap[VertexId].getIdMutex();
          break;
        default:
          std::cerr << "ERROR: No such Mutex in VertexLock\n";
          exit(0);
      }
      switch (Lock) {   
        ///Shared lock
        case SH:
          return MutexPtr->try_lock_shared();
        ///Exclusive lock
        case EX:
          return MutexPtr->try_lock(); 
        default:
          std::cerr << "ERROR: No such Mutex in VertexLock\n";
          exit(0);
      }
    } 
  }

  auto releaseVertexLock(unsigned int VertexId, MutexType Mutex, LockType Lock) 
    -> bool {
    if(VertexLockMap.find(VertexId) == VertexLockMap.end()) {
      std::cerr << "Error : No such vertex " << VertexId <<" in map \n";
      exit(0);
    }
    else {
      MutexPointer MutexPtr = nullptr;
      switch (Mutex) {
        case Pp:
          MutexPtr = VertexLockMap[VertexId].getPpMutex();
          break;
        case LE:
          MutexPtr = VertexLockMap[VertexId].getLEMutex();
          break;
        case NE:
          MutexPtr = VertexLockMap[VertexId].getNEMutex();
          break;
        case ID:
          MutexPtr = VertexLockMap[VertexId].getIdMutex();
          break;
        default:
          std::cerr << "ERROR: No such Mutex in VertexLock\n";
          exit(0);
      }

      switch (Lock) {   
        case SH:
          MutexPtr->unlock_shared();
          break;
        case EX:
          MutexPtr->unlock(); 
          break;
        default:
          std::cerr << "ERROR: No such Mutex in VertexLock\n";
          exit(0);
      }
    }
    return true;
  }

  auto getEdgeLock(unsigned int EdgeId, MutexType Mutex, LockType Lock) 
    -> bool {
    if (EdgeLockMap.find(EdgeId) == EdgeLockMap.end()) {
      std::cerr << "Error : No such edge" << EdgeId <<" in map \n";
      exit(0);
    }

    else {
      MutexPointer MutexPtr = nullptr;
      switch (Mutex) {
        case ID:
          MutexPtr = EdgeLockMap[EdgeId].getIdMutex();
          break;
        case Pp:
          MutexPtr = EdgeLockMap[EdgeId].getPpMutex();
          break;
        case FV:
          MutexPtr = EdgeLockMap[EdgeId].getFVMutex();
          break;
        case SV:
          MutexPtr = EdgeLockMap[EdgeId].getSVMutex();
          break;
        case FNE:
          MutexPtr = EdgeLockMap[EdgeId].getFNEMutex();
          break;
        case FPE:
          MutexPtr = EdgeLockMap[EdgeId].getFPEMutex();
          break;
        case SNE:
          MutexPtr = EdgeLockMap[EdgeId].getSNEMutex();
          break;
        case SPE:
          MutexPtr = EdgeLockMap[EdgeId].getSPEMutex();
          break;
        default:
          std::cerr << "ERROR: No such Mutex in EdgeLock\n";
          exit(0);
      }

      switch (Lock) {   
        case SH:
          return MutexPtr->try_lock_shared();
        case EX:
          return MutexPtr->try_lock(); 
        default:
          std::cerr  << "ERROR: No such Mutex in EdgeLock\n";
          exit(0);
      }
    }
  }

  auto releaseEdgeAll(ELockListType & EdgeLocks) 
    -> void {
    for (auto it = EdgeLocks.begin(), itend = EdgeLocks.end(); 
          it != itend; ++it) {
      releaseEdgeLock((*it).first, (*it).second.first, (*it).second.second);
    }
  }

  auto releaseVertexAll(VLockListType & VertexLocks) 
    -> void {
    for (auto it = VertexLocks.begin(), itend = VertexLocks.end(); 
        it != itend; ++it) {
      releaseVertexLock((*it).first, 
          (*it).second.first, (*it).second.second);
    }
  }

  auto releaseAll(VLockListType & VertexLocks, ELockListType & EdgeLocks) 
    -> void {
      releaseVertexAll(VertexLocks);
      releaseEdgeAll(EdgeLocks);
  }

  auto releaseEdgeLock(unsigned int EdgeId, MutexType Mutex, LockType Lock) 
    -> bool {
    if (EdgeLockMap.find(EdgeId) == EdgeLockMap.end()) {
      exit(0);
    }
    else {
      MutexPointer MutexPtr(nullptr);
      switch (Mutex) {
        case ID:
          MutexPtr = EdgeLockMap[EdgeId].getIdMutex();
          break;
        case Pp:
          MutexPtr = EdgeLockMap[EdgeId].getPpMutex();
          break;
        case FV:
          MutexPtr = EdgeLockMap[EdgeId].getFVMutex();
          break;
        case SV:
          MutexPtr = EdgeLockMap[EdgeId].getSVMutex();
          break;
        case FNE:
          MutexPtr = EdgeLockMap[EdgeId].getFNEMutex();
          break;
        case FPE:
          MutexPtr = EdgeLockMap[EdgeId].getFPEMutex();
          break;
        case SNE:
          MutexPtr = EdgeLockMap[EdgeId].getSNEMutex();
          break;
        case SPE:
          MutexPtr = EdgeLockMap[EdgeId].getSPEMutex();
          break;
        default:
          std::cerr << "ERROR: No such Mutex in EdgeLock\n";
          exit(0);
      }

      switch (Lock) {   
        case SH:
          MutexPtr->unlock_shared();
          break;
        case EX:
          MutexPtr->unlock(); 
          break;
        default:
          std::cerr << "ERROR: No such Mutex in EdgeLock\n";
          exit(0);
      }
    }
    return true;
  }

  auto addToVertexLockMap(unsigned int VertexId) 
    -> void  {
      VertexLock NewVertex;
      VertexLockMap.insert(VLockPair(VertexId, NewVertex));
  }

  auto addToEdgeLockMap(unsigned int EdgeId) 
    -> void  {
      EdgeLock NewEdge;
      EdgeLockMap.insert(ELockPair(EdgeId, NewEdge)); 
  }
 
  auto buildLockMap(GraphType & Graph) 
  -> void {
    typedef GraphType::VertexPointer VertexPointer;
    typedef GraphType::EdgePointer EdgePointer;
    std::map<unsigned int, VertexPointer> VertexMap;
    std::map<unsigned int, EdgePointer> EdgeMap;
    VertexMap = Graph.getVertexMap();
    EdgeMap = Graph.getEdgeMap();

    for (auto iter = VertexMap.begin(), iter_end = VertexMap.end();
        iter != iter_end; iter++) {
      VertexLock* NewVertexLock = new VertexLock();
      VertexLockMap.insert(VLockPair((*iter).first, *NewVertexLock));
      (*iter).second->setVertexLock(NewVertexLock); 
    }

    for (auto it = EdgeMap.begin(), it_end = EdgeMap.end();
        it != it_end; it++) {
      EdgeLock* NewEdgeLock = new EdgeLock();
      EdgeLockMap.insert(ELockPair((*it).first, *NewEdgeLock));
      (*it).second->setEdgeLock(NewEdgeLock);
    }
  }
 

  auto getVertexLockMap() 
    -> VertexLockMapType {
      return VertexLockMap;
  }
  auto getEdgeLockMap() 
    -> EdgeLockMapType {
      return EdgeLockMap;
  }

#else
  ///locks are encoded in Vertex and Edge
public:
  /// TODO const & g
  LocksManager(GraphType & g) : Graph(g) {};

  auto getVertexLock(VertexPtr Vertex, MutexType Mutex, LockType Lock) 
    -> bool {
    if (Vertex == nullptr) {
      std::cerr  << "Error : No such vertex  in map \n";
      ///TODO: shoould be exception here, need to be fixed.
      /**
      *exiting process would give out ownership of this mutex automatically,
      * no need to release it before exiting
      * but consistency state?
      * need transaction rollback and redo
      * anyway, it won't happen if we don't delete any lock from manager
      */
      exit(0);
    }
    MutexPointer MutexPtr = nullptr;
    switch (Mutex) {
      case Pp:
        MutexPtr = Vertex->getVertexLock()->getPpMutex();
        break;
      case LE:
        MutexPtr = Vertex->getVertexLock()->getLEMutex();
        break;
      case NE:
        MutexPtr = Vertex->getVertexLock()->getNEMutex();
        break;
      case ID:
        MutexPtr = Vertex->getVertexLock()->getIdMutex();
        break;
      default:
        std::cerr << "ERROR: No such Mutex in VertexLock\n";
        exit(0);
    }
    switch (Lock) {   
      ///Shared lock
      case SH:
        return MutexPtr->try_lock_shared();
      case EX:
        return MutexPtr->try_lock(); 
      default:
        std::cerr << "ERROR: No such Mutex in VertexLock\n";
        exit(0);
    }
  }

  auto releaseVertexLock(VertexPtr Vertex, MutexType Mutex, LockType Lock) 
    -> bool {
    if (Vertex == nullptr) {
      std::cerr << "Error : No such vertex  in map \n";
      exit(0);
    }

    MutexPointer MutexPtr = nullptr;
    switch (Mutex) {
      case Pp:
        MutexPtr = Vertex->getVertexLock()->getPpMutex();
        break;
      case LE:
        MutexPtr = Vertex->getVertexLock()->getLEMutex();
        break;
      case NE:
        MutexPtr = Vertex->getVertexLock()->getNEMutex();
        break;
      case ID:
        MutexPtr = Vertex->getVertexLock()->getIdMutex();
        break;
      default:
        std::cerr << "ERROR: No such Mutex in VertexLock\n";
        exit(0);
    }

    switch (Lock) {   
      ///Shared lock
      case SH:
        MutexPtr->unlock_shared();
        break;
      case EX:
        MutexPtr->unlock(); 
        break;
      default:
        ///TODO Should be exception here
        std::cerr << "ERROR: No such Mutex in VertexLock\n";
        exit(0);
    }
    return true;
  }

  auto getEdgeLock(EdgePtr Edge, MutexType Mutex, LockType Lock) 
    -> bool {
    if (Edge == nullptr) {
      std::cerr << "Error : No such edge in map \n";
      exit(0);
    }
    MutexPointer MutexPtr = nullptr;
    switch (Mutex) {
      case ID:
        MutexPtr = Edge->getEdgeLock()->getIdMutex();
        break;
      case Pp:
        MutexPtr = Edge->getEdgeLock()->getPpMutex();
        break;
      case FV:
        MutexPtr = Edge->getEdgeLock()->getFVMutex();
        break;
      case SV:
        MutexPtr = Edge->getEdgeLock()->getSVMutex();
        break;
      case FNE:
        MutexPtr = Edge->getEdgeLock()->getFNEMutex();
        break;
      case FPE:
        MutexPtr = Edge->getEdgeLock()->getFPEMutex();
        break;
      case SNE:
        MutexPtr = Edge->getEdgeLock()->getSNEMutex();
        break;
      case SPE:
        MutexPtr = Edge->getEdgeLock()->getSPEMutex();
        break;
      default:
        std::cerr << "ERROR: No such Mutex in EdgeLock\n";
        exit(0);
    }
    switch (Lock) {   
      case SH:
        return MutexPtr->try_lock_shared();
      case EX:
        return MutexPtr->try_lock(); 
      default:
        std::cerr  << "ERROR: No such Mutex in EdgeLock\n";
        exit(0);
    }
  }

  auto releaseEdgeLock(EdgePtr Edge, MutexType Mutex, LockType Lock) 
    -> bool {
    if (Edge == nullptr) {
      exit(0);
    }

    MutexPointer MutexPtr = nullptr;
    switch (Mutex) {
      case ID:
        MutexPtr = Edge->getEdgeLock()->getIdMutex();
        break;
      case Pp:
        MutexPtr = Edge->getEdgeLock()->getPpMutex();
        break;
      case FV:
        MutexPtr = Edge->getEdgeLock()->getFVMutex();
        break;
      case SV:
        MutexPtr = Edge->getEdgeLock()->getSVMutex();
        break;
      case FNE:
        MutexPtr = Edge->getEdgeLock()->getFNEMutex();
        break;
      case FPE:
        MutexPtr = Edge->getEdgeLock()->getFPEMutex();
        break;
      case SNE:
        MutexPtr = Edge->getEdgeLock()->getSNEMutex();
        break;
      case SPE:
        MutexPtr = Edge->getEdgeLock()->getSPEMutex();
        break;
      default:
        std::cerr << "ERROR: No such Mutex in EdgeLock\n";
        exit(0);
    }

    switch (Lock) {   
      case SH:
        MutexPtr->unlock_shared();
        break;
      case EX:
        MutexPtr->unlock(); 
        break;
      default:
        std::cerr << "ERROR: No such Mutex in EdgeLock\n";
        exit(0);
    }
    return true;
  }

  auto releaseEdgeAll(ELockListType & EdgeLocks) 
    -> void {
    for (auto it = EdgeLocks.begin(), it_end = EdgeLocks.end();
        it != it_end; ++it) {
      releaseEdgeLock((*it).first, (*it).second.first, (*it).second.second);
    }
  }

  auto releaseVertexAll(VLockListType & VertexLocks) 
    -> void {
    for (auto it = VertexLocks.begin(), it_end = VertexLocks.end(); 
        it != it_end; ++it) {
      releaseVertexLock((*it).first, 
          (*it).second.first, (*it).second.second);
    }
  }

  auto releaseAll(VLockListType & VertexLocks, ELockListType & EdgeLocks) 
    -> void {
      releaseVertexAll(VertexLocks);
      releaseEdgeAll(EdgeLocks);
  }


  auto addToVertexLockMap(unsigned int VertexId) 
    -> void  {
      VertexPtr Vertex = Graph.getVertexPointer(VertexId);
      if (Vertex == nullptr) {
        std::cerr  << "Error : No such vertex " << VertexId <<" in map \n";
        exit(0);
      }

      VertexLock* NewVertexLock = new VertexLock();
      Vertex->setVertexLock(NewVertexLock);
    }

  auto addToEdgeLockMap(unsigned int EdgeId) 
    -> void  {
      EdgePtr Edge = Graph.getEdgePointer(EdgeId);
      if (Edge == nullptr) {
        std::cerr  << "Error : No such edge" << EdgeId <<" in map \n";
        exit(0);
      }
      EdgeLock*  NewEdgeLock = new EdgeLock();
      Edge->setEdgeLock(NewEdgeLock);
  }
 
  auto buildLockMap(GraphType & Graph) 
  -> void {
    typedef GraphType::VertexPointer VertexPointer;
    typedef GraphType::EdgePointer EdgePointer;
    std::vector<VertexPointer> VertexList;
    std::vector<EdgePointer> EdgeList;
    VertexList = Graph.getAllVertices();
    EdgeList = Graph.getAllEdges();

    for (auto iter = VertexList.begin(), iter_end = VertexList.end();
        iter != iter_end; iter++) {
      VertexLock* NewVertexLock = new VertexLock();
      (*iter)->setVertexLock(NewVertexLock); 
    }

    for (auto it = EdgeList.begin(), it_end = EdgeList.end();
        it != it_end; it++) {
      EdgeLock* NewEdgeLock = new EdgeLock();
      (*it)->setEdgeLock(NewEdgeLock);
    }
  }
#endif

protected:
#ifndef _LOCKING_
  VertexLockMapType VertexLockMap;
  EdgeLockMapType EdgeLockMap;
#else 
  GraphType & Graph;
#endif
    
};

#endif /*_LOCKSMANAGER_H_*/
