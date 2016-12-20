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

#include "Transaction.h"
#include "global.h"

//#define _PRINTLOG_ true

#ifdef _PRINTLOG_
#include <iostream>
#endif

#define CLOCK_ID  CLOCK_THREAD_CPUTIME_ID
//#define CLOCK_ID  CLOCK_REALTIME
#define MILLION 1000000
#define NANO 1000000000
#define SCALE 1000

//  Transaction::Transaction() : TransId(0), TransStatus(T_EXPANDING) {

  Transaction::Transaction(IdType id) : TransId(id), TransStatus(T_EXPANDING){

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
#ifdef _PRINTLOG_
    std::cout <<"Transaction\t" << TransId << "\tSTART\n";
#endif 

#ifdef _TRANX_STATS_
    setBeginTime();
#endif
    return true;
  }

  bool Transaction::expand()  {
    TransStatus = T_EXPANDING;
#ifdef _PRINTLOG_
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
#ifdef _PRINTLOG_
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
#ifdef _PRINTLOG_
        std::cout <<"Error in Abort:\t" << TransId << "\tis Shrinking\n";
        exit(0);
#endif
        break;
      case T_ABORT:
#ifdef _PRINTLOG_
        std::cout <<"Error in Abort:\t" << TransId << "\tis ABORT\n";
#endif
        TransStatus = T_ABORT;
        break;
      case T_ROLLBACK:
        TransStatus = T_EXPANDING;
#ifdef _PRINTLOG_
        std::cout <<"Error in Abort:\t" << TransId << "\tis ROLLBACK\n";
        exit(0);
#endif
        break;
      case T_COMMIT:
#ifdef _PRINTLOG_
        std::cout <<"Error in Abort:\t" << TransId << "\tis COMMIT\n";
        exit(0);
#endif
        break;
    }
#ifdef _PRINTLOG_
    if (retValue)
    std::cout <<"Transaction\t" << TransId << "\tABORT\n";
#endif  

#ifdef _TRANX_STATS_
    if (retValue)
      NumAbort++;
#endif
    
    return retValue;
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
    if (checkVertexLock(mptr, lktype))  {
      /// This mutex exists in the map -> Transaction already holds this lock
      return false;
    }
    VertexLockMap.insert(VertexLockPairType(mptr, VLockPairType(vptr, lktype)));
    return true;
  }

  bool Transaction::registerEdgeLock(EdgePointer eptr, MutexPointer mptr, LockType lktype) {
    if (checkEdgeLock(mptr, lktype))  {
      return false;
    }
    EdgeLockMap.insert(EdgeLockPairType(mptr, ELockPairType(eptr, lktype)));
    return true;
  }

  /// True - lock already acquired; False - lock does not exist in map
  bool Transaction::checkVertexLock(MutexPointer mptr, LockType lt) {
//    auto mptr = vptr->getLockPtr()->getMutexPtr(mt);
    if (VertexLockMap.find(mptr) != VertexLockMap.end()) 
      ///TODO check lock type 
      return true;
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
    if (checkVertexLock(mptr, lt)) {
      return true;
    }
    ///Lock not acquired && lock not available
    if (!lockptr->tryLock(mt, lt)) {
      return false;
    }
    return true;
  }

  bool Transaction::getEdgeLock(EdgePointer eptr, MutexType mt, LockType lt) {
    auto lockptr = eptr->getLockPtr();
    assert(lockptr != nullptr && "Edge lock pointer invalid");
    auto mptr = lockptr->getMutexPtr(mt);
    if (checkEdgeLock(mptr, lt)) {
      return true;
    }

    if (!lockptr->tryLock(mt, lt)) {
      return false;
    }
    return true;
  }

  bool Transaction::releaseVertexLock(VertexPointer vptr, MutexType mt, LockType lt) {
    return true;
  }

  bool Transaction::releaseEdgeLock(EdgePointer eptr, MutexType mt, LockType lt)  {
    return true;
  }


  void Transaction::releaseVertexLock( ) {
    for (auto LockEntry : VertexLockMap) {
      auto lockptr = LockEntry.second.first->getLockPtr();
      lockptr->tryUnlock(LockEntry.first, LockEntry.second.second);
//#ifndef _NO_WAIT_
//      LockManager.retireFromLockMap(TransId, LockEntry.first, LockEntry.second.second);
//#endif
    }
    VertexLockMap.clear();
    return ;
  }

  void Transaction::releaseEdgeLock( ) {
    for (auto LockEntry : EdgeLockMap) {
      auto lockptr = LockEntry.second.first->getLockPtr();
      lockptr->tryUnlock(LockEntry.first, LockEntry.second.second);
//#ifndef _NO_WAIT_
//      LockManager.retireFromLockMap(TransId, LockEntry.first, LockEntry.second.second);
//#endif
    }
    EdgeLockMap.clear();
    return ;
  }

  void Transaction::releaseLock()  {
    releaseVertexLock();
    releaseEdgeLock();
  }


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
    close();
  }
 
#endif /*_TRANSACTION_CPP_*/
