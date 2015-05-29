//===-- transaction/LockManager.h - Lock class type -------------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the main class for managing locks .
///
//===----------------------------------------------------------------------===//

#ifndef _LOCKMANAGER_H_
#define _LOCKMANAGER_H_

///TODO c++14
#include <shared_mutex>
#include <iostream>
//#include <boost/thread/shared_mutex.hpp>
#include <thread>
#include <chrono>
#include <map>

#include "GraphType.h"

//typedef boost::shared_mutex Mutex;
//typedef boost::shared_lock<Mutex> ReadLock;
//typedef boost::upgrade_lock<Mutex> UpgradeLock;
//typedef boost::upgrade_to_unique_lock<Mutex> WriteLock;

//typedef std::mutex Mutex;
typedef std::shared_timed_mutex Mutex;
//typedef boost::shared_mutex Mutex;
//typedef std::unique_lock< Mutex > Lock;
typedef std::shared_ptr<Mutex> MutexPointer;
//TODO delete mutexPointer or uniqueMutexPointer
//typedef std::unique_ptr<Mutex> UniqueMutexPointer;
typedef std::map<unsigned int, MutexPointer> LockMapType;
typedef std::pair<unsigned int, MutexPointer> LockPair;

struct NullDeleter {
 void operator() (void const *) const { }
};

class LockManager {
public:
  typedef vector<unsigned int> LockIdListType;
public:
  /// needs a mutex to guard LockMap in order not to 
  //allow multiple change concurrently
  auto testVertexMutex(unsigned int id) 
    -> bool {
      if(VertexLockMap.find(id) == VertexLockMap.end())
        return false;
      else {
        MutexPointer mp(nullptr);
        mp = VertexLockMap[id];
        if(mp) {
          if(mp->try_lock()) {
            cout << "get lock\n "; 
            std::this_thread::sleep_for(std::chrono::seconds(1));
            mp->unlock();
          } else {
            cout <<"no lock\n";
          }
        }
        else {
          cout << "pointer taken\n";
        }
        return true;
      }
    }
  
  auto getVertexSharedLock(unsigned int id) 
    -> bool {
      if(VertexLockMap.find(id) == VertexLockMap.end())
        ///TODO: shoould be exception here, need to be fixed.
      {
        cout << "vertex lock num " << VertexLockMap.size() << endl;
        cout << "Error : No such vertex " << id <<" in map \n";
        return false;
      }
      else {
        MutexPointer mp(nullptr);
        mp = VertexLockMap[id];
        if(mp) {
          if(mp->try_lock_shared()) {
            cout << "get shared_lock: vertex " << id << "\n"; 
          } else {
            cout <<"no shared_lock: vertex " << id << "\n";
            return false;
          }
        }
        else {
          cout << "pointer taken: " << id << "\n";
          return false;
        }
        return true;
      }
  }

  auto releaseVertexSharedLock(unsigned int id) 
    -> bool {
      if(VertexLockMap.find(id) == VertexLockMap.end())
        ///TODO: shoould be exception here, need to be fixed.
      { 
        cout << "Error : No such vertex " << id <<" in map \n";
        return false;
      }
      else {
        MutexPointer mp(nullptr);
        mp = VertexLockMap[id];
        if(mp) {
          mp->unlock_shared();
          cout << "release shard_lock: vertex " << id << "\n"; 
          return true;
        }
        else {
          cout << "pointer taken\n";
          return false;
        }
        return true;
      }
  }


  auto getEdgeSharedLock(unsigned int id) 
    -> bool {
      if(EdgeLockMap.find(id) == EdgeLockMap.end()) {
        cout << "edge lock num " << EdgeLockMap.size() << endl;
        cout << "Error : No such edge " << id <<" in map \n";
        return false;
      }
      else {
        MutexPointer mp(nullptr);
        mp = EdgeLockMap[id];
        if(mp) {
          if(mp->try_lock_shared()) {
            cout << "get shared_lock: edge " << id << "\n"; 
          } else {
            cout <<"no shared_lock: edge " << id << "\n";
            return false;
          }
        }
        else {
          cout << "pointer taken\n";
          return false;
        }
        return true;
      }
  }

