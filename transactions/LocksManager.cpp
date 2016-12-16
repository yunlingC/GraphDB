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
#include "global.h"

#include <stack>
#include <string>


#if _DEBUG_ENABLE_
#include <iostream>
#endif
  void  LocksManager::lockVertex()  {
    VertexProtector->lock();
  }

  void LocksManager::unlockVertex()  {
    VertexProtector->unlock();
  }

  void LocksManager::lockEdge()  {
    EdgeProtector->lock(); 
  }

  void LocksManager::unlockEdge() {
    EdgeProtector->unlock();
  }

#ifndef _LOCKING_STORAGE_

  LocksManager::LocksManager() {
    VertexProtector = std::shared_ptr<std::mutex>(new std::mutex);
    EdgeProtector = std::shared_ptr<std::mutex>(new std::mutex);
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
  

  auto LocksManager::releaseEdgeAll(ELockListType & EdgeLocks) 
    -> void {
    for (auto it = EdgeLocks.begin(), itend = EdgeLocks.end(); 
          it != itend; ++it) {
      releaseEdgeLock((*it).first->getId(), (*it).second.first, (*it).second.second);
    }
  }

  auto LocksManager::releaseVertexAll(VLockListType & VertexLocks) 
    -> void {
    for (auto it = VertexLocks.begin(), itend = VertexLocks.end(); 
        it != itend; ++it) {
      releaseVertexLock((*it).first->getId(), 
          (*it).second.first, (*it).second.second);
    }
  }

  auto LocksManager::releaseAll(VLockListType & VertexLocks, ELockListType & EdgeLocks) 
    -> void {
      releaseVertexAll(VertexLocks);
      releaseEdgeAll(EdgeLocks);
  }

  auto LocksManager::releaseEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock) 
    -> void {
      auto MutexPtr = getEdgeLockPointer(EdgeId, Mutex);
      return tryUnlock(MutexPtr, Lock);
    }

  auto LocksManager::addToVertexLockMap(IdType VertexId) 
    -> void  {
      VertexLock *NewVertex = new VertexLock();
      VertexLockMap.insert(VLockPair(VertexId, *NewVertex));
#if _DEBUG_PRINT_
      std::cout << "Add vertex lock\t" <<  VertexId
              <<"\t to map\n";
#endif
  }

  auto LocksManager::addToEdgeLockMap(IdType EdgeId) 
    -> void  {
      EdgeLock *NewEdge = new EdgeLock();
      EdgeLockMap.insert(ELockPair(EdgeId, *NewEdge)); 
#if _DEBUG_PRINT_
      std::cout << "Add edge lock\t" << EdgeId
              <<"\t to map\n";
#endif
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
        case T_NextEdge:
          return VertexLockMap[VertexId].getNEMutex();
        case T_ID:
          return VertexLockMap[VertexId].getIdMutex();
        case T_Label:
          return VertexLockMap[VertexId].getLbMutex();
        default:
          // assert(false);
          std::cerr << "ERROR: No such Mutex Type\t" << Mutex << "\tin VertexLock\n";
          exit(0);
      }
    }
 
  auto LocksManager::getEdgeLockPointer(IdType EdgeId, MutexType Mutex) 
    ->  MutexPointer {
#if _DEBUG_ENABLE_
      if (EdgeLockMap.find(EdgeId) == EdgeLockMap.end()) {
        std::cerr << "Error : No such edge\t" << EdgeId <<"\tin map when getting pointer\n";
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
      if (MutexPtr == nullptr) {
        std::cout <<"Error in tryLock: MutexPointer not valid\n";
      }
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
          std::cerr << "ERROR: No such Lock Type\t" << LType << "\tin VertexLock\n";
          exit(0);
#endif
      }
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
      return false;
  }

  auto LocksManager::checkWaitOn(IdType TransId, LockPointer LockPtr, LockType LType) 
    ->  LockRequestRetType {
      /// If current trans is waiting for some other lock, then give up
      /// Usually it won't happen because this trans must be spining on that lock
      /// TODO to be deleted
#ifndef _WAIT_DIE_  /*Deadlock detection only*/

#if _DEBUG_ENABLE_
      if (WaitMap.find(TransId) != WaitMap.end()) {
        std::cerr << "Error : Transaction " << TransId << " is waiting for a lock\n";
        return T_Abort;
      }
#endif
      DeadlockDetector->lock();
#endif

      TransStackType  TransStack;
      TransSetType CheckedTransList;
      LockGuardSetType GuardSet;
      auto Guard = ResrGuardMap.find(LockPtr);

#if _DEBUG_ENABLE_
      if (Guard == ResrGuardMap.end()) {
        std::cout << "Transaction\t" << TransId
                  << "\tcould not get ResrGuard when checkWaitON on\t" << LockPtr
                  << "\n";
        return T_Abort;
      }
#endif /*DEBUG_ENABLE*/

      while (!Guard->second->try_lock() ) {
#if _DEBUG_PRINT_
        std::cout << "Transaction\t" << TransId
                  << "\tspins on ResrGuard on\t" << LockPtr
                  << "\n";
//        if (tryLock(LockPtr, LType))
#endif /*DEBUG_PRINT*/
      }

      GuardSet.insert(Guard->second);

      auto TxList = ResrMap.find(LockPtr);

      if (TxList == ResrMap.end()) {
#if _DEBUG_PRINT_
        std::cout << "Error : Lock is not available but no record found\n";
#endif
        dismissGuard(GuardSet);
        return T_Wait;
      }

      for (auto iter = (*TxList).second.begin(), iter_end = (*TxList).second.end();
           iter != iter_end; iter++) {
#if _DEBUG_PRINT_
//        std::cout << "Transaction\t" << (*iter).first 
//                  << "\t holds this lock\t"<< LockPtr
//                  << "\n";
#endif
#ifdef _WAIT_DIE_
        ///Just check Transaction Id, which indicates priority
        if ((*iter).first == TransId) {
          dismissGuard(GuardSet);
          return T_Ignore;
        }
        /// Larger Id, lower priority
        else if (iter->first < TransId)  {
          dismissGuard(GuardSet);
          return T_Abort;
        }
#else /*DEADLOCK_DETECTION*/

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
/*
          if (iter->second.size()>1)  {
            std::cout << "Error: Transaction\t" << TransId 
                      << "\tholds both SH and EX locks on\t" << LockPtr
                      << "\t";
            return T_Ignore;
///            LockPtr->unlock()
          }
          auto Locktype = iter->second.begin();
          switch (*Locktype) {
            /// <w, *>
            case  T_EX:
              std::cout << "Ignore\n";
              return T_Ignore;
            case  T_SH:
              /// <r, w>
              if (LType == T_EX) {
                /// No other transactions are holding this lock

                bool retValue = ((*TxList).second.size() == 1) ? true : false;
                if (retValue) 
                  std::cout << "Upgrade\n";
                else 
                  std::cout << "Abort\n";

                return (*TxList).second.size() == 1 ? T_Upgrade : T_Abort;
              }
#if _DEBUG_ENABLE_
              else {
                std::cerr <<"Error : Transaction\t" << TransId 
                          << "\trequests the same shared lock again, ptr\t" 
                          << LockPtr << "\n";
                return T_Ignore;
//                  exit(0);
              }
#endif
          }// switch
*/
          dismissGuard(GuardSet);
          DeadlockDetector->unlock();
          return  T_Ignore;
        }// if 

        TransStack.push((*iter).first);
        if (!checkWaitOnRecursive(TransId, (*iter).first, TransStack, CheckedTransList, GuardSet)) {
          /// checkWaitOnRecursive returns false in case of deadlock
          //TransStack.pop();
#if _DEBUG_PRINT_
          std::cout <<"CheckWaitOnRecursive: Abort\n";
#endif
          dismissGuard(GuardSet);
          DeadlockDetector->unlock();
          return T_Abort;
        } //if 
        TransStack.pop();
#endif /*DEADLOCK_DETECTION*/
      } //for
      
      /// If there is no deadlock, wait for this lock
