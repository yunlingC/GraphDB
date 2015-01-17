#ifndef _BASE_GRAPH_H_
#define _BASE_GRAPH_H_

#include <vector>

#include "Node.h"
#include "Edge.h"

using namespace std;

class BaseGraph {
public:
  typedef unsigned int VertexDescriptor;
  typedef unsigned int EdgeDescriptor;
  typedef Node* NodePtr;
  typedef Edge* EdgePtr;

public:
  NodePtr getNodePointer(VertexDescriptor v) {
    if ( (v > _vertices.size() - 1) || (v < 0) ) {
      return NULL;
    }
    return _vertices[v];
  }

  VertexDescriptor addVertex() {
    // Create new node
    NodePtr n = new Node();
    n->setId(nv); 
    nv++;
    // Insert into list.
    _vertices.push_back(n);
    return n->getId();
  }

  EdgeDescriptor addEdge(VertexDescriptor vs, VertexDescriptor vd) {
    // Create new edge by retrieving NodePtr from vertices.
     EdgePtr e = new Edge(_vertices[vs], _vertices[vd]);

     e->setId(ne);    
     ne++;
     _edges.push_back(e);
     return e->getId();
  }

  void dump() {
    for (int i = 0; i < _vertices.size(); i++) {
      _vertices[i]->dump();
    }
  }

  BaseGraph(): nv(0), ne(0) {}

  ~BaseGraph() {
    // Must manually delete the objects.  
    // However, only one place is necessary since everywhere else, I am storing pointers.
    // Thus, _vertices and _edges contain all newly created objects.
    for (int i =0; i < _vertices.size(); i++) {
      delete _vertices[i];
    }
    for (int i =0; i < _edges.size(); i++) {
      delete _edges[i];
    }
  }

  void updateEdges() {
    // The basic idea is to iterate over all the vertices and update their first/second next/previous pointers.
    // Since ordering of these pointers does not matter as long as they form a doubly linked list -- I use the vector of edges stored within a node to specify the order.  
    // There are two main if statements here: first one sets pointers based on the firstNode and the second on the secondNode.
    for (int i = 0; i < _vertices.size(); i++) {
      NodePtr np = _vertices[i];
      std::vector<EdgePtr> & nodeEdges = np->getEdges();
      if (! nodeEdges.empty()) {

	EdgePtr ep = nodeEdges[0];
	if (ep != NULL) {
	  ep->setFirstPreviousEdge(NULL); //_firstPreviousEdge = NULL;
	  np->setNextEdge(ep);

	  EdgePtr nextEdge = NULL;
	  // Connect the secondNode
	  if (np->getId() == ep->getFirstId()) {
	    // Iterate over all the edges after the first one.
	    // Recall that first edge is set in the node object.
	    for (int j = 1; j < nodeEdges.size(); j++) {
	      // Get the edge into nextEdge.
	      nextEdge = nodeEdges[j];
	      // Use it to point to the next edge.
	      ep->setFirstNextEdge(nextEdge); //_firstNextEdge = nextEdge;
	      // The previous of this next edge would point to the current edge.
	      nextEdge->setFirstPreviousEdge(ep); //_firstPreviousEdge = ep;
	      // Update the iterating pointer.
	      ep = nextEdge;
	    }
	    // Last one will be NULL>
	    ep->setFirstNextEdge(NULL); //_firstNextEdge = NULL;
	  } else {
	    // Connect the secondNode
	    if (np->getId() == ep->getSecondId()) {
	      for (int j = 1; j < nodeEdges.size(); j++) {
		nextEdge = nodeEdges[j];
		ep->setSecondNextEdge(nextEdge); //_secondNextEdge = nextEdge;
		nextEdge->setSecondPreviousEdge(ep); //_secondPreviousEdge = ep;
	      ep = nextEdge;
	      }
	    }
	  }
	}

      }
    }
  }
  

protected:
  std::vector<NodePtr> _vertices;
  std::vector<EdgePtr> _edges;
  unsigned int nv;
  unsigned int ne;

};

#endif /* _BASE_GRAPH_H */

