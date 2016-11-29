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
#include "GraphType.h"
#include "LocksManager.h"
#include "global.h"

#include <stdlib.h>
#include <unordered_map>

#define _TRANX_STATS_ true

class Transaction {
public:
  typedef unsigned int IdType;
  typedef GraphType::VertexPointer VertexPointer;
  typedef GraphType::EdgePointer EdgePointer;
  typedef VertexLock::MutexPointer MutexPointer;
  typedef std::pair<VertexPointer, LockType> VLockPairType;
  typedef std::pair<EdgePointer, LockType> ELockPairType;
  typedef std::pair<MutexPointer, VLockPairType> VertexLockPairType; 
  typedef std::unordered_map<MutexPointer, VLockPairType> VertexLockMapType; 
  typedef std::pair<MutexPointer, ELockPairType> EdgeLockPairType;
  typedef std::unordered_map<MutexPointer, ELockPairType> EdgeLockMapType;
#ifdef _TRANX_STATS_
  typedef std::pair<MutexPointer, int> MutexPairType;
  typedef std::unordered_map<MutexPointer, int> MutexMapType;
#endif
public:
  Transaction(); 

  Transaction(IdType id); 

  Transaction(IdType id, LocksManager & LockManager); 

  IdType getId();

  bool begin();

  bool expand();

  bool commit();

  bool abort(); 

  bool rollBack();

  void releaseVertexLock();

  void releaseEdgeLock();

  void releaseLock();

  bool registerVertexLock(MutexPointer , VertexPointer, LockType );

  bool registerEdgeLock(MutexPointer , EdgePointer, LockType );

  VertexLockMapType getVertexLockMap();

  EdgeLockMapType getEdgeLockMap();

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
  LocksManager & LockManager;
  IdType  TransId;
  TransStatusType TransStatus;
  EdgeLockMapType EdgeLockMap;
  VertexLockMapType VertexLockMap;
#ifdef _TRANX_STATS_
  int NumAbort;
  MutexMapType AbortedMap;
  MutexMapType VisitedMap;
  void initStats();
#endif 
};

#endif /*_TRANSACTION_H_*/
