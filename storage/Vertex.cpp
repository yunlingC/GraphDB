//===-- storage/Vertex.cpp - Edge implementation ------------------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the implementation for the Vertex class.
///
//===----------------------------------------------------------------------===//
#include "Vertex.h"
#include "Edge.h"

/// New format for return types
auto Vertex::getPropertyValue(KeyType Key) 
  -> ReturnPropertyValueType {
  return VertexPropertyList.get(Key);
}

auto Vertex::getPropertyValue(std::string Key) 
  -> ReturnPropertyValueType {
  return VertexPropertyList.get(Key);
}

auto Vertex::setProperty(KeyType Key, ValueType Value)
  -> void {
  VertexPropertyList.set(Key,Value);
}

auto Vertex::setPropertyList(PropertyListTypeReference InitialList) 
  -> void {
  VertexPropertyList = InitialList;
}

// Remember you can change this when returned.
auto Vertex::getPropertyList() 
  -> PropertyListTypeReference  {
  return VertexPropertyList;
}

Vertex::Vertex(): 
  VertexId(-1) {
  NextEdge = nullptr;
  LastEdge = nullptr;
}

auto Vertex::setId(unsigned int Id) 
  -> void {
  VertexId = Id;
}

auto Vertex::getId() 
  -> unsigned int {
  return VertexId;
}

auto Vertex::setType (string Label)
  -> void {
  VertexLabel = Label;
}

auto Vertex::getType() 
  ->LabelType {
    return VertexLabel;
}

auto Vertex::setNextEdge(EdgePtr NewEdge) 
  -> void {
    NextEdge = NewEdge;
}
  
auto Vertex::getNextEdge() -> EdgePtr {
  return NextEdge;
}

auto Vertex::setLastEdge(EdgePtr NewEdge) 
  -> void {
   LastEdge = NewEdge;
}

auto Vertex::getLastEdge() -> EdgePtr {
  return LastEdge;
}

auto Vertex::dump() 
  -> void {

//    std::cout << " ========================================= " << std::endl;
    std::cout << " Vertex: " << VertexId << ", addr:" << this << ", node size: " << sizeof(Vertex) << std::endl;
//    std::cout << " Outedge ids: ";
//    // for ( auto i = 0; i < AllEdges.size(); i++ ) {
//    //   EdgePtr ep = AllEdges[i];
//    //   std::cout << ep->getId() << ": (" << ep->getFirstId() << ", " << ep->getSecondId() << ") ";
//    // }
//    
//    // For this node's next first edge
//    if ( NextEdge != nullptr ) {
//      std::cout << "\nPrint edge traversals:" << NextEdge << std::endl;
//      cout << "=> current edge id: " << NextEdge->getId() << ", previous edge ids: \n";
//      EdgePtr p = NextEdge;
//      std::cout << "\n=> firstEdges\n";
//      std::cout << "=> previous eids\n";
//      while ( p != nullptr ) {
//	std::cout << "=> " << p->getId() << " ";
//	p = p->getFirstPreviousEdge();
//      }
//
//      cout << "\n= next eids\n";
//      p = NextEdge;
//      while ( p != nullptr) {
//	std::cout << "=> " << p->getId() << " ";
//	p = p->getFirstNextEdge();
//      }
//
//      std::cout << "\n\n=> secondEdges\n";
//      std::cout << "=> previous eids\n";
//      while ( p != nullptr) {
//	std::cout << "=> " << p->getId() << " ";
//	p = p->getSecondPreviousEdge();
//      }
//
//      cout << "\n= next eids\n";
//      p = NextEdge;
//      while ( p != nullptr) {
//	std::cout << "=> " << p->getId() << " ";
//	p = p->getSecondNextEdge();
//      }
//    }
//
//    std::cout << "\n"; //std::endl;
//
//    std::cout << "\nVertex Propertylist:\n";
//    VertexPropertyList.print();
//    std::cout << " ========================================= \n";

  }

auto Vertex::deleteVertex() 
  -> void {
  VertexPropertyList.deletePropertyList();
}
