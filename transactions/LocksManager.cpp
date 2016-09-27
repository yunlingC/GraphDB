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

  auto LocksManager::releaseVertexLock(IdType VertexId, MutexType Mutex, LockType Lock) 
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

  auto LocksManager::getEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock) 
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

  auto LocksManager::releaseEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock) 
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
    std::map<IdType, VertexPointer> VertexMap;
    std::map<IdType, EdgePointer> EdgeMap;
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

  auto LocksManager::getVertexLock(IdType VertexId, MutexType Mutex, LockType Lock) 
    -> LockRetPairType  {
#if _DEBUG_ENABLE_
    if (VertexLockMap.find(VertexId) == VertexLockMap.end()) {
      std::cerr  << "Error : No such vertex " << VertexId <<" in map \n";
      exit(0);
    }
#endif
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
    bool  RetValue  = false;
    switch (Lock) {   
      ///Shared lock
      case SH:
        RetValue = MutexPtr->try_lock_shared();
        break;
      ///Exclusive lock
      case EX:
        RetValue = MutexPtr->try_lock(); 
        break;
#if _DEBUG_ENABLE_
      default:
        std::cerr << "ERROR: No such Mutex in VertexLock\n";
        exit(0);
#endif
    }
    return std::make_pair(LockRetPairType(RetValue, MutexPtr));
  }

  auto LocksManager::getEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock) 
    -> LockRetPairType  {
#if _DEBUG_ENABLE_
    if (EdgeLockMap.find(EdgeId) == EdgeLockMap.end()) {
      std::cerr << "Error : No such edge" << EdgeId <<" in map \n";
      exit(0);
    }
#endif
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
#if _DEBUG_ENABLE_
      default:
        std::cerr << "ERROR: No such Mutex in EdgeLock\n";
        exit(0);
#endif
    }

    bool RetValue = false;
    switch (Lock) {   
      case SH:
        RetValue  = MutexPtr->try_lock_shared();
        break;
      case EX:
        RetValue  = MutexPtr->try_lock(); 
        break;
#if _DEBUG_ENABLE_
      default:
        std::cerr  << "ERROR: No such Mutex in EdgeLock\n";
        exit(0);
#endif
    }
    return std::make_pair(LockRetPairType(RetValue, MutexPtr));
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
          TransMapType TxList = ResrMap.find(LockPtr);
          /// TxList should NOT be empty, otherwise getLock should work
#if _DEBUG_ENABLE_
          if (TxList == ResrMap.end()) {
            std::cerr << "Error : Lock is not available but no record found\n";
            exit(0);
          }
#endif
          auto iter = (*TxList).begin();
          while (iter != (*TxList).end()) {
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
                case  EX:
                  return T_Ignore;
                case  SH:
                  /// <r, w>
                  if (LType == EX) {
                    /// No other transaction is holding this lock
                    if (TxList.size() == 1) {
                      return T_Upgrade;
                    }
                    else {
                      /// Unlock it and go on
                      releaseLock();
                      /// remove itself from list
                      removeLock();
                      auto Ret = checkWaitOn(TransId, LockPtr, LType) ? T_Wait : T_Abort;
                      return Ret;
                    }
                  }
              }// switch
            } // if 
            TransStack.push(*iter.first);
