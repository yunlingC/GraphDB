//===-- traversals/CustomVisitor.h - Customed visitor class -----*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the customed class for Graph visitors.
/// 
//===----------------------------------------------------------------------===//

#ifndef _RECURSIVE_DFS_CUSTOMVISITOR_H_
#define _RECURSIVE_DFS_CUSTOMVISITOR_H_

#include "CustomVisitor.h"

class RecursiveDFSReachabilityVisitor: public Visitor { 
public:
  typedef std::map<VertexPointer,  unsigned int> VertexTargetMapType;
  typedef std::pair<VertexPointer, unsigned int> VertexTargetPair;
  typedef std::set<VertexPointer> VertexSetType;
  typedef std::vector<VertexPath> PathStackType;
public:
  RecursiveDFSReachabilityVisitor() {}

  void setFilter(FilterType & Filter) {
    FilterList.push_back(Filter);
  }

  void setDepth(unsigned int Depth) {
    DepthSetting = Depth;
  }

  virtual VertexSetType & getTargetSet() {
    return VertexSet;
  }

  virtual bool discoverVertex(VertexPointer Vertex) {
    return true;
  }

  virtual void visitStartVertex(VertexPointer Vertex) { 
    VertexPath NewPath;
    NewPath.push_back(Vertex);
    PathStack.push_back(NewPath);
  }

  virtual bool visitVertex(VertexPointer vertex) {
    auto PrevPath = PathStack.back();
    return (PrevPath.size() > DepthSetting);
  }

  virtual bool scheduleBranch(VertexPointer first
                            , EdgePointer edge
                            , VertexPointer second) {

    /// In default, don't go further into the graph.
    /// Only explore the next hop when :
    /// 1. The current depth is not further than the depthsetting
    /// 2. The type and direction of the second vertex matches requirements.
    TypeMatch = false;
    DirectionMatch = false;

    ///Don't pop out until the current branch won't be visited again
    auto PrevPath = PathStack.back();

    unsigned int FirstDepth = 1000;
    if (PrevPath.back() == first) {
      FirstDepth = PrevPath.size() - 1; 
    }

    FilterType Filter;
    if (FirstDepth >= 0 && FirstDepth < DepthSetting) {
      Filter = FilterList[FirstDepth];
    }
    else {
      Filter.setDefault();
    }

    if (second != StartVertex) {
      TypeMatch = checkType(edge, Filter);
      DirectionMatch = checkDirection(second, edge, Filter);
    }

    return false;
  }

  virtual bool scheduleEdge(EdgePointer Edge) {
    return  TypeMatch;
  }

  virtual bool visitDirection(VertexPointer Vertex, EdgePointer Edge) {
    return DirectionMatch;
  }
 
  virtual bool scheduleTree(VertexPointer FirstVertex, EdgePointer NextEdge
                          , VertexPointer SecondVertex) {

    /// Move this logic from scheduleBranch to scheduleTree
    /// This is because some vertex might be visited 
    /// (Revisited is disabled in default)
    if (TypeMatch && DirectionMatch) {
      VertexPath NewPath = PathStack.back();
      NewPath.push_back(SecondVertex);
      PathStack.push_back(NewPath);
    }
    return false;
  }

  virtual bool lastVisit(VertexPointer Vertex) {
    auto LastPath = PathStack.back();
    if (LastPath[LastPath.size()-1] == Vertex) {
      PathStack.pop_back();    
    }
    if (LastPath.size() ==  DepthSetting + 1) {
      VertexSet.insert(LastPath[DepthSetting]);
    }
    return false;
  }

protected:
  bool TypeMatch;
  bool DirectionMatch;
  unsigned int DepthSetting;
  VertexPointer StartVertex;
  std::vector<FilterType> FilterList;
  PathStackType PathStack;
  VertexSetType VertexSet;

};

class RecursiveDFSPathVisitor : public Visitor {
public:


protected:

};

#endif /*_RECURSIVE_DFS_CUSTOMVISITOR_H_*/
