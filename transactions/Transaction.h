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

/// TODO to be deleted
enum OperationType { NOOP, READ, UPDATE, INSERT, DELETE };

class Transaction {
public:
	typedef GraphType::VertexPointer VertexPointer;
	typedef GraphType::EdgePointer    EdgePointer;
	typedef LocksManager::MutexPointer MutexPointer;
  typedef GraphType::VertexPropertyList VProp;
  typedef GraphType::EdgePropertyList EProp;
  typedef unsigned int IdType;
  typedef std::pair<VertexPointer, VertexPointer> VertexPair;
  typedef std::pair<EdgePointer,   EdgePointer> EdgePair;
  typedef std::pair<VProp,  VProp> VPropPair;
  typedef std::pair<EProp,  EProp> EPropPair;
//  typedef std::vector<VertexPair> VertexPairListType;
//  typedef std::vector<EdgePair>   EdgePairListType;
//  typedef std::vector<VPropPair> VertexPropPairListType;
//  typedef std::vector<EPropPair> EdgePropPairListType;

  typedef std::pair<VertexPointer, std::pair<MutexType, LockType> >  VLockPair;
  typedef std::pair<EdgePointer, std::pair<MutexType, LockType> >  ELockPair;
  typedef std::vector<VLockPair> VLockListType;
  typedef std::vector<ELockPair> ELockListType;

public:
  Transaction();
  Transaction (IdType id) : Commit(false), Abort(false), Id(id) {} 

//  auto requireTxId(TransactionManager & TxManager, unsigned int id)
  auto requireTxId(unsigned int id)
    -> void {
      Id = id;
  }

  auto getId() 
    -> IdType {
    return Id;
  }

//	auto setOperationType();
//	auto checkOperationType
//     -> OperationType ();

 // auto setVertexPropertyPair(const VPropPair & vp)
 //   -> void {
 //     VertexPropList.push_back(vp);
 //   }

  auto commit() 
    -> void {
      Commit = true;
  }

  auto abort() 
    -> void {
      Abort = true;
      exit(0);
  }

  auto checkStatus() 
    -> bool {
    /// no need to rollback
    /// TODO change
    if(Commit)  
      return true;
    /// must rollback
    if(Abort)
      return false;
    /// could be not finished, no rollback
    return true;
  }


//    auto getVertexLockList(VLockListType & VertexLocks)
//            -> void {
//      VertexLockList  = VertexLocks;
//    }
//
//    auto getEdgeLockList(ELockListType & EdgeLocks)
//            -> void {
//      EdgeLockList = EdgeLocks;
//    }

    /// In the following functions, ask LockManager for locks and invoke RagManager for deadlock detection
    /// throw exception if waiting for lock is not a good decision
  
//    bool getVertexLock(VertexPointer & Vertex, MutexType Mutex, LockType Lock);
//    bool getEdgeLock(EdgePointer & Edge, MutexType Mutex, LockType Lock);
//
//    void registerVertexLock(VertexPointer & Vertex, MutexType Mutex, LockType Lock );
//    void registerEdgeLock(EdgePointer & Edge, MutexType Mutex, LockType Lock);
//
//		bool releaseAll();

		~Transaction();

protected:
  bool Commit ;
  bool Abort ;
  IdType  Id;
//  VertexPairListType VertexList;
//  EdgePairListType   EdgeList;
//  VertexPropPairListType VertexPropList;
//  EdgePropPairListType EdgePropList;
//  VLockListType VertexLockList;
//  ELockListType EdgeLockList;
};

#endif /*_TRANSACTION_H_*/