  auto releaseEdgeSharedLock(unsigned int id) 
    -> bool {
      if(EdgeLockMap.find(id) == EdgeLockMap.end()) {
        ///TODO: shoould be exception here, need to be fixed.
        cout << "Error : No such edge" << id <<" in map \n";
        return false;
      }
      else {
        MutexPointer mp(nullptr);
        mp = EdgeLockMap[id];
        if(mp) {
          mp->unlock_shared();
          cout << "release shared_lock: edge " << id << "\n"; 
          return true;
        }
        else {
          cout << "pointer taken\n";
          return false;
        }
        return true;
      }
  }


  auto getVertexSharedLocks(LockIdListType idList) 
    -> bool {
      for(auto it = idList.begin(); 
          it != idList.end(); ++it) {
        auto id = *it;
        if(VertexLockMap.find(id) == VertexLockMap.end())
          return false;
        else {
          ///TODO wrong logic here, need to be fixed
          MutexPointer mp(nullptr);
          mp = VertexLockMap[id];
          if(mp) {
            if(mp->try_lock()) {
              cout << "get lock\n "; 
            } else {
              cout <<"no lock\n";
              return false;
            }
          }
          else {
            cout << "pointer taken\n";
            return false;
          }
          return true;
        }
      }
  }


  /// TODO check map
  auto addToVertexLockMap(unsigned int id) 
    -> void  {
      cout << "add to vertexlockmap " << id << endl;
      std::shared_ptr<Mutex> NewMP (new Mutex, NullDeleter());
      //NewMP = std::make_shared<Mutex>;
      //VertexLockMap.insert(LockPair(id, MutexPointer(new Mutex)));
      VertexLockMap.insert(LockPair(id, NewMP));
      cout << "vertex lock nums " << VertexLockMap.size() << endl;
  }

  auto addToEdgeLockMap(unsigned int id) 
    -> void  {
      cout << "add to edgelockmap " << id << endl;
      EdgeLockMap.insert(LockPair(id, MutexPointer(new Mutex)));
      cout << "edge lock num " << EdgeLockMap.size() << endl;
  }
 
  auto buildLockMap(GraphType & graph) 
  -> void {
    typedef GraphType::VertexPointer VertexPointer;
    typedef GraphType::EdgePointer EdgePointer;
    std::map<unsigned int, VertexPointer> VertexMap;
    std::map<unsigned int, EdgePointer> EdgeMap;
    VertexMap = graph.getVertexMap();
    EdgeMap = graph.getEdgeMap();

    for(auto iter = VertexMap.begin();
        iter != VertexMap.end(); iter++) {
      VertexLockMap.insert(LockPair((*iter).first, MutexPointer(new Mutex)));
      cout << "add lock for vertex: " << (*iter).first << endl;
    }
    for(auto it = EdgeMap.begin();
        it != EdgeMap.end(); it++) {
      EdgeLockMap.insert(LockPair((*it).first, MutexPointer(new Mutex)));
      cout << "add lock for edge: " << (*it).first << endl;
    }

    cout << "after build maps, vertex lock num " << VertexLockMap.size() << " edge lock num " << EdgeLockMap.size() << endl;
  }
 
   auto getVertexLockMap() 
    -> LockMapType {
      return VertexLockMap;
  }
   auto getEdgeLockMap() 
    -> LockMapType {
      return EdgeLockMap;
  }

  /// no need to delete; 
  /**
  LockManager();

  ~LockManager() {
    for (auto it = VertexLockMap.begin(); 
          it != VertexLockMap.end(); it++) {
      delete (*it).second;
    }
    for (auto it = EdgeLockMap.begin(); 
          it != EdgeLockMap.end(); it++) {
      delete (*it).second;
    }
  }
  */
protected:
  LockMapType VertexLockMap;
  LockMapType EdgeLockMap;
};

#endif /*_LOCKMANAGER_H_*/
