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

  Transaction::Transaction() : TransId(0), TransStatus(T_PROCESS) {} 

  Transaction::Transaction(IdType id) : TransId(id), TransStatus(T_PROCESS){} 

  auto Transaction::getId() 
    -> IdType {
    return TransId;
  }

  auto Transaction::commit() 
    -> void {
      TransStatus = T_COMMIT;
  }

  auto Transaction::abort() 
    -> void {
      TransStatus = T_ABORT;
      exit(0);
  }

  auto Transaction::rollBack()
    -> bool {
      TransStatus = T_ROLLBACK;
      ///TODO roll back
      return true;
  }

  auto Transaction::checkStatus() 
    -> TransStatusType  {
      return TransStatus;
  }

  Transaction::~Transaction(){}
 
#endif /*_TRANSACTION_CPP_*/
