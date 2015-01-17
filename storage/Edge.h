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
  EdgeDescriptor getId();
  VertexDescriptor getFirstId();
  VertexDescriptor getSecondId();

  void setFirstNextEdge(EdgePtr e);
  EdgePtr getFirstNextEdge();
  void setFirstPreviousEdge(EdgePtr e);
  EdgePtr getFirstPreviousEdge();

  // Second
  void setSecondNextEdge(EdgePtr e);
  EdgePtr getSecondNextEdge();
  void setSecondPreviousEdge(EdgePtr e);
  EdgePtr getSecondPreviousEdge();

  // Independent of who is first/second.
  NodePtr getTarget(NodePtr from);
  EdgePtr getNextEdge(NodePtr from);


  Edge();
  Edge(NodePtr vs, NodePtr vd);
  
 protected:
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