#if _DEBUG_PRINT_
      std::cout <<"CheckWaitOnRecursive: Wait\n";
#endif
      dismissGuard(GuardSet);
#ifndef _WAIT_DIE_  /*Deadlock detection only*/
      DeadlockDetector->unlock();
#endif
      return T_Wait;
    }

    auto LocksManager::checkWaitOnRecursive(IdType WaitingTrans
                                            , IdType LockingTrans
                                            , TransStackType  TransStack
                                            , TransSetType ChkTransList
                                            , LockGuardSetType GuardSet)
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

#if _DEBUG_PRINT_
        std::cout << Circle << "\n";
#endif
        return false;
      }
      /// check further if there is deadlock
      ChkTransList.insert(LockingTrans);
      /// Each transaction waits for one lock only
      
      bool missGuard = false;
      if (WaitGuardMap.find(LockingTrans) == WaitGuardMap.end()) {
#if _DEBUG_ENABLE_
        std::cout << "Transaction\t" << LockingTrans
                  << "\t encounters Lock guard missing in WaitMap when checkWaitOnRecursive"
                  << "\n";
        missGuard = true;
#endif
      }
      
      if (!missGuard) {
        while (!WaitGuardMap[LockingTrans]->try_lock()) {
#if _DEBUG_PRINT_
          std::cout << "Transaction\t" << LockingTrans
                    << "\tspin on lock_guard to checkWaitOnRecursive"
                    << "\tfrom wait map\n";
#endif
        }
        GuardSet.insert(WaitGuardMap[LockingTrans]);
      }

      auto LockPtr  = WaitMap.find(LockingTrans);
      if (LockPtr == WaitMap.end()) return true;

      bool missResrGuard = false;
      auto Guard = ResrGuardMap.find(LockPtr->second);
      if (Guard == ResrGuardMap.end()) {
        missResrGuard = true;
#if _DEBUG_ENABLE_
        std::cout << "Lock\t" << LockPtr->second 
                  << "\t encounters Lock guard missing in LockMap when checkWaitOnRecursive"
                  << "\n";
#endif
      }
      if (!missResrGuard)  {
        while (!ResrGuardMap[LockPtr->second]->try_lock()) {
#if _DEBUG_PRINT_
        std::cout << "Lock\t" << LockPtr->second
                  << "\tspin on lock_guard to checkWaitOnRecursive\t"
                  << "\tfrom lock map\n";
#endif
        }
        GuardSet.insert(ResrGuardMap[LockPtr->second]);
      }

      auto TxList  =  ResrMap.find(LockPtr->second);
      /// This lock is available
      if (TxList == ResrMap.end()) return true;

      auto it_end = (*TxList).second.begin();
      for (auto it  = (*TxList).second.begin(); it!= it_end; it++) {
        /// This transaction has NOT been checked
        if (ChkTransList.find(it->first) == ChkTransList.end()) {
          TransStack.push(it->first);
          auto WaitOn = checkWaitOnRecursive(WaitingTrans, it->first, TransStack, ChkTransList, GuardSet);
          TransStack.pop();
//          ChkTransList.insert(LockingTrans);
          /// If a deadlock is found, return cotrol  
          if (!WaitOn)  {
            return false;
          }
        }//if
      }

      return false;
    }

  void LocksManager::dismissGuard(LockGuardSetType & Guards)  {
    for (auto guard : Guards) {
      guard->unlock(); 
    }
    return;
  }


