//===-- transactions/TransactionManager.h - transaction class --*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the main class for managing transactions
///
//===----------------------------------------------------------------------===//

#ifndef _TRANSACTIONMANAGER_H_
#define _TRANSACTIONMANAGER_H_

#include "Transaction.h"

#include <unordered_map>
#include <shared_mutex>

class TransactionManager {
public:
  typedef Transaction *  TransactionPointer;
  typedef Transaction::MutexMapType MutexMapType;
  typedef Transaction::MutexPairType MutexPairType;
  
  typedef unsigned int IdType;
  typedef std::pair<IdType, TransactionPointer> TransactionEntryType;
  typedef std::unordered_map<IdType, TransactionPointer> TransactionTableType;
public:

  TransactionManager();

  TransactionEntryType  addTransaction();

  TransactionPointer getTransaction(IdType TxId);

  ~TransactionManager();

#ifdef _TRANX_STATS_
  int getTotalAbortNum();
  MutexMapType getVisitedMap();
  MutexMapType getAbortedMap();
  void SumAbortedMap();
  void SumVisitedMap();
#endif 

private:
  IdType assignTransId();

//  bool  undoUpdate(GraphType & graph, TransactionPointer log);
//
//  bool  undoInsert(GraphType & graph, TransactionPointer log);
//
//  bool  undoDelete(GraphType & graph, TransactionPointer log);

protected:
  std::shared_ptr<std::mutex> TransIdGuard;
  IdType TransNumber;
  TransactionTableType TransTable; 

#ifdef _TRANX_STATS_
  int totalAbortNum;
  MutexMapType totalVisitedMap;
  MutexMapType totalAbortedMap;
  
#endif 

};

#endif /**_TRANSACTIONMANAGER_H_*/
