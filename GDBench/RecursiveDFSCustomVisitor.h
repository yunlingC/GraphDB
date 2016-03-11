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

/// This is not efficient in terms of algorithm
/// It takes like forever to find a shortest path between 0 and 1
/// without going into a loop
class RecursiveDFSShortestPathVisitor : public Visitor {
public:
  typedef std::vector<VertexPath> PathStackType;
public:

  VertexPath & getShortestPath() {
    return ShortestPath;
  }

  void setEndVertex(VertexDescriptor  EndVertexId) {
    EndVertex = EndVertexId; 
  }

  /// Set to true to allow revisit to vertices 
  /// that show up in different depths
  virtual bool discoverVertex(VertexPointer Vertex) {
    return true;
  }

  virtual void visitStartVertex(VertexPointer Vertex) {
    /// If the given vertices are the same, no shortest path found;
    /// return immediately.
    if (Vertex->getId() == EndVertex) {
      exit(0);
    }

    VertexPath NewPath;
    NewPath.push_back(Vertex);
    PathStack.push_back(NewPath);

    /// Initialize the tmpShortestlength to be a big number
    TmpShortestLength = 1000;
    ReturnFlag = false;
  }

  /// Check if the current path is already longer than the current shortest one
  virtual bool visitVertex(VertexPointer Vertex) {
    auto PrevPath = PathStack.back();
//    std::cout << "--vertex " << Vertex->getId() + 1 
//              << " -- depth " << PrevPath.size() -1 << "\n";
    return (PrevPath.size() > TmpShortestLength);
  }

  virtual bool scheduleBranch(VertexPointer FirstVertex
                            , EdgePointer Edge
                            , VertexPointer SecondVertex) {

    if (ReturnFlag) {
      /// Flip back so that recursive functions won't return forever.
      ReturnFlag = false;
      return true;;
    }

    /// This is to make sure the same vertex won't be revisited again 
    /// in the current path --> prevent infinite loop
    auto PrevPath = PathStack.back();
    for (auto it = PrevPath.begin(), it_end = PrevPath.end();
            it != it_end; it++) {
      if ((*it) == SecondVertex) {
        return true;
      }
    }

    auto CurrentPath = PrevPath;
    CurrentPath.push_back(SecondVertex);

    if (SecondVertex->getId() == EndVertex) {
      ShortestPath = CurrentPath;
      TmpShortestLength = CurrentPath.size() - 1 ;
      /// Set to true so that the previous level can exit without 
      /// visiting more vertices of the same depth
      ReturnFlag = true;
    } else {
      PathStack.push_back(CurrentPath);
    }
    return false;
  }

  virtual bool lastVisit(VertexPointer Vertex) {
    auto LastPath = PathStack.back();
    if (LastPath.back() == Vertex) {
      PathStack.pop_back(); 
    }
    return false;
  }

protected:
 bool ReturnFlag;
 unsigned int TmpShortestLength;
 VertexDescriptor EndVertex;
 PathStackType PathStack;
 VertexPath ShortestPath;
};


class RecursiveDFSPathVisitor : public Visitor {
public:
  typedef std::vector<VertexPath> PathStackType;
public:
  VertexPath & getTargetPath() {
    return TargetPath;
  }

  void setEndVertex(VertexDescriptor  EndVertexId) {
    EndVertex = EndVertexId; 
  }

  /// We don't revisit the vertex if we cannot find a 
  /// connection the first time we viist it
  virtual bool discoverVertex(VertexPointer Vertex) {
    return false;
  }

  virtual void visitStartVertex(VertexPointer Vertex) {
    /// If the given vertices are the same, no shortest path found;
    /// return immediately.
    if (Vertex->getId() == EndVertex) {
      exit(0);
    }

    VertexPath NewPath;
    NewPath.push_back(Vertex);
    PathStack.push_back(NewPath);

    /// Initialize the exit flag 
    ExitMatch = false;
  }

  virtual bool checkVertex(VertexPointer Vertex) {
//    auto PrevPath = PathStack.back();
//    std::cout << "--vertex " << Vertex->getId() + 1 
//              << " -- depth " << PrevPath.size() -1 << "\n";
//    if (ExitMatch) {
//      std::cout <<"exit now \n";
//    }
    return ExitMatch;
  }

  virtual bool scheduleTree(VertexPointer FirstVertex
                            , EdgePointer Edge
                            , VertexPointer SecondVertex) {

    ///TODO Might be redundant
    if (ExitMatch) {
      return true;;
    }

    /// This is to make sure the same vertex won't be revisited again 
    /// in the current path --> prevent infinite loop
    auto PrevPath = PathStack.back();
    for (auto it = PrevPath.begin(), it_end = PrevPath.end();
            it != it_end; it++) {
      if ((*it) == SecondVertex) {
        return true;
      }
    }

    auto CurrentPath = PrevPath;
    CurrentPath.push_back(SecondVertex);

    if (SecondVertex->getId() == EndVertex) {
      TargetPath = CurrentPath;
      ExitMatch = true;
      return true;
    } else {
      PathStack.push_back(CurrentPath);
    }
    return false;
  }

  virtual bool lastVisit(VertexPointer Vertex) {
    auto LastPath = PathStack.back();
    if (LastPath.back() == Vertex) {
      PathStack.pop_back(); 
    }
    return false;
  }

protected:
  bool ExitMatch;
  VertexDescriptor EndVertex;
  PathStackType PathStack;
  VertexPath TargetPath;
};

#endif /*_RECURSIVE_DFS_CUSTOMVISITOR_H_*/