#ifdef _NO_WAIT_
    bool LocksManager::getLock(MutexPointer MutexPtr
                              , LockType Lock
                              , IdType TxId
                              ){

      if (checkTransMap(TxId, MutexPtr, Lock))  
        return true;
      
      auto getLock = tryLock(MutexPtr, Lock);
      if (getLock)  {
        registerTransMap(TxId, MutexPtr, Lock);
        return true;
      }
      return false;
  }

#elif defined _WAIT_DIE_
    bool LocksManager::getLock(MutexPointer MutexPtr
                              , LockType Lock
                              , IdType TxId
                              ){

        if (checkTransMap(TxId, MutexPtr, Lock))  
          return true;

        auto getLock = tryLock(MutexPtr, Lock);
        if (getLock) {
          while(!registerLockMap(TxId, MutexPtr, Lock)) {
#if _DEBUG_PRINT_
//            std::cout << "WAIT_DIE: Transaction\t" << TxId
//                      << "\tspins to register lock\t" << MutexPtr
//                      << "\n";
#endif
          }

          registerTransMap(TxId, MutexPtr, Lock);
          return true;
        }

        switch (checkWaitOn(TxId, MutexPtr, Lock))  {
          /// If there is no potential deadlock, wait for it
          case T_Wait:  {
#if _DEBUG_PRINT_
            std::cout << "Transaction\t" << TxId
                      << "Waits for\t" << MutexPtr
                      << "\ton type\t" << Lock
                      << "\n";
#endif
            int trial = 10; 
            while (!tryLock(MutexPtr, Lock) && trial > 0) {
              trial--;
            }
            if (trial < 0) {
              std::cout << "Wait does not work. Abort\n";
              releaseAll(TxId);
              return false;
            }

            while (!registerLockMap(TxId, MutexPtr, Lock)) {
#if _DEBUG_PRINT_
              std::cout << "WAIT_DIE: Transaction\t" << TxId
                        << "\tspins to register lock\t" << MutexPtr
                        << "\n";
#endif
            }
            registerTransMap(TxId, MutexPtr, Lock);
            return true;
                        }
          case T_Abort:
            releaseAll(TxId);
            return false;
          case T_Ignore:
            return true;
            /// Release SH Lock on data and then request EX Lock
            /// If lock can be acquired, return true
            /// else return false and then restart
          case T_Upgrade:
            auto isUpgraded = upgradeLock(TxId, MutexPtr);
#ifdef  _DEBUG_PRINT_
            std::cout <<"Transaction\t" << TxId
                      << "\tupgrades on\t" << MutexPtr
                      << "\tand\t" << isUpgraded
                      << "\n";
#endif 
            return isUpgraded;
        }//switch
        return true;
  }

