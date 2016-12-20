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
#include <unordered_map>

#if _DEBUG_ENABLE_
#include <iostream>
#endif

class MutexPointerType {
public:
  typedef std::shared_timed_mutex Mutex;
  typedef std::mutex ExMutex;
  typedef std::shared_ptr<Mutex> MutexPointer;
  typedef std::shared_ptr<ExMutex> ExMutexPointer;
  typedef unsigned int TransIdType;
  typedef std::unordered_map<TransIdType, LockType> TxMapType;
public:
  MutexPointerType() : MutexPtr (MutexPointer(new Mutex)) 
#if _LOCK_GUARD_
                      , MutexGuardPtr(ExMutexPointer(new ExMutex))
#endif
                        {}
#if _LOCK_GUARD_
#endif

protected:
  MutexPointer MutexPtr;
#if _LOCK_GUARD_
  ExMutexPointer MutexGuardPtr;
  TxMapType TxMap;

#endif
};

class Lock {
public:
//  typedef MutexPointerType MutexPointer;
  typedef std::shared_timed_mutex Mutex;
  typedef std::shared_ptr<Mutex> MutexPointer;
public:
  Lock(): IdMutex (MutexPointer(new Mutex))
          , LbMutex (MutexPointer(new Mutex))
          , PpMutex (MutexPointer(new Mutex))
          {}

  auto getPpMutex()
    -> MutexPointer {
    return PpMutex;
  }

  auto getIdMutex() 
    -> MutexPointer {
    return IdMutex;
  }

  auto getLbMutex() 
    -> MutexPointer {
    return LbMutex;
  }

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

protected:
  MutexPointer IdMutex;
  MutexPointer LbMutex;
  MutexPointer PpMutex;
};

class VertexLock : public Lock {
public:
  VertexLock() : NEMutex (MutexPointer(new Mutex))
                 {} 

  using Lock::tryLock;
  using Lock::tryUnlock;

  auto getNEMutex()
    -> MutexPointer {
    return NEMutex;
  }

  bool tryLock(MutexType Mutex, LockType LType) {
    return Lock::tryLock(getMutexPtr(Mutex), LType);
  }

  void tryUnlock(MutexType Mutex, LockType LType) {
    return Lock::tryUnlock(getMutexPtr(Mutex), LType);
  }

  MutexPointer getMutexPtr(MutexType Mutex) {
    switch(Mutex) {
      case T_ID:
        return IdMutex;
      case T_NextEdge:
        return NEMutex;
      case T_Property:
        return PpMutex;
      case T_Label:
        return LbMutex;
      default:
        assert(false && "Mutex type invalid");
    }
  }

protected:
  MutexPointer NEMutex;
};

class EdgeLock : public Lock {
public:
  EdgeLock() : FVMutex (MutexPointer(new Mutex))
               , SVMutex (MutexPointer(new Mutex))
               , FNEMutex (MutexPointer(new Mutex))
               , FPEMutex (MutexPointer(new Mutex))
               , SNEMutex (MutexPointer(new Mutex))
               , SPEMutex (MutexPointer(new Mutex))
               {}

  using Lock::tryLock;
  using Lock::tryUnlock;

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

  MutexPointer getMutexPtr(MutexType Mutex) {
    switch(Mutex) {
      case T_ID:
        return IdMutex;
      case T_FirstVertex:
        return FVMutex;
      case T_SecondVertex:
        return SVMutex;
      case T_FirstNextEdge:
        return FNEMutex;
      case T_SecondNextEdge:
        return SNEMutex;
      case T_FirstPrevEdge:
        return FPEMutex;
      case T_SecondPrevEdge:
        return SPEMutex;
      case T_Property:
        return PpMutex;
      case T_Label:
        return LbMutex;
      default:
        assert(false && "Mutex type invalid");
    }
  }

  bool tryLock(MutexType Mutex, LockType LType) {
    return Lock::tryLock(getMutexPtr(Mutex), LType);
  }

  void tryUnlock(MutexType Mutex, LockType LType) {
    return Lock::tryUnlock(getMutexPtr(Mutex), LType);
  }

protected:
  MutexPointer FVMutex;
  MutexPointer SVMutex;
  MutexPointer FNEMutex;
  MutexPointer FPEMutex;
  MutexPointer SNEMutex;
  MutexPointer SPEMutex;
};

#endif /**_LOCK_H_*/
