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
  VertexDescriptor addVertex() {
    // Create new node
    NodePtr n = new Node();
    // set Id.
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
     //     e->setEdges();
     _edges.push_back(e);
     return e->getId();
  }

  void dump() {
    for (int i = 0; i < _vertices.size(); i++) {
      _vertices[i]->dump();
    }
    
    for (int i =0; i < _edges.size(); i++) {
      cout << "eid: " << _edges[i]->getId(); 
      cout << ", prev: ";
      if (_edges[i]->_firstPreviousEdge != NULL) {
	cout << _edges[i]->getId() <<", ";
      }
      cout << ", next: ";
      if (_edges[i]->_firstNextEdge != NULL) {
	cout << _edges[i]->_firstNextEdge->getId();
      }
      cout << "\n";
    }
  }

  BaseGraph(): nv(0), ne(0) {}

  ~BaseGraph() {
    for (int i =0; i < _vertices.size(); i++) {
      delete _vertices[i];
    }
    for (int i =0; i < _edges.size(); i++) {
      delete _edges[i];
    }
  }

  void updateEdges() {
    // Set the first node edges
    for (int i = 0; i < _vertices.size(); i++) {
      NodePtr np = _vertices[i];

      cout << "= Processing node: " << np->getId() << "\n";
      std::vector<EdgePtr> & nodeEdges = np->getEdges();
      if (! nodeEdges.empty()) {

	EdgePtr ep = nodeEdges[0];
	if (ep != NULL) {
	  cout << "== Processing edge id: " << ep->getId() << "\n";
	  ep->_firstPreviousEdge = NULL;
	  np->setNextEdge(ep);

	  cout << "== Need to hook up # of edges: " << nodeEdges.size() << "\n";
	  EdgePtr nextEdge = NULL;
	  // Iterate over all the edges and set the first/second
	  if (np->getId() == ep->getFirstId()) {
	    for (int j = 1; j < nodeEdges.size(); j++) {
	      // Set the first
	      nextEdge = nodeEdges[j];
	      ep->_firstNextEdge = nextEdge;
	      cout << " ==== setting: " << ep->getId() << ", " << nextEdge->getId() << "\n";
	      nextEdge->_firstPreviousEdge = ep;
	      ep = nextEdge;
	    }
	    ep->_firstNextEdge = NULL;
	  } else {
	    if (np->getId() == ep->getSecondId()) {
	      for (int j = 1; j < nodeEdges.size(); j++) {
		// Set the first
		nextEdge = nodeEdges[j];
		ep->_secondNextEdge = nextEdge;
		cout << " ==== setting: " << ep->getId() << ", " << nextEdge->getId() << "\n";
	      nextEdge->_secondPreviousEdge = ep;
	      ep = nextEdge;
	      }
	    }
	  }
	}

      }
    }
  }
  

protected:
  std::vector<Node*> _vertices;
  std::vector<Edge*> _edges;
  unsigned int nv;
  unsigned int ne;

};

#endif /* _BASE_GRAPH_H */

