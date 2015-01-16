#include <iostream>

#include "Node.h"
#include "Edge.h"

using namespace std;

Edge::EdgePtr Edge::getFirstNextEdge() {
  return _firstNextEdge;
}

Edge::EdgePtr Edge::getFirstPreviousEdge() {
  return _firstPreviousEdge;
}


Edge::VertexDescriptor Edge::getFirstId() {
  return _firstNode->getId();
}

Edge::VertexDescriptor Edge::getSecondId() {
  return _secondNode->getId();
}

void Edge::setId(unsigned int id) {
    _id = id;
  }

Edge::EdgeDescriptor Edge::getId() {
    return _id;
  }

void Edge::setEdges() {

  // Must set the _id before doing this.

  // Set first node's Next and previous pointer
  _firstNextEdge = _firstNode->getNextEdge(_id);
  _firstPreviousEdge = _firstNode->getPreviousEdge(_id);

  cout << "==> this: " << _id << ", fn: ";
  if (_firstNextEdge != NULL) 
    cout << _firstNextEdge->getId();
  else cout << "XX";

  cout << ", fp: ";
  if (_firstPreviousEdge != NULL)  {
    cout << _firstPreviousEdge->getId();

  }
  else cout << "YY";

  cout << "\n";
  // Set second node's next and previous pointer.
  _secondNextEdge = _secondNode->getNextEdge(_id);
  _secondPreviousEdge = _secondNode->getPreviousEdge(_id);

}

Edge::Edge(): _id(-1) { 
    _firstNode = NULL;     _secondNode = NULL;
    _firstNextEdge = NULL; _firstPreviousEdge = NULL;
    _secondNextEdge = NULL; _secondPreviousEdge = NULL;
  }

Edge::Edge(NodePtr vs, NodePtr vd) {
    // first is vs, second is vd
  _firstNode = vs; 
  _firstNode->addEdge(this);
  _secondNode = vd; 
  _secondNode->addEdge(this);
  }
  
