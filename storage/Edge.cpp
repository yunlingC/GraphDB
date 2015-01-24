#include <iostream>

#include "Vertex.h"
#include "Edge.h"

using namespace std;

void Edge::dump() {
  cout << "Edge id: " << _id << "\n";
  _list.print();  

}

Edge::VertexPtr Edge::getFirstVertexPtr() {
  return _firstVertex;
}

Edge::VertexPtr Edge::getSecondVertexPtr() {
  return _secondVertex;
}
  
Edge::PropertyListType & Edge::getPropertyList() {
  return _list;
}

Edge::ReturnValueType Edge::getPropertyValue(Edge::KeyType k) {
  return _list.get(k);
}

void Edge::setProperty(Edge::KeyType k, Edge::ValueType v) {
  _list.set(k,v);
}

void Edge::setPropertyList(Edge::PropertyListType l) {
  _list = l;
}

void Edge::setType(string t) {
  _type = t;
}

Edge::VertexPtr Edge::getTarget(Edge::VertexPtr from) {
  if (from == _firstVertex) {
    return _secondVertex;
  } else if (from == _secondVertex) {
    return _firstVertex;
  }
  return NULL;
}

Edge::EdgePtr Edge::getNextEdge(Edge::VertexPtr from) {
  if (from == _firstVertex) {
    return _firstNextEdge;
  } else if (from == _secondVertex) {
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
  return _firstVertex->getId();
}

Edge::VertexDescriptor Edge::getSecondId() {
  return _secondVertex->getId();
}

void Edge::setId(unsigned int id) {
    _id = id;
  }

Edge::EdgeDescriptor Edge::getId() {
  return _id;
}


Edge::Edge(): _id(-1) { 
    _firstVertex = NULL;     _secondVertex = NULL;
    _firstNextEdge = NULL; _firstPreviousEdge = NULL;
    _secondNextEdge = NULL; _secondPreviousEdge = NULL;
  }

Edge::Edge(VertexPtr vs, VertexPtr vd) {
    // first is vs, second is vd
  _firstVertex = vs; 
  _firstVertex->addEdge(this);
  _secondVertex = vd; 
  _secondVertex->addEdge(this);
  }
  
