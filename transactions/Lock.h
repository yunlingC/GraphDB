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

#if _LOCK_GUARD_
class MutexPointerType {
public:
  typedef std::shared_timed_mutex Mutex;
  typedef std::mutex ExMutex;
  typedef std::shared_ptr<Mutex> MutexPointer;
  typedef std::shared_ptr<ExMutex> ExMutexPointer;
  typedef unsigned int TransIdType;
  typedef std::unordered_map<TransIdType, LockType> TxMapType;
  typedef std::pair<TransIdType, LockType> TxLockPairType;
public:
  MutexPointerType() : MutexPtr (MutexPointer(new Mutex)) 
                      , MutexGuardPtr(ExMutexPointer(new ExMutex))
                        {}

  bool try_lock_shared()  {
    return MutexPtr->try_lock_shared();
  }

  bool try_lock() {
    return MutexPtr->try_lock(); 
  }

  void unlock_shared() {
    MutexPtr->unlock_shared();
  }

  void unlock() {
    MutexPtr->unlock(); 
  }

#ifdef _DEADLOCK_DETECTION_
  ExMutexPointer getGuardPtr() {
    return MutexGuardPtr;
  }

#endif

  bool registerTx(TransIdType txid, LockType lt) {
//    MutexGuardPtr->lock();
    if (!MutexGuardPtr->try_lock())
      return false;

    ///TODO need to do a check before insertion
    if(TxMap.find(txid) != TxMap.end()) {
      if (TxMap[txid] != lt) {
        assert(false && "Transaction already registers this lock");
      }
      assert(false && "Transaction already registers this lock");
    }

    TxMap.insert(TxLockPairType(txid, lt));

    MutexGuardPtr->unlock();
    return true;
  }

  void retireTx(TransIdType txid, LockType lt) {
//    std::cout << "Transaction\t" << txid
//              << "\twaits to retire lock\t" << MutexPtr
//              << "\n";
    MutexGuardPtr->lock();

    if (TxMap.find(txid) != TxMap.end())  {
      if (TxMap[txid] == lt)
        TxMap.erase(txid);
      else {
        assert(false && "Transaction register lock type incorrectly");
      }
    }
    else {
      assert(false && "Transaction does NOT register this lock");
    }
    MutexGuardPtr->unlock();
  }

  TxMapType getTx() {
    return TxMap;
  }

  ///True - wait 
  bool checkTx(TransIdType txid)  {
    bool wait = true;
    MutexGuardPtr->lock();

    for (auto Tx : TxMap) {
      ///<TranxId, LockType> 
      assert(Tx.first != txid && "Transaction does not wait for itself");
      if (Tx.first < txid)  {
        ///Don't wait for this lock
        wait =  false;
#if _DEBUG_PRINT_
        std::cout << "Tx\t" << txid << "\t does not wait for\t" << Tx.first << "\n";
#endif 
        break;
      }
    }
    MutexGuardPtr->unlock();
#if _DEBUG_PRINT_
    if (wait) 
      std::cout << "Tx\t" << txid << "\t wait for lock\n";
#endif 
    return wait;
  }

protected:
  MutexPointer MutexPtr;
  ExMutexPointer MutexGuardPtr;
  TxMapType TxMap;
};
#endif

class Lock {
public:
#if _LOCK_GUARD_
  typedef MutexPointerType * MutexPointer;
  typedef MutexPointerType::ExMutex ExMutex;
  typedef MutexPointerType::ExMutexPointer ExMutexPointer;
#else 
  typedef std::shared_timed_mutex Mutex;
  typedef std::shared_ptr<Mutex> MutexPointer;
#endif
public:
#if _LOCK_GUARD_
  Lock()  {
    IdMutex = new MutexPointerType();
    LbMutex = new MutexPointerType();
    PpMutex = new MutexPointerType();
  }
  
#else 
  Lock(): IdMutex (MutexPointer(new Mutex))
          , LbMutex (MutexPointer(new Mutex))
          , PpMutex (MutexPointer(new Mutex))
          {}
#endif
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
    assert(MutexPtr && "Mutex pointer invalid\n");
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
//    return MutexPtr->tryLock(LType);
  }
  
  void tryUnlock(MutexPointer MutexPtr, LockType LType) {
    assert(MutexPtr && "Mutex pointer invalid\n");
    switch (LType) {   
      case T_SH:
        return MutexPtr->unlock_shared();
      case T_EX:
        return MutexPtr->unlock(); 
      default:
        assert(false && "Lock type invalid\n");
    }
    return;
//    MutexPtr->tryUnlock(LType);
  }

protected:
  MutexPointer IdMutex;
  MutexPointer LbMutex;
  MutexPointer PpMutex;
};

class VertexLock : public Lock {
public:
#if _LOCK_GUARD_
  VertexLock()  {
    NEMutex = new MutexPointerType();
  } 
#else 
  VertexLock() : NEMutex (MutexPointer(new Mutex))
                 {} 
#endif

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
#if _LOCK_GUARD_
  EdgeLock() {
    FVMutex = new MutexPointerType();
    SVMutex = new MutexPointerType();
    FNEMutex = new MutexPointerType();
    FPEMutex = new MutexPointerType();
    SNEMutex = new MutexPointerType();
    SPEMutex = new MutexPointerType();
  }
#else
  EdgeLock() : FVMutex (MutexPointer(new Mutex))
               , SVMutex (MutexPointer(new Mutex))
               , FNEMutex (MutexPointer(new Mutex))
               , FPEMutex (MutexPointer(new Mutex))
               , SNEMutex (MutexPointer(new Mutex))
               , SPEMutex (MutexPointer(new Mutex))
               {}
#endif

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