#else //DEADLOCK_DETECTION only
    bool LocksManager::getLock(MutexPointer MutexPtr
                              , LockType Lock
                              , IdType TxId
                              ){

        if (checkTransMap(TxId, MutexPtr, Lock))  
          return true;

        auto getLock = tryLock(MutexPtr, Lock);
        if (getLock) {
          if (registerLockMap(TxId, MutexPtr, Lock)) {
            registerTransMap(TxId, MutexPtr, Lock);
            return true;
          }
          else {
            /// Give up on locks if not registered to LockMap -- Deadlock detection
            tryUnlock(MutexPtr, Lock);
            return false;
          }
        }

    /// Couldn't get lock
    /// Check if waiting will cause potential deadlock

#if _DEBUG_PRINT_
//        std::cout <<"Transaction\t" << TxId 
//                  << "\tcannot get vertex lock " << VId 
//                  << "\tType " << Lock 
//                  << "\n";
#endif

        switch (checkWaitOn(TxId, MutexPtr, Lock))  {
          /// If there is no potential deadlock, wait for it
          case T_Wait:  {
            std::cout <<"Transction\t" << TxId
                      << "\twaits to register lock\t" << MutexPtr
                      << "\ton type\t" << Lock
                      << "\n";
            registerWaitingMap(TxId, MutexPtr);

#if _DEBUG_PRINT_
            std::cout << "Transaction\t" << TxId
                      << "Waits for\t" << MutexPtr
                      << "\n";
#endif
            int trial = 10; 
            while (!tryLock(MutexPtr, Lock) && trial > 0) {
              trial--;
            }
            if (trial < 0) {
              std::cout << "Wait does not work. Abort\n";
              releaseAll(TxId);
              return false;
            }

            /// TODO Problem with deadlock_detection 
            // still need to register
//            registerTransMap();
//            registerLockMap();
            return true;
                        }
          case T_Abort:
            LocksManager::releaseAll(TxId);
            return false;
          case T_Ignore:
            return true;
            /// Release SH Lock on data and then request EX Lock
            /// If lock can be acquired, return true
            /// else return false and then restart
          case T_Upgrade:
            auto isUpgraded = upgradeLock(TxId, MutexPtr);
#ifdef  _DEBUG_PRINT_
            std::cout <<"Transaction\t" << TxId
                      << "\tupgrades on\t" << MutexPtr
                      << "\tand\t" << isUpgraded
                      << "\n";
#endif 
            return isUpgraded;
        }//switch
        return true;
  }
