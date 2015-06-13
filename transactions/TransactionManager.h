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
//.
///
//===----------------------------------------------------------------------===//

#ifndef _TRANSACTIONMANAGER_H_
#define _TRANSACTIONMANAGER_H_

#include <map>

#include "GraphType.h"
#include "Transaction.h"

class TransactionManager {
public:
  typedef Transaction *  TransactionPointer;
  typedef std::map<unsigned int, TransactionPointer> TransactionTableType;
public:
  TransactionManager() : TransNumber(0) {}

  ///TODO not used yet
  auto initTransaction() 
    -> TransactionPointer {
      TransactionPointer log = new Transaction;
      return log;
  }

  auto addTransaction(unsigned int tid, TransactionPointer log) 
    -> bool  {
      if(TransTable.find(tid) != TransTable.end())
        return false;
      TransTable.insert(std::pair<unsigned int, TransactionPointer>(tid, log));
      return true;
  }

  auto addTransaction()
    -> TransactionPointer {
      TransactionPointer log = new Transaction;
      log->setTxId(TransNumber);
      TransTable.insert(std::pair<unsigned int, TransactionPointer>(TransNumber, log));
      TransNumber++;
      return log;
  }

  auto rollBack(GraphType & graph) 
    -> bool {
    for (auto it = TransTable.begin(); 
          it != TransTable.end(); it++) {
      if(!(*it).second->checkStatus()) {
        switch ((*it).second->checkOperationType()) {
          case NOOP:
            break;
          case UPDATE:
            undoUpdate(graph, (*it).second);
            break;
          case INSERT:
            undoInsert(graph, (*it).second);
            break;
          case DELETE:
            undoDelete(graph, (*it).second);
            break;
          default:
            break;
        }//switch
      }//if
    }
    return true;
  }

  ~TransactionManager() { 
    
    for(auto it = TransTable.begin(); 
        it != TransTable.end(); it++) {
      delete (*it).second;
    }
  }

private:
  auto undoUpdate(GraphType & graph, TransactionPointer log) 
    -> void {
  }

  auto undoInsert(GraphType & graph, TransactionPointer log) 
    -> void {
    
  }

  auto undoDelete(GraphType & graph, TransactionPointer log) 
    -> void {
    
  }

protected:
  unsigned int TransNumber;
  TransactionTableType TransTable; 
};

#endif /**_TRANSACTIONMANAGER_H_*/
