#ifndef _EDGE_H_
#define _EDGE_H_

//#include "Node.h"
class Node;

class Edge {
public:
  typedef Node* NodePtr;
  typedef Edge* EdgePtr;
  typedef unsigned int EdgeDescriptor;
  typedef unsigned int VertexDescriptor;

public:

  VertexDescriptor getFirstId();
  EdgePtr getFirstNextEdge();
  EdgePtr getFirstPreviousEdge();
  // Independent of who is first/second.
  EdgePtr getNextEdge(NodePtr from);
  EdgePtr getSecondNextEdge();
  EdgePtr getSecondPreviousEdge();
  // Independent of who is first/second.
  NodePtr getTarget(NodePtr from);

  EdgeDescriptor getId();
  VertexDescriptor getSecondId();

  void setFirstNextEdge(EdgePtr e);
  void setFirstPreviousEdge(EdgePtr e);
  void setId(unsigned int id);
  void setSecondNextEdge(EdgePtr e);
  void setSecondPreviousEdge(EdgePtr e);

  // Constructors
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