#endif


  bool LocksManager::getVertexLock(IdType VId
                                  , MutexType Mutex
                                  , LockType Lock
                                  , IdType TxId
                                  ){
      auto MutexPtr = getVertexLockPointer(VId, Mutex);
      return getLock(MutexPtr, Lock, TxId);
  }

  bool LocksManager::getEdgeLock(IdType EId
                                , MutexType Mutex
                                , LockType Lock
                                , IdType TxId
                                ){
      auto MutexPtr = getEdgeLockPointer(EId, Mutex);
      return getLock(MutexPtr, Lock, TxId);
    }


  auto LocksManager::registerWaitingMap(IdType TxId, LockPointer LockPtr)
    -> bool {
      if (WaitGuardMap.find(TxId) == WaitGuardMap.end()) {
        ExMutexPointer NewGuard(new std::mutex);
        WaitGuardMap.insert(WaitGuardPairType(TxId, NewGuard));
      }
      auto Guard = WaitGuardMap[TxId];
      ///TODO A potential deadlock point
      ///In  theory there won't be deadlock existing
      while (!Guard->try_lock())  {
#if _DEBUG_PRINT_
        std::cout << "Transaction\t" << TxId 
                  << "\tspins on waiting map for lock\t" << LockPtr
                  << "\n";
#endif
//        return false;
      }

      auto it = WaitMap.find(TxId);
      /// If TxId is found waiting for some lock, an error happens
      if (it != WaitMap.end())  {
#if _DEBUG_PRINT_
        std::cout << "Transaction " << TxId << " is busy\n";
#endif
        Guard->unlock();
        return false; 
      }

      WaitMap.insert(std::make_pair(TxId, LockPtr));
      Guard->unlock();
      return true;
  }

  auto LocksManager::registerTransMap(IdType TransId
                                    , LockPointer LockPtr
                                    , LockType LType)
    ->  bool  {
#ifdef _DEBUG_PRINT_
//      std::cout << "Transaction\t" << TransId 
//                << "\tregisters lock in trans map\t" << LockPtr 
//                << "\twith type\t" << LType 
//                << "\n";
#endif
      auto it = TransMap.find(TransId);
      /// If current transaction never registers itself with any lock
      if (it == TransMap.end()) {
        LockMapType LockMap;
        LockMap.insert(LockEntryType(LockPtr, LType));
        TransMap.insert(TxRrEntryType(TransId, LockMap));
        return true;
      }
      /// If current lock exists in list 
      auto LockPair = (it->second).find(LockPtr);
      if ( LockPair == (it->second).end()) {
        (it->second).insert(LockEntryType(LockPtr, LType));
#ifdef _DEBUG_PRINT_
//        std::cout << "Transaction\t" << TransId
//                  << "\talready registers lock\t" << LockPtr
//                  << "\tin Trans Map\n";
#endif
      }
      ///SH + EX
      else {
        if (LType == T_EX) {
#ifdef _DEBUG_PRINT_
        std::cout << "Transaction\t" << TransId
                  << "\talready registers lock\t" << LockPtr
                  << "\t with type\t" << LockPair->second
                  << "\tin Trans Map\n";
#endif
        LockPair->second = T_EX;
        }
      }
      return true;
  }

  ///YES TransMap has it NO Map does NOT have LockPtr
  bool  LocksManager::checkTransMap(IdType TransId
                                    , LockPointer LockPtr
                                    , LockType LType) {
      auto it = TransMap.find(TransId);
      if (it == TransMap.end()) {
        return false;
      }

      auto LockPair = (it->second).find(LockPtr);
      if ( LockPair == (it->second).end()) {
        return  false;
      }
      /// EX + SH return TRUE 
      /// SH + EX return FALSE
      if (LockPair->second != LType ) {
#if _DEBUG_PRINT_
        std::cout << "Transaction\t" << TransId
                  << "\talready registers lock\t" << LockPtr
                  << "\t with type\t" << LockPair->second
                  << "\tin Trans Map\n";
#endif
        return  LType == T_SH; 
      }
      return true;
  }

  auto LocksManager::registerLockMap(IdType TransId
                                    , LockPointer LockPtr
                                    , LockType LType)
    ->  bool  {
#ifdef _DEBUG_PRINT_
//      std::cout << "Transaction " << TransId << " registers lock in lock map " << LockPtr << "\n";
#endif
      /// No entry found -> create and add to map
      if (ResrGuardMap.find(LockPtr)== ResrGuardMap.end())  {
        ExMutexPointer NewGuard(new std::mutex);
        ResrGuardMap.insert(ResourceGuardPairType(LockPtr, NewGuard));
      }
      auto Guard = ResrGuardMap[LockPtr];
      ///Don't wait to register to LockMap; otherwise there might be a deadlock
      /// Tx1 getlocks on x
      /// Tx1 tries to regisetr x
      /// x register locks was acquired by one deadlock detector
      /// Tx1 spins on register lock on x
      /// deadlock detector cannot see tx1 alreadys holds x but 
      /// return yes to let tx2 to wait
      /// Deadlock!
      if (!Guard->try_lock()) {
        return false;
      }

      auto it = ResrMap.find(LockPtr);
      if (it == ResrMap.end())  {
        std::set<LockType> lockset;
        lockset.insert(LType);
        TransMapType TxMap{{TransId, lockset}};
        ResrMap.insert(std::make_pair(LockPtr, TxMap)); 
        Guard->unlock();
        return true;
      }
      auto TxRec  = (it->second).find(TransId);
      /// If this transaction never registers itself with this lock, register
      if (TxRec == (it->second).end()) {
        std::set<LockType> LockSet;
        LockSet.insert(LType);
        (it->second).insert(std::make_pair(TransId, LockSet));
        Guard->unlock();
        return true;
      }
      /// If this transaction already holds the lock with same LockType
      ///   Ignore it
      auto LTypeSet = TxRec->second;
      if (LTypeSet.empty()) {
        LTypeSet.insert(LType);
        Guard->unlock();
        return true;
      }

#ifdef _DEBUG_ENABLE_
      ///EX + SH
      ///This wouldn't happen; just for debugging
      if (LTypeSet.find(LType) == LTypeSet.end()) {
        std::cout << "Transaction\t" << TransId 
                  << "\tholds\t" << LockPtr 
                  << "\tand tries lock type\t" << LType
                  << "\n";
        ///already got T_EX, no need to get T_SH
        if (LType == T_EX)  {
          LTypeSet.insert(LType);
        }
      }
#if _DEBUG_PRINT_
      else {
        std::cout << "Transaction\t" << TransId 
                  << "\tholds\t" << LockPtr 
                  << "\twith lock type\t" << LType
                  << "\n";
      }
#endif

#endif

      Guard->unlock();
      return true;
  }
