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

#include <stack>
#include <string>
#include <iostream>

#define _DEBUG_ENABLE_ true

#ifndef _LOCKING_
  LocksManager::LocksManager() {
    DeadlockDetector = std::shared_ptr<std::mutex>(new std::mutex) ;
  }

  auto LocksManager::getVertexLock(IdType VertexId, MutexType Mutex, LockType Lock) 
    -> bool {
#if _DEBUG_ENABLE_
    if (VertexLockMap.find(VertexId) == VertexLockMap.end()) {
      std::cerr  << "Error : No such vertex " << VertexId <<" in map \n";
      exit(0);
    }
#endif
    MutexPointer MutexPtr = nullptr;
    switch (Mutex) {
      case T_Property:
        MutexPtr = VertexLockMap[VertexId].getPpMutex();
        break;
      case T_LastEdge:
        MutexPtr = VertexLockMap[VertexId].getLEMutex();
        break;
      case T_NextEdge:
        MutexPtr = VertexLockMap[VertexId].getNEMutex();
        break;
      case T_ID:
        MutexPtr = VertexLockMap[VertexId].getIdMutex();
        break;
      default:
        std::cerr << "ERROR: No such Mutex in VertexLock\n";
        exit(0);
    }
    switch (Lock) {   
      ///Shared lock
      case T_SH:
        return MutexPtr->try_lock_shared();
      ///Exclusive lock
      case T_EX:
        return MutexPtr->try_lock(); 
      default:
        std::cerr << "ERROR: No such Mutex in VertexLock\n";
        exit(0);
    }
  }

  auto LocksManager::releaseVertexLock(IdType VertexId, MutexType Mutex, LockType Lock) 
    -> bool {
    if(VertexLockMap.find(VertexId) == VertexLockMap.end()) {
      std::cerr << "Error : No such vertex " << VertexId <<" in map \n";
      exit(0);
    }
    else {
      MutexPointer MutexPtr = nullptr;
      switch (Mutex) {
        case T_Property:
          MutexPtr = VertexLockMap[VertexId].getPpMutex();
          break;
        case T_LastEdge:
          MutexPtr = VertexLockMap[VertexId].getLEMutex();
          break;
        case T_NextEdge:
          MutexPtr = VertexLockMap[VertexId].getNEMutex();
          break;
        case T_ID:
          MutexPtr = VertexLockMap[VertexId].getIdMutex();
          break;
        default:
          std::cerr << "ERROR: No such Mutex in VertexLock\n";
          exit(0);
      }

      switch (Lock) {   
        case T_SH:
          MutexPtr->unlock_shared();
          break;
        case T_EX:
          MutexPtr->unlock(); 
          break;
        default:
          std::cerr << "ERROR: No such Mutex in VertexLock\n";
          exit(0);
      }
    }
    return true;
  }

  auto LocksManager::getEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock) 
    -> bool {
    if (EdgeLockMap.find(EdgeId) == EdgeLockMap.end()) {
      std::cerr << "Error : No such edge" << EdgeId <<" in map \n";
      exit(0);
    }

    else {
      MutexPointer MutexPtr = nullptr;
      switch (Mutex) {
        case T_ID:
          MutexPtr = EdgeLockMap[EdgeId].getIdMutex();
          break;
        case T_Property:
          MutexPtr = EdgeLockMap[EdgeId].getPpMutex();
          break;
        case T_FirstVertex:
          MutexPtr = EdgeLockMap[EdgeId].getFVMutex();
          break;
        case T_SecondVertex:
          MutexPtr = EdgeLockMap[EdgeId].getSVMutex();
          break;
        case T_FirstNextEdge:
          MutexPtr = EdgeLockMap[EdgeId].getFNEMutex();
          break;
        case T_FirstPrevEdge:
          MutexPtr = EdgeLockMap[EdgeId].getFPEMutex();
          break;
        case T_SecondNextEdge:
          MutexPtr = EdgeLockMap[EdgeId].getSNEMutex();
          break;
        case T_SecondPrevEdge:
          MutexPtr = EdgeLockMap[EdgeId].getSPEMutex();
          break;
        default:
          std::cerr << "ERROR: No such Mutex in EdgeLock\n";
          exit(0);
      }

      switch (Lock) {   
        case T_SH:
          return MutexPtr->try_lock_shared();
        case T_EX:
          return MutexPtr->try_lock(); 
        default:
          std::cerr  << "ERROR: No such Mutex in EdgeLock\n";
          exit(0);
      }
    }
  }

