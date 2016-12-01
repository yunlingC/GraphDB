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
#include "Concurrency_control_config.h"
#include "global.h"

///std=c++14 
#include <unordered_map>
#include <set>
#include <stack>
#include <iostream>

/// currently PLock is only supported in _LOCKING_STORAGE_
/// i.e. in the LockMap we are still use shared_mutex from C++ lib

class LocksManager {
public:
  typedef std::shared_timed_mutex Mutex;
  typedef std::shared_ptr<std::mutex> ExMutexPointer;
  typedef VertexLock::MutexPointer  MutexPointer;
  typedef MutexPointer  LockPointer;
  typedef GraphType::VertexPointer VertexPtr;
  typedef GraphType::EdgePointer EdgePtr;
  typedef unsigned int  IdType;
  typedef std::unordered_map<IdType, VertexLock>  VertexLockMapType;
  typedef std::unordered_map<IdType, EdgeLock>    EdgeLockMapType;
  typedef std::pair<IdType, VertexLock> VLockPair;
  typedef std::pair<IdType, EdgeLock>   ELockPair;
  typedef std::vector<std::pair<VertexPtr,std::pair<MutexType,LockType> > > VLockListType; 
  typedef std::vector<std::pair<EdgePtr, std::pair<MutexType, LockType> > > ELockListType; 
  typedef std::set<LockPointer> LockListType;
  typedef std::unordered_map<LockPointer, LockType> LockMapType;
  typedef std::pair<LockPointer, LockType> LockEntryType;
  typedef std::set<LockType> LockTpSetType;
  typedef std::unordered_map<IdType,  LockTpSetType> TransMapType;
  typedef std::unordered_map<IdType, LockMapType> TransactionResourceMap;
  typedef std::pair<IdType, LockMapType> TxRrEntryType;
  typedef std::unordered_map<LockPointer, TransMapType> ResourceTransactionMap;
  typedef std::unordered_map<LockPointer, ExMutexPointer> ResourceGuardMapType;
  typedef std::pair<LockPointer, ExMutexPointer> ResourceGuardPairType;
  typedef std::unordered_map<IdType, LockPointer> WaitingTransactionMap;
  typedef std::unordered_map<IdType, ExMutexPointer> WaitGuardMapType;
  typedef std::pair<IdType, ExMutexPointer> WaitGuardPairType;
  typedef std::stack<IdType> TransStackType; 
  typedef std::set<IdType> TransSetType;
  typedef std::pair<bool, LockPointer> LockRetPairType;
  typedef std::unordered_set<ExMutexPointer> LockGuardSetType;
public:

#ifndef _LOCKING_STORAGE_
/// locks stored in a map 
  LocksManager();

  /// TODO getVertexLock has to do check if current tx has hold this lock
  /// So does getEdgeLock
  bool  getVertexLock(IdType VertexId, MutexType Mutex, LockType Lock); 

  void  releaseVertexLock(IdType VertexId, MutexType Mutex, LockType Lock);

  bool  getEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock); 

  void  releaseEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock);

  void  releaseEdgeAll(ELockListType & EdgeLocks); 

  void  releaseVertexAll(VLockListType & VertexLocks); 

  void  releaseAll(VLockListType & VertexLocks, ELockListType & EdgeLocks); 

  void  addToVertexLockMap(IdType VertexId); 

  void  addToEdgeLockMap(IdType EdgeId); 
 
  void  buildLockMap(GraphType & Graph); 
  
  VertexLockMapType getVertexLockMap(); 

  EdgeLockMapType getEdgeLockMap(); 

  LockRetPairType requireVertexLock(IdType VertexId, MutexType Mutex, LockType Lock);

  LockRetPairType requireEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock);

  MutexPointer  getVertexLockPointer(IdType VertexId, MutexType Mutex);

  MutexPointer  getEdgeLockPointer(IdType VertexId, MutexType Mutex);

  /*  New functions added to support deadlock detection */