//#endif

  auto LocksManager::registerToMap(IdType TransId, LockPointer LockPtr, LockType LType)
    ->  bool  {
      bool regTrans = registerTransMap(TransId, LockPtr, LType);
      bool regLock = registerLockMap(TransId, LockPtr, LType);
      return regTrans && regLock;
  }

  auto LocksManager::retireFromWaitingMap(IdType TxId, LockPointer LockPtr)
    -> bool {
#if _DEBUG_PRINT_
//      std::cout << "Transaction " << TxId 
//                << "\tretires from waiting map\t" << LockPtr 
//                << "\n";
#endif
      bool missGuard = false;
      if (WaitGuardMap.find(TxId) == WaitGuardMap.end()) {
        if (WaitMap.find(TxId) != WaitMap.end()) {
          missGuard = true;
#if _DEBUG_ENABLE_
          std::cout << "Transaction\t" << TxId 
                    << "\t encounters Lock guard missing in WaitMap on\t" << LockPtr
                    << "\n";
#endif
        }
        else {
#if _DEBUG_ENABLE_
        std::cout << "Transaction\t" << TxId 
                  << "\t holds is not waiting for" << LockPtr
                  << "\n";
#endif
          return true;
        }
      }

      if (!missGuard) {
        while (!WaitGuardMap[TxId]->try_lock())  {
#if _DEBUG_PRINT_
          std::cout << "Transaction\t" << TxId
                    << "\tspin on lock_guard to retire\t" << LockPtr
                    << "\tfrom wait map\n";
#endif
        }
      }

      auto it = WaitMap.find(TxId);
      if (it == WaitMap.end()) {
#if _DEBUG_ENABLE_
        std::cout <<"Error : Transaction " << TxId << " is not waiting for any lock\n";
#endif
        if (!missGuard) { WaitGuardMap[TxId]->unlock();}
        return false;
      }
      if (it->second != LockPtr)  {
#if _DEBUG_ENABLE_
        std::cout <<"Error : Transaction " << TxId << " is waiting for " << it->second << " not " << LockPtr <<"\n";
#endif
        if (!missGuard) { WaitGuardMap[TxId]->unlock();}
        return false;
      }

      WaitMap.erase(it);
      if (!missGuard) { WaitGuardMap[TxId]->unlock();}
      ///TODO should delete lockGuard too?
      return true;
  }

  auto LocksManager::retireFromTransMap(IdType TxId, LockPointer LockPtr, LockType LType)
    -> bool {
    auto it = TransMap.find(TxId); 
#if _DEBUG_PRINT_
//      std::cout << "Transaction " << TxId 
//                << "\tretires from trans map\t" << LockPtr 
//                << "\n";
 
#endif

    if (it == TransMap.end()) {
#if _DEBUG_ENABLE_
      std::cout <<"Error : Transaction " << TxId << " is not holding any lock\n";
#endif
      return false;
    }
    /// it->second is a set holding all lock pointers
    auto iter = it->second.find(LockPtr);
    if (iter == it->second.end()) {
#if _DEBUG_ENABLE_
      std::cout <<"Error : Transaction " << TxId << " does NOT have this lock\n";
#endif
      return false;
    }
    it->second.erase(LockPtr);
    return true;
  }

  auto LocksManager::retireFromLockMap(IdType TxId
                                      , LockPointer LockPtr
                                      , LockType LType)
    -> bool {
#if _DEBUG_PRINT_
//      std::cout << "Transaction\t" << TxId 
//                << "\tretires from lock map\t" << LockPtr 
//                << "\n";
#endif 

    bool missGuard = false;
    if (ResrGuardMap.find(LockPtr) == ResrGuardMap.end()) {
      if (ResrMap.find(LockPtr) != ResrMap.end()) {
      /// In this case, an error happened
        missGuard = true;
#if _DEBUG_ENABLE_
        std::cout << "Transaction\t" << TxId 
                  << "\t encounters Lock guard missing in LockMap on\t" << LockPtr
                  << "\n";
#endif
//        return false;
      }
      else {
#if _DEBUG_ENABLE_
        std::cout << "Transaction\t" << TxId 
                  << "\t holds no lock on" << LockPtr
                  << "\n";
#endif
        return true;
      }
    }
    
    if (!missGuard) {
      /// This statements blocks if lock is not available 
      /// but there is no potential deadlock since we didn't create
      /// "circular wait" situations
      while (!ResrGuardMap[LockPtr]->try_lock()) {
#if _DEBUG_PRINT_
      std::cout << "Transaction\t" << TxId
                << "\tspin on lock_guard to retire\t" << LockPtr
                << "\tfrom lock map\n";
#endif
      }
    }

    auto it = ResrMap.find(LockPtr);
    if (it == ResrMap.end())  {
#if _DEBUG_ENABLE_
      std::cout <<"Error : Lock " << LockPtr << " is not held by any transaction\n";
#endif
      if (!missGuard) { ResrGuardMap[LockPtr]->unlock(); }
      return false;
    }
    auto iter = it->second.find(TxId);
    if (iter == it->second.end()) {
#if _DEBUG_ENABLE_
      std::cout <<"Error : Lock " << LockPtr << " is not held by transaction " << TxId << "\n";
#endif
      if (!missGuard) { ResrGuardMap[LockPtr]->unlock(); }
      return false;
    }

//    auto LockSet = iter->second.find(LType);
    if (iter->second.find(LType) == iter->second.end()) {

#if _DEBUG_ENABLE_
      std::cout <<"Error : Lock " << LockPtr 
                << "\tis not held by transaction\t" << TxId 
                << "\twith\t" << LType 
                <<"\n";
#endif
      if (!missGuard) { ResrGuardMap[LockPtr]->unlock(); }
      return false;
    }
    /// Remove the entry created by TxId
    iter->second.erase(LType);
//    it->second.find(TxId)->second.find(LType)
    if (iter->second.empty()) {
      it->second.erase(TxId);
    }
#if _DEBUG_ENABLE_
    else {
      std::cout << "Transaction\t" << TxId
                << "\tstill holds lock type other than\t" << LType
                << "\n";
    }
#endif
    if (!missGuard) { ResrGuardMap[LockPtr]->unlock(); }
    return true;
  }

