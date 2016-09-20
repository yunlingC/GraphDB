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

auto RagManager::getVertexLock(VertexPtr Vertex, MutexType Mutex, LockType Lock) 
  ->  bool {
    /// TODO Check if lock exists from LockManager
    /// If yes, check if this lock has been acquired by others
    ///     If no, get lock and go on;
    ///     If yes, check if spining on this lock will result in a deadlock
    ///         If no, wait for current lock
    ///         If yes, stop wait on this lock
    ///            Abort
    ///            Or other techniques e.g. pick up later

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
