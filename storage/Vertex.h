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

  // PropertyList typedefs.
  typedef PropertyList< KeyType, ValueType > PropertyListType;
  typedef std::pair<ValueType, bool> ReturnPropertyValueType;

public:
  Vertex();
  void addEdge(EdgePtr ep);
  void addOutEdge(EdgePtr ep);
  void dump();
  std::vector<EdgePtr> & getEdges();
  std::vector<EdgePtr> & getOutEdges();
  unsigned int getId();
  EdgePtr getNextEdge();
  ReturnPropertyValueType getPropertyValue(KeyType k);
  void setId(unsigned int id);
  void setNextEdge(EdgePtr e);
  void setProperty(KeyType k, ValueType v);
  void setPropertyList(PropertyListType & l);
  PropertyListType &  getPropertyList();
  void deleteVertex();

protected:
  // Vertex id.
  unsigned int _id;
  // Pointer to next relationship
  EdgePtr _nextEdge;

  std::vector<EdgePtr> AllEdges;
  std::vector<EdgePtr> OutEdges;
  PropertyListType PropertyList;
};

#endif /* _VERTEX_H_ */
