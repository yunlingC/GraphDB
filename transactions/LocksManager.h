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
#include <stack>
#include <iostream>

enum MutexType { T_ID, T_Property, T_LastEdge, T_NextEdge, T_FirstVertex, T_SecondVertex, T_FirstNextEdge, T_FirstPrevEdge, T_SecondNextEdge, T_SecondPrevEdge, T_Label};
enum LockType { T_SH, T_EX };
enum DLRetType { T_Abort,  T_Ignore,  T_Upgrade, T_Wait};

/// currently PLock is only supported in _LOCKING_
/// i.e. in the LockMap we are still use shared_mutex from C++ lib
/// TODO: support PLock with LockMap 
/// but why?

class LocksManager {
public:
  typedef std::shared_timed_mutex Mutex;
  typedef VertexLock::MutexPointer  MutexPointer;
  typedef MutexPointer  LockPointer;
  typedef GraphType::VertexPointer VertexPtr;
  typedef GraphType::EdgePointer EdgePtr;
  typedef unsigned int  IdType;
  typedef std::unordered_map<IdType, VertexLock>  VertexLockMapType;
  typedef std::unordered_map<IdType, EdgeLock>    EdgeLockMapType;
  typedef std::pair<IdType, VertexLock> VLockPair;
  typedef std::pair<IdType, EdgeLock>   ELockPair;
  typedef std::vector<std::pair<VertexPtr, std::pair<MutexType, LockType> > > VLockListType; 
  typedef std::vector<std::pair<EdgePtr, std::pair<MutexType, LockType> > > ELockListType; 
  typedef std::set<LockPointer> LockListType;
  typedef std::unordered_map<IdType,  LockType> TransMapType;
  typedef std::unordered_map<IdType, LockListType> TransactionResourceMap;
  typedef std::unordered_map<LockPointer, TransMapType>ResourceTransactionMap;
  typedef std::unordered_map<IdType, LockPointer> WaitingTransactionMap;
  typedef std::stack<IdType> TransStackType; 
  typedef std::set<IdType> TransSetType;
  typedef std::pair<bool, LockPointer> LockRetPairType;
public:

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
  
  VertexLockMapType getVertexLockMap(); 

  EdgeLockMapType getEdgeLockMap(); 

  /*  New functions added to support deadlock detection */
//  bool getLock(IdType ObjectId, MutexType Mutex, LockType Lock, IdType TxId);

  bool getVertexLock(IdType VertexId, MutexType Mutex, LockType Lock, IdType TxId);

  bool getEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock, IdType TxId);

  LockRetPairType requireVertexLock(IdType VertexId, MutexType Mutex, LockType Lock);

  LockRetPairType requireEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock);

//  bool releaseVertexLock(IdType VertexId, MutexType Mutex, LockType Lock, IdType TxId);

//  bool releaseEdgeLock(IdType EdgeId, MutexType Mutex, LockType Lock, IdType TxId);

	DLRetType  checkWaitOn(IdType, LockPointer, LockType);

  /// Yes - wait  No - deadlock
  bool  checkWaitOnRecursive(IdType, IdType, TransStackType, TransSetType);

  /// TODO lock
  bool  releaseAll(IdType TxId);

  /// TODO lock
  bool  registerWaitingMap(IdType TransId,  LockPointer  LockPtr); 

  /// TODO lock
  bool  registerTransMap(IdType TransId,  LockPointer  LockPtr);

  /// TODO lock
  bool  registerLockMap(IdType TransId, LockPointer  LockPtr,  LockType  LType);

  bool  registerToMap(IdType TransId,  LockPointer  LockPtr, LockType LType);

  bool  upgradeLock(IdType TransId, LockPointer LockPtr);
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
  std::mutex  DeadlockDetector;
	TransactionResourceMap  TransMap;
	ResourceTransactionMap  ResrMap;
  WaitingTransactionMap WaitMap;
#endif /*_DL_DETECTION_ */
    
};

#endif /*_LOCKSMANAGER_H_*/
