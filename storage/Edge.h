//===-- storage/Edge.h - Edge class type -------------------------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the main class for the Edge structure.
///
//===----------------------------------------------------------------------===//
#ifndef _EDGE_H_
#define _EDGE_H_

#include <string>

#include "PropertyList.h"
#include "FixedString.h"

class Vertex;

class Edge {
public:
  typedef Vertex* VertexPointer;
  typedef Edge* EdgePtr;
  typedef unsigned int EdgeDescriptor;
  typedef unsigned int VertexDescriptor;
  
  typedef FixedString KeyType;
  typedef FixedString ValueType;
  typedef FixedString LabelType;
  typedef PropertyList<KeyType, ValueType > PropertyListType;
  typedef PropertyListType& PropertyListReference;
  typedef std::pair<ValueType, bool> ReturnValueType;

public:

  void dump();
  VertexDescriptor getFirstId();
  VertexPointer getFirstVertexPtr();
  VertexPointer getSecondVertexPtr();
  EdgePtr getFirstNextEdge();
  EdgePtr getFirstPreviousEdge();

  /// Independent of who is first/second.
  EdgePtr getNextEdge(VertexPointer from);
  PropertyListReference getPropertyList();
  ReturnValueType getPropertyValue(KeyType k);
  EdgePtr getSecondNextEdge();
  EdgePtr getSecondPreviousEdge();

  // Independent of who is first/second.
  VertexPointer getTarget(VertexPointer from);

  EdgeDescriptor getId();
  VertexDescriptor getSecondId();
  LabelType getType();

  void setFirstNextEdge(EdgePtr e);
  void setFirstPreviousEdge(EdgePtr e);
  void setId(unsigned int id);
  void setProperty(KeyType k, ValueType v);
  void setPropertyList(PropertyListType l);
  void setSecondNextEdge(EdgePtr e);
  void setSecondPreviousEdge(EdgePtr e);
  void setType(string t);

  /// Constructors.
  Edge();
  Edge(VertexPointer vs, VertexPointer vd);
  void deleteEdge();

protected:
  unsigned int EdgeId;

  // Pointer to first node.
  VertexPointer  FirstVertex;
  // Pointer to second node.
  VertexPointer SecondVertex;

  // Pointer to first pointer's next relationship.
  EdgePtr FirstNextEdge;
  // Pointer to first pointer's previous relationship.
  EdgePtr FirstPreviousEdge;

  // Repeat for second.
  EdgePtr SecondNextEdge;
  EdgePtr SecondPreviousEdge;
  
  // Label to denote type.
  LabelType EdgeLabel;

  PropertyListType EdgePropertyList;
};

#endif /* _EDGE_H_ */
