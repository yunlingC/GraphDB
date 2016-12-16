#include "Lock.h"
#include "Vertex.h"
#include "Edge.h"

#include <cassert>
#include <iostream>

using namespace std;


int main(int argc, char *argv[]) {
  assert(1+1==2);

//  std::mutex Mutex;

  Vertex * v = new Vertex;

  Edge * e = new Edge; 
 
  MutexPointerType *MuPtr = new MutexPointerType();

  Lock* lock = new Lock();

  VertexLock *vlock = new VertexLock();

  EdgeLock *elock = new EdgeLock();

  cout << "Vertex size\t" << sizeof(*v) << endl;
  cout << "Edge size\t" << sizeof(*e) << endl;
//  cout << " Mutex size\t" << sizeof(Mutex) << endl;
  cout << "MuPtr size\t" << sizeof(*MuPtr) << endl;
  cout << "lock size\t" << sizeof(*lock) << endl;
  cout << "vlock size\t" << sizeof(*vlock) << endl;
  cout << "elock size\t" << sizeof(*elock) << endl;
//  Vlock->tryLock(nullptr, T_SH);
//  Vlock->tryUnlock(nullptr, T_SH);

//  Vlock->tryLock(Vlock->IDMutex, T_SH);
//  Vlock->tryUnlock(Vlock->IDMutex, T_SH);
//
//  VertexLock *V = new VertexLock();
//  V->tryLock(T_NextEdge, T_SH);
//  V->tryUnlock(T_NextEdge, T_SH);

  std::cout << "Finish\n";
}

