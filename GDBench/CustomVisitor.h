//===-- traversals/CustomVisitor.h - Customed visitor class -----*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the customed class for Graph visitors.
/// //===----------------------------------------------------------------------===//

#ifndef _CUSTOMVISITOR_H_
#define _CUSTOMVISITOR_H_

#include "Visitor.h"
#include "Utils.h"

#include <queue>
#include <set>
#include <iostream>

/// visitor to deal with query of selection : query1 and query 4
class SelectionVisitor: public Visitor {
public:
  typedef std::pair<FixedString, bool> ReturnValueType;
public:
  SelectionVisitor() { }

  virtual bool visitVertex(VertexPointer VertexPtr) {
    bool VertexMatch = checkProperty<ReturnValueType>(VertexPtr, getFilter());
    if (VertexMatch == true) 
      VertexList.push_back(VertexPtr);

    /// Terminate when the number of retrieved vertices reach a threshold
    return TerminateAtVertex(1, VertexList);
  }

  virtual bool  scheduleEdge(EdgePointer EdgePtr) {
    return checkType(EdgePtr, getFilter() );
  }

};

/// visitor to deal with adjacency query : query 2 to query 3
class AdjacencyVisitor: public Visitor {
public:
  AdjacencyVisitor() { }

  virtual bool visitVertex(VertexPointer VertexPtr) {
    if (DepthList.find(VertexPtr) != DepthList.end())
      return (DepthList[VertexPtr] >= 1);
    else 
      return false;
  }

  virtual bool scheduleEdge(EdgePointer EdgePtr) {
    return checkType(EdgePtr, Filter);
  }


  virtual bool visitDirection(VertexPointer Second, EdgePointer EdgePtr) {
    return DirectionMatch;
  }

  virtual bool scheduleBranch(VertexPointer First
                            , EdgePointer Edge
                            , VertexPointer Second) {

    bool TypeMatch = checkType(Edge, Filter);
    DirectionMatch = checkDirection(Second, Edge, Filter);

    if (DirectionMatch && TypeMatch ) {
      VertexList.push_back(Second);
    }

    computeDepth(First, Edge, Second, DepthList);
    return false;
  }

protected:
  DepthListType DepthList;
  bool DirectionMatch;
};


/// visitor to deal with multiple typefilters: from query 5 to 7 
class ReachabilityVisitor : public Visitor {
public:
  typedef std::queue<VertexPath>  PathQueueType;
  typedef std::set<VertexPointer> VertexSetType;
public:
  ReachabilityVisitor () { }

  virtual void setFilter(FilterType & f) {
    FilterList.push_back(f);
  }

  virtual void setDepth(unsigned int Depth) {
    DepthSetting = Depth;
  }

  virtual VertexSetType & getTargetSet() {
    return TargetSet;
  }

  virtual void visitStartVertex(VertexPointer StartVertex ) {
    VertexPath NewPath;
    NewPath.push_back(StartVertex);
    PathQueue.push(NewPath);
  }

  /*
   Allow the vertex to be visited multiple times
   Be careful when use it; there could be an infinate loop here
   Limiting the depth to explore in the graph could be a way to avoid the loop
  */
  virtual bool discoverVertex(VertexPointer Vertex) {
    return true;
  }

  virtual bool visitVertex(VertexPointer Vertex) {
    PrevPath = PathQueue.front();
    PathQueue.pop();
    if (PrevPath.size() > DepthSetting) {
      VertexList.push_back(PrevPath.at(DepthSetting));
      TargetSet.insert(PrevPath.at(DepthSetting));
      while (!PathQueue.empty()) {
        auto Path = PathQueue.front(); PathQueue.pop();
        TargetSet.insert(Path.at(DepthSetting));
      }
      return true;
    }
    return false;
  }


  virtual bool visitDirection(VertexPointer target, EdgePointer Edge) {
    return DirectionMatch;
  }

  virtual bool scheduleEdge(EdgePointer Edge) {
    return TypeMatch;
  }

