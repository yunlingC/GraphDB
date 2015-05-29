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
class Edge;

class Vertex { 
public:
  //  typedef Vertex* VertexPtr;
  typedef Edge* EdgePtr;
  typedef unsigned int EdgeDescriptor;
  typedef FixedString KeyType;
  typedef FixedString ValueType;
  typedef FixedString LabelType;

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
  LabelType getType();
  ReturnPropertyValueType getPropertyValue(KeyType k);
  ReturnPropertyValueType getPropertyValue(std::string k);
  void setId(unsigned int id);
  void setType(string t);
  void setNextEdge(EdgePtr e);
  bool updateProperty(KeyType k, ValueType v);
  bool updateProperty(std::string k, std::string v);
  void setProperty(KeyType k, ValueType v);
  void setPropertyList(PropertyListType & l);
  PropertyListType &  getPropertyList();
  void deleteVertex();

protected:
  // Vertex id.
  unsigned int VertexId;
  // Pointer to next relationship
  EdgePtr NextEdge;

  LabelType VertexLabel;

  PropertyListType VertexPropertyList;
};

#endif /* _VERTEX_H_ */
