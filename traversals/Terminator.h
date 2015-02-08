#ifndef _TERMINATOR_H_
#define _TERMINATOR_H_

#include "Collector.h"

//template<typename VertexPointer, typename EdgePointer>
struct Terminator {
public:
//  typedef PathCollector<VeretxPointer, EdgePointer> PathType;

public:
  
  //set to a large number so traversecould go on in default setting
  Terminator() : _depth(8192),       
                 _pathNum(8192),
                 _targetNum(8192) { }

  ///interface with query
  void atDepth(unsigned int depth) {
    _depth = depth;
  }
  
  void atPathNumber(unsigned int pathNum) {
    _pathNum = pathNum;
  } 
  void atTargetNumber(unsigned int tNum) {
    _targetNum = tNum;
  }


  ///implementations of terminating logic
  bool checkPath(PathType paths) {

   if(paths.size() >= pathNum) {
     return true;
   else // not yet
     return false;
  }

  bool checkTarget(TargetType targets) { 
    if (targets.size() >= _targetNum)
      return true;
    else 
      return false;

  }

  bool checkDepth(unsigned int depth) {
    if()
      return true;
    else 
      return false;
  }

private:
  
  Filter _filter;

  unsigned int _depth;
  unsigned int _pathNum;
  ///# of useful nodes/edges
  unsigned int _targetNum;
//  std::vector<PathType> _path;

#endif /*_TERMINATOR_H_*/
