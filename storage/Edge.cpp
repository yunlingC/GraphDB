//===-- storage/Edge.cpp - Edge implementation ------------------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the implementation for the Edge class.
///
//===----------------------------------------------------------------------===//
#include <iostream>

#include "Vertex.h"
#include "Edge.h"

using namespace std;

void Edge::dump() {
  cout << "\nEdge id: " << EdgeId << " (" << FirstVertex->getId() << ", " << SecondVertex->getId() << ") " << endl;
  //  cout << "first next eid: " << FirstVertex->getNextEdge()->getId() << "\n";
  cout << "\nEdge PropertyList\n";
  EdgePropertyList.print();

  EdgePtr fne = getFirstNextEdge();
  EdgePtr fpe = getFirstPreviousEdge();
  EdgePtr sne = getSecondNextEdge();
  EdgePtr spe = getSecondPreviousEdge();

  if (fpe != nullptr) {
    cout << "- firstPreviousEdge: " << fpe->getId() << endl;
  } else     cout << "- firstPreviousEdge: nullptr" << endl;

  if (fne != nullptr) {
    cout << "- firstNextEdge: " << fne->getId() << endl;
  } else     cout << "- firstNextEdge: nullptr" << endl;

  if (spe != nullptr) {
    cout << "- secondPreviousEdge: " << spe->getId() << endl;
  } else     cout << "- secondPreviousEdge: nullptr" << endl;

  if (sne != nullptr) {
    cout << "- secondNextEdge: " << sne->getId() << endl;
  } else     cout << "- secondNextEdge: nullptr" << endl;
  
  cout << "traverse firstNext: " ;
  fne = FirstVertex->getNextEdge();
  while (fne != nullptr) {
    cout << " => " << fne->getId();
    fne = fne->getFirstNextEdge();
  }
 
  cout << "\ntraverse secondNext: ";
  fne = SecondVertex->getNextEdge();
  while (fne != nullptr) {
    cout << " => " << fne->getId();
    fne = fne->getSecondNextEdge();
  }
  cout << "\ntraverse FirstPrevious: ";
  fne = FirstVertex->getNextEdge();
  while (fne != nullptr) {
    cout << " => " << fne->getId();
    fne = fne->getFirstPreviousEdge();
  }

  cout << "\ntraverse secondPrevious: ";
  fne = FirstVertex->getNextEdge(); 
  while (fne != nullptr) {
    cout << "=> " << fne->getId();
    fne = fne->getSecondPreviousEdge();
  }
}

auto Edge::getFirstVertexPtr() -> VertexPointer {
  return FirstVertex;
}

auto Edge::getSecondVertexPtr() -> VertexPointer {
  return SecondVertex;
}
  
auto Edge::getPropertyList() -> PropertyListReference {
  return EdgePropertyList;
}

auto Edge::getPropertyValue(Edge::KeyType Key) -> ReturnValueType {
  return EdgePropertyList.get(Key);
}

void Edge::setProperty(Edge::KeyType Key, Edge::ValueType Value) {
  EdgePropertyList.set(Key,Value);
}

void Edge::setPropertyList(Edge::PropertyListType InitialList) {
  EdgePropertyList = InitialList;
}

void Edge::setType(string Label) {
  EdgeLabel = Label;
}

auto Edge::getType() -> LabelType {
  return EdgeLabel;
}

auto Edge::getTarget(Edge::VertexPointer from) -> VertexPointer {
  if (from == FirstVertex) {
    return SecondVertex;
  } else if (from == SecondVertex) {
    return FirstVertex;
  }
  return nullptr;
}

auto Edge::getNextEdge(Edge::VertexPointer from) -> EdgePtr  {
  if ( from == FirstVertex ) {
    return FirstNextEdge;
  } else if (from == SecondVertex) {
    return SecondNextEdge;
  }
  return nullptr;
}

void Edge::setFirstNextEdge(EdgePtr e) {
  FirstNextEdge = e;
}

auto Edge::getFirstNextEdge() -> EdgePtr {
  return FirstNextEdge;
}

void Edge::setFirstPreviousEdge(EdgePtr e) {
  FirstPreviousEdge = e;
}

auto Edge::getFirstPreviousEdge() -> EdgePtr {
  return FirstPreviousEdge;
}

void Edge::setSecondNextEdge(EdgePtr e) {
  SecondNextEdge = e;
}

auto Edge::getSecondNextEdge() -> EdgePtr {
  return SecondNextEdge;
}

void Edge::setSecondPreviousEdge(EdgePtr e) {
  SecondPreviousEdge = e;
}

auto Edge::getSecondPreviousEdge() -> EdgePtr {
  return SecondPreviousEdge;
}

auto Edge::getFirstId() -> VertexDescriptor {
  return FirstVertex->getId();
}

auto Edge::getSecondId() -> VertexDescriptor {
  return SecondVertex->getId();
}

void Edge::setId(unsigned int id) {
    EdgeId = id;
}

auto Edge::getId() -> EdgeDescriptor {
  return EdgeId;
}

Edge::Edge(): EdgeId(-1) { 
    FirstVertex = nullptr;     
    SecondVertex = nullptr;
    FirstNextEdge = nullptr; 
    FirstPreviousEdge = nullptr;
    SecondNextEdge = nullptr; 
    SecondPreviousEdge = nullptr;
  }

Edge::Edge(VertexPointer StartVertex, VertexPointer EndVertex) {
  FirstVertex = StartVertex; 
  FirstVertex->addEdge(this);
  SecondVertex = EndVertex; 
  SecondVertex->addEdge(this);
  FirstNextEdge = nullptr; 
  FirstPreviousEdge = nullptr;
  SecondNextEdge = nullptr; 
  SecondPreviousEdge = nullptr;
}
  
void Edge::deleteEdge() {
  EdgePropertyList.deletePropertyList();
}
