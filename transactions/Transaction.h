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

#include "Concurrency_control_config.h"

#include <stdlib.h>

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
#ifdef _DEADLOCK_DETECTION_
  /// TODO dependency list <transaction_ptr>
#endif
};

#endif /*_TRANSACTION_H_*/
