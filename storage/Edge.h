#ifndef _EDGE_H_
#define _EDGE_H_

//#include "Node.h"
class Node;

class Edge {
public:
  typedef Node * NodePtr;
  typedef Edge* EdgePtr;
  typedef unsigned int EdgeDescriptor;
  typedef unsigned int VertexDescriptor;

public:

  void setId(unsigned int id);
  void setEdges();
  VertexDescriptor getFirstId();
  VertexDescriptor getSecondId();
  EdgePtr getFirstNextEdge();
  EdgePtr getFirstPreviousEdge();
  EdgeDescriptor getId();

  // Independent of who is first/second.
  NodePtr getTarget(NodePtr from);
  EdgePtr getNextEdge(NodePtr from);


  Edge();
  Edge(NodePtr vs, NodePtr vd);
  
  //protected:
  unsigned int _id;

  // Pointer to first node.
  NodePtr  _firstNode;
  // Pointer to second node.
  NodePtr _secondNode;

  // Pointer to first pointer's next relationship.
  EdgePtr _firstNextEdge;
  // Pointer to first pointer's previous relationship.
  EdgePtr _firstPreviousEdge;

  // Repeat for second.
  EdgePtr _secondNextEdge;
  EdgePtr _secondPreviousEdge;
  
  // Properties can be done later.
};

#endif /* _EDGE_H_ */