/**
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
  */

  auto LocksManager::releaseEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock) 
    -> bool {
    if (EdgeLockMap.find(EdgeId) == EdgeLockMap.end()) {
      exit(0);
    }
    else {
      MutexPointer MutexPtr(nullptr);
      switch (Mutex) {
        case T_ID:
          MutexPtr = EdgeLockMap[EdgeId].getIdMutex();
          break;
        case T_Property:
          MutexPtr = EdgeLockMap[EdgeId].getPpMutex();
          break;
        case T_FirstVertex:
          MutexPtr = EdgeLockMap[EdgeId].getFVMutex();
          break;
        case T_SecondVertex:
          MutexPtr = EdgeLockMap[EdgeId].getSVMutex();
          break;
        case T_FirstNextEdge:
          MutexPtr = EdgeLockMap[EdgeId].getFNEMutex();
          break;
        case T_FirstPrevEdge:
          MutexPtr = EdgeLockMap[EdgeId].getFPEMutex();
          break;
        case T_SecondNextEdge:
          MutexPtr = EdgeLockMap[EdgeId].getSNEMutex();
          break;
        case T_SecondPrevEdge:
          MutexPtr = EdgeLockMap[EdgeId].getSPEMutex();
          break;
        default:
          std::cerr << "ERROR: No such Mutex in EdgeLock\n";
          exit(0);
      }

      switch (Lock) {   
        case T_SH:
          MutexPtr->unlock_shared();
          break;
        case T_EX:
          MutexPtr->unlock(); 
          break;
        default:
          std::cerr << "ERROR: No such Mutex in EdgeLock\n";
          exit(0);
      }
    }
    return true;
  }

  auto LocksManager::addToVertexLockMap(IdType VertexId) 
    -> void  {
      VertexLock NewVertex;
      VertexLockMap.insert(VLockPair(VertexId, NewVertex));
  }

  auto LocksManager::addToEdgeLockMap(IdType EdgeId) 
    -> void  {
      EdgeLock NewEdge;
      EdgeLockMap.insert(ELockPair(EdgeId, NewEdge)); 
  }
 
  auto LocksManager::buildLockMap(GraphType & Graph) 
  -> void {
    typedef GraphType::VertexPointer VertexPointer;
    typedef GraphType::EdgePointer EdgePointer;
    std::unordered_map<IdType, VertexPointer> VertexMap;
    std::unordered_map<IdType, EdgePointer> EdgeMap;
    VertexMap = Graph.getVertexMap();
    EdgeMap = Graph.getEdgeMap();

    for (auto iter = VertexMap.begin(), iter_end = VertexMap.end();
        iter != iter_end; iter++) {
      VertexLock* NewVertexLock = new VertexLock();
      VertexLockMap.insert(VLockPair((*iter).first, *NewVertexLock));
//      (*iter).second->setVertexLock(NewVertexLock); 
    }

    for (auto it = EdgeMap.begin(), it_end = EdgeMap.end();
        it != it_end; it++) {
      EdgeLock* NewEdgeLock = new EdgeLock();
      EdgeLockMap.insert(ELockPair((*it).first, *NewEdgeLock));
//      (*it).second->setEdgeLock(NewEdgeLock);
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

  auto LocksManager::requireVertexLock(IdType VertexId, MutexType Mutex, LockType Lock) 
    -> LockRetPairType  {
#if _DEBUG_ENABLE_
    if (VertexLockMap.find(VertexId) == VertexLockMap.end()) {
      std::cerr  << "Error : No such vertex " << VertexId <<" in map \n";
      exit(0);
    }
#endif
    MutexPointer MutexPtr = nullptr;
    switch (Mutex) {
      case T_Property:
        MutexPtr = VertexLockMap[VertexId].getPpMutex();
        break;
      case T_LastEdge:
        MutexPtr = VertexLockMap[VertexId].getLEMutex();
        break;
      case T_NextEdge:
        MutexPtr = VertexLockMap[VertexId].getNEMutex();
        break;
      case T_ID:
        MutexPtr = VertexLockMap[VertexId].getIdMutex();
        break;
      default:
        std::cerr << "ERROR: No such Mutex in VertexLock\n";
        exit(0);
    }
    bool  RetValue  = false;
    switch (Lock) {   
      ///Shared lock
      case T_SH:
        RetValue = MutexPtr->try_lock_shared();
        break;
      ///Exclusive lock
      case T_EX:
        RetValue = MutexPtr->try_lock(); 
        break;
#if _DEBUG_ENABLE_
      default:
        std::cerr << "ERROR: No such Mutex in VertexLock\n";
        exit(0);
#endif
    }
    return LockRetPairType(RetValue, MutexPtr);
  }

  auto LocksManager::requireEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock) 
    -> LockRetPairType  {
#if _DEBUG_ENABLE_
    if (EdgeLockMap.find(EdgeId) == EdgeLockMap.end()) {
      std::cerr << "Error : No such edge" << EdgeId <<" in map \n";
      exit(0);
    }
#endif
    MutexPointer MutexPtr = nullptr;
    switch (Mutex) {
      case T_ID:
        MutexPtr = EdgeLockMap[EdgeId].getIdMutex();
        break;
      case T_Property:
        MutexPtr = EdgeLockMap[EdgeId].getPpMutex();
        break;
      case T_FirstVertex:
        MutexPtr = EdgeLockMap[EdgeId].getFVMutex();
        break;
      case T_SecondVertex:
        MutexPtr = EdgeLockMap[EdgeId].getSVMutex();
        break;
      case T_FirstNextEdge:
        MutexPtr = EdgeLockMap[EdgeId].getFNEMutex();
        break;
      case T_FirstPrevEdge:
        MutexPtr = EdgeLockMap[EdgeId].getFPEMutex();
        break;
      case T_SecondNextEdge:
        MutexPtr = EdgeLockMap[EdgeId].getSNEMutex();
        break;
      case T_SecondPrevEdge:
        MutexPtr = EdgeLockMap[EdgeId].getSPEMutex();
        break;
#if _DEBUG_ENABLE_
      default:
        std::cerr << "ERROR: No such Mutex in EdgeLock\n";
        exit(0);
#endif
    }

    bool RetValue = false;
    switch (Lock) {   
      case T_SH:
        RetValue  = MutexPtr->try_lock_shared();
        break;
      case T_EX:
        RetValue  = MutexPtr->try_lock(); 
        break;
#if _DEBUG_ENABLE_
      default:
        std::cerr  << "ERROR: No such Mutex in EdgeLock\n";
        exit(0);
#endif
    }
    return LockRetPairType(RetValue, MutexPtr);
  }

  auto LocksManager::checkWaitOn(IdType TransId, LockPointer LockPtr, LockType LType) 
    ->  DLRetType  {
      /// If current trans is waiting for some other lock, then give up
      /// Usually it won't happen because this trans must be spining on that lock
      /// TODO to be deleted
//        if (WaitMap.find(TransId) == WaitMap.end())
//          return false;
//        TransMapType TxList = TransMap.find(LockPtr);
//        /// If Lock is free, acquire it and register
//        if (TxList.empty()) {
//          registerTransMap(TransId, LockPtr);
//          registerLockMap(TransId, LockPtr);
//        }
//        /// If lock is acquired by trans, check if deadlock exists.
//        else {
          TransStackType  TransStack;
          TransSetType CheckedTransList;
          auto TxList = ResrMap.find(LockPtr);
          /// TxList should NOT be empty, otherwise getLock should work
#if _DEBUG_ENABLE_
          if (TxList == ResrMap.end()) {
            std::cerr << "Error : Lock is not available but no record found\n";
            exit(0);
          }
#endif
          auto iter = (*TxList).second.begin();
          while (iter != (*TxList).second.end()) {
            /// Check if current transaction already holds this lock 
            if ((*iter).first == TransId) {
              /*
              There can be 4 scenarios when one transaction holds a lock 
              and requests it again
              <r, r>, <r, w>, <w, r>, <w, w>
              Since TxList is a set, we can rule out <r, r>
                S1 <r, w> 
                    if not r'ed by others
                        =>  r -> w
                    otherwise
                      =>  continue
                        {
                          =>  unlock_sh
                          =>  check deadlock
                        }
                        
                S2 <w, r> ignore
                S3 <w, w> ignore
              */

              /// TODO problem with this
              /// might come into this situation
              /// T1 has lock <L1, SH> and requests <L1, EX> with sucess 
              /// BUT without registering!
              /// This is possible because in STL thread with shared_timed_mutex:
              /// Undefined behavior might happen when one thread requests
              /// the same mutex again
              switch((*iter).second) {
                /// <w, *>
                case  T_EX:
                  return T_Ignore;
                case  T_SH:
                  /// <r, w>
                  if (LType == T_EX) {
                    /// No other transaction is holding this lock
                    if ((*TxList).second.size() == 1) {
                      return T_Upgrade;
                    }
                    else {
                      /// Unlock it and go on
//                      releaseLock();
                      /// remove itself from list
//                      removeLock();
                      auto Ret = checkWaitOn(TransId, LockPtr, LType) ? T_Wait : T_Abort;
                      return Ret;
                    }
                  }
              }// switch
            } // if 
            TransStack.push((*iter).first);
//            CheckedTransList.insert(*iter.first);
            if (!checkWaitOnRecursive(TransId, (*iter).first, TransStack, CheckedTransList)) {
              /// There is a potential deadlock
              return T_Abort;
            } //if 
            TransStack.pop();
            iter++;
          }//while

//          registerWaitingMap();
          return T_Wait;
        }
            
    auto LocksManager::checkWaitOnRecursive(IdType WaitingTrans
                                            , IdType LockingTrans
                                            , TransStackType  TransStack
                                            , TransSetType ChkTransList)
      ->  bool  {
        /// Deadlock exists if any of LockingTrans is the waitingTrans
        /// need return true; True - yes deadlock
        if (WaitingTrans == LockingTrans) {
          /// Print out deadlock
          std::string Circle(std::to_string(LockingTrans)); 
          while (!TransStack.empty()) {
          
            auto TX = TransStack.top();
            TransStack.pop();
            Circle.append("<--" + std::to_string(TX));
          }
          Circle.append("<--" + std::to_string(WaitingTrans));
          return false;
        }
        /// check further if there is deadlock
        ChkTransList.insert(LockingTrans);
        /// Each transaction waits for one lock only
        auto LockPtr  = WaitMap.find(LockingTrans);
        if (LockPtr == WaitMap.end())
          return true;
        auto TxList  =  ResrMap.find((*LockPtr).second);
        /// This lock is available
        if (TxList == ResrMap.end()) 
          return true;

        auto it_end = (*TxList).second.begin();
        for (auto it  = (*TxList).second.begin(); it!= it_end; it++) {
          /// This transaction has NOT been checked
          if (ChkTransList.find(it->first) == ChkTransList.end()) {
            TransStack.push(it->first);
            checkWaitOnRecursive(WaitingTrans, it->first, TransStack, ChkTransList);
            TransStack.pop();
          }//if
        }
      }

    bool LocksManager::getVertexLock(IdType VId, MutexType Mutex, LockType Lock, IdType TxId)
//      ->  bool  {
    {
        LockRetPairType getLock = LocksManager::requireVertexLock(VId, Mutex, Lock);
        
        std::lock_guard<std::mutex> DlLock(*DeadlockDetector);
        /// If lock available, assign lock to TxId, register to maps(Trans, Lock)
        if (getLock.first) {
          /// TODO separate getting lock from getVertexLock
          LocksManager::registerToMap(TxId, getLock.second, Lock);
          return true;
        }
        switch (LocksManager::checkWaitOn(TxId, getLock.second, Lock))  {
          /// If there is no potential deadlock, wait for it
          case T_Wait:
            while (!LocksManager::getVertexLock(VId, Mutex, Lock));
            return true;
          case T_Abort:
            LocksManager::releaseAll(TxId);
            return false;
          case T_Ignore:
            return true;
            /// Release SH Lock on data and then request EX Lock
            /// If lock can be acquired, return true
            /// else return false and then restart
          case T_Upgrade:
            return LocksManager::upgradeLock(TxId, getLock.second);
      }//switch
  }

  auto LocksManager::getEdgeLock(IdType EId, MutexType Mutex, LockType Lock, IdType TxId)
    ->  bool  {
      LockRetPairType  getLock = LocksManager::requireEdgeLock(EId, Mutex, Lock);

      std::lock_guard<std::mutex> DlLock(*DeadlockDetector);
      /// If this lock is available, assign lock to TxId, register to maps(Trans, Lock)
      if (getLock.first) {
        /// TODO separate getting lock from getVertexLock
        LocksManager::registerToMap(TxId, getLock.second, Lock);
        return true;
      }
      switch (LocksManager::checkWaitOn(TxId, getLock.second, Lock))  {
        /// If there is no potential deadlock, wait for it
        case T_Wait:
          while (!(LocksManager::getEdgeLock(EId, Mutex, Lock)));
          return true;
        case T_Abort:
          LocksManager::releaseAll(TxId);
          return false;
        case T_Ignore:
          return true;
        case T_Upgrade:
          if (LocksManager::upgradeLock(TxId, getLock.second)) return true;
          LocksManager::releaseAll(TxId);
          return false;
      }
    }

  auto LocksManager::registerWaitingMap(IdType TxId, LockPointer LockPtr)
    -> bool {
      auto it = WaitMap.find(TxId);
      if (it != WaitMap.end())  {
        std::cerr << "Transaction " << TxId << " is busy\n";
        return false; 
      }

      WaitMap.insert(std::make_pair(TxId, LockPtr));
      return true;
  }

  auto LocksManager::registerTransMap(IdType TransId, LockPointer LockPtr)
    ->  bool  {
      auto it = TransMap.find(TransId);
      /// If current transaction never registers itself with any lock
      if (it == TransMap.end()) {
        LockListType LockList{LockPtr}; 
        TransMap.insert(std::make_pair(TransId, LockList));
        return true;
      }
      /// If current lock exists in list 
      if ((it->second).find(LockPtr) != (it->second).end()) {
        /// TODO do something ?
        /// So far there is no need to update transMap, which means no lock 
        /// Update ResrMap only 
        return true;
      }

      (it->second).insert(LockPtr);
      return true;
  }

  auto LocksManager::registerLockMap(IdType TransId, LockPointer LockPtr, LockType LType)
    ->  bool  {
      auto it = ResrMap.find(LockPtr);
      if (it == ResrMap.end())  {
        TransMapType TxMap{{TransId, LType}};
        ResrMap.insert(std::make_pair(LockPtr, TxMap)); 
        return true;
      }
      auto TxRec  = (it->second).find(TransId);
      /// If this transaction never registers itself with this lock, register
      if (TxRec == (it->second).end()) {
        (it->second).insert(std::make_pair(TransId, LType));
        return true;
      }
      /// If this transaction already holds the lock with same LockType
      ///   Ignore it
      if ((TxRec->second == T_SH) && (LType == T_EX)) {
        TxRec->second = T_EX;
//          return true;
      }
      return true;
  }

  auto LocksManager::registerToMap(IdType TransId, LockPointer LockPtr, LockType LType)
    ->  bool  {
      return LocksManager::registerTransMap(TransId, LockPtr) && LocksManager::registerLockMap(TransId, LockPtr, LType);

  }

  auto LocksManager::releaseAll(IdType TxId)
    ->  bool  {
    auto AcqLocks  = TransMap.find(TxId);
    auto WaitLocks  = WaitMap.find(TxId);
    /// Remove entry <TxId, Lock> in WaitMap if it exists
    if (WaitLocks != WaitMap.end()) {
      WaitMap.erase(TxId);
    }
    /// This transaction does NOT have any lock
    if (AcqLocks == TransMap.end())  {
      return true;
    }

    /// AcqLocks is a set
    /// it is a iterator to this set
    auto it_end = (AcqLocks->second).end();
    for (auto it = (AcqLocks->second).begin(); it!= it_end; it++) {
      /// Remove entries from ResrMap one by one
      /// If this lock is not found in ResrMap, there is an error
      auto TransRec = ResrMap.find(*it);
      if (TransRec == ResrMap.end()) {
        std::cerr << "Error: Lock not found\n";
        return false;
        /// TODO shoud be exit?
      }
      /// If this transaction is not found with this lock, there is an error
      auto Tx = (TransRec->second).find(TxId);
      if (Tx  == (TransRec->second).end()) {
        std::cerr << "Error: Transaction " << TxId << " not found \n"; 
        return false;
      }
      (TransRec->second).erase(TxId);
    }
    /// Now remove records of TxId from TransMap
    TransMap.erase(TxId);
    return true;
  }

  auto LocksManager::upgradeLock(IdType TransId, LockPointer LockPtr)
      ->  bool  {
       /// unlock sh(), try_ex(), update table
       /// return false if exclusive lock cannot be acquired
        LockPtr->unlock_shared();
        auto getEXLock = LockPtr->try_lock();
        if (!getEXLock) return false;

        auto TxMap = ResrMap.find(LockPtr);
#if _DEBUG_ENABLE_
        if (TxMap == ResrMap.end()) {
          std::cerr << "Error: lock not found in Resource Map\n";
          exit(0);
        }
#endif  
        auto TxRec = (TxMap->second).find(TransId);
        TxRec->second = T_EX;
        return true;
      }
#else
  ///locks are encoded in Vertex and Edge
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
      case T_Property:
        MutexPtr = Vertex->getVertexLock()->getPpMutex();
        break;
      case T_LastEdge:
        MutexPtr = Vertex->getVertexLock()->getLEMutex();
        break;
      case T_NextEdge:
        MutexPtr = Vertex->getVertexLock()->getNEMutex();
        break;
      case T_ID:
        MutexPtr = Vertex->getVertexLock()->getIdMutex();
        break;
      default:
        std::cerr << "ERROR: No such Mutex in VertexLock\n";
        exit(0);
    }
    switch (Lock) {   
      ///Shared lock
      case T_SH:
        return MutexPtr->try_lock_shared();
      case T_EX:
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
      case T_Property:
        MutexPtr = Vertex->getVertexLock()->getPpMutex();
        break;
      case T_LastEdge:
        MutexPtr = Vertex->getVertexLock()->getLEMutex();
        break;
      case T_NextEdge:
        MutexPtr = Vertex->getVertexLock()->getNEMutex();
        break;
      case T_ID:
        MutexPtr = Vertex->getVertexLock()->getIdMutex();
        break;
      default:
        std::cerr << "ERROR: No such Mutex in VertexLock\n";
        exit(0);
    }

    switch (Lock) {   
      ///Shared lock
      case T_SH:
        MutexPtr->unlock_shared();
        break;
      case T_EX:
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
      case T_ID:
        MutexPtr = Edge->getEdgeLock()->getIdMutex();
        break;
      case T_Property:
        MutexPtr = Edge->getEdgeLock()->getPpMutex();
        break;
      case T_FirstVertex:
        MutexPtr = Edge->getEdgeLock()->getFVMutex();
        break;
      case T_SecondVertex:
        MutexPtr = Edge->getEdgeLock()->getSVMutex();
        break;
      case T_FirstNextEdge:
        MutexPtr = Edge->getEdgeLock()->getFNEMutex();
        break;
      case T_FirstPrevEdge:
        MutexPtr = Edge->getEdgeLock()->getFPEMutex();
        break;
      case T_SecondNextEdge:
        MutexPtr = Edge->getEdgeLock()->getSNEMutex();
        break;
      case T_SecondPrevEdge:
        MutexPtr = Edge->getEdgeLock()->getSPEMutex();
        break;
      default:
        std::cerr << "ERROR: No such Mutex in EdgeLock\n";
        exit(0);
    }
    switch (Lock) {   
      case T_SH:
        return MutexPtr->try_lock_shared();
      case T_EX:
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
      case T_ID:
        MutexPtr = Edge->getEdgeLock()->getIdMutex();
        break;
      case T_Property:
        MutexPtr = Edge->getEdgeLock()->getPpMutex();
        break;
      case T_FirstVertex:
        MutexPtr = Edge->getEdgeLock()->getFVMutex();
        break;
      case T_SecondVertex:
        MutexPtr = Edge->getEdgeLock()->getSVMutex();
        break;
      case T_FirstNextEdge:
        MutexPtr = Edge->getEdgeLock()->getFNEMutex();
        break;
      case T_FirstPrevEdge:
        MutexPtr = Edge->getEdgeLock()->getFPEMutex();
        break;
      case T_SecondNextEdge:
        MutexPtr = Edge->getEdgeLock()->getSNEMutex();
        break;
      case T_SecondPrevEdge:
        MutexPtr = Edge->getEdgeLock()->getSPEMutex();
        break;
      default:
        std::cerr << "ERROR: No such Mutex in EdgeLock\n";
        exit(0);
    }

    switch (Lock) {   
      case T_SH:
        MutexPtr->unlock_shared();
        break;
      case T_EX:
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


  auto LocksManager::addToVertexLockMap(IdType VertexId) 
    -> void  {
      VertexPtr Vertex = Graph.getVertexPointer(VertexId);
      if (Vertex == nullptr) {
        std::cerr  << "Error : No such vertex " << VertexId <<" in map \n";
        exit(0);
      }

      VertexLock* NewVertexLock = new VertexLock();
      Vertex->setVertexLock(NewVertexLock);
    }

  auto LocksManager::addToEdgeLockMap(IdType EdgeId) 
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
