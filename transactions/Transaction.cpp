//===-- transactions/Transaction.cpp - RAG class type -----------*- C++ -*-===//
//
//                     CAESR Graph Database
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the main class for resource-locks allocation graph management
///
//===----------------------------------------------------------------------===//

#ifndef _TRANSACTION_CPP_
#define _TRANSACTION_CPP_

#include "global.h"
#include "Transaction.h"

#ifdef _TRANX_STATUS_
#include <iostream>
#endif

#define CLOCK_ID  CLOCK_THREAD_CPUTIME_ID
//#define CLOCK_ID  CLOCK_REALTIME
#define MILLION 1000000
#define NANO 1000000000
#define SCALE 1000

//  Transaction::Transaction() : TransId(0), TransStatus(T_EXPANDING) {

  Transaction::Transaction(IdType id) : TransId(id), TransStatus(T_EXPANDING){
#ifdef _DEADLOCK_DETECTION_
    WaitGuardPtr = ExMutexPointer(new ExMutex);
    WaitMutexPtr = nullptr;
#endif

//  Transaction::Transaction(IdType id, LocksManager & LkManager) : LockManager(LkManager)
//                                                                    , TransId(id)
//                                                                    , TransStatus(T_EXPANDING){
#ifdef _TRANX_STATS_
    initStats();
#endif
//    begin();
  } 


#ifdef _TRANX_STATS_
  void Transaction::initStats()  {
    NumAbort = 0;
    AbortedMap.clear();
    VisitedMap.clear();
    BeginTime = 0;
    ExpandTime = 0; 
    CommitTime = 0;
  }
