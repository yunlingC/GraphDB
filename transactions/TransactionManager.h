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

#include <map>
#include <shared_mutex>

#define _TRANX_STATS_ true

class TransactionManager {
public:
  typedef Transaction *  TransactionPointer;
  typedef unsigned int IdType;
  typedef std::pair<IdType, TransactionPointer> TransactionEntryType;
  typedef std::map<IdType, TransactionPointer, std::less<IdType> > TransactionTableType;

#ifdef _TRANX_STATS_
  typedef Transaction::MutexMapType MutexMapType;
  typedef Transaction::MutexPairType MutexPairType;
  typedef std::map<int, int, std::less<int> > StatsMapType;
#endif
  
public:

  TransactionManager();

//  TransactionEntryType  addTransaction();

  TransactionEntryType  addTransaction(LocksManager & LkManager);

  TransactionPointer getTransaction(IdType TxId);

  ~TransactionManager();

#ifdef _TRANX_STATS_
  int getTotalAbortNum();
  MutexMapType getVisitedMap();
  MutexMapType getAbortedMap();
  void sumTx();
  void sumAbortNum();
  void sumAbortedMap();
  void sumVisitedMap();
  void sumStats();
  void dumpStats();
  friend class LocksManager;
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
  StatsMapType VisitStatsMap; 
  StatsMapType AbortStatsMap; 
  
#endif 

};

#endif /**_TRANSACTIONMANAGER_H_*/
