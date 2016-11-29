//===-- transactions/Transaction.h - Log class --*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the main class for managing log records for transactions
///
//===----------------------------------------------------------------------===//

#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include "Concurrency_control_config.h"
#include "Lock.h"

#include <stdlib.h>
#include <unordered_map>

#define _TRANX_STATS_ true

class Transaction {
public:
  typedef unsigned int IdType;
#ifdef _TRANX_STATS_
  typedef VertexLock::MutexPointer MutexPointer;
  typedef std::pair<MutexPointer, int> MutexPairType;
  typedef std::unordered_map<MutexPointer, int> MutexMapType;
#endif
public:
  Transaction(); 

  Transaction(IdType id); 

  IdType getId();

  bool begin();

  bool expand();

  bool commit();

  bool abort(); 

  bool  rollBack();

  TransStatusType checkStatus();

  ~Transaction();

#ifdef _TRANX_STATS_
  void visitMutex(MutexPointer MutexPtr);
  void abortMutex(MutexPointer MutexPtr);
  int getAbortNum();
  MutexMapType getAbortedMap();
  MutexMapType getVisitedMap();
  void dumpAbortNum();
  void dumpAbortedMap();
  void dumpVisitedMap();
  void dumpStats();
#endif

protected:
  IdType  TransId;
  TransStatusType TransStatus;

#ifdef _TRANX_STATS_
  int NumAbort;
  MutexMapType AbortedMap;
  MutexMapType VisitedMap;
  void initStats();
#endif 
};

#endif /*_TRANSACTION_H_*/
