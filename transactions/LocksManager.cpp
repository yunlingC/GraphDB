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

  auto LocksManager::getVertexLockPointer(IdType VertexId, MutexType Mutex) 
    -> MutexPointer {
#if _DEBUG_ENABLE_
      if (VertexLockMap.find(VertexId) == VertexLockMap.end()) {
        std::cerr  << "Error : No such vertex " << VertexId <<" in map \n";
        exit(0);
      }
#endif
      switch (Mutex) {
        case T_Property:
          return VertexLockMap[VertexId].getPpMutex();
        case T_LastEdge:
          return VertexLockMap[VertexId].getLEMutex();
        case T_NextEdge:
          return VertexLockMap[VertexId].getNEMutex();
        case T_ID:
          return VertexLockMap[VertexId].getIdMutex();
        case T_Label:
          return VertexLockMap[VertexId].getLbMutex();
        default:
          // assert(false);
          std::cerr << "ERROR: No such Mutex in VertexLock\n";
          exit(0);
      }
    }
 
  auto LocksManager::getEdgeLockPointer(IdType EdgeId, MutexType Mutex) 
    ->  MutexPointer {
#if _DEBUG_ENABLE_
      if (EdgeLockMap.find(EdgeId) == EdgeLockMap.end()) {
        std::cerr << "Error : No such edge" << EdgeId <<" in map \n";
        exit(0);
      }
#endif
      switch (Mutex) {
        case T_ID:
          return EdgeLockMap[EdgeId].getIdMutex();
        case T_Property:
          return EdgeLockMap[EdgeId].getPpMutex();
        case T_FirstVertex:
          return EdgeLockMap[EdgeId].getFVMutex();
        case T_SecondVertex:
          return EdgeLockMap[EdgeId].getSVMutex();
        case T_FirstNextEdge:
          return EdgeLockMap[EdgeId].getFNEMutex();
        case T_FirstPrevEdge:
          return EdgeLockMap[EdgeId].getFPEMutex();
        case T_SecondNextEdge:
          return EdgeLockMap[EdgeId].getSNEMutex();
        case T_SecondPrevEdge:
          return EdgeLockMap[EdgeId].getSPEMutex();
        case T_Label:
          return EdgeLockMap[EdgeId].getLbMutex();
#if _DEBUG_ENABLE_
          // assert("ERROR: No such Mutex in EdgeLock" && false);
        default:
          std::cerr << "ERROR: No such Mutex in EdgeLock\n";
          exit(0);
#endif
      }
  }

  auto LocksManager::tryLock(MutexPointer MutexPtr, LockType LType)
    -> bool {
#if _DEBUG_ENABLE_
      // assert(MutexPtr != nullptr);
#endif
      switch (LType) {   
        ///Shared lock
        case T_SH:
          return MutexPtr->try_lock_shared();
        ///Exclusive lock
        case T_EX:
          return MutexPtr->try_lock(); 
#if _DEBUG_ENABLE_
        default:
          std::cerr << "ERROR: No such Mutex in a lock\n";
          exit(0);
#endif
      }
    }

  auto LocksManager::tryUnlock(MutexPointer MutexPtr, LockType LType)
    -> void {
      switch (LType) {   
        case T_SH:
          return MutexPtr->unlock_shared();
        case T_EX:
          return MutexPtr->unlock(); 
#if _DEBUG_ENABLE_
        default:
          std::cerr << "ERROR: No such Mutex in VertexLock\n";
          exit(0);
#endif
      }
    }

