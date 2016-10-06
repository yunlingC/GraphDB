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

#include <stdlib.h>

/// TODO to be deleted
enum OperationType { NOOP, READ, UPDATE, INSERT, DELETE };

/// TODO need T_WAITING ?
enum TransStatusType {T_COMMIT, T_ABORT, T_ROLLBACK, T_PROCESS};

class Transaction {
public:
  typedef unsigned int IdType;
public:
  Transaction(); 

  Transaction(IdType id); 

  IdType getId();

  void  commit();

  void  abort(); 

  bool  rollBack();

  TransStatusType checkStatus();

  ~Transaction();

protected:
  IdType  TransId;
  TransStatusType TransStatus;
};

#endif /*_TRANSACTION_H_*/
