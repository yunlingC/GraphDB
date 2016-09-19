//===-- transaction/RagManager.cpp - RAG class type -------------*- C++ -*-===//
//
//                     CAESR Graph Database
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the source for header RagManager.h
///
//===----------------------------------------------------------------------===//

#ifndef _RAGMANAGER_CPP_
#define _RAGMANAGER_CPP_

#include "RagManager.h"

auto RagManager::getDeadLockCount() 
  ->  unsigned int  {
	return DeadLockCount;
}

auto RagManager::getVertxLock() 
  ->  bool {

}

auto RagManager::getEdgeLock() 
  ->  bool  {

}

auto RagManager::registerVertexLock() 
  ->  bool  {

}

auto RagManager::registerEdgeLock() 
  ->  bool  {

}

auto RagManager::checkWaitOn() 
  ->  bool  {

}

auto RagManager::stopWaitOn() 
  ->  bool  {

}


#endif /*_RAGMANAGER_CPP_*/
