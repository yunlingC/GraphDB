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
public:

  void requestTransaction(TransactionPointer log) {
    TxLog = log;
  }

protected:
  ///TODO need another variable to record 
  /// whether lockManager is requested
  //  LockManagerType  LockManager;
  TransactionPointer TxLog;
};

#endif /*_CONCURRENTVISITORS_H_ */
