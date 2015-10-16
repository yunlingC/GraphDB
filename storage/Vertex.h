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
//===----------------------------------------------------------------------===//

#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <vector>
#include <iostream>
#include <string>

#include "PropertyList.h"
#include "FixedString.h"

/// Forward declaration.
#ifdef _LOCKING_
class VertexLock;
#endif

class Edge;

class Vertex { 
public:
  //  typedef Vertex* VertexPtr;
  typedef Edge* EdgePtr;
  typedef unsigned int EdgeDescriptor;
  typedef FixedString KeyType;
  typedef FixedString ValueType;
  typedef FixedString LabelType;
#ifdef _LOCKING_
  typedef VertexLock* VertexLockPointer;
#endif

  // PropertyList typedefs.
  typedef PropertyList< KeyType, ValueType > PropertyListType;
  typedef PropertyListType& PropertyListTypeReference;
  typedef std::vector<EdgePtr>& EdgeListReference;
  typedef std::pair<ValueType, bool> ReturnPropertyValueType;

public:
  Vertex();
  void dump();

  unsigned int getId();
  EdgePtr getNextEdge();
  EdgePtr getLastEdge();
  LabelType getType();
  ReturnPropertyValueType getPropertyValue(KeyType k);
  ReturnPropertyValueType getPropertyValue(std::string k);
  void setId(unsigned int id);
  void setType(string t);
  void setNextEdge(EdgePtr e);
  void setLastEdge(EdgePtr e);
  bool updateProperty(KeyType k, ValueType v);
  bool updateProperty(std::string k, std::string v);
  void setProperty(KeyType k, ValueType v);
  void setPropertyList(PropertyListType & l);
  PropertyListType &  getPropertyList();
#ifdef _LOCKING_
  void setVertexLock(VertexLockPointer NewLock);
  VertexLockPointer getVertexLock();
#endif
  void deleteVertex();

protected:
  // Vertex id.
  unsigned int VertexId;
  // Pointer to next relationship
  EdgePtr NextEdge;

  EdgePtr LastEdge;

  LabelType VertexLabel;

  PropertyListType VertexPropertyList;

#ifdef _LOCKING_
  VertexLockPointer  VLock; 
#endif 

};

#endif /* _VERTEX_H_ */
