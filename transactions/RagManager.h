//===-- transaction/RagManager.h - RAG class type -------------*- C++ -*-===//
//
//                     CAESR Graph Database
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the main class for resource-locks allocation graph management
///
//===----------------------------------------------------------------------===//


#ifndef _RAGMANAGER_H_
#define _RAGMANAGER_H_
#include "LocksManager.h"
#include "Transaction.h"

#include <unordered_map>

enum Lock {VertexLock, EdgeLock};

class RagManager {
public:
//  typedef unsigned int  Lock;
/// Lock also contains information on mutextype locktype
/// Lock + MutexType + LockType
  typedef std::unordered_map<Transaction*, Lock> TransactionResourceMap;
  typedef std::unordered_map<Lock, Transaction*> ResourceTransactionMap;

public:
  RagManager(LocksManager & lockM)  : LockManager(lockM){};

	unsigned int getDeadLockCount();
	bool  getVertexLock(VertexPtr Vertex);
  bool  getVertexLock(VertexPtr , MutexType, LockType);
	bool  getEdgeLock();
	bool  registerVertexLock();
	bool  registerEdgeLock();
	bool  checkWaitOn();
	bool  stopWaitOn();

///TODO declare LockManager and TransactionManager as friends or ...
private:
  LocksManager & LockManager;
	unsigned int  DeadLockCount;
	TransactionResourceMap  TranxMap;
	ResourceTransactionMap  ResrMap;

};

#endif /*_RAGMANAGER_H_ */