//            CheckedTransList.insert(*iter.first);
            if (!checkWaitOnRecursive(TransId, *iter.first, TransStack, checkedTransList)) {
              /// There is a potential deadlock
              return T_Abort;
            } //if 
            TransStack.pop();
            iter++;
          }//while

          registerWaitingMap();
          return T_Wait;
        }
            
    auto LocksManager::checkWaitOnRecursive(TransIdType WaitingTrans
                                            , TransIdType LockingTrans
                                            , TransStackType  TransStack
                                            , TransSetType ChkTransList)
      ->  bool  {
        /// Deadlock exists if any of LockingTrans is the waitingTrans
        /// need return true; True - yes deadlock
        if (WaitingTrans == LockingTrans) {
          /// Print out deadlock
          std::string Circle(itoa(LockingTrans)); 
          while (!TransStack.empty()) {
          
            auto TX = TransStack.top();
            TransStack.pop();
            Circle.append("<--" + itoa(TX));
          }
          Circle.append("<--" + itoa(WaitingTrans));
          return false;
        }
        /// check further if there is deadlock
        ChKTransList.push_back(LockingTrans);
        /// Each transaction waits for one lock only
        auto LockPtr  = WaitMap.find(LockingTrans);
        if (LockPtr == WaitMap.end())
          return true;
        auto TxList  =  ResrMap.find(*LockPtr);
        /// This lock is available
        if (TxList == ResrMap.end()) 
          return true;

        auto it_end = (*TxList).begin();
        for (auto it  = (*TxList).begin(); it!= it_end; it++) {
          /// This transaction has NOT been checked
          if (ChkTransList.find(*it) == ChkTransList.end()) {
            TransStack.push(*it);
            checkWaitOnRecursive(WaitingTrans, *it, TransStack, ChkTransList);
            TransStack.pop();
          }//if
        }
      }

    auto LocksManager::getVertexLock(IdType VId, MutexType Mutex, LockType Lock, IdType TxId)
      ->  bool  {
       LockRetPairType getLock = LocksManager::getVertexLock(VId, Mutex, Lock);
      
       
       std::lock_guard<std::mutex> Lock(DeadlockDetector);
       /// If this lock is available, assign lock to TxId, register to maps(Trans, Lock)
       if (getLock.first) {
         /// TODO separate getting lock from getVertexLock
         LocksManager::registerToMap(TxId, getLock.second, Lock);
         return true;
       }
       switch (LocksManager::checkWaitOn(TxId, getLock.second, Lock))  {
         /// If there is no potential deadlock, wait for it
         case T_Wait:
           while (!(LocksManager::getVertexLock(VID, Mutex, Lock).first));
           return true;
         case T_Abort:
           LocksManager::releaseAll(TxId);
           return false;
         case T_Ignore:
           return true;
         case T_Upgrade:
           /// Release SH Lock on data and then request EX Lock
           /// If lock can be acquired, return true
           /// else return false and then restart
           break;
       }
    }

    auto LocksManager::getEdgeLock(IdType EId, MutexType Mutex, LockType Lock, IdType TxId)
      ->  bool  {
       LockRetPairType  getLock = getEdgeLock(EId, Mutex, Lock);
       /// If this lock is available, assign lock to TxId, register to maps(Trans, Lock)
       if (getLock.first) {
         /// TODO separate getting lock from getVertexLock
         LocksManager::registerToMap(TxId, getLock.second, Lock);
         return true;
       }
       switch (checkWaitOn(TxId, getLock.second, Lock))  {
         /// If there is no potential deadlock, wait for it
         case T_Wait:
           while (!getEdgeLock(EID, Mutex, Lock));
           break;
         case T_Abort:
           return false;
         case T_Ignore:
           return true;
       }
    }

//    auto LocksManager::getLock(IdType Id, MutexType Mutex, LockType Lock, IdType TxId)
//      ->  bool  {
//       if (getLock) {
//         /// TODO separate getting lock from getVertexLock
//         registerToMap(TxId, LockPtr);
//         return true;
//       }
//       switch (checkWaitOn(TxId, LockPtr, Lock))  {
//         /// If there is no potential deadlock, wait for it
//         case T_Wait:
//           while (!getEdgeLock(EID, Mutex, Lock));
//           break;
//         case T_Abort:
//           return false;
//         case T_Ignore:
//           return true;
//       }
//      }

    auto LocksManager::registerWaitingMap(IdType TxId, LockPoiner LockPtr)
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
        if ((*it).find(LockPtr) != (*it).end()) {
          /// TODO do something ?
          /// So far there is no need to update transMap, which means no lock 
          /// Update ResrMap only 
          return true;
        }

        (*it).insert(std::make_pair(TransId, LockList));
        return true;
    }

    auto LocksManager::registerLockMap(IdType TransId, LockPointer LockPtr, LockType LType)
      ->  bool  {
        auto it = ResrMap.find(LockPtr);
        if (it == ResrMap.end())  {
          TransactionMapType TxMap{TransId, LType};
          ResrMap.insert(std::make_pair(LockPtr, TxMap)); 
          return true;
        }
        auto TxRec  = (*it).find(TransId);
        /// If this transaction never registers itself with this lock, register
        if (TxRec == (*it).end()) {
          (*it).insert(std::make_pair(TransId, LType));
          return true;
        }
        /// If this transaction already holds the lock with same LockType
        ///   Ignore it
        if (((*TxRec).second == SH) && (LType == EX)) {
          (*TxRec).second = EX;
//          return true;
        }
        return true;
    }

    auto LocksManager::registerToMap(IdType TransId, LockPointer LockPtr, LockType LType)
      ->  bool  {
        return LocksManager::registerTransMap(TransId, LockPtr) && LocksManager::registerLockMap(TransId, LockPtr, LType);

    }

    auto releaseAll(IdType TxId)
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
      auto it_end = (*AcqLocks).end();
      for (auto it = (*AcqLocks).begin(); it!= it_end; it++) {
        /// Remove entries from ResrMap one by one
        /// If this lock is not found in ResrMap, there is an error
        auto TransRec = ResrMap.find(*it);
        if (TransRec == ResrMap.end()) {
          std::cerr << "Error: Lock not found\n";
          return false;
          /// TODO shoud be exit?
        }
        /// If this transaction is not found with this lock, there is an error
        auto Tx = (*TransRec).find(TxId);
        if (Tx  == (*TransRec).end()) {
          std::cerr << "Error: Transaction " << TxId << " not found \n"; 
          return false;
        }
        (*TransRec).erase(TxId);
      }
      /// Now remove records of TxId from TransMap
      TransMap.erase(TxId);
      return true;
    }

    auto LocksManager::upgradeLock(IdType TransId, LockPointer LockPtr)
      ->  bool  {
       /// TODO work till here
       /// unlock sh(), try_ex(), update table
       /// return false if exclusive lock cannot be acquired
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