#endif
  
  Transaction::IdType Transaction::getId() 
  {
    return TransId;
  }


  bool Transaction::begin() {
    TransStatus = T_EXPANDING;
#ifdef _TRANX_STATUS_
    std::cout <<"Transaction\t" << TransId << "\tSTART\n";
#endif 

#ifdef _TRANX_STATS_
    setBeginTime();
#endif
    return true;
  }

  bool Transaction::expand()  {
    TransStatus = T_EXPANDING;
#ifdef _TRANX_STATUS_
    std::cout <<"Transaction\t" << TransId << "\tEXPAND\n";
#endif 

#ifdef _TRANX_STATS_
    setExpandTime();
#endif
    return true;
//    bool retValue = false; 
//    switch()
  }

  bool Transaction::commit() {
    bool retValue = false;
    switch(TransStatus) {
      case T_EXPANDING:
        TransStatus = T_SHRINKING;
        /// Comment the following line:
        /// This is because we are usig strict strong 2pl 
        /// Releasing locks is not allowed until we finishi committing
        /// break;
      case T_SHRINKING:
        TransStatus = T_COMMIT;
        retValue = true;
        break;
      case T_ABORT:
        TransStatus = T_ROLLBACK;
        break;
      case T_ROLLBACK:
        TransStatus = T_EXPANDING;
        break;
      case T_COMMIT:
        retValue = true;
        break;
    }
#ifdef _TRANX_STATUS_
    if (retValue)
    std::cout <<"Transaction\t" << TransId << "\tCOMMIT\n";
#endif 

#ifdef _TRANX_STATS_
    setCommitTime();
#endif
    return retValue;
  }

  bool Transaction::close() {
#ifdef _TRANX_STATS_
    setCloseTime();
#endif
    releaseLock();
#ifdef _TRANX_STATUS_
        std::cout <<"Transaction\t" << TransId 
                  << "\tCLOSE\n";
#endif
    
    return true;
  }

  bool Transaction::abort() 
  {
    bool retValue = false;

    switch(TransStatus) {
      case T_EXPANDING:
        TransStatus = T_ABORT;
        retValue = true;
        break;
      case T_SHRINKING:
        TransStatus = T_ROLLBACK;
        /// Abort is not allowed in shrinking phase with SS2PL
#ifdef _TRANX_STATUS_
        std::cout <<"Error in Abort:\t" << TransId << "\tis Shrinking\n";
        exit(0);
#endif
        break;
      case T_ABORT:
#ifdef _TRANX_STATUS_
        std::cout <<"Error in Abort:\t" << TransId << "\tis ABORT\n";
#endif
        TransStatus = T_ABORT;
        retValue = true;
        break;
      case T_ROLLBACK:
        TransStatus = T_EXPANDING;
#ifdef _TRANX_STATUS_
        std::cout <<"Error in Abort:\t" << TransId << "\tis ROLLBACK\n";
        exit(0);
#endif
        break;
      case T_COMMIT:
#ifdef _TRANX_STATUS_
        std::cout <<"Error in Abort:\t" << TransId << "\tis COMMIT\n";
        exit(0);
#endif
        break;
    }
#ifdef _TRANX_STATUS_
    if (retValue)
    std::cout <<"Transaction\t" << TransId << "\tABORT\n";
#endif  

//    if (retValue) {
      releaseLock();
//    }
#ifdef _TRANX_STATS_
//    if (retValue)
      NumAbort++;
#endif
    
    return retValue;
  }


  bool Transaction::abort(VertexPointer vptr, MutexType mt, LockType lt) {
    abortMutex(vptr->getLockPtr()->getMutexPtr(mt));
    return abort();
  }

  bool Transaction::abort(EdgePointer eptr, MutexType mt, LockType lt) {
    abortMutex(eptr->getLockPtr()->getMutexPtr(mt));
    return abort();
  }

  bool Transaction::rollBack()
  {
      TransStatus = T_ROLLBACK;
      return true;
  }

  TransStatusType Transaction::checkStatus() 
  {
      return TransStatus;
  }


  bool Transaction::registerVertexLock(VertexPointer vptr, MutexPointer mptr, LockType lktype) {

#ifdef _WAIT_DIE_
    while (!mptr->registerTx(TransId, lktype))
#elif defined _DEADLOCK_DETECTION_
    if (!mptr->registerTx(TransId, lktype)) {
#if _DEBUG_PRINT_
//#ifdef _TRANX_STATUS_
      std::cout << "Transaction\t" << TransId
                << "\t cannot register vertex lock\t" << vptr->getId()
                << "\n";
#endif
      return false;
    }
#endif

    VertexLockMap.insert(VertexLockPairType(mptr, VLockPairType(vptr, lktype)));
    return true;
  }

  bool Transaction::registerEdgeLock(EdgePointer eptr, MutexPointer mptr, LockType lktype) {

#ifdef _WAIT_DIE_
    while (!mptr->registerTx(TransId, lktype))
#elif defined _DEADLOCK_DETECTION_
    if (!mptr->registerTx(TransId, lktype)) {
//#ifdef _TRANX_STATUS_
#if _DEBUG_PRINT_
      std::cout << "Transaction\t" << TransId
                << "\t cannot register edge lock\t" << eptr->getId()
                << "\n";
#endif
      return false;
    }
#endif

    EdgeLockMap.insert(EdgeLockPairType(mptr, ELockPairType(eptr, lktype)));
    return true;
  }

  /// True - lock already acquired; False - lock does not exist in map
  bool Transaction::checkVertexLock(MutexPointer mptr, LockType lt) {
//    auto mptr = vptr->getLockPtr()->getMutexPtr(mt);
    auto LockPair = VertexLockMap.find(mptr);
    if (LockPair != VertexLockMap.end()) {
      ///Check lock type 
      if (LockPair->second.second == lt) {
        return true;
      }
      else {
        assert(false && "Transaction requires a lock of both types");
        return true;
      }
    }
    return false;
  }

  bool Transaction::checkEdgeLock(MutexPointer mptr, LockType lt) {
//    auto mptr = eptr->getLockPtr()->getMutexPtr(mt);
    if (EdgeLockMap.find(mptr) != EdgeLockMap.end())  
      return true;
    return false;
  }

  bool Transaction::getVertexLock(VertexPointer vptr, MutexType mt, LockType lt) {
    auto lockptr = vptr->getLockPtr();
    assert(lockptr != nullptr && "Vertex lock pointer invalid");
    auto mptr = lockptr->getMutexPtr(mt);
#ifdef _TRANX_STATS_
    visitMutex(mptr);
#endif
    if (checkVertexLock(mptr, lt)) {
      return true;
    }

    int trial = 2;

    while (trial-- > 0) {
    if (lockptr->tryLock(mt, lt)) {
      if (registerVertexLock(vptr, mptr, lt)) {
        return true;
      } else {
        lockptr->tryUnlock(mt, lt);
      }
    }
    }

    return false;

//    if (!registerVertexLock(vptr, mptr, lt)) {
//      lockptr->tryUnlock(mt, lt);
//      return false;
//    }
//    return true;
  }

  bool Transaction::getEdgeLock(EdgePointer eptr, MutexType mt, LockType lt) {
    auto lockptr = eptr->getLockPtr();
    assert(lockptr != nullptr && "Edge lock pointer invalid");
    auto mptr = lockptr->getMutexPtr(mt);
#ifdef _TRANX_STATS_
    visitMutex(mptr);
#endif
    if (checkEdgeLock(mptr, lt)) {
      return true;
    }

    int trial = 5;

    while (trial-- > 0) {
    if (lockptr->tryLock(mt, lt)) {
      if (registerEdgeLock(eptr, mptr, lt)) {
        return true;
      } else {
        lockptr->tryUnlock(mt, lt);
      }
    }
    }

    return false;

//    if (!registerEdgeLock(eptr, mptr, lt)) {
//      lockptr->tryUnlock(mt, lt);
//      return false;
//    }
//    }

//    return true;
  }

  /**
   * TODO
   * The following two funcs are not active
   * */

  bool Transaction::releaseVertexLock(VertexPointer vptr, MutexType mt, LockType lt) {
    return true;
  }

  bool Transaction::releaseEdgeLock(EdgePointer eptr, MutexType mt, LockType lt)  {
    return true;
  }