  virtual bool scheduleBranch(VertexPointer first
                            , EdgePointer edge
                            , VertexPointer second) {
    unsigned int DepthSecond = 0;
    if (PrevPath.back() == first) {
      DepthSecond = PrevPath.size();
    }

    FilterType filter;
    if (DepthSecond > 0)
      filter = FilterList[DepthSecond-1];
    else
      filter.setDefault();
   
    TypeMatch = checkType(edge, filter);
    DirectionMatch = checkDirection(second, edge, filter);
    if (TypeMatch && DirectionMatch) {
      VertexPath NewPath = PrevPath;
      NewPath.push_back(second);
      PathQueue.push(NewPath);
    }
    return false;
  }

protected:
  unsigned int  TypeMatch;
  unsigned int  DirectionMatch;
  unsigned int  DepthSetting;
  std::vector<FilterType> FilterList;
  VertexPath   PrevPath;
  PathQueueType  PathQueue;
  VertexSetType TargetSet;
};


class PathVisitor : public Visitor {
public:
  typedef std::queue<VertexPath>  PathQueueType;
public:
  PathVisitor () { }

  void setEndVertex(VertexDescriptor vertex) {
    EndVertex =  vertex;
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    StartVertex = startVertex->getId();
    VertexPath NewPath;
    NewPath.push_back(startVertex);
    PathQueue.push(NewPath);
  }

  virtual bool visitVertex(VertexPointer Vertex) {
    PrevPath = PathQueue.front();
    PathQueue.pop();
    return false;
  }

  virtual bool scheduleTree(VertexPointer first
                          , EdgePointer edge
                          , VertexPointer second) {
      VertexPath NewPath = PrevPath;
      NewPath.push_back(second);
      PathQueue.push(NewPath);
      if (second->getId() == EndVertex) {
        VertexList = NewPath;
        return true;
      }
    return false;
  }
  
protected:
  VertexDescriptor EndVertex;
  VertexDescriptor StartVertex;
  VertexPath       PrevPath;
  PathQueueType    PathQueue;
};

class PatternVisitor : public Visitor {
public:
  typedef std::queue<VertexPath>  PathQueueType;
public:
  PatternVisitor () { }

  void setFilter(FilterType & f) {
    FilterList.push_back(f);
  }


  void setDepth(unsigned int depth) {
    DepthSetting = depth;
  }
  
  void setEndVertex(VertexDescriptor vertex) {
    EndVertex =  vertex;
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    VertexPath NewPath;
    NewPath.push_back(startVertex);
    PathQueue.push(NewPath);
  }

  virtual bool visitVertex(VertexPointer vertex) {
    PrevPath = PathQueue.front();
    PathQueue.pop();
    if (PrevPath.size() > DepthSetting) {
      if (PrevPath.at(DepthSetting)->getId() == EndVertex) 
        VertexSet.insert(PrevPath.at(1));
      while (!PathQueue.empty()) {
        auto path = PathQueue.front(); PathQueue.pop();
        if (path.at(DepthSetting)->getId() == EndVertex) {
          VertexSet.insert(path.at(1));
        }
      }
      return true;
    }
    return false;
  }

  virtual bool visitDirection(VertexPointer target, EdgePointer edge) {
    return DirectionMatch;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    return TypeMatch;
  }

  virtual bool discoverVertex(VertexPointer vertex) {
    return true; 
  }

  virtual bool scheduleBranch(VertexPointer first
                            , EdgePointer edge
                            , VertexPointer second) {

    unsigned int DepthSecond = 0;
    if (PrevPath.back() == first) {
      DepthSecond = PrevPath.size();
    }

    FilterType filter;
    if (DepthSecond > 0)
        filter = FilterList[DepthSecond-1];
    else
      filter.setDefault();
   
    TypeMatch = checkType(edge, filter);
    DirectionMatch = checkDirection(second, edge, filter);
    return false;
  }

  virtual bool scheduleTree(VertexPointer first
                          , EdgePointer edge
                          , VertexPointer second) {

    if (TypeMatch && DirectionMatch) {
      VertexPath NewPath = PrevPath;
      NewPath.push_back(second);
      PathQueue.push(NewPath);
    }
    return false;
  }

protected:
  bool TypeMatch;
  bool DirectionMatch;
  unsigned int  DepthSetting;
  VertexDescriptor EndVertex;
  std::vector<FilterType> FilterList;
  VertexPath   PrevPath;
  PathQueueType  PathQueue;
};


class DFSReachabilityVisitor: public ReachabilityVisitor {
public:
  typedef std::map<VertexPointer,  unsigned int> VertexTargetMapType;
  typedef std::pair<VertexPointer, unsigned int> VertexTargetPair;
public:
  DFSReachabilityVisitor() {}

