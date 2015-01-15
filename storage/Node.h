#ifndef _NODE_H_
#define _NODE_H_

#include <vector>
#include <iostream>

class Edge;

class Node { 
public:
  typedef Node* NodePtr;
  typedef Edge* EdgePtr;
  typedef unsigned int EdgeDescriptor;

  Node();
  void setId(unsigned int id);
  unsigned int getId();
  void addEdge(EdgePtr ep);
  EdgePtr getNextEdge(EdgeDescriptor ed);
  EdgePtr getPreviousEdge(EdgeDescriptor ed);
  void dump();


protected:
  // Node id.
  unsigned int _id;
  // Pointer to next relationship
  EdgePtr _nextEdge;

  std::vector<EdgePtr> _outEdges;

  // TODO: pointer to property

};

#endif /* _NODE_H_ */