/*
 * TODO
 * Implement SKIP technique in releasing locks (vertex and edge):
 * Release locks that can be retired first 
 * and then release others later
 **/
  void Transaction::releaseVertexLock( ) {
    for (auto LockEntry : VertexLockMap) {
      /// LockEntry <MutexPtr, <VertexPtr, LockType>>
      auto lockptr = LockEntry.second.first->getLockPtr();
#ifndef _NO_WAIT_
      LockEntry.first->retireTx(TransId, LockEntry.second.second);
#endif
      lockptr->tryUnlock(LockEntry.first, LockEntry.second.second);
    }
    VertexLockMap.clear();
    return ;
  }

  void Transaction::releaseEdgeLock( ) {
    for (auto LockEntry : EdgeLockMap) {
      auto lockptr = LockEntry.second.first->getLockPtr();
#ifndef _NO_WAIT_
      LockEntry.first->retireTx(TransId, LockEntry.second.second);
#endif
      lockptr->tryUnlock(LockEntry.first, LockEntry.second.second);
    }
    EdgeLockMap.clear();
    return ;
  }

  void Transaction::releaseLock()  {
    releaseVertexLock();
    releaseEdgeLock();
  }

  bool Transaction::waitOn(VertexPointer vptr, MutexType mt, LockType lt) {
    ///TODO need waitMap lock 
    auto lptr = vptr->getLockPtr();
    auto mptr = lptr->getMutexPtr(mt);
    int trial = 10;
    bool retValue = false;
    startWait(mptr);
#ifdef _TRANX_STATUS_
    std::cout << "Transaction\t" << TransId
              << "\twaits for vertex lock\t" << vptr->getId()
              << "\n";
#endif
    while (trial-- > 0) {
      retValue = getVertexLock(vptr, mt, lt);
      if (retValue) {
        break;
      }
    }
//    while(!getVertexLock(vptr, mt, lt));
//    while(!lptr->tryLock(mt, lt));
    endWait(mptr);
    ///TODO bug here need to return ret=getVertexLock()
//    return registerVertexLock(vptr, mptr, lt);
    return retValue;
  }

  bool Transaction::waitOn(EdgePointer eptr, MutexType mt, LockType lt) {
    ///TODO need waitMap lock 
    auto lptr = eptr->getLockPtr();
    auto mptr = lptr->getMutexPtr(mt);
    int trial = 10;
    bool retValue = false;
    startWait(mptr);
#ifdef _TRANX_STATUS_
    std::cout << "Transaction\t" << TransId
              << "\twaits for edge lock\t" << eptr->getId()
              << "\n";
#endif
    while (trial-- > 0) {
      retValue = getEdgeLock(eptr, mt, lt);
      if (retValue) 
        break;
    }
//    while(!getEdgeLock(eptr, mt, lt));
//    while(!lptr->tryLock(mt, lt));
    endWait(mptr);
//    return registerEdgeLock(eptr, mptr, lt);
    ///TODO bug here need to return ret=getVertexLock()
//    return true;
    return retValue;
  }

  bool Transaction::startWait(MutexPointer MuPtr) {
#ifdef _DEADLOCK_DETECTION_
    WaitGuardPtr->lock();

    WaitMutexPtr = MuPtr;

    WaitGuardPtr->unlock();
#endif
    return true;
  }


  bool Transaction::endWait(MutexPointer MuPtr) {
#ifdef _DEADLOCK_DETECTION_
    WaitGuardPtr->lock();

    WaitMutexPtr = nullptr;

    WaitGuardPtr->unlock();
#endif
    return true;
  }

