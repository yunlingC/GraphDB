#ifndef _EDGE_H_
#define _EDGE_H_

#include <string>

#include "PropertyList.h"

class Vertex;

class Edge {
public:
  typedef Vertex* VertexPtr;
  typedef Edge* EdgePtr;
  typedef unsigned int EdgeDescriptor;
  typedef unsigned int VertexDescriptor;
  
  typedef std::string KeyType;
  typedef std::string ValueType;
  typedef PropertyList<KeyType, ValueType > PropertyListType;
  typedef pair<ValueType, bool> ReturnValueType;

public:

  VertexDescriptor getFirstId();
  EdgePtr getFirstNextEdge();
  EdgePtr getFirstPreviousEdge();
  // Independent of who is first/second.
  EdgePtr getNextEdge(VertexPtr from);
  PropertyListType & getPropertyList();
  ReturnValueType getPropertyValue(KeyType k);
  EdgePtr getSecondNextEdge();
  EdgePtr getSecondPreviousEdge();
  // Independent of who is first/second.
  VertexPtr getTarget(VertexPtr from);

  EdgeDescriptor getId();
  VertexDescriptor getSecondId();

  void setFirstNextEdge(EdgePtr e);
  void setFirstPreviousEdge(EdgePtr e);
  void setId(unsigned int id);
  void setProperty(KeyType k, ValueType v);
  void setPropertyList(PropertyListType l);
  void setSecondNextEdge(EdgePtr e);
  void setSecondPreviousEdge(EdgePtr e);
  void setType(string t);
  // Constructors
  Edge();
  Edge(VertexPtr vs, VertexPtr vd);
  
 protected:
  unsigned int _id;

  // Pointer to first node.
  VertexPtr  _firstVertex;
  // Pointer to second node.
  VertexPtr _secondVertex;

  // Pointer to first pointer's next relationship.
  EdgePtr _firstNextEdge;
  // Pointer to first pointer's previous relationship.
  EdgePtr _firstPreviousEdge;

  // Repeat for second.
  EdgePtr _secondNextEdge;
  EdgePtr _secondPreviousEdge;
  
  // Properties can be done later.
  // Label to denote type.
  string _type;

  // Property list/map.
  PropertyListType _list;
};

#endif /* _EDGE_H_ */
