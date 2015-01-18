#ifndef _EDGE_H_
#define _EDGE_H_

//#include "Vertex.h"
class Vertex;

class Edge {
public:
  typedef Vertex* VertexPtr;
  typedef Edge* EdgePtr;
  typedef unsigned int EdgeDescriptor;
  typedef unsigned int VertexDescriptor;

public:

  VertexDescriptor getFirstId();
  EdgePtr getFirstNextEdge();
  EdgePtr getFirstPreviousEdge();
  // Independent of who is first/second.
  EdgePtr getNextEdge(VertexPtr from);
  EdgePtr getSecondNextEdge();
  EdgePtr getSecondPreviousEdge();
  // Independent of who is first/second.
  VertexPtr getTarget(VertexPtr from);

  EdgeDescriptor getId();
  VertexDescriptor getSecondId();

  void setFirstNextEdge(EdgePtr e);
  void setFirstPreviousEdge(EdgePtr e);
  void setId(unsigned int id);
  void setSecondNextEdge(EdgePtr e);
  void setSecondPreviousEdge(EdgePtr e);

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
};

#endif /* _EDGE_H_ */
