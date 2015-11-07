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
#include "Vertex.h"
#include "Edge.h"

#if DEBUG
#include <iostream>
#endif

//using namespace std;

#if DEBUG
auto Edge::dump() 
  -> void {
  std::cout << "\nEdge id: " << EdgeId 
            << " (" << FirstVertex->getId() 
            << ", " << SecondVertex->getId() << ") \n";

  std::cout << "\nEdge PropertyList\n";
  EdgePropertyList.print();

  EdgePointer FirstNexEdge = getFirstNextEdge();
  EdgePointer FirstPrevEdge = getFirstPreviousEdge();
  EdgePointer SecdNextEdge = getSecondNextEdge();
  EdgePointer SecdPrevEdge = getSecondPreviousEdge();

  if (FirstPrevEdge != nullptr) {
    std::cout << "- firstPreviousEdge: " 
              << FirstPrevEdge->getId() << "\n";
  } else     std::cout << "- firstPreviousEdge: nullptr" << "\n";

  if (FirstNexEdge != nullptr) {
    std::cout << "- firstNextEdge: " 
              << FirstNexEdge->getId() << "\n";
  } else     std::cout << "- firstNextEdge: nullptr" << "\n";

  if (SecdPrevEdge != nullptr) {
    std::cout << "- secondPreviousEdge: " 
              << SecdPrevEdge->getId() << "\n";
  } else     std::cout << "- secondPreviousEdge: nullptr" << "\n";

  if (SecdNextEdge != nullptr) {
    std::cout << "- secondNextEdge: " 
              << SecdNextEdge->getId() << "\n";
  } else     std::cout << "- secondNextEdge: nullptr" << "\n";
  
  std::cout << "traverse firstNext: " ;
  FirstNexEdge = FirstVertex->getNextEdge();
  while (FirstNexEdge != nullptr) {
    std::cout << " => " << FirstNexEdge->getId();
    FirstNexEdge = FirstNexEdge->getFirstNextEdge();
  }
 
  std::cout << "\ntraverse secondNext: ";
  FirstNexEdge = SecondVertex->getNextEdge();
  while (FirstNexEdge != nullptr) {
    std::cout << " => " << FirstNexEdge->getId();
    FirstNexEdge = FirstNexEdge->getSecondNextEdge();
  }
  std::cout << "\ntraverse FirstPrevious: ";
  FirstNexEdge = FirstVertex->getNextEdge();
  while (FirstNexEdge != nullptr) {
    std::cout << " => " << FirstNexEdge->getId();
    FirstNexEdge = FirstNexEdge->getFirstPreviousEdge();
  }

  std::cout << "\ntraverse secondPrevious: ";
  FirstNexEdge = FirstVertex->getNextEdge(); 
  while (FirstNexEdge != nullptr) {
    std::cout << "=> " << FirstNexEdge->getId();
    FirstNexEdge = FirstNexEdge->getSecondPreviousEdge();
  }
}
#endif

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

auto Edge::setProperty(KeyType Key, ValueType Value) 
  -> void {
  EdgePropertyList.set(Key,Value);
}

auto Edge::setPropertyList(PropertyListType InitialList)
  -> void {
  EdgePropertyList = InitialList;
}

auto Edge::setType(std::string Label) 
  -> void {
  EdgeLabel = Label;
}

auto Edge::getType() 
  -> LabelType {
  return EdgeLabel;
}

/// get the other endpoint of this edge, without direction
auto Edge::getTarget(VertexPointer Vertex) 
  -> VertexPointer {
  if (Vertex == FirstVertex) {
    return SecondVertex;
  } else if (Vertex == SecondVertex) {
    return FirstVertex;
  }
  return nullptr;
}

/// get the next edge that shares the same endpoint Vertex
/// Ignoring the direction
auto Edge::getNextEdge(VertexPointer Vertex) 
  -> EdgePointer  {
  if ( Vertex == FirstVertex ) {
    return FirstNextEdge;
  } else if (Vertex == SecondVertex) {
    return SecondNextEdge;
  }
  return nullptr;
}

auto Edge::getPreviousEdge(Edge::VertexPointer Vertex) 
  -> EdgePointer  {
  if ( Vertex == FirstVertex ) {
    return FirstPreviousEdge;
  } else if (Vertex == SecondVertex) {
    return SecondPreviousEdge;
  }
  return nullptr;
}

auto Edge::setFirstNextEdge(EdgePointer Edge) 
  -> void {
  FirstNextEdge = Edge;
}

auto Edge::getFirstNextEdge() 
  -> EdgePointer {
  return FirstNextEdge;
}

auto Edge::setFirstPreviousEdge(EdgePointer Edge) 
  -> void {
  FirstPreviousEdge = Edge;
}

auto Edge::getFirstPreviousEdge() 
  -> EdgePointer {
  return FirstPreviousEdge;
}

auto Edge::setSecondNextEdge(EdgePointer Edge) 
  -> void {
  SecondNextEdge = Edge;
}

auto Edge::getSecondNextEdge() 
  -> EdgePointer {
  return SecondNextEdge;
}

auto Edge::setSecondPreviousEdge(EdgePointer Edge) 
-> void {
  SecondPreviousEdge = Edge;
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

auto Edge::setId(unsigned int Id)
  -> void {
    EdgeId = Id;
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
