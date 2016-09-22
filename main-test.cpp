#include <thread>
#include <shared_mutex>

#include <iostream>

using namespace std;

typedef std::shared_timed_mutex Mutex;
typedef std::shared_ptr<Mutex> MutexPtr;

bool requireLockS(MutexPtr lock) {
  return lock->try_lock_shared();
}

bool requireLockX(MutexPtr lock) {
  return lock->try_lock();
}

bool getlock(bool input) {
  if (input) {
    std::cout <<"YES" << "\n";
    return true;
  }
  else  {
    std::cout <<"No" << "\n";
    return false;
  }
}

bool getlock_recursive(MutexPtr lock) {
  unsigned int i = 0;
  while (lock->try_lock_shared() && (i++<5)) {
    std::cout <<  i <<" YES" << "\n";
    return true;
  }
}

int main() {

  MutexPtr  lock(new Mutex);
  
//  std::thread  t(getlock_recursive, lock);
//  t(getlock, requireLockS(lock));
//  std::thread  t(requireLockS,  lock);
//  t.join();

//  while (lock->try_lock_shared()) {
//    std::cout <<"YES" << "\n";
//    return true;
//  }
  
  getlock(requireLockS(lock));
//  getlock(requireLockX(lock));
  getlock(requireLockS(lock));
  getlock(requireLockS(lock));

  unsigned int i = 0;
  while (getlock(requireLockS(lock)) &&( i++ < 3)) {
  };

}

