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
  typedef std::unordered_map<unsigned int, TransactionPointer>
      TransactionTableType;
public:

  TransactionManager() : TransNumber(0);

  ///TODO not used yet
  auto initTransaction();

  auto addTransaction(unsigned int tid, TransactionPointer log);

  auto addTransaction();

  auto addTransaction(Query & query);

  auto rollBack(GraphType & graph);

  ~TransactionManager();

private:
  auto undoUpdate(GraphType & graph, TransactionPointer log);

  auto undoInsert(GraphType & graph, TransactionPointer log);

  auto undoDelete(GraphType & graph, TransactionPointer log);

protected:
  unsigned int TransNumber;
  TransactionTableType TransTable; 
};

#endif /**_TRANSACTIONMANAGER_H_*/
