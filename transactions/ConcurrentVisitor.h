//===-- transactions/ConcurrenctVisitor.h -  visitor class ----------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the base class for concurrent visitors.
///
//===----------------------------------------------------------------------===//
#ifndef _CONCURRENTVISITOR_H_
#define _CONCURRENTVISITOR_H_

#include "LocksManager.h"
#include "TransactionManager.h"

class ConcurrentVisitor : public Visitor {
public:
  typedef Transaction * TransactionPointer;
  typedef LocksManager   LockManagerType;
public:
//  ConcurrentVisitor(LockManagerType lm) : LockManager(lm) {};
//  ConcurrentVisitor() {};

//  void requestLockManager(LockManagerType & lm) {
//    LockManager = lm;
//  }

  void requestTransaction(TransactionPointer log) {
    TxLog = log;
  }

  void setSleepTime(unsigned int time) {
    SleepTime = time;
  }

protected:
  unsigned int SleepTime = 1;
  ///TODO need another variable to record 
  // whether lockManager is requested
//  LockManagerType  LockManager;
  TransactionPointer TxLog;
};

#endif /*_CONCURRENTVISITORS_H_ */