  virtual void setFilter(FilterType & f) {
    FilterList.push_back(f);
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    StartVertex = startVertex;
    DepthList.insert(VertexTargetPair(startVertex, 0));
  }

  VertexTargetMapType & getVertexTargetMap() {
    return VertexList;
  }

  virtual void setDepth(unsigned int depth) {
    DepthSetting = depth;
  }

  virtual bool visitVertex(VertexPointer vertex) {
    if (checkMaxDepth(DepthList) >= DepthSetting) {
     for (auto it = DepthList.begin(); it != DepthList.end(); ++it ) {
       if ((*it).second == DepthSetting) {
         VertexList.insert(VertexTargetPair((*it).first, true));
       }
     }
    }
    return false;
  }

  virtual bool scheduleBranch(VertexPointer first
                            , EdgePointer edge
                            , VertexPointer second) {

    TypeMatch = false;
    DirectionMatch = false;

    /// TODO I forgot what I was doing here, add comments later
    for (auto it = DepthList.equal_range(first).first; 
        it != DepthList.equal_range(first).second; ++it) {
      if ((*it).first == first) {
        if ((*it).second < DepthSetting) {
          FilterType filter = FilterList[(*it).second];
          if (DepthList.count(second) <= DepthSetting) {
            TypeMatch = checkType(edge, filter);
            DirectionMatch = checkDirection(second, edge, filter);
            if (TypeMatch && DirectionMatch)
              recordDepth(first, edge, second, DepthList);
          } //end if 
        }
      }
    }

   return false;
  }

  virtual bool visitDirection(VertexPointer target, EdgePointer edge) {
    return DirectionMatch;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    return TypeMatch;
  }

protected:
  FilterType Filter;
  MultiDepthList   DepthList;
  VertexPointer    StartVertex;
  bool TypeMatch;
  bool DirectionMatch;
  unsigned int     DepthSetting;
  VertexTargetMapType     VertexList;
  std::vector<FilterType> FilterList;
};


class DFSPatternVisitor : public Visitor {
public:
  typedef std::vector<VertexPath>  PathStackType;
public:
  DFSPatternVisitor() {}

  void setFilter(FilterType & f) {
    FilterList.push_back(f);
  }

  void setDepth(unsigned int depth) {
    DepthSetting = depth;
  }
  
  void setEndVertex(VertexDescriptor vertex) {
    EndVertex =  vertex;
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    StartVertex = startVertex;
    VertexPath NewPath;
    NewPath.push_back(startVertex);
    PathStack.push_back(NewPath);
  }

  virtual bool visitVertex(VertexPointer vertex) {
    PrevPath = PathStack.back();
    PathStack.pop_back();
    
    /// TODO The same... add comments later
    if (PrevPath.size() > DepthSetting) {
      if (PrevPath.at(DepthSetting)->getId() == EndVertex) {
        VertexSet.insert(PrevPath.at(1));
      }
    }
    return false;
  }

  virtual bool discoverVertex(VertexPointer vertex) {
    return true; 
  }

  virtual bool visitDirection(VertexPointer target, EdgePointer edge) {
    return DirectionMatch;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    return TypeMatch;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    TypeMatch = false;
    DirectionMatch = false;
    if (PrevPath.size() > DepthSetting) {
      return true;
    }

    unsigned int firstDepth = 1000; 
    /// Default: get default filter later
    if (PrevPath.back() == first) {
      firstDepth = PrevPath.size() - 1;
    }

    FilterType filter;
      if (firstDepth >= 0 && firstDepth < DepthSetting)
        filter = FilterList[firstDepth];
    else
      filter.setDefault();

    if (second != StartVertex) {
       TypeMatch = checkType(edge, filter);
       DirectionMatch = checkDirection(second, edge, filter);
    }
    return false;
  }

  virtual bool scheduleTree(VertexPointer first
                          , EdgePointer edge
                          , VertexPointer second) {

    if (TypeMatch && DirectionMatch) {
      VertexPath NewPath = PrevPath;
      NewPath.push_back(second);
      PathStack.push_back(NewPath);
    }
    return false;
  }

protected:
  bool TypeMatch;
  bool DirectionMatch;
  unsigned int  DepthSetting;
  VertexPointer StartVertex;
  VertexDescriptor EndVertex;
  std::vector<FilterType> FilterList;
  VertexPath   PrevPath;
  PathStackType PathStack;
};


