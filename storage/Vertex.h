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

#include "global.h"
#include "PropertyList.h"

#include <vector>

#ifdef _LOCKING_STORAGE_
#include "Lock.h"
#endif

/// Forward declaration.
class Edge;

class Vertex { 
public:
  typedef Edge* EdgePtr;
  typedef unsigned int EdgeDescriptor;
  /// Alternate type std::string
  typedef FixedString KeyType;
  typedef FixedString ValueType;
  typedef FixedString LabelType;
  // PropertyList typedefs.
  typedef PropertyList< KeyType, ValueType > PropertyListType;
  typedef PropertyListType& PropertyListTypeReference;
  typedef std::vector<EdgePtr>& EdgeListReference;
  typedef std::pair<ValueType, bool> ReturnPropertyValueType;

#ifdef _LOCKING_STORAGE_
  typedef VertexLock* VertexLockPointer;
#endif

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
  void setLockPtr(VertexLockPointer NewLock);
  VertexLockPointer getLockPtr();
#endif
  void deleteVertex();

#ifdef _DEBUG_
  void dump();
#endif


protected:
  /// Vertex id.
  unsigned int VertexId;
  /// Pointer to next relationship
  EdgePtr NextEdge;
  /// LastEdge is not in use
  // EdgePtr LastEdge;
  /// LabelType can be string or FixedString
  LabelType VertexLabel;
  /// PropertyList <KeyType, ValueType>
  PropertyListType VertexPropertyList;
  /// Embedded lock for each vertex
  /// macros are defined in "global.h"
#ifdef _LOCKING_STORAGE_
  VertexLockPointer  LockPtr; 
#endif 

};

#endif /* _VERTEX_H_ */