#ifndef _LOCKING_STORAGE_
  LocksManager::LocksManager() {
#ifdef _DEADLOCK_DETECTION_
    DeadlockDetector = std::shared_ptr<std::mutex>(new std::mutex) ;
#endif
  }

  auto LocksManager::getVertexLock(IdType VertexId, MutexType Mutex, LockType Lock) 
    -> bool {
      auto MutexPtr = getVertexLockPointer(VertexId, Mutex);
      return tryLock(MutexPtr, Lock);
  }

  auto LocksManager::releaseVertexLock(IdType VertexId, MutexType Mutex, LockType Lock) 
    -> void {
      auto MutexPtr = getVertexLockPointer(VertexId, Mutex);
      return tryUnlock(MutexPtr, Lock);
  }


  auto LocksManager::getEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock) 
    -> bool {
      auto MutexPtr = getEdgeLockPointer(EdgeId, Mutex);
      return tryLock(MutexPtr, Lock);
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
    -> void {
      auto MutexPtr = getEdgeLockPointer(EdgeId, Mutex);
      return tryUnlock(MutexPtr, Lock);
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
    }

    for (auto it = EdgeMap.begin(), it_end = EdgeMap.end();
        it != it_end; it++) {
      EdgeLock* NewEdgeLock = new EdgeLock();
      EdgeLockMap.insert(ELockPair((*it).first, *NewEdgeLock));
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
      auto MutexPtr = getVertexLockPointer(VertexId, Mutex);
      auto RetValue  = tryLock(MutexPtr, Lock);
      return LockRetPairType(RetValue, MutexPtr);
    }

  auto LocksManager::requireEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock) 
    -> LockRetPairType  {
      auto MutexPtr = getEdgeLockPointer(EdgeId, Mutex);
      auto RetValue  = tryLock(MutexPtr, Lock);
      return LockRetPairType(RetValue, MutexPtr);
    }

#ifdef _DEADLOCK_DETECTION_
/// Need to figure out all circles 
/// TODO need to chect transaction status before aborting any transaction
  auto LocksManager::detectDeadlock()
    -> bool {
      /* WaitMap
       * TxId --  LockPointer
       * 0    --  L1
       * 2    --  L2
       * */
//      unsigned int DeadlockCount = 0;
      /// TODO problem here? what if WaitMap.end() changes? It will never end...
      /// TODO work till here
      for (auto it = WaitMap.begin(), it_end = WaitMap.end(); 
            it != it_end; it ++) {
        
      }
  }
#endif /*_DEADLOCK_DETECTION_*/

#ifdef _DEADLOCK_DETECTION_
  auto LocksManager::checkWaitOn(IdType TransId, LockPointer LockPtr, LockType LType) 
    ->  LockRequestRetType {
      /// If current trans is waiting for some other lock, then give up
      /// Usually it won't happen because this trans must be spining on that lock
      /// TODO to be deleted
#if _DEBUG_ENABLE_
      if (WaitMap.find(TransId) != WaitMap.end()) {
        std::cerr << "Error : Transaction " << TransId << " is waiting for a lock\n";
        return T_Abort;
      }
#endif
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
      for (auto iter = (*TxList).second.begin(), iter_end = (*TxList).second.end();
           iter != iter_end; iter++) {
        /// Check if current transaction already holds this lock 
        if ((*iter).first == TransId) {
          /*
          There can be 4 cases when one transaction holds a lock 
          and requests it again
          <r, r>, <r, w>, <w, r>, <w, w>
          Since TxList is a set, we can rule out <r, r>
            S1 <r, w> 
                if not r'ed by others
                    =>  upgrade r -> w
                otherwise
                    => abort
            S2 <w, r> ignore
            S3 <w, w> ignore
          */
          /// might come into this situation
          /// T1 has lock <L1, SH> and requests <L1, EX> with sucess 
          /// BUT without registering!
          /// This is possible because in STL thread with shared_timed_mutex:
          /// Undefined behavior might happen when one thread requests
          /// the same mutex again
          /// Solved with upgradeLock()
          switch ((*iter).second) {
            /// <w, *>
            case  T_EX:
              return T_Ignore;
            case  T_SH:
              /// <r, w>
              if (LType == T_EX) {
                /// No other transactions are holding this lock
                return (*TxList).second.size() == 1 ? T_Upgrade : T_Abort;
              }
#if _DEBUG_ENABLE_
              else {
                std::cerr <<"Error : Transaction " << TransId << " requests the same shared lock again, ptr\t" << LockPtr << "\n";
                  exit(0);
              }
#endif
          }// switch
        }// if 

        TransStack.push((*iter).first);
        if (!checkWaitOnRecursive(TransId, (*iter).first, TransStack, CheckedTransList)) {
          /// checkWaitOnRecursive returns false in case of deadlock
          //TransStack.pop();
          return T_Abort;
        } //if 
        TransStack.pop();
      } //for
      
      /// If there is no deadlock, wait for this lock
      registerWaitingMap(TransId, LockPtr);
      return T_Wait;
    }
