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

    // cout << "vertices: " << _vertices.size() << endl;
    // cout << "get: (" << vs << ", " << vd << ")\n" ;
    // cout << "create new edge: (" << _vertices[vs] << ", " << _vertices[vd] << ")\n";

    EdgePtr e = new Edge(_vertices[vs], _vertices[vd]);

    e->setId(ne);    ne++;

    e->setEdges();
    
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
    for (int i =0; i < _vertices.size(); i++) {
      delete _vertices[i];
    }
    for (int i =0; i < _edges.size(); i++) {
      delete _edges[i];
    }
  }

protected:
  std::vector<Node*> _vertices;
  std::vector<Edge*> _edges;
  unsigned int nv;
  unsigned int ne;

};

#endif /* _BASE_GRAPH_H */
