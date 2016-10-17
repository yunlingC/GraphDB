//===-- transactions/Transaction.cpp - RAG class type -----------*- C++ -*-===//
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

#ifndef _TRANSACTION_CPP_
#define _TRANSACTION_CPP_

#include "Transaction.h"

  Transaction::Transaction() : TransId(0), TransStatus(T_EXPANDING) {} 

  Transaction::Transaction(IdType id) : TransId(id), TransStatus(T_EXPANDING){} 

  Transaction::IdType Transaction::getId() 
  {
    return TransId;
  }

  void Transaction::commit() 
  {
      TransStatus = T_COMMIT;
  }

  void Transaction::abort() 
  {
      TransStatus = T_ABORT;
      exit(0);
  }

  bool Transaction::rollBack()
  {
      TransStatus = T_ROLLBACK;
      ///TODO roll back
      return true;
  }

  TransStatusType Transaction::checkStatus() 
  {
      return TransStatus;
  }

  Transaction::~Transaction(){}
 
#endif /*_TRANSACTION_CPP_*/
