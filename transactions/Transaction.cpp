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

#define _PRINTLOG_ true

#ifdef _PRINTLOG_
#include <iostream>
#endif

#define CLOCK_ID  CLOCK_THREAD_CPUTIME_ID
#define MILLION 1000000
#define NANO 1000000000
#define SCALE 1000

//  Transaction::Transaction() : TransId(0), TransStatus(T_EXPANDING) {

//  Transaction::Transaction(IdType id) : TransId(id), TransStatus(T_EXPANDING){

  Transaction::Transaction(IdType id, LocksManager & LkManager) : LockManager(LkManager)
                                                                    ,TransId(id)
                                                                    , TransStatus(T_EXPANDING){
#ifdef _TRANX_STATS_
    initStats();
#endif
    begin();
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
        exit(0);
#endif
        TransStatus = T_ROLLBACK;
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


  bool Transaction::registerVertexLock(MutexPointer mptr, VertexPointer vptr, LockType lktype) {
    if (VertexLockMap.find(mptr) != VertexLockMap.end())  {
      /// This mutex exists in the map -> Transaction already holds this lock
      return false;
    }
    VertexLockMap.insert(VertexLockPairType(mptr, VLockPairType(vptr, lktype)));
    return true;
  }

  bool Transaction::registerEdgeLock(MutexPointer mptr, EdgePointer eptr, LockType lktype) {
    if (EdgeLockMap.find(mptr) != EdgeLockMap.end())  {
      return false;
    }
    EdgeLockMap.insert(EdgeLockPairType(mptr, ELockPairType(eptr, lktype)));
    return true;
  }


  void Transaction::releaseVertexLock( ) {
    for (auto LockEntry : VertexLockMap) {
      LockManager.tryUnlock(LockEntry.first, LockEntry.second.second);
#ifndef _NO_WAIT_
      LockManager.retireFromLockMap(TransId, LockEntry.first, LockEntry.second.second);
#endif
    }
    return ;
  }

  void Transaction::releaseEdgeLock( ) {
    for (auto LockEntry : EdgeLockMap) {
      LockManager.tryUnlock(LockEntry.first, LockEntry.second.second);
#ifndef _NO_WAIT_
      LockManager.retireFromLockMap(TransId, LockEntry.first, LockEntry.second.second);
#endif
    }
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

#endif

  Transaction::~Transaction(){}
 
#endif /*_TRANSACTION_CPP_*/