#endif 

#ifdef _DEADLOCK_DETECTION_
/// TODO problem here
/// If T_Wait is returned, lock for checkWaitOn() can be released
/// If T_Abort is returned, lock should be held
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
        /// TODO delete this?
        ChkTransList.insert(LockingTrans);
        /// Each transaction waits for one lock only
        auto LockPtr  = WaitMap.find(LockingTrans);
        if (LockPtr == WaitMap.end())
          return true;
        auto TxList  =  ResrMap.find(LockPtr->second);
        /// This lock is available
        if (TxList == ResrMap.end()) 
          return true;

        auto it_end = (*TxList).second.begin();
        for (auto it  = (*TxList).second.begin(); it!= it_end; it++) {
          /// This transaction has NOT been checked
          if (ChkTransList.find(it->first) == ChkTransList.end()) {
            TransStack.push(it->first);
            auto WaitOn = checkWaitOnRecursive(WaitingTrans, it->first, TransStack, ChkTransList);
            TransStack.pop();
            /// If a deadlock is found, return cotrol  
            if (!WaitOn)  {
              return false;
            }
          }//if
        }
        return true;
      }
#endif

#ifdef _DEADLOCK_DETECTION_
    bool LocksManager::getVertexLock(IdType VId, MutexType Mutex, LockType Lock, IdType TxId)
    {
        auto MutexPtr = getVertexLockPointer(VId, Mutex);
        DeadlockDetector->lock();
        registerLockMap(TxId, MutexPtr, Lock);
        auto getLock = tryLock(MutexPtr, Lock);

        if (getLock) {
          registerTransMap(TxId, MutexPtr);
          DeadlockDetector->unlock();
          return true;
        }

        retireFromLockMap(TxId, MutexPtr, Lock);
        registerWaitingMap(TxId, MutexPtr);

        std::cout <<"Transaction " << TxId << " cannot get lock " << VId << " Type " << Lock << "\n";
        switch (LocksManager::checkWaitOn(TxId, MutexPtr, Lock))  {
          /// If there is no potential deadlock, wait for it
          case T_Wait:
            /// Release lock
            while (!LocksManager::getVertexLock(VId, Mutex, Lock));
            /// Acquire lock again for registration
            LocksManager::registerToMap(TxId, MutexPtr, Lock);
            DeadlockDetector->unlock();
            return true;
          case T_Abort:
            LocksManager::releaseAll(TxId);
            DeadlockDetector->unlock();
            return false;
          case T_Ignore:
            DeadlockDetector->unlock();
            return true;
            /// Release SH Lock on data and then request EX Lock
            /// If lock can be acquired, return true
            /// else return false and then restart
          case T_Upgrade:
            auto isUpgraded = upgradeLock(TxId, MutexPtr);
            DeadlockDetector->unlock();
            return isUpgraded;
      }//switch
  }

  auto LocksManager::getEdgeLock(IdType EId, MutexType Mutex, LockType Lock, IdType TxId)
    ->  bool  {
      auto MutexPtr = getEdgeLockPointer(EId, Mutex);
      DeadlockDetector->lock();
      registerLockMap(TxId, MutexPtr, Lock);
      auto getLock =  tryLock(MutexPtr, Lock);
      if (getLock) {
        registerTransMap(TxId, MutexPtr);
        DeadlockDetector->unlock();
        return true;
      }
      retireFromLockMap(TxId, MutexPtr, Lock);
      registerWaitingMap(TxId, MutexPtr);

      /**
      std::lock_guard<std::mutex> DlLock(*DeadlockDetector);
      /// If this lock is available, assign lock to TxId, register to maps(Trans, Lock)
      if (getLock.first) {
        /// TODO separate getting lock from getVertexLock
        LocksManager::registerToMap(TxId, getLock.second, Lock);
        return true;
      }
      */
      std::cout <<"Transaction " << TxId << " cannot get lock " << EId 
                  << " Ptr " << MutexPtr << " Type " << Lock << "\n";

      switch (LocksManager::checkWaitOn(TxId, MutexPtr, Lock))  {
        /// If there is no potential deadlock, wait for it
        case T_Wait:
          while (!(LocksManager::getEdgeLock(EId, Mutex, Lock)));
          LocksManager::registerToMap(TxId, MutexPtr, Lock);
            DeadlockDetector->unlock();
          return true;
        case T_Abort:
          LocksManager::releaseAll(TxId);
            DeadlockDetector->unlock();
          return false;
        case T_Ignore:
            DeadlockDetector->unlock();
          return true;
        case T_Upgrade:
          if (LocksManager::upgradeLock(TxId, MutexPtr)) return true;
          LocksManager::releaseAll(TxId);
            DeadlockDetector->unlock();
          return false;
      }
    }
