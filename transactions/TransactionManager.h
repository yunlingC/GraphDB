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

#include "Transaction.h"

#include <unordered_map>

class TransactionManager {
public:
  typedef Transaction *  TransactionPointer;
  typedef unsigned int IdType;
  typedef std::unordered_map<IdType, TransactionPointer> TransactionTableType;
public:

  TransactionManager();

  ///TODO not used yet
  TransactionPointer  initTransaction();

  bool  addTransaction(IdType TxId, TransactionPointer log);

  TransactionPointer addTransaction(IdType Id);

//  IdType  addTransaction();

  TransactionPointer  addTransaction();

  TransactionPointer getTransaction(IdType TxId);

  bool  rollBack(GraphType & graph);

  ~TransactionManager();

private:
//  bool  undoUpdate(GraphType & graph, TransactionPointer log);
//
//  bool  undoInsert(GraphType & graph, TransactionPointer log);
//
//  bool  undoDelete(GraphType & graph, TransactionPointer log);

protected:
  unsigned int TransNumber;
  TransactionTableType TransTable; 
};

#endif /**_TRANSACTIONMANAGER_H_*/
