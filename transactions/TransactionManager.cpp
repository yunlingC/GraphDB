//===-- transactions/TransactionManager.cpp - TransactionManager--*- C++-*-===//
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

#include <iostream>

  TransactionManager::TransactionManager() : TransNumber(0) {
    TransIdGuard = std::shared_ptr<std::mutex>(new std::mutex);
  }

  auto TransactionManager::assignTransId()  
    -> IdType {
      IdType id = 0;
      TransIdGuard->lock();
      id = TransNumber++;
      TransIdGuard->unlock();
      return id;
  }

  auto TransactionManager::addTransaction()
  -> TransactionEntryType {
      auto Id = assignTransId();
//      std::cout <<"add transaction " << Id << std::endl;
  		TransactionPointer TxPtr = new Transaction(Id);
  		TransTable.insert(std::pair<IdType, TransactionPointer>(Id, TxPtr));
  		return TransactionEntryType(Id, TxPtr);
  }
  
/// TODO exception 
  auto TransactionManager::getTransaction(IdType TxId)
    -> TransactionPointer {
      TransactionPointer TxPtr = nullptr;
  		if (TransTable.find(TxId) == TransTable.end()) return TxPtr;
      return TransTable.at(TxId);
  }

  int TransactionManager::getTotalAbortNum() {
    int Sum = 0;
    for (auto Tx : TransTable) {
      Sum += Tx.second->getAbortNum();
    }
    return Sum;
  }

  void TransactionManager::SumVisitedMap() {
    totalVisitedMap.clear();
    for (auto Tx : TransTable)  {
      auto Map = Tx.second->getVisitedMap();
      for (auto Entry : Map)  {
        if (totalVisitedMap.find(Entry.first) == totalVisitedMap.end()) {
          totalVisitedMap.insert(MutexPairType(Entry.first, 0));
        }
        totalVisitedMap[Entry.first]+= Entry.second;
      }
    }
    return;
  }

  void TransactionManager::SumAbortedMap() {
    totalAbortedMap.clear();
    for (auto Tx : TransTable)  {
      auto Map = Tx.second->getAbortedMap();
      for (auto Entry : Map)  {
        if (totalAbortedMap.find(Entry.first) == totalAbortedMap.end()) {
          totalAbortedMap.insert(MutexPairType(Entry.first, 0));
        }
        totalAbortedMap[Entry.first]+= Entry.second;
      }
    }
    return;
  }

  TransactionManager::MutexMapType TransactionManager::getVisitedMap() {
    return totalVisitedMap;
  }

  TransactionManager::MutexMapType TransactionManager::getAbortedMap() {
    return totalAbortedMap;
  }
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
//		}
//
//		auto TransactionManager::undoDelete(GraphType & graph, TransactionPointer
//		log)
//		->  bool  {
//				///TODO
//		}

#endif /**_TRANSACTIONMANAGER_CPP_*/
