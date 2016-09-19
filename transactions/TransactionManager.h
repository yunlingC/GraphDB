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

  TransactionManager();

  ///TODO not used yet
  TransactionPointer  initTransaction();

  bool  addTransaction(unsigned int tid, TransactionPointer log);

  TransactionPointer addTransaction();

  bool  addTransaction(Query & query);

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
