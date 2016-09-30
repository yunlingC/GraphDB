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

  Transaction::Transaction() {

  }

  Transaction::~Transaction() {

  }

/** Not needed so far
 * TODO delete
auto Transaction::getVertexLock(VertexPointer &Vertex, MutexType Mutex,
                                LockType Lock) 
  ->  bool  {
	///TODO

}

auto Transaction::getEdgeLock(EdgePointer &Edge, MutexType Mutex,
                              LockType Lock) 
  ->  bool  {
	/// TODO
}

auto Transaction::registerVertexLock(VertexPointer &Vertex, MutexType Mutex,
                                     LockType Lock) 
  ->  void  {
	/// TODO
}

auto Transaction::registerEdgeLock(EdgePointer &Edge, MutexType Mutex,
                                   LockType Lock) 
  ->  void  {
	/// TODO
}

auto  Transaction::releaseAll()  
    ->  bool  {
    
}
*/



#endif /*_TRANSACTION_CPP_*/
