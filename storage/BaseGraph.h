#ifndef _BASE_GRAPH_H_
#define _BASE_GRAPH_H_

#include <vector>
#include <new>
#include <stdlib.h>

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
    if (_nodeMemory == NULL) {
      cerr << "ERROR: Edge space not allocated\n";
      exit(1);
    }

    // Create new node
    char* placePtr = _nodeMemory + _numVertices*sizeof(Node);
    cout << "Place node at: " << reinterpret_cast<int*>(placePtr) << endl;

    NodePtr n = new(placePtr) Node();

    n->setId(_numVertices); 
    _numVertices++;
    // Insert into list.
    _vertices.push_back(n);
    return n->getId();
  }

  EdgeDescriptor addEdge(VertexDescriptor vs, VertexDescriptor vd) {
    if (_edgeMemory == NULL) {
      cerr << "ERROR: Edge space not allocated\n";
      exit(1);
    }
    // Create new edge by retrieving NodePtr from vertices.
    char * placePtr = _edgeMemory + _numEdges*sizeof(Edge);
    cout << "Place edge at: " << reinterpret_cast<int*>(placePtr) << endl;
    EdgePtr e = new (placePtr) Edge(_vertices[vs], _vertices[vd]);

    e->setId(_numEdges);    
    _numEdges++;
    _edges.push_back(e);
    return e->getId();
  }

  void dump() {
    for (int i = 0; i < _vertices.size(); i++) {
      _vertices[i]->dump();
    }
  }

  BaseGraph(): _numVertices(0), _numEdges(0), _nodeMemory(NULL), _edgeMemory(NULL) {
  }

  ~BaseGraph() {
    // Must manually delete the objects.  
    // However, only one place is necessary since everywhere else, I am storing pointers.
    // Thus, _vertices and _edges contain all newly created objects.
    // for (int i =0; i < _vertices.size(); i++) {
    //   //      delete _vertices[i];
    // }
    // for (int i =0; i < _edges.size(); i++) {
    //   //      delete _edges[i];
    // }

    // Delete the memory spaces.
    delete _nodeMemory;
    //cout << "Delete edge  memory\n";
    delete _edgeMemory;
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

  void allocNodeMemory(unsigned int sz) {

    // Allocation sz number of Node objects.
    _nodeMemory = new char[sizeof(Node)*sz];
    cout << "Node Memory\n + Starting address: " << reinterpret_cast<int*>(_nodeMemory) 
	 << ", ending address: " << reinterpret_cast<int*>(_nodeMemory + sizeof(Node)*sz) << "\n";
  }

  void allocEdgeMemory(unsigned int sz) {
    // Allocation sz number of Node objects.
    //cout << "Edge space: " << sizeof(Edge)*sz << "\n";
    _edgeMemory = new char[sizeof(Edge)*sz];
    cout << "Edge Memory\n + Starting address: " << reinterpret_cast<int*>(_edgeMemory) 
	 << ", ending address: " << reinterpret_cast<int*>(_edgeMemory + sizeof(Edge)*sz) << "\n" << endl;
  }

protected:
  vector<NodePtr> _vertices;
  vector<EdgePtr> _edges;
  unsigned int _numVertices;
  unsigned int _numEdges;
  char* _nodeMemory;
  char* _edgeMemory;

};

#endif /* _BASE_GRAPH_H */