#ifdef _NO_WAIT_
  auto LocksManager::releaseAll(IdType TxId)
    ->  bool  {
#ifdef _DEBUG_PRINT_
      std::cout <<"Transaction\t" << TxId << "\tSHRINKING\n";
#endif

    auto AcqLocks  = TransMap.find(TxId);
    /// This transaction does NOT have any lock
    if (AcqLocks == TransMap.end())  {
#ifdef _DEBUG_PRINT_
      std::cout << "Transaction\t" << TxId << "\tholds NO lock\n";
      std::cout << "Transaction\t" << TxId << "\tFinishes\n";
#endif
      return true;
    }

    /// AcqLocks is a set
    /// it is a iterator to this set
//    auto it_end = (*AcqLocks).second.end();
//    std::cout <<"Locks num\t" << AcqLocks->second.size() << "\n";
    for (auto it = (*AcqLocks).second.begin(); it!= (*AcqLocks).second.end(); it++) {
//      tryUnlock(it->first, it->second);
      tryUnlock(it->first, T_SH);
      tryUnlock(it->first, T_EX);
    }

    /// Now remove records of TxId from TransMap
    TransMap.erase(TxId);
#ifdef _DEBUG_PRINT_
    std::cout << "Transaction\t" << TxId << "\tFinishes\n";
#endif
    return true;
  }