//  bool getLock(IdType ObjectId, MutexType Mutex, LockType Lock, IdType TxId);

#ifdef _DEADLOCK_DETECTION_
  bool getVertexLock(IdType VertexId, MutexType Mutex, LockType Lock, IdType TxId);

  bool getEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock, IdType TxId);

  bool getLock(MutexPointer MutexPtr, LockType Lock, IdType TxId);

//  bool releaseVertexLock(IdType VertexId, MutexType Mutex, LockType Lock, IdType TxId);

//  bool releaseEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock, IdType TxId);

  bool detectDeadlock();

	LockRequestRetType  checkWaitOn(IdType, LockPointer, LockType);

  /// Yes - wait  No - deadlock
  bool  checkWaitOnRecursive(IdType, IdType, TransStackType, TransSetType, LockGuardSetType );

  void  dismissGuard(LockGuardSetType & Guards);

  bool  releaseAll(IdType TxId);

  bool  registerWaitingMap(IdType TransId,  LockPointer  LockPtr); 

  bool  retireFromWaitingMap(IdType TransId, LockPointer LockPtr);

  bool  registerTransMap(IdType TransId,  LockPointer  LockPtr, LockType LType);

  bool  retireFromTransMap(IdType TransId,  LockPointer  LockPtr, LockType LType);

  bool  registerLockMap(IdType TransId, LockPointer  LockPtr,  LockType  LType);

  bool  retireFromLockMap(IdType TransId, LockPointer  LockPtr,  LockType  LType);

  bool  registerToMap(IdType TransId,  LockPointer  LockPtr, LockType LType);

  bool  retireFromMap(IdType TransId,  LockPointer  LockPtr, LockType LType);

  bool  checkTransMap(IdType TransId,  LockPointer  LockPtr, LockType LType);

  bool  upgradeLock(IdType TransId, LockPointer LockPtr);

#ifdef _DEBUG_PRINT_
  void  dumpMaps();
  void  dumpTransMap();
  void  dumpResrMap();
  void  dumpTransMap(IdType TransId);
#endif
#endif /*_DEADLOCK_DETECTION_*/

  /// HOW to get lock and avoid deadlock
  /// TODO Check if lock exists from LockManager
  /// If yes, check if this lock can be acquired 
  ///     If yes, get lock and go on;
  ///     If no, check if spining on this lock will result in a deadlock
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

  void  releaseVertexLock(VertexPtr Vertex, MutexType Mutex, LockType Lock);

  bool  getEdgeLock(EdgePtr Edge, MutexType Mutex, LockType Lock); 

  void  releaseEdgeLock(EdgePtr Edge, MutexType Mutex, LockType Lock); 

  void  releaseEdgeAll(ELockListType & EdgeLocks); 

  void  releaseVertexAll(VLockListType & VertexLocks);

  void  releaseAll(VLockListType & VertexLocks, ELockListType & EdgeLocks); 

  void  addToVertexLockMap(IdType VertexId);

  void  addToEdgeLockMap(IdType EdgeId);
 
  void  buildLockMap(GraphType & Graph); 

#endif

  // LockType can be either SH or EX
  bool  tryLock(MutexPointer MutexPtr, LockType LType);

  void  tryUnlock(MutexPointer MutexPtr, LockType LType);

protected:

#ifndef _LOCKING_STORAGE_
  VertexLockMapType VertexLockMap;
  EdgeLockMapType EdgeLockMap;
#else 
  GraphType & Graph;
#endif

#ifdef _DEADLOCK_DETECTION_
  ExMutexPointer DeadlockDetector;
	TransactionResourceMap  TransMap;
	ResourceTransactionMap  ResrMap;
  WaitingTransactionMap WaitMap;
  ResourceGuardMapType  ResrGuardMap;
  WaitGuardMapType   WaitGuardMap;

#else 

#endif /*_DEADLOCK_DETECTION_ */
    
};

#endif /*_LOCKSMANAGER_H_*/
