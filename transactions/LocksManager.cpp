//===-- transaction/LocksManager.cpp - Lock class type -----------*- C++ -*-===//
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

#ifndef _LOCKSMANAGER_CPP_
#define _LOCKSMANAGER_CPP_

#include "LocksManager.h"

#include <iostream>

  auto LocksManager::getVertexLock(unsigned int VertexId, MutexType Mutex, LockType Lock) 
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

  auto LocksManager::releaseVertexLock(unsigned int VertexId, MutexType Mutex, LockType Lock) 
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

  auto LocksManager::getEdgeLock(unsigned int EdgeId, MutexType Mutex, LockType Lock) 
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

  auto LocksManager::releaseEdgeAll(ELockListType & EdgeLocks) 
    -> void {
    for (auto it = EdgeLocks.begin(), itend = EdgeLocks.end(); 
          it != itend; ++it) {
      releaseEdgeLock((*it).first, (*it).second.first, (*it).second.second);
    }
  }

  auto LocksManager::releaseVertexAll(VLockListType & VertexLocks) 
    -> void {
    for (auto it = VertexLocks.begin(), itend = VertexLocks.end(); 
        it != itend; ++it) {
      releaseVertexLock((*it).first, 
          (*it).second.first, (*it).second.second);
    }
  }

  auto LocksManager::releaseAll(VLockListType & VertexLocks, ELockListType & EdgeLocks) 
    -> void {
      releaseVertexAll(VertexLocks);
      releaseEdgeAll(EdgeLocks);
  }

  auto LocksManager::releaseEdgeLock(unsigned int EdgeId, MutexType Mutex, LockType Lock) 
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

  auto LocksManager::addToVertexLockMap(unsigned int VertexId) 
    -> void  {
      VertexLock NewVertex;
      VertexLockMap.insert(VLockPair(VertexId, NewVertex));
  }

  auto LocksManager::addToEdgeLockMap(unsigned int EdgeId) 
    -> void  {
      EdgeLock NewEdge;
      EdgeLockMap.insert(ELockPair(EdgeId, NewEdge)); 
  }
 
  auto LocksManager::buildLockMap(GraphType & Graph) 
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
 

  auto LocksManager::getVertexLockMap() 
    -> VertexLockMapType {
      return VertexLockMap;
  }
  auto LocksManager::getEdgeLockMap() 
    -> EdgeLockMapType {
      return EdgeLockMap;
  }

#else
  ///locks are encoded in Vertex and Edge
public:
  /// TODO const & g
  LocksManager::LocksManager(GraphType & g) : Graph(g) {};

  auto LocksManager::getVertexLock(VertexPtr Vertex, MutexType Mutex, LockType Lock) 
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

  auto LocksManager::releaseVertexLock(VertexPtr Vertex, MutexType Mutex, LockType Lock) 
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
    } return true;
  }

  auto LocksManager::getEdgeLock(EdgePtr Edge, MutexType Mutex, LockType Lock) 
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

  auto LocksManager::releaseEdgeLock(EdgePtr Edge, MutexType Mutex, LockType Lock) 
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

  auto LocksManager::releaseEdgeAll(ELockListType & EdgeLocks) 
    -> void {
    for (auto it = EdgeLocks.begin(), it_end = EdgeLocks.end();
        it != it_end; ++it) {
      releaseEdgeLock((*it).first, (*it).second.first, (*it).second.second);
    }
  }

  auto LocksManager::releaseVertexAll(VLockListType & VertexLocks) 
    -> void {
    for (auto it = VertexLocks.begin(), it_end = VertexLocks.end(); 
        it != it_end; ++it) {
      releaseVertexLock((*it).first, 
          (*it).second.first, (*it).second.second);
    }
  }

  auto LocksManager::releaseAll(VLockListType & VertexLocks, ELockListType & EdgeLocks) 
    -> void {
      releaseVertexAll(VertexLocks);
      releaseEdgeAll(EdgeLocks);
  }


  auto LocksManager::addToVertexLockMap(unsigned int VertexId) 
    -> void  {
      VertexPtr Vertex = Graph.getVertexPointer(VertexId);
      if (Vertex == nullptr) {
        std::cerr  << "Error : No such vertex " << VertexId <<" in map \n";
        exit(0);
      }

      VertexLock* NewVertexLock = new VertexLock();
      Vertex->setVertexLock(NewVertexLock);
    }

  auto LocksManager::addToEdgeLockMap(unsigned int EdgeId) 
    -> void  {
      EdgePtr Edge = Graph.getEdgePointer(EdgeId);
      if (Edge == nullptr) {
        std::cerr  << "Error : No such edge" << EdgeId <<" in map \n";
        exit(0);
      }
      EdgeLock*  NewEdgeLock = new EdgeLock();
      Edge->setEdgeLock(NewEdgeLock);
  }
 
  auto LocksManager::buildLockMap(GraphType & Graph) 
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

#endif /*_LOCKSMANAGER_CPP_*/
