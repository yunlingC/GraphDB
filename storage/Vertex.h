#ifndef _NODE_H_
#define _NODE_H_

#include <vector>
#include <iostream>

#include "PropertyList.h"

class Edge;

class Vertex { 
public:
  typedef Vertex* VertexPtr;
  typedef Edge* EdgePtr;
  typedef unsigned int EdgeDescriptor;

  Vertex();
  void addEdge(EdgePtr ep);
  void dump();
  std::vector<EdgePtr> & getEdges();
  unsigned int getId();
  EdgePtr getNextEdge();
  void setId(unsigned int id);
  void setNextEdge(EdgePtr e);

protected:
  // Vertex id.
  unsigned int _id;
  // Pointer to next relationship
  EdgePtr _nextEdge;

  std::vector<EdgePtr> _outEdges;

  // TODO: pointer to property

};

#endif /* _NODE_H_ */
