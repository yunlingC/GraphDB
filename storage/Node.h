#ifndef _NODE_H_
#define _NODE_H_

#include <vector>
#include <iostream>

//#include "Edge.h"

class Edge;

class Node { 
public:
  typedef Node* NodePtr;
  typedef Edge* EdgePtr;
  typedef unsigned int EdgeDescriptor;

  Node();
  void addEdge(EdgePtr ep);
  void dump();
  std::vector<EdgePtr> & getEdges();
  unsigned int getId();
  EdgePtr getNextEdge();
  void setId(unsigned int id);
  void setNextEdge(EdgePtr e);

protected:
  // Node id.
  unsigned int _id;
  // Pointer to next relationship
  EdgePtr _nextEdge;

  std::vector<EdgePtr> _outEdges;

  // TODO: pointer to property

};

#endif /* _NODE_H_ */