#endif /*_DEADLOCK_DETECTION_*/

///#ifdef _DEADLOCK_DETECTION_
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
#ifdef _DEBUG_PRINT_
//      std::cout << "Transaction " << TransId << " registers lock " << LockPtr << "\n";
//      fflush(stdout);
#endif
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

  auto LocksManager::retireFromWaitingMap(IdType TxId, LockPointer LockPtr)
    -> bool {
      /// TODO to be done
      auto it = WaitMap.find(TxId);
#if _DEBUG_ENABLE_
      if (it == WaitMap.end()) {
        std::cout <<"Error : Transaction " << TxId << " is not waiting for any lock\n";
        return false;
      }
      if (it->second != LockPtr)  {
        std::cout <<"Error : Transaction " << TxId << " is waiting for " << it->second << " not " << LockPtr <<"\n";
        return false;
      }
#endif
      WaitMap.erase(it);
      return true;
  }

  auto LocksManager::retireFromTransMap(IdType TxId, LockPointer LockPtr)
    -> bool {
    auto it = TransMap.find(TxId); 
#if _DEBUG_ENABLE_
    if (it == TransMap.end()) {
      std::cout <<"Error : Transaction " << TxId << " is not holding any lock\n";
      return false;
    }
    /// it->second is a set holding all lock pointers
    auto iter = it->second.find(LockPtr);
    if (iter == it->second.end()) {
      std::cout <<"Error : Transaction " << TxId << " does NOT have this lock\n";
      return false;
    }
#endif
    it->second.erase(LockPtr);
    return true;
  }

  auto LocksManager::retireFromLockMap(IdType TxId, LockPointer LockPtr, LockType LType)
    -> bool {
    auto it = ResrMap.find(LockPtr);
#if _DEBUG_ENABLE_
    if (it == ResrMap.end())  {
      std::cout <<"Error : Lock " << LockPtr << " is not holding by any transaction\n";
      return false;
    }
    auto iter = it->second.find(TxId);
    if (iter == it->second.end()) {
      std::cout <<"Error : Lock " << LockPtr << " is not holding by transaction " << TxId << "\n";
      return false;
    }
    if ((*iter).second != LType) {
      std::cout <<"Error : Lock " << LockPtr << " is not holding by transaction " << TxId << " with " << LType <<"\n";
      return false;
    }
#endif 
    /// Remove the entry created by TxId
    it->second.erase(TxId);
    return true;
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

#ifdef _DEBUG_PRINT_
  void LocksManager::dumpMaps() {
    std::cout << "** Dump lock information **\n";
    dumpTransMap();
    dumpResrMap();
  }

  void LocksManager::dumpTransMap()  {
    std::cout <<"+++ TransMap +++ \n";
    for (auto it = TransMap.begin(), it_end = TransMap.end();
         it != it_end; it++)  {
      std::cout <<"TxId\t" << it->first << "\tLock number\t" << it->second.size() <<"\n";
      for (auto iter = it->second.begin(), iter_end = it->second.end();
          iter != iter_end; iter++) {
        std::cout << *iter  <<"\t";
      }
    }

  }

  void LocksManager::dumpResrMap()  {
    std::cout <<"+++ LockMap +++ \n";
    for (auto it = ResrMap.begin(), it_end = ResrMap.end();
         it != it_end; it++)  {
      std::cout <<"Lock\t" << it->first << "\tTrans number\t" << it->second.size() <<"\n";
      for (auto iter = it->second.begin(), iter_end = it->second.end();
          iter != iter_end; iter++) {
        std::cout << (*iter).first  <<"\t" << (*iter).second << "\n";
      }
    }
  }
#endif /*_DEBUG_PRINT_*/
//#endif /*_DEADLOCK_DETECTION_*/

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
