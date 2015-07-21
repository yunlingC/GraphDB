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

auto Edge::dump() 
  -> void {
  std::cout << "\nEdge id: " << EdgeId << " (" << FirstVertex->getId() 
            << ", " << SecondVertex->getId() << ") \n";
  std::cout << "\nEdge type: " << "( " << EdgeLabel << " )\n";

  std::cout << "\nEdge PropertyList\n";
  EdgePropertyList.print();

  EdgePointer fne = getFirstNextEdge();
  EdgePointer fpe = getFirstPreviousEdge();
  EdgePointer sne = getSecondNextEdge();
  EdgePointer spe = getSecondPreviousEdge();

  if (fpe != nullptr) {
    std::cout << "- firstPreviousEdge: " << fpe->getId() << "\n";
  } else     std::cout << "- firstPreviousEdge: nullptr" << "\n";

  if (fne != nullptr) {
    std::cout << "- firstNextEdge: " << fne->getId() << "\n";
  } else     std::cout << "- firstNextEdge: nullptr" << "\n";

  if (spe != nullptr) {
    std::cout << "- secondPreviousEdge: " << spe->getId() << "\n";
  } else     std::cout << "- secondPreviousEdge: nullptr" << "\n";

  if (sne != nullptr) {
    std::cout << "- secondNextEdge: " << sne->getId() << "\n";
  } else     std::cout << "- secondNextEdge: nullptr" << "\n";
  
  std::cout << "traverse firstNext: " ;
  fne = FirstVertex->getNextEdge();
  while (fne != nullptr) {
    std::cout << " => " << fne->getId();
    fne = fne->getFirstNextEdge();
  }
 
  std::cout << "\ntraverse secondNext: ";
  fne = SecondVertex->getNextEdge();
  while (fne != nullptr) {
    std::cout << " => " << fne->getId();
    fne = fne->getSecondNextEdge();
  }
  std::cout << "\ntraverse FirstPrevious: ";
  fne = FirstVertex->getNextEdge();
  while (fne != nullptr) {
    std::cout << " => " << fne->getId();
    fne = fne->getFirstPreviousEdge();
  }

  std::cout << "\ntraverse secondPrevious: ";
  fne = FirstVertex->getNextEdge(); 
  while (fne != nullptr) {
    std::cout << "=> " << fne->getId();
    fne = fne->getSecondPreviousEdge();
  }
}

auto Edge::getFirstVertexPtr() 
  -> VertexPointer {
  return FirstVertex;
}

auto Edge::getSecondVertexPtr() 
  -> VertexPointer {
  return SecondVertex;
}
  
auto Edge::getPropertyList() 
  -> PropertyListReference {
  return EdgePropertyList;
}

auto Edge::getPropertyValue(KeyType Key) 
  -> ReturnValueType {
  return EdgePropertyList.get(Key);
}

auto Edge::getPropertyValue(std::string Key) 
  -> ReturnValueType {
  FixedString key(Key);
  return Edge::getPropertyValue(key);
}

auto Edge::setProperty(KeyType Key, ValueType Value) 
  -> void {
  EdgePropertyList.set(Key,Value);
}

auto Edge::setPropertyList(PropertyListType InitialList)
  -> void {
  EdgePropertyList = InitialList;
}

auto Edge::setType(string Label) 
  -> void {
  EdgeLabel = Label;
}

auto Edge::getType() 
  -> LabelType {
  return EdgeLabel;
}

auto Edge::getTarget(VertexPointer from) 
  -> VertexPointer {
  if (from == FirstVertex) {
    return SecondVertex;
  } else if (from == SecondVertex) {
    return FirstVertex;
  }
  return nullptr;
}

auto Edge::getNextEdge(VertexPointer from) 
  -> EdgePointer  {
  if ( from == FirstVertex ) {
    return FirstNextEdge;
  } else if (from == SecondVertex) {
    return SecondNextEdge;
  }
  return nullptr;
}

auto Edge::getPreviousEdge(Edge::VertexPointer from) 
  -> EdgePointer  {
  if ( from == FirstVertex ) {
    return FirstPreviousEdge;
  } else if (from == SecondVertex) {
    return SecondPreviousEdge;
  }
  return nullptr;
}

auto Edge::setFirstNextEdge(EdgePointer e) 
  -> void {
  FirstNextEdge = e;
}

auto Edge::getFirstNextEdge() 
  -> EdgePointer {
  return FirstNextEdge;
}

auto Edge::setFirstPreviousEdge(EdgePointer e) 
  -> void {
  FirstPreviousEdge = e;
}

auto Edge::getFirstPreviousEdge() 
  -> EdgePointer {
  return FirstPreviousEdge;
}

auto Edge::setSecondNextEdge(EdgePointer e) 
  -> void {
  SecondNextEdge = e;
}

auto Edge::getSecondNextEdge() 
  -> EdgePointer {
  return SecondNextEdge;
}

auto Edge::setSecondPreviousEdge(EdgePointer e) 
-> void {
  SecondPreviousEdge = e;
}

auto Edge::getSecondPreviousEdge() 
  -> EdgePointer {
  return SecondPreviousEdge;
}

auto Edge::getFirstId() 
  -> VertexDescriptor {
  return FirstVertex->getId();
}

auto Edge::getSecondId() 
  -> VertexDescriptor {
  return SecondVertex->getId();
}

auto Edge::setId(unsigned int id)
  -> void {
    EdgeId = id;
}

auto Edge::getId() 
  -> EdgeDescriptor {
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
  SecondVertex = EndVertex; 
  FirstNextEdge = nullptr; 
  FirstPreviousEdge = nullptr;
  SecondNextEdge = nullptr; 
  SecondPreviousEdge = nullptr;
}
  
auto Edge::deleteEdge()
  -> void {
  EdgePropertyList.deletePropertyList();
}
