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
#include <iostream>

/// TODO full name
enum MutexType { ID, Pp, LE, NE, FV, SV, FNE, FPE, SNE, SPE, LB};
enum LockType { SH, EX };

/// currently PLock is only supported in _LOCKING_
/// i.e. in the LockMap we are still use shared_mutex from C++ lib
/// TODO: support PLock with LockMap

class LocksManager {
public:
  typedef std::shared_timed_mutex Mutex;
  typedef VertexLock::MutexPointer  MutexPointer;
  typedef GraphType::VertexPointer VertexPtr;
  typedef GraphType::EdgePointer EdgePtr;
  typedef std::unordered_map<unsigned int, VertexLock>  VertexLockMapType;
  typedef std::unordered_map<unsigned int, EdgeLock>    EdgeLockMapType;
  typedef std::pair<unsigned int, VertexLock> VLockPair;
  typedef std::pair<unsigned int, EdgeLock>   ELockPair;
  typedef std::vector<std::pair<VertexPtr, std::pair<MutexType, LockType> > > VLockListType; 
  typedef std::vector<std::pair<EdgePtr, std::pair<MutexType, LockType> > > ELockListType; 
  typedef std::unordered_map<Transaction*, Lock> TransactionResourceMap;
  typedef std::unordered_map<Lock, Transaction*> ResourceTransactionMap;
public:
/// TODO declaration
  friend class RagManager;
#ifndef _LOCKING_
/// locks stored in a map
  LocksManager() {}

  bool  getVertexLock(unsigned int VertexId, MutexType Mutex, LockType Lock); 

  bool  releaseVertexLock(unsigned int VertexId, MutexType Mutex, LockType Lock);

  bool  getEdgeLock(unsigned int EdgeId, MutexType Mutex, LockType Lock); 

  bool  releaseEdgeLock(unsigned int EdgeId, MutexType Mutex, LockType Lock);

  void  releaseEdgeAll(ELockListType & EdgeLocks); 

  void  releaseVertexAll(VLockListType & VertexLocks); 

  void  releaseAll(VLockListType & VertexLocks, ELockListType & EdgeLocks); 

  void  addToVertexLockMap(unsigned int VertexId); 

  void  addToEdgeLockMap(unsigned int EdgeId); 
 
  void  buildLockMap(GraphType & Graph); 

	bool  checkWaitOn();

	bool  stopWaitOn();

  VertexLockMapType getVertexLockMap(); 

  EdgeLockMapType getEdgeLockMap(); 

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

  void  addToVertexLockMap(unsigned int VertexId);

  void  addToEdgeLockMap(unsigned int EdgeId);
 
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
	unsigned int  DeadLockCount;
	TransactionResourceMap  TranxMap;
	ResourceTransactionMap  ResrMap;
#endif /*_DL_DETECTION_ */
    
};

#endif /*_LOCKSMANAGER_H_*/
