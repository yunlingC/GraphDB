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
/// \brief This is the main class for locks structure.
///
//===----------------------------------------------------------------------===//

#ifndef _LOCK_H_
#define _LOCK_H_

#include <shared_mutex>

//enum Lock {VertexLock, EdgeLock};

class VertexLock {
public:
  typedef std::shared_timed_mutex Mutex;
  typedef std::shared_ptr<Mutex> MutexPointer;
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


protected:
///No mutex for label or id
/// won't be changed anyway;
  MutexPointer IdMutex;
  MutexPointer NEMutex;
  MutexPointer LEMutex;
  MutexPointer LbMutex;
  MutexPointer PpMutex;
};

class EdgeLock {
public:
  typedef std::shared_timed_mutex Mutex;
  typedef std::shared_ptr<Mutex> MutexPointer;
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


protected:
  ///No mutex for label
  /// won't be changed anyway;
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

};

#endif /**_LOCK_H_*/
