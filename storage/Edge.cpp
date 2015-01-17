#include <iostream>

#include "Node.h"
#include "Edge.h"

using namespace std;

Edge::NodePtr Edge::getTarget(Edge::NodePtr from) {
  if (from == _firstNode) {
    return _secondNode;
  } else if (from == _secondNode) {
    return _firstNode;
  }
  return NULL;
}

Edge::EdgePtr Edge::getNextEdge(Edge::NodePtr from) {
  if (from == _firstNode) {
    return _firstNextEdge;
  } else if (from == _secondNode) {
    return _secondNextEdge;
  }
  return NULL;
}

void Edge::setFirstNextEdge(EdgePtr e) {
  _firstNextEdge = e;
}

Edge::EdgePtr Edge::getFirstNextEdge() {
  return _firstNextEdge;
}

void Edge::setFirstPreviousEdge(EdgePtr e) {
  _firstPreviousEdge = e;
}

Edge::EdgePtr Edge::getFirstPreviousEdge() {
  return _firstPreviousEdge;
}



void Edge::setSecondNextEdge(EdgePtr e) {
  _secondNextEdge = e;
}

Edge::EdgePtr Edge::getSecondNextEdge() {
  return _secondNextEdge;
}

void Edge::setSecondPreviousEdge(EdgePtr e) {
  _secondPreviousEdge = e;
}

Edge::EdgePtr Edge::getSecondPreviousEdge() {
  return _secondPreviousEdge;
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
  
