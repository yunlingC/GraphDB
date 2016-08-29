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

#include "GraphType.h"
#include "LocksManager.h"
//#include "TransactionManager.h"

#include <map>

/// TODO to be deleted
//enum OperationType { NOOP, READ, UPDATE, INSERT, DELETE };

class Transaction {
public:
  typedef GraphType::VertexPointer Vertex;
  typedef GraphType::EdgePointer   Edge;
  typedef GraphType::VertexPropertyList VProp;
  typedef GraphType::EdgePropertyList EProp;
  typedef std::pair<Vertex, Vertex> VertexPair;
  typedef std::pair<Edge,   Edge> EdgePair;
  typedef std::pair<VProp,  VProp> VPropPair;
  typedef std::pair<EProp,  EProp> EPropPair;
  typedef std::vector<VertexPair> VertexPairListType;
  typedef std::vector<EdgePair>   EdgePairListType;
  typedef std::vector<VPropPair> VertexPropPairListType;
  typedef std::vector<EPropPair> EdgePropPairListType;

  typedef pair<VertexPointer, pair<MutexType, LockType> >  VLockPair;
  typedef pair<EdgePointer, pair<MutexType, LockType> >  ELockPair;
  typedef vector<VLockPair> VLockListType;
  typedef vector<ELockPair> ELockListType;

public:
  Transaction () : Commit(false), Abort(false), OpType(NOOP) { }

  auto setOperationType (OperationType opT)
    -> void {
      OpType = opT; 
  }

//  auto requireTxId(TransactionManager & TxManager, unsigned int id)
  auto requireTxId(unsigned int id)
    -> void {
      Id = id;
  }

  auto getTxId() 
    -> unsigned int {
    return Id;
  }

 // auto setVertexPropertyPair(const VPropPair & vp)
 //   -> void {
 //     VertexPropList.push_back(vp);
 //   }

  auto setCommit() 
    -> void {
      Commit = true;
  }

  auto setAbort() 
    -> void {
      Abort = true;
  }

  auto checkStatus() 
    -> bool {
    /// no need to rollback
    if(Commit)  
      return true;
    /// must rollback
    if(Abort)
      return false;
    /// could be not finished, no rollback
    return true;
  }

  auto checkOperationType()
    -> OperationType {
    return OpType;
  }

    auto getVertexLockList(VLockListType & VertexLocks)
            -> void {
      VertexLockList  = VertexLocks;
    }

    auto getEdgeLockList(ELockListType & EdgeLocks)
            -> void {
      EdgeLockLis = EdgeLocks;
    }

    /// In the following functions, ask LockManager for locks and invoke RagManager for deadlock detection
    /// throw exception if waiting for lock is not a good decision
	bool getVertexLock(VertexPointer & Vertex, MutexType Mutex, LockType Lock);
    bool getEdgeLock(EdgePointer & Edge, MutexType Mutex, LockType Lock);

    void registerVertexLock(VertexPointer & Vertex, MutexType Mutex, LockType Lock );
    void registerEdgeLock(EdgePointer & Edge, MutexType Mutex, LockType Lock);

	bool releaseAll();

  ~Transaction() { }

protected:
  unsigned int Id;
  bool Commit ;
  bool Abort ;
//  OperationType  OpType;
//  VertexPairListType VertexList;
//  EdgePairListType   EdgeList;
//  VertexPropPairListType VertexPropList;
//  EdgePropPairListType EdgePropList;
  VLockListType VertexLockList;
  ELockListType EdgeLockList;
};

#endif /*_TRANSACTION_H_*/
