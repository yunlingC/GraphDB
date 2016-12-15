//===-- storage/Vertex.h - Vertex class type --------------------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the main class for the Vertex structure.
///
//===---------------------------------------------------------------------===//

#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "PropertyList.h"

#include <vector>

/// Forward declaration.
#ifdef _LOCKING_STORAGE_
class VertexLock;
#endif

class Edge;

class Vertex { 
public:
  typedef Edge* EdgePtr;
  typedef unsigned int EdgeDescriptor;
  typedef FixedString KeyType;
  typedef FixedString ValueType;
  typedef FixedString LabelType;
#ifdef _LOCKING_STORAGE_
  typedef VertexLock* VertexLockPointer;
#endif

  // PropertyList typedefs.
  typedef PropertyList< KeyType, ValueType > PropertyListType;
  typedef PropertyListType& PropertyListTypeReference;
  typedef std::vector<EdgePtr>& EdgeListReference;
  typedef std::pair<ValueType, bool> ReturnPropertyValueType;

public:
  Vertex();
  unsigned int getId();
  EdgePtr getNextEdge();
//  EdgePtr getLastEdge();
  LabelType getType();
  ReturnPropertyValueType getPropertyValue(KeyType Key);
  ReturnPropertyValueType getPropertyValue(std::string Key);
  void setId(unsigned int Id);
  void setType(std::string Type);
  void setType(LabelType Type);
  void setNextEdge(EdgePtr Edge);
//  void setLastEdge(EdgePtr Edge);
  void setProperty(KeyType Key, ValueType Value);
  void setPropertyList(PropertyListType & List);
  bool updateProperty(KeyType Key, ValueType Value);
  bool updateProperty(std::string Key, std::string Value);
  PropertyListType &  getPropertyList();

#ifdef _LOCKING_STORAGE_
  void setVertexLock(VertexLockPointer NewLock);
  VertexLockPointer getVertexLock();
#endif
  void deleteVertex();

#ifdef _DEBUG_
  void dump();
#endif


protected:
  // Vertex id.
  unsigned int VertexId;
  // Pointer to next relationship
  EdgePtr NextEdge;

//  EdgePtr LastEdge;

  LabelType VertexLabel;

  PropertyListType VertexPropertyList;

#ifdef _LOCKING_STORAGE_
  VertexLockPointer  VertexLock; 
#endif 

};

#endif /* _VERTEX_H_ */
