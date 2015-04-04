#ifndef _UTIL_H_
#define _UTIL_H_

#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include "sim_api.h"
using namespace std;

typedef struct NodeHint{
  //long long addr;
  uintptr_t addr;
}NodeHint;

typedef struct MappedWinodw{
  long long startAddr;
  long long position;
  long long recordSize;
  long long windowSize;
  long long totalSize;
}MappedWindow;

void setupWindow(MappedWindow &window, long long s, long long p, long long r, long long w, long long t){
  window.startAddr = s;
  window.position = p;
  window.recordSize = r;
  window.windowSize = w;
  window.totalSize = t;
}

void setupNode(NodeHint &nodeHint, long long a){
  nodeHint.addr = a;
}

/*
void passNodeHint(void* nodeAddr){
  //std::cout<<"passing node from software"<<endl;
  NodeHint* nodeHint = new NodeHint();
  setupNode(*nodeHint,(long long)nodeAddr);
  NodeHint* hint[1];
  hint[0]=nodeHint;
  //must exist
  std::cout<<"the software info:"<<hint[0]->addr<<std::endl;
  SimNode(1,(long long)hint);
}
*/

void passNodeHint(void* nodeAddr){
  //std::cout<<"passing node from software"<<endl;
  NodeHint* nodeHint = new NodeHint();
  setupNode(*nodeHint,(long long)nodeAddr);
  //NodeHint* hint[1];
  NodeHint** hint = new NodeHint*[1];
  hint[0]=nodeHint;
  //must exist
  std::cout<<"the software info:"<<hint[0]->addr<<std::endl;
  SimNode(1,(long long)hint);
}

void passPtr(void* nodeStartPtr, long long nodeSize, void* relStartPtr, long long relSize, void* propStartPtr, long long propSize){
  MappedWindow * nodeWindow = new MappedWindow();
  MappedWindow * relWindow = new MappedWindow();
  MappedWindow * propWindow = new MappedWindow();

  long long nodeStart = (long long)nodeStartPtr;
  long long relStart = (long long)relStartPtr;
  long long propStart = (long long)propStartPtr;

  setupWindow(*nodeWindow,nodeStart,0,1,nodeSize,nodeSize);
  setupWindow(*relWindow,relStart,0,1,relSize,relSize);
  setupWindow(*propWindow,propStart,0,1,propSize,propSize);

  MappedWindow* hint[3];
  hint[0] = nodeWindow;
  hint[1] = relWindow;
  hint[2] = propWindow;

  /*
  cout<<"node start addr: "<<hex<<(*nodeWindow).startAddr<<endl;
  cout<<"node window size: "<<hex<<(*nodeWindow).windowSize<<endl;
  cout<<"node total size: "<<hex<<(*nodeWindow).totalSize<<endl;
  cout<<"rel start addr: "<<(*relWindow).startAddr<<endl;
  cout<<"rel window addr: "<<(*relWindow).windowSize<<endl;
  cout<<"rel total size: "<<(*relWindow).totalSize<<endl;
  cout<<"prop start addr: "<<(*propWindow).startAddr<<endl;
  */
  cout<<"trigger hint "<<(long long)hint<<endl;
  SimUser(3,(long long)hint);
}

#endif /* _UTIL_H */
