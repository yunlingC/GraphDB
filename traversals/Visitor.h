//===-- traversals/Visitor.h - Customed visitor class ----------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the base class for Graph visitors.
///
//===----------------------------------------------------------------------===//
#ifndef _VISITOR_H_
#define _VISITOR_H_

#include "FilterType.h"

#include <unordered_set> 

class GraphType;

/// This is a base class of visitor
/// With default settings in this class, BFT and DFT traverses 
/// the whole graph and visits each vertex andd edge once.
/// Return values of each function below can be customized according to queries.
class Visitor {
public:
  /// FixedString can be replaced with std::string
  typedef FixedString KeyType;
  typedef FixedString ValueType;
  typedef GraphType::VertexPointer VertexPointer;
  typedef GraphType::EdgePointer   EdgePointer;
  typedef GraphType::VertexPropertyList VertexPropertyList;
  typedef GraphType::EdgePropertyList EdgePropertyList;
  typedef GraphType::VertexDescriptor VertexDescriptor;
  typedef std::vector<VertexPointer> VertexTargetType;
  typedef std::unordered_set<VertexPointer> VertexSetType;
  typedef std::vector<VertexPointer> VertexPath;
public:
  Visitor() {
    NumVertices = 0;
    NumEdges = 0;
  }

  const int getNumVertices() {
    return NumVertices;
  }

  const int getNumEdges() {
    return NumEdges;
  }

  virtual void setFilter(FilterType & filter) {
    Filter = filter;
  }

  virtual FilterType & getFilter() {
    return Filter;
  }

  virtual VertexTargetType & getVertexList() {
    return VertexList;
  }

  virtual VertexSetType & getVertexSet() {
    return VertexSet;
  }

  virtual void  visitStartVertex(VertexPointer VertexPtr) { }

  virtual bool discoverVertex(VertexPointer VertexPtr) {
    return false;
  }

  void countVertex(VertexPointer VertexPtr) {
    NumVertices++;
  }

  void countEdge(EdgePointer EdgePtr) {
    NumEdges++;
  }

  virtual bool visitVertex(VertexPointer VertexPtr) {
    return false;
  }

  ///For RecursiveDepthFirstSearch only
  virtual bool checkVertex(VertexPointer VertexPtr) {
    return false;
  }

  virtual bool scheduleVertex(VertexPointer VertexPtr, EdgePointer EdgePtr) {
    return false;
  }

  virtual  bool visitEdge(EdgePointer EdgePtr) {
    return false;
  }
  virtual bool scheduleEdge(EdgePointer EdgePtr) {
    /// Default value = true so that the vertex will be pushed into queue 
    return true; 
  }

  /// For bfs only
  virtual bool visitDirection(VertexPointer TargetVertex, EdgePointer EdgePtr){
    /// The same reason as above
    return true; 
  }

  virtual bool scheduleBranch(VertexPointer FirstVertex, 
                              EdgePointer EdgePtr, 
                              VertexPointer SecondVertex){
     return false;
  }

  virtual bool scheduleTree(VertexPointer FirstVertex, 
                            EdgePointer EdgePtr, 
                            VertexPointer SecondVertex) {
    return false;
  }

  virtual bool revisitVertex(VertexPointer VertexPtr) {
    return false;
  }

  virtual bool lastVisit(VertexPointer VertexPtr) {
    return false;
  }

  virtual bool finishVisit()
  {
    return false;
  }

protected:
  VertexTargetType VertexList; 
  VertexSetType   VertexSet;
  FilterType Filter;
//#ifdef _STATS_
/// Visitor collects stats during each traversal
  int NumVertices;
  int NumEdges;
//#endif 
};

#endif /*_VISITORS_H_ */