#ifdef _DEADLOCK_DETECTION_
  Transaction::MutexPointer Transaction::checkTxWaitOn()  {
    WaitGuardPtr->lock();

    auto retPtr = WaitMutexPtr;

    WaitGuardPtr->unlock();

    return retPtr;
  }

  Transaction::MutexPointer Transaction::getWaitMutexPtr()  {
    return WaitMutexPtr;
  }

  Transaction::ExMutexPointer Transaction::getGuardPtr()  {
    return WaitGuardPtr;
  }
#endif

#ifdef _TRANX_STATS_
  void Transaction::visitMutex(MutexPointer mptr) {
    if ( VisitedMap.find(mptr) == VisitedMap.end() )  {
      VisitedMap.insert(MutexPairType(mptr, 0));
    }
    VisitedMap[mptr]++;
  }
  
  void Transaction::abortMutex(MutexPointer mptr)  {
    if ( AbortedMap.find(mptr) == AbortedMap.end() )  {
      AbortedMap.insert(MutexPairType(mptr, 0));
    }
    AbortedMap[mptr]++;
  }

  int Transaction::getAbortNum()  {
    return NumAbort;
  }

  Transaction::MutexMapType Transaction::getAbortedMap()  {
    return AbortedMap;
  }

  Transaction::MutexMapType Transaction::getVisitedMap()  {
    return VisitedMap;
  }

  void Transaction::dumpAbortNum() {
    std::cout << "Transaction\t" << TransId
              << "\tabort_num\t" << getAbortNum()
              << "\n";
  }
  
  void Transaction::dumpAbortedMap() {
    std::cout << "Transaction\t" << TransId
              << "\tabort_obj\t" << AbortedMap.size()
              << "\n";

//    for (auto obj : AbortedMap) {
//      std::cout << obj.first << "\t"
//                << obj.second 
//                << "\n";
//    }
  }

  void Transaction::dumpVisitedMap() {
    std::cout << "Transaction\t" << TransId
              << "\tvisit_obj\t" << VisitedMap.size()
              << "\n";

//    for (auto obj : VisitedMap) {
//      std::cout << obj.first << "\t"
//                << obj.second 
//                << "\n";
//    }
  }

  void Transaction::dumpStats() {
    dumpAbortNum();
    dumpAbortedMap();
    dumpVisitedMap();
  }


  uint64_t Transaction::setTime() {
    timespec Time = (struct timespec){ 0 };
    if ( clock_gettime( CLOCK_ID, &Time ) == -1) {
      std::cout << "Error in get time\n";
    }
    return  Time.tv_sec * NANO + Time.tv_nsec ;
  }

  void Transaction::setBeginTime() {
    BeginTime = setTime();
  }

  void Transaction::setCommitTime()  {
    CommitTime = setTime();
  }

  void Transaction::setCloseTime()  {
    CloseTime = setTime();
  }

  void Transaction::setExpandTime()  {
    ExpandTime  = setTime();
  }

  uint64_t Transaction::getBeginTime()  {
    return BeginTime;
  }

  uint64_t Transaction::getExpandTime()  {
    return ExpandTime;
  }

  uint64_t Transaction::getCommitTime()  {
    return CommitTime;
  }

  uint64_t Transaction::getActiveTime()  {
    ///Active time: begin() -- Commit()
    return (CommitTime - BeginTime) / SCALE;
  }

  uint64_t Transaction::getExecTime()  {
    /// Exec time: expand() -- commit()
    return (CommitTime- ExpandTime) / SCALE;
  }
  
  uint64_t Transaction::getShrinkTime()  {
    /// Shrink time: commit() -- close()
    return (CloseTime- CommitTime) / SCALE;
  }
  
  uint64_t Transaction::getProcTime() {
    ///begin() -- close()
    return (CloseTime- BeginTime) / SCALE;
  }

#endif

  Transaction::~Transaction(){
#ifdef _TRANX_STATUS_
    std::cout << "Transaction\t" << TransId 
              << "\tstatus\t" << TransStatus
              << "\tVertex Lock num\t" << VertexLockMap.size()
              << "\tEdge Lock num\t" << EdgeLockMap.size()
              << "\n";

#endif 
//    close();
  }
 
#endif /*_TRANSACTION_CPP_*/
