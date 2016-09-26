//===-- transaction/LocksManager.h - Lock class type -------------*- C++ -*-===//
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

#ifndef _LOCKSMANAGER_H_
#define _LOCKSMANAGER_H_

#include "GraphType.h"
#include "Lock.h"
#include "Transaction.h"

///std=c++14 
#include <unordered_map>
#include <set>
#include <iostream>

/// TODO full name
enum MutexType { ID, Pp, LE, NE, FV, SV, FNE, FPE, SNE, SPE, LB};
enum LockType { SH, EX };
enum DLRetType {T_Abort,  T_Ignore,  T_Wait};

/// currently PLock is only supported in _LOCKING_
/// i.e. in the LockMap we are still use shared_mutex from C++ lib
/// TODO: support PLock with LockMap

/// TODO waitingmap
/// resolve deadlock 
/// 
class LocksManager {
public:
  typedef std::shared_timed_mutex Mutex;
  typedef VertexLock::MutexPointer  MutexPointer;
  typedef GraphType::VertexPointer VertexPtr;
  typedef GraphType::EdgePointer EdgePtr;
  typedef std::unordered_map<IdType, VertexLock>  VertexLockMapType;
  typedef std::unordered_map<IdType, EdgeLock>    EdgeLockMapType;
  typedef std::pair<IdType, VertexLock> VLockPair;
  typedef std::pair<IdType, EdgeLock>   ELockPair;
  typedef std::vector<std::pair<VertexPtr, std::pair<MutexType, LockType> > > VLockListType; 
  typedef std::vector<std::pair<EdgePtr, std::pair<MutexType, LockType> > > ELockListType; 
  typedef std::set<Lock> LockListType;
  typedef unsigned int  IdType;
  typedef std::unordered_map<Transaction*,  LockType> TransMapType;
  typedef std::unordered_map<Transaction*, LockListType> TransactionResourceMap;
  typedef std::unordered_map<Lock, TransMapType>ResourceTransactionMap;
  typedef std::unordered_map<Transaction*, Lock> WaitingTransactionMap;
  typedef std::stack<IdType> TransStackType; 
  typedef std::set<IdType> TransSetType;
public:
/// TODO declaration
//  friend class RagManager;
#ifndef _LOCKING_
/// locks stored in a map
  LocksManager() {}

  /// TODO getVertexLock has to do check if current tx has hold this lock
  /// So does getEdgeLock
  bool  getVertexLock(IdType VertexId, MutexType Mutex, LockType Lock); 

  bool  releaseVertexLock(IdType VertexId, MutexType Mutex, LockType Lock);

  bool  getEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock); 

  bool  releaseEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock);

  void  releaseEdgeAll(ELockListType & EdgeLocks); 

  void  releaseVertexAll(VLockListType & VertexLocks); 

  void  releaseAll(VLockListType & VertexLocks, ELockListType & EdgeLocks); 

  void  addToVertexLockMap(IdType VertexId); 

  void  addToEdgeLockMap(IdType EdgeId); 
 
  void  buildLockMap(GraphType & Graph); 

  bool getVertexLock(IdType VertexId, MutexType Mutex, LockType Lock, IdType TxId);

  bool getEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock, IdType TxId);

  bool releaseVertexLock(IdType VertexId, MutexType Mutex, LockType Lock, IdType TxId);

  bool releaseEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock, IdType TxId);

	CheckRetType  checkWaitOn(IdType, Lock, LockType);

  /// Yes - wait  No - deadlock
  bool  checkWaitOnRecursive();

  VertexLockMapType getVertexLockMap(); 

  EdgeLockMapType getEdgeLockMap(); 

  /// TODO lock
  bool releaseAll(IdType TxId);

  /// TODO lock
  bool  registerWaitingMap(IdType TransId,  Lock  LockPtr); 

  /// TODO lock
  bool  registerTransMap(IdType TransId,  Lock  LockPtr);

  /// TODO lock
  bool  registerLockMap(IdType TransId, Lock  LockPtr,  LockType  LType);

  bool registerToMap(IdType TransId,  Lock  LockPtr, LockType LType);
  /// HOW to get lock and avoid deadlock
  /// TODO Check if lock exists from LockManager
  /// If yes, check if this lock can be acquired 
  ///     If no, get lock and go on;
  ///     If yes, check if spining on this lock will result in a deadlock
  ///         If no, wait for current lock
  ///         If yes, stop wait on this lock
  ///            Abort
  ///            Or other techniques e.g. pick up later
  /// getLock --> checkWaitOn --> registerWaitingMap  

#else
  ///locks are encoded in Vertex and Edge
public:
  /// TODO const & g
  LocksManager(GraphType & g) : Graph(g) {};

  bool  getVertexLock(VertexPtr Vertex, MutexType Mutex, LockType Lock);

  bool  releaseVertexLock(VertexPtr Vertex, MutexType Mutex, LockType Lock);

  bool  getEdgeLock(EdgePtr Edge, MutexType Mutex, LockType Lock); 

  bool  releaseEdgeLock(EdgePtr Edge, MutexType Mutex, LockType Lock); 

  void  releaseEdgeAll(ELockListType & EdgeLocks); 

  void  releaseVertexAll(VLockListType & VertexLocks);

  void  releaseAll(VLockListType & VertexLocks, ELockListType & EdgeLocks); 

  void  addToVertexLockMap(IdType VertexId);

  void  addToEdgeLockMap(IdType EdgeId);
 
  void  buildLockMap(GraphType & Graph); 

#endif

protected:
#ifndef _LOCKING_
  VertexLockMapType VertexLockMap;
  EdgeLockMapType EdgeLockMap;
#else 
  GraphType & Graph;
#endif

#ifdef _DL_DETECTION_
//	unsigned int DeadLockCount;
  /// TODO need lock for transMap, ResrMap, WaitMap separately
	TransactionResourceMap  TransMap;
	ResourceTransactionMap  ResrMap;
  WaitingTransactionMap WaitMap;
#endif /*_DL_DETECTION_ */
    
};

#endif /*_LOCKSMANAGER_H_*/
