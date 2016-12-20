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

#include "global.h"
#include "PropertyList.h"

#ifdef _LOCKING_STORAGE_
#include "Lock.h"
#endif 

class Vertex;

class Edge {
public:
  typedef Vertex* VertexPointer;
  typedef Edge* EdgePtr;
  typedef Edge* EdgePointer;
  typedef unsigned int EdgeDescriptor;
  typedef unsigned int VertexDescriptor;
#ifdef _LOCKING_STORAGE_
  typedef EdgeLock* EdgeLockPointer;
#endif

  typedef FixedString KeyType;
  typedef FixedString ValueType;
  typedef FixedString LabelType;
  typedef PropertyList<KeyType, ValueType > PropertyListType;
  typedef PropertyListType& PropertyListReference;
  typedef std::pair<ValueType, bool> ReturnValueType;

public:

#ifdef _DEBUG_
  void dump();
#endif
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

  void setFirstVertexPtr(VertexPointer Vertex);
  void setSecondVertexPtr(VertexPointer Vertex);
  void setFirstNextEdge(EdgePointer e);
  void setFirstPreviousEdge(EdgePointer e);
  void setId(unsigned int id);
  void setProperty(KeyType k, ValueType v);
  void setPropertyList(PropertyListType l);
  void setSecondNextEdge(EdgePointer e);
  void setSecondPreviousEdge(EdgePointer e);
  void setType(std::string t);
  void setType(LabelType Type);

#ifdef _LOCKING_STORAGE_
  void setLockPtr(EdgeLockPointer el);
  EdgeLockPointer getLockPtr();
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

#ifdef _LOCKING_STORAGE_
  EdgeLockPointer LockPtr;
#endif 

};

#endif /* _EDGE_H_ */
