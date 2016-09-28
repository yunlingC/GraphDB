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

#include "GraphType.h"
#include "QueryDescription.h"
#include "Transaction.h"

#include <unordered_map>

class TransactionManager {
public:
  typedef Transaction *  TransactionPointer;
  typedef unsigned int IdType;
//  typedef RagManager  RagManagerType;
  typedef std::unordered_map<IdType, TransactionPointer>
      TransactionTableType;
public:

  TransactionManager();

//  TransactionManager(RagManagerType & RagM) : RagManager(RagM){};

  ///TODO not used yet
  TransactionPointer  initTransaction();

  bool  addTransaction(IdType TxId, TransactionPointer log);

  TransactionPointer addTransaction(IdType Id){};

  bool  addTransaction(Query & query);

  IdType  addTransaction();

  TransactionPointer getTransaction(IdType TxId){};

//  void  addTransaction(void (*TranxBreadthFirstSearch)(
//                      GraphType & graph
//                      , const GraphType::VertexDescriptor & StartVertex
//                      , Visitor & GraphVisitor
//                      , LocksManager & LockManager
//                      ));

  bool  rollBack(GraphType & graph);

  ~TransactionManager();

private:
//  bool  undoUpdate(GraphType & graph, TransactionPointer log);
//
//  bool  undoInsert(GraphType & graph, TransactionPointer log);
//
//  bool  undoDelete(GraphType & graph, TransactionPointer log);

protected:
//  RagManagerType  & RagManager;
  unsigned int TransNumber;
  TransactionTableType TransTable; 
};

#endif /**_TRANSACTIONMANAGER_H_*/
