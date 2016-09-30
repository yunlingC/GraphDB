//===-- transactions/TransactionManager.cpp - Tranx Manager--*- C++-*-===//
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

#ifndef _TRANSACTIONMANAGER_CPP_
#define _TRANSACTIONMANAGER_CPP_

#include "TransactionManager.h"

//  TransactionManager::TransactionManager() {}

  TransactionManager::TransactionManager() : TransNumber(0) {
			/// TODO create ragmanager
	}

		///TODO not used yet
  auto TransactionManager::initTransaction()
  -> TransactionPointer {
  		TransactionPointer log = new Transaction();
  		return log;
  }
  
  auto TransactionManager::addTransaction(IdType tid,
                                          TransactionPointer log)
  -> bool  {
  		if(TransTable.find(tid) != TransTable.end()) return false;
  		TransTable.insert(std::pair<IdType, TransactionPointer>(tid, log));
  		return true;
  }
  
  auto TransactionManager::addTransaction()
  -> TransactionPointer {
  		TransactionPointer log = new Transaction();
  		log->requireTxId(TransNumber);
  		TransTable.insert(std::pair<IdType, TransactionPointer>(TransNumber, log));
  		TransNumber++;
  		return log;
  }
  
  auto TransactionManager::getTransaction(IdType TxId)
    -> TransactionPointer {
  		if(TransTable.find(TxId) != TransTable.end()) return nullptr;
      return TransTable.at(TxId);
  }

//  auto TransactionManager::addTransaction(Query & query) 
//    ->  bool  {
//  	/// TODO
//  }

/**
		auto TransactionManager::rollBack(GraphType & graph)
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
*/
		TransactionManager::~TransactionManager() {
				/// TODO check transaction status before delete the table
				for(auto it = TransTable.begin();
				    it != TransTable.end(); it++) {
						delete (*it).second;
				}
		}

//		auto TransactionManager::undoUpdate(GraphType & graph, TransactionPointer
//		log)
//		->  bool {
//				///TODO
//		}
//
//		auto TransactionManager::undoInsert(GraphType & graph, TransactionPointer
//		log)
//		->  bool  {
//				///TODO
//
//		}
//
//		auto TransactionManager::undoDelete(GraphType & graph, TransactionPointer
//		log)
//		->  bool  {
//				///TODO
//
//		}

#endif /**_TRANSACTIONMANAGER_CPP_*/
