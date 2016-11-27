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

#define _PRINTLOG_ true

#ifdef _PRINTLOG_
#include <iostream>
#endif

  Transaction::Transaction() : TransId(0), TransStatus(T_EXPANDING) {
    begin();
  } 

  Transaction::Transaction(IdType id) : TransId(id), TransStatus(T_EXPANDING){
    begin();
  } 

  Transaction::IdType Transaction::getId() 
  {
    return TransId;
  }

  bool Transaction::begin() {
    TransStatus = T_EXPANDING;
#ifdef _PRINTLOG_
    std::cout <<"Transaction\t" << TransId << "\tSTART\n";
#endif 
    return true;
  }

  bool Transaction::expand()  {
    TransStatus = T_EXPANDING;
#ifdef _PRINTLOG_
    std::cout <<"Transaction\t" << TransId << "\tEXPAND\n";
#endif 
    return true;
//    bool retValue = false; 
//    switch()
  }

  bool Transaction::commit() {
    bool retValue = false;
    switch(TransStatus) {
      case T_EXPANDING:
        TransStatus = T_SHRINKING;
        /// Comment the following line:
        /// This is because we are usig strict strong 2pl 
        /// Releasing locks is not allowed until we finishi committing
        /// break;
      case T_SHRINKING:
        TransStatus = T_COMMIT;
        retValue = true;
        break;
      case T_ABORT:
        TransStatus = T_ROLLBACK;
        break;
      case T_ROLLBACK:
        TransStatus = T_EXPANDING;
        break;
      case T_COMMIT:
        retValue = true;
        break;
    }
//    std::cout << TransId << "\t commits\n";
#ifdef _PRINTLOG_
    if (retValue)
    std::cout <<"Transaction\t" << TransId << "\tCOMMIT\n";
#endif 
    return retValue;
  }

  bool Transaction::abort() 
  {
    bool retValue = false;

    switch(TransStatus) {
      case T_EXPANDING:
        TransStatus = T_ABORT;
        retValue = true;
        break;
      case T_SHRINKING:
        TransStatus = T_ROLLBACK;
        /// Abort is not allowed in shrinking phase with SS2PL
#ifdef _PRINTLOG_
        std::cout <<"Error in Abort:\t" << TransId << "\tis Shrinking\n";
        exit(0);
#endif
        break;
      case T_ABORT:
//        TransStatus = T_ABORT;
#ifdef _PRINTLOG_
        std::cout <<"Error in Abort:\t" << TransId << "\tis ABORT\n";
        exit(0);
#endif
        break;
      case T_ROLLBACK:
        TransStatus = T_EXPANDING;
#ifdef _PRINTLOG_
        std::cout <<"Error in Abort:\t" << TransId << "\tis ROLLBACK\n";
        exit(0);
#endif
        break;
      case T_COMMIT:
#ifdef _PRINTLOG_
        std::cout <<"Error in Abort:\t" << TransId << "\tis COMMIT\n";
        exit(0);
#endif
        break;
    }
#ifdef _PRINTLOG_
    if (retValue)
    std::cout <<"Transaction\t" << TransId << "\tABORT\n";
#endif  
    return retValue;
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
