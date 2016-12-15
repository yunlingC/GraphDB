//===-- transaction/Lock.h - Lock class type -------------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the main class for lock structure.
///
//===----------------------------------------------------------------------===//

#ifndef _LOCK_H_
#define _LOCK_H_

#include "global.h"
#include "Concurrency_control_config.h"

#include <shared_mutex>
#include <cassert>

#if _DEBUG_ENABLE_
#include <iostream>
#endif

class Lock {
public:
  typedef std::shared_timed_mutex Mutex;
  typedef std::shared_ptr<Mutex> MutexPointer;
public:
  MutexPointer IDMutex;
public:
  Lock() : IDMutex (MutexPointer(new Mutex)) {}

  bool tryLock(MutexPointer MutexPtr, LockType LType) {
    assert(MutexPtr && "MutexPointer invalid\n");
    switch (LType) {   
      ///Shared lock
      case T_SH:
        return MutexPtr->try_lock_shared();
      ///Exclusive lock
      case T_EX:
        return MutexPtr->try_lock(); 
      default:
        assert(false && "Lock type invalid\n");
      }
    return true;
  }
  
  void tryUnlock(MutexPointer MutexPtr, LockType LType) {
    assert(MutexPtr != nullptr && "MutexPointer invalid\n");
    switch (LType) {   
      case T_SH:
        return MutexPtr->unlock_shared();
      case T_EX:
        return MutexPtr->unlock(); 
      default:
        assert(false && "Lock type invalid\n");
    }
  }

};

class VertexLock : public Lock {
public:
  VertexLock() : IdMutex (MutexPointer(new Mutex)),
                 NEMutex (MutexPointer(new Mutex)),
                 LEMutex (MutexPointer(new Mutex)),
                 LbMutex (MutexPointer(new Mutex)),
                 PpMutex (MutexPointer(new Mutex)){} 

  auto getPpMutex()
    -> MutexPointer {
    return PpMutex;
  }

  auto getNEMutex()
    -> MutexPointer {
    return NEMutex;
  }
 
  auto getLEMutex()
    -> MutexPointer {
    return LEMutex;
  }

  auto getIdMutex() 
    -> MutexPointer {
    return IdMutex;
  }

  auto getLbMutex() 
    -> MutexPointer {
    return LbMutex;
  }

  bool tryLock(MutexType Mutex, LockType LType) {
    switch(Mutex) {
      case T_ID:
        return Lock::tryLock(IdMutex, LType);
      case T_NextEdge:
        return Lock::tryLock(NEMutex, LType);
    }
    return true;
  }

  void tryUnlock(MutexType Mutex, LockType LType) {
    return ;
  }
protected:
  MutexPointer IdMutex;
  MutexPointer NEMutex;
  MutexPointer LEMutex;
  MutexPointer LbMutex;
  MutexPointer PpMutex;
#ifdef _DEADLOCK_DETECTION_
  /// TODO Mutex tables 
#endif 
};

class EdgeLock : public Lock {
public:
  EdgeLock() : IdMutex (MutexPointer(new Mutex)),
               FVMutex (MutexPointer(new Mutex)),
               SVMutex (MutexPointer(new Mutex)),
               FNEMutex (MutexPointer(new Mutex)),
               FPEMutex (MutexPointer(new Mutex)),
               SNEMutex (MutexPointer(new Mutex)),
               SPEMutex (MutexPointer(new Mutex)),
               LbMutex (MutexPointer(new Mutex)),
               PpMutex (MutexPointer(new Mutex)){}

  auto getIdMutex() 
    -> MutexPointer {
    return IdMutex;
  }

  auto getPpMutex()
    -> MutexPointer {
    return PpMutex;
  }

  auto getFVMutex()
    -> MutexPointer {
    return FVMutex;
  }
 
  auto getSVMutex()
    -> MutexPointer {
    return SVMutex;
  }

  auto getFNEMutex()
    -> MutexPointer {
    return FNEMutex;
  }

  auto getFPEMutex()
    -> MutexPointer {
    return FPEMutex;
  }

  auto getSNEMutex()
    -> MutexPointer {
    return SNEMutex;
  }

  auto getSPEMutex()
    -> MutexPointer {
    return SPEMutex;
  }

  auto getLbMutex() 
    -> MutexPointer {
    return LbMutex;
  }

  bool tryLock(MutexType Mutex, LockType LType) {
    return true;
  }

  void tryUnlock(MutexType Mutex, LockType LType) {
    return ;
  }

protected:
  MutexPointer IdMutex;
  MutexPointer FVMutex;
  MutexPointer SVMutex;
  MutexPointer FNEMutex;
  MutexPointer FPEMutex;
  MutexPointer SNEMutex;
  MutexPointer SPEMutex;
  /// Lb mutex for prefetching convenience(temporal)
  MutexPointer LbMutex;
  MutexPointer PpMutex;
#ifdef _DEADLOCK_DETECTION_
  /// TODO a mutex table <mutexPointer, <transaction_ptr, type> >
#endif 
};

#endif /**_LOCK_H_*/