class DFSShortestPathVisitor : public Visitor {
public:
  typedef std::vector<VertexPath>  PathStackType;
  typedef std::multimap<unsigned int, VertexPath> PathMapType;
  typedef std::pair<unsigned int, VertexPath> PathPair;
public:
  void setDepth(unsigned int depth) {
    DepthSetting = depth;
  }
  
  void setEndVertex(VertexDescriptor vertex) {
    EndVertex =  vertex;
  }

  PathMapType & getPathList() {
    return PathMap;
  }

  virtual void visitStartVertex(VertexPointer startVertex ) {
    StartVertex = startVertex;
    TempMinDepth = 500;
    TurnFlag = false;
    VertexPath NewPath;
    NewPath.push_back(startVertex);
    PathStack.push_back(NewPath);
  }

  virtual bool visitVertex(VertexPointer vertex) {
    PrevPath = PathStack.back();
    PathStack.pop_back();
    if (PrevPath.size() >=  TempMinDepth) {
      TurnFlag = true; //prune not to go on with this branch
    } else 
      TurnFlag = false;
    return false;
  }

  virtual bool discoverVertex(VertexPointer vertex) {
    return true; 
  }

  virtual bool visitDirection(VertexPointer target, EdgePointer edge) {
    return DirectionMatch;
  }

  virtual bool scheduleEdge(EdgePointer edge) {
    return TypeMatch;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    TypeMatch = false;
    DirectionMatch = false;
    /// If the depth of this branch already exceeds the shortest path, then prune
    if (TurnFlag == true) {
      return true;
    }

    VertexPath NewPath = PrevPath;
    NewPath.push_back(second);
    /// If the endVertex is found, add to map and never visit this branch and following branches again
    if (second->getId() == EndVertex) {
      if (NewPath.size() <= TempMinDepth) {
        if (NewPath.size() < TempMinDepth)
          PathMap.clear();
        PathMap.insert(PathPair(NewPath.size(), NewPath));
        TempMinDepth = NewPath.size();
      }
      return true;
    } else {
      TypeMatch = true;
      DirectionMatch = true;
      return false;
    }
  }

  virtual bool scheduleTree(VertexPointer first, EdgePointer edge, VertexPointer second){

    if (TypeMatch && DirectionMatch) {
      VertexPath NewPath = PrevPath;
      NewPath.push_back(second);
      PathStack.push_back(NewPath);
    }
    return false;
  }

protected:
  bool TurnFlag;
  unsigned int  TempMinDepth;
  unsigned int  TypeMatch;
  unsigned int  DirectionMatch;
  unsigned int  DepthSetting;
  VertexPointer StartVertex;
  VertexDescriptor EndVertex;
  std::vector<FilterType> FilterList;
  PathMapType     PathMap;
  VertexPath   PrevPath;
  PathStackType PathStack;
};

class DFSPathVisitor : public DFSShortestPathVisitor {
public:
  virtual bool visitVertex(VertexPointer vertex) {
    PrevPath = PathStack.back();
    PathStack.pop_back();
    if (PrevPath.size() >=  TempMinDepth) {
      /// Prune not to go on with this branch
      TurnFlag = true; 
    } else 
      TurnFlag = false;
      return TerminateFlag;
  }

  virtual bool scheduleBranch(VertexPointer first, EdgePointer edge, VertexPointer second) {
    TypeMatch = false;
    DirectionMatch = false;
    /// if the depth of this branch already exceeds the shortest path, then prune
    if (TurnFlag == true) {
      return true;
    }

    VertexPath NewPath = PrevPath;
    NewPath.push_back(second);
    if (second->getId() == EndVertex) {
//      if (NewPath.size() <= TempMinDepth) {
//        if(NewPath.size() < TempMinDepth)
//          PathMap.clear();
        PathMap.insert(PathPair(NewPath.size(), NewPath));
        TempMinDepth = NewPath.size();
        TerminateFlag = true;
//      }
      return true;
    } else {
      TypeMatch = true;
      DirectionMatch = true;
      return false;
    }
  }

protected:
  bool TerminateFlag = false;
};

#endif /**_CUSOTMVISITOR_H_*/
