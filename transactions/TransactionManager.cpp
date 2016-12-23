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

#ifdef _TRANX_STATS_
#include <iostream>
#endif

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
  		TransactionPointer TxPtr = new Transaction(Id);
  		TransTable.insert(std::pair<IdType, TransactionPointer>(Id, TxPtr));
  		return TransactionEntryType(Id, TxPtr);
  }
  
  auto TransactionManager::getTransaction(IdType TxId)
    -> TransactionPointer {
      TransactionPointer TxPtr = nullptr;
  		if (TransTable.find(TxId) == TransTable.end()) return TxPtr;
      return TransTable.at(TxId);
  }

#ifdef _TRANX_STATS_

  float TransactionManager::getPercent(uint64_t top, uint64_t base) {
    return (float)(100 * ((float) top/ (float)(base)));
  }

  int TransactionManager::getTotalAbortNum() {
    return totalAbortNum;
  }
  
  void TransactionManager::sumAbortNum()  {
    totalAbortNum= 0;
    for (auto Tx : TransTable) {
      totalAbortNum += Tx.second->getAbortNum();
      Tx.second->dumpAbortNum();
    }
    return ;
  }

  void TransactionManager::sumVisitedMap() {
    totalVisitedMap.clear();
    for (auto Tx : TransTable)  {
      auto Map = Tx.second->getVisitedMap();
      for (auto Entry : Map)  {
        if (totalVisitedMap.find(Entry.first) == totalVisitedMap.end()) {
          totalVisitedMap.insert(MutexPairType(Entry.first, 0));
        }
        totalVisitedMap[Entry.first]+= Entry.second;
      }
      Tx.second->dumpVisitedMap();

    }
    return;
  }

  void TransactionManager::sumAbortedMap() {
    totalAbortedMap.clear();
    for (auto Tx : TransTable)  {
      auto Map = Tx.second->getAbortedMap();
      for (auto Entry : Map)  {
        if (totalAbortedMap.find(Entry.first) == totalAbortedMap.end()) {
          totalAbortedMap.insert(MutexPairType(Entry.first, 0));
        }
        totalAbortedMap[Entry.first]+= Entry.second;
      }
      Tx.second->dumpAbortedMap();
    }
    return;
  }

  void TransactionManager::sumTx()  {
    sumAbortNum();
    sumAbortedMap();
    sumVisitedMap();
    sumStats();
  }

  void TransactionManager::sumStats()  {
    AbortStatsMap.clear();
    for(auto Entry : totalAbortedMap)  {
      /// Entry.second is the abort num for Entry.first
      if (AbortStatsMap.find(Entry.second) == AbortStatsMap.end()) {
        AbortStatsMap.insert(std::pair<int, int> (Entry.second, 0));
      }
      AbortStatsMap[Entry.second]++;
    }

    VisitStatsMap.clear();
    for (auto VisitEntry : totalVisitedMap) {
      if (VisitStatsMap.find(VisitEntry.second) == VisitStatsMap.end()) {
        VisitStatsMap.insert(std::pair<int, int>(VisitEntry.second, 0) );
      }
      VisitStatsMap[VisitEntry.second]++;
    }
  }

  TransactionManager::MutexMapType TransactionManager::getVisitedMap() {
    return totalVisitedMap;
  }

  TransactionManager::MutexMapType TransactionManager::getAbortedMap() {
    return totalAbortedMap;
  }

  void TransactionManager::dumpStats() {
//    auto FAEntry = AbortStatsMap.begin();
//    auto FVEntry = VisitStatsMap.begin();
    std::cout << "Total_Abort_Num\t" << totalAbortNum
              << "\nTotal_Abort_Obj\t" << totalAbortedMap.size()
              << "\nTotal_Visit_Obj\t" << totalVisitedMap.size()
  //            << "\n"
  //            << "Diff_Abort_Num\t" << AbortStatsMap.size()
  //            << "\nDiff_Visit_Num\t" << VisitStatsMap.size()
  //            << "\n"
  //            << "Single_Obj_Largest_Abort_Num\t" << FAEntry->first
  //            << "\nObj_Num\t" << FAEntry->second
  //            << "\nSingle_Obj_Largest_Visit_Num\t" << FVEntry->first
  //            << "\nObj_Num\t" << FVEntry->second
              << "\n";

    std::cout <<"Abort Map\n";
    for(auto AEntry : AbortStatsMap)  {
      std::cout << "AbortNum\t" << AEntry.first
                << "\tObjNum\t" << AEntry.second
                << "\n";
    }

    std::cout <<"Visit Map\n";
    for(auto VEntry : VisitStatsMap)  {
      std::cout << "VisitNum\t" << VEntry.first
                << "\tObjNum\t" << VEntry.second
                << "\n";
    }

    std::cout << "Processing time\n";
//    std::cout << "TxId\tExecTime\tActiveTime\n";
    uint64_t totalExec = 0, totalActive = 0, totalProc = 0, totalShrink = 0;
    for (auto Tx : TransTable)  {
      std::cout <<"TxId\t" << Tx.second->getId()
                << "\tBegin\t" << Tx.second->getBeginTime()
                << "\tExpand\t" << Tx.second->getExpandTime()
                << "\tCommit\t" << Tx.second->getCommitTime()
                << "\tExecTime\t" <<Tx.second->getExecTime()
                << "\tActiveTime\t" << Tx.second->getActiveTime()
                << "\tShrinkTime\t" << Tx.second->getShrinkTime()
                << "\tProcTime\t" << Tx.second->getProcTime()
                << "\tExec/Active\t" << getPercent(Tx.second->getExecTime(), Tx.second->getActiveTime())
                << "\tExec/Proc\t" << getPercent(Tx.second->getExecTime(), Tx.second->getProcTime())
                << "\tShrink/Proc\t" << getPercent(Tx.second->getShrinkTime(), Tx.second->getProcTime())
                << "\n";

      totalExec += Tx.second->getExecTime();
      totalActive += Tx.second->getActiveTime();
      totalProc += Tx.second->getProcTime();
      totalShrink += Tx.second->getShrinkTime();
    }
    std::cout << "Total_Exec_Time\t" << totalExec
              << "\tTotal_Active Time\t" << totalActive
              << "\tTotal_Shrink Time\t" << totalShrink
              << "\tTotal_Proccess Time\t" << totalProc
              << "\tExec/Active_Ratio\t" << getPercent(totalExec, totalActive)
              << "\tExec/Proc_Ratio\t" << getPercent(totalExec, totalProc)
              << "\tShrink/Proc_Ratio\t" << getPercent(totalShrink, totalProc)
              << "\n";

  }

#endif
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