#else /*WAIT_DIE or DEADLOCK_DETECTION*/

  auto LocksManager::releaseAll(IdType TxId)
    ->  bool  {
//      dumpTransMap();
#ifdef _DEBUG_PRINT_
      std::cout <<"Transaction\t" << TxId << "\tSHRINKING\n";
#endif

#ifndef _WAIT_DIE_
    auto WaitLocks  = WaitMap.find(TxId);
    auto WaitGuard = WaitGuardMap.find(TxId);
    if (WaitGuard != WaitGuardMap.end())  {
      WaitGuard->second->lock();
    }
    /// Remove entry <TxId, Lock> in WaitMap if it exists
    if (WaitLocks != WaitMap.end()) {
      WaitMap.erase(TxId);
    }

    if (WaitGuard != WaitGuardMap.end())  {
      WaitGuard->second->unlock();
    }
    WaitGuardMap.erase(TxId);
#endif /*DEADLOCK_DETECTION_ */

    auto AcqLocks  = TransMap.find(TxId);
    /// This transaction does NOT have any lock
    if (AcqLocks == TransMap.end())  {
#ifdef _DEBUG_PRINT_
      std::cout << "Transaction\t" << TxId << "\tholds NO lock\n";
      std::cout << "Transaction\t" << TxId << "\tFinishes\n";
#endif
      return true;
    }

    for (auto it = (*AcqLocks).second.begin(); it!= (*AcqLocks).second.end(); it++) {
      /// Remove entries from ResrMap one by one
      /// If this lock is not found in ResrMap, there is an error
      auto ResrGuard = ResrGuardMap.find(it->first);
      if (ResrGuard != ResrGuardMap.end()) {
        ResrGuard->second->lock();
      }

      auto TransRec = ResrMap.find(it->first);
      if (TransRec == ResrMap.end()) {
        std::cerr << "Error: Lock\t" << it->first
                  << "\tnot found\n";
        tryUnlock(it->first, it->second);

        if (ResrGuard != ResrGuardMap.end()) {
          ResrGuard->second->unlock();
        }
        continue;
        /// TODO shoud be exit?
      }
      /// If this transaction is not found with this lock, there is an error
      auto Tx = (*TransRec).second.find(TxId);
      if (Tx  == (*TransRec).second.end()) {
        std::cerr << "Error: Transaction " << TxId << " not found in ResrMap \n"; 
        tryUnlock(it->first, T_SH);
        tryUnlock(it->first, T_EX);

        if (ResrGuard != ResrGuardMap.end()) {
          ResrGuard->second->unlock();
        }
        continue;
      }
//      std::cout << "Transaction\t" << TxId << "\treleases locks\t" << *it << "\n";
      if (Tx->second.size() > 1)  {
        std::cout << "Error in unlocking:\t" << TxId 
                  << "\tholds both SH and EX locks on\t" <<it->first
                  << "\n";
      }

      for (auto lock : Tx->second)  {
        tryUnlock(it->first, lock);
      }

      TransRec->second.erase(TxId);

      if (ResrGuard != ResrGuardMap.end()) {
        ResrGuard->second->unlock();
      }

    }///for
    /// Now remove records of TxId from TransMap
    TransMap.erase(TxId);
#ifdef _DEBUG_PRINT_
    std::cout << "Transaction\t" << TxId << "\tFinishes\n";
#endif
    return true;
  }
#endif

  auto LocksManager::upgradeLock(IdType TransId, LockPointer LockPtr)
      ->  bool  {
       /// unlock sh(), try_ex(), update table
       /// return false if exclusive lock cannot be acquired
        LockPtr->unlock_shared();

        auto getEXLock = LockPtr->try_lock();
        if (!getEXLock) {
          return false;
        }

//        auto TxMap = ResrMap.find(LockPtr);
//#if _DEBUG_ENABLE_
//        if (TxMap == ResrMap.end()) {
//          std::cerr << "Error: lock not found in Resource Map\n";
//          return false;
////          exit(0);
//        }
//#endif  
//        auto TxRec = (TxMap->second).find(TransId);
//#if _DEBUG_ENABLE_
//        if (TxRec == TxMap->second.end()) {
//          std::cerr << "Error: lock is not upgradable \t";
//        }
//#endif 
//        TxRec->second.clear();
//        TxRec->second.insert(T_EX);
        retireFromLockMap(TransId, LockPtr, T_SH);
        registerLockMap(TransId, LockPtr, T_EX);
        std::cout << "Transaction\t" << TransId 
                  << "\tupgrade lock on\t" << LockPtr
                  << "\n";

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
        std::cout << iter->first << "\t"
                  << iter->second <<"\t";
      }
    }

  }

  void LocksManager::dumpResrMap()  {
    std::cout <<"+++ LockMap +++ \n";
    for (auto it = ResrMap.begin(), it_end = ResrMap.end();
         it != it_end; it++)  {
      int transNum = it->second.size();
      if (transNum > 0) {
        std::cout <<"Lock\t" << it->first << "\tTrans number\t" << transNum <<"\n";
        for (auto iter = it->second.begin(), iter_end = it->second.end();
          iter != iter_end; iter++) {
          for (auto lock: iter->second) {
            std::cout << (*iter).first  <<"\t" << lock << "\n";
          }
        }
      }
    }
  }
#endif /*_DEBUG_PRINT_*/

#endif /*_DEADLOCK_DETECTION_*/

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

#if _DEBUG_PRINT_
      std::cout << "Add vertex lock\t" <<  VertexId
              <<"\t to map\n";
#endif
    }

  auto LocksManager::addToEdgeLockMap(IdType EdgeId) 
    -> void  {
      EdgePtr Edge = Graph.getEdgePointer(EdgeId);
      if (Edge == nullptr) {
        std::cerr  << "Error : No such edge\t" << EdgeId << "\tin map \n";
        return ;
//        exit(0);
      }
      EdgeLock*  NewEdgeLock = new EdgeLock();
      Edge->setEdgeLock(NewEdgeLock);
#if _DEBUG_PRINT_
      std::cout << "Add edge lock\t" << EdgeId
              <<"\t to map\n";
#endif
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
