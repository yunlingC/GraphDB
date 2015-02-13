#ifndef _TERMINATOR_H_
#define _TERMINATOR_H_

#include "Filter.h"
#include "GraphType.h"
#include <vector>

struct Terminator {
public:
  typedef std::vector<GraphType::VertexPtr> VertexTargetSet;
  typedef std::vector<GraphType::VertexPtr> VertexPath;
  typedef std::vector<GraphType::EdgePtr>   EdgeTargetSet;
  typedef std::vector<VertexPath>           PathType;
public:
  
  //set to a large number so traversecould go on in default setting
  Terminator() : _depth(8192),       
                 _pathNum(8192),
                 _targetNum(8192) { }

  ///interface with query

  void setFilter(Filter filter) {
    _filter = filter;
  }

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

//  bool checkTarget(TargetType targets) { 
//    if (targets.size() >= _targetNum)
//      return true;
//    else 
//      return false;
//  }

  bool TerminateAtDepth(std::map<GraphType::VertexPtr,int> depthlist) {
    for(auto it = depthlist.begin(); it != depthlist.end(); ++ it) {
      if(it->second > _depth)
        return true;
    }
//    else 
//      return false;
    return false;
  }

  bool TerminateAtVertex(VertexTargetSet vertexSet) {
    if(vertexSet.size() >= _targetNum) 
    {
//      cout << "targetNum: " << _targetNum << " targetSet size: " <<vertexSet.size() << endl;
      return true;
    }
    else // not yet
      return false;
  }

  bool TerminateAtEdge(EdgeTargetSet edgeSet) {
    if(edgeSet.size() >= _targetNum) { 
      cout << "targetNum: " << _targetNum << " targetSet size: " <<edgeSet.size() << endl;
      return true;
    }
    else // not yet
      return false;
    
  }

  bool TerminateAtPath(PathType paths) {
    if(paths.size() >= _pathNum) {
      cout << "pathnum " << _pathNum << " path size: " << paths.size() << endl;
      return true;
    }
    else // not yet
      return false;
  }


  bool customTerminator() {
    return false;
  }



private:
  
  Filter _filter;

  unsigned int _depth;
  unsigned int _pathNum;
  ///# of useful nodes/edges
  unsigned int _targetNum;
//  std::vector<PathType> _path;
};
#endif /*_TERMINATOR_H_*/
