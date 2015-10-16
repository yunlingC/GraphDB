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

#ifdef _LOCKING_
class EdgeLock;
#endif 

class Vertex;

class Edge {
public:
  typedef Vertex* VertexPointer;
  typedef Edge* EdgePtr;
  typedef Edge* EdgePointer;
  typedef unsigned int EdgeDescriptor;
  typedef unsigned int VertexDescriptor;
#ifdef _LOCKING_
  typedef EdgeLock* EdgeLockPointer;
#endif

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
  EdgePointer getFirstNextEdge();
  EdgePointer getFirstPreviousEdge();

  /// Independent of who is first/second.
  EdgePointer getNextEdge(VertexPointer from);
  EdgePointer getPreviousEdge(VertexPointer from);
  PropertyListReference getPropertyList();
  ReturnValueType getPropertyValue(KeyType k);
  ReturnValueType getPropertyValue(std::string k);
  EdgePointer getSecondNextEdge();
  EdgePointer getSecondPreviousEdge();

  // Independent of who is first/second.
  VertexPointer getTarget(VertexPointer from);

  EdgeDescriptor getId();
  VertexDescriptor getSecondId();
  LabelType getType();

  void setFirstNextEdge(EdgePointer e);
  void setFirstPreviousEdge(EdgePointer e);
  void setId(unsigned int id);
  void setProperty(KeyType k, ValueType v);
  void setPropertyList(PropertyListType l);
  void setSecondNextEdge(EdgePointer e);
  void setSecondPreviousEdge(EdgePointer e);
  void setType(string t);

#ifdef _LOCKING_
  void setEdgeLock(EdgeLockPointer el);
  EdgeLockPointer getEdgeLock();
#endif

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
  EdgePointer FirstNextEdge;
  // Pointer to first pointer's previous relationship.
  EdgePointer FirstPreviousEdge;

  // Repeat for second.
  EdgePointer SecondNextEdge;
  EdgePointer SecondPreviousEdge;
  
  // Label to denote type.
  LabelType EdgeLabel;

  PropertyListType EdgePropertyList;

#ifdef _LOCKING_
  EdgeLockPointer ELock;
#endif 

};

#endif /* _EDGE_H_ */
