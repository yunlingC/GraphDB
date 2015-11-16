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

auto Vertex::setType(LabelType Label)
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
    std::cout <<"\n";
    std::cout << " Vertex: " << VertexId 
              << ", addr:" << this 
              << ", node size: " << sizeof(Vertex) 
              << std::endl;
//    std::cout << " Outedge ids: ";
//    // for ( auto i = 0; i < AllEdges.size(); i++ ) {
//    //   EdgePtr ep = AllEdges[i];
//    //   std::cout << ep->getId() << ": (" << ep->getFirstId() << ", " << ep->getSecondId() << ") ";
//    // }
//    
//    // For this node's next first edge
    if ( NextEdge != nullptr ) {
//      std::cout << "Print edge traversals:" << NextEdge << "\n";
      std::cout << "=> first edge id: " << NextEdge->getId() 
                << "\n";
     
      auto FirstPtr = NextEdge->getFirstVertexPtr();
      auto SecondPtr = NextEdge->getSecondVertexPtr();

      EdgePtr p = NextEdge;
      std::cout << "=> firstEdges\n";
      std::cout << "=> previous eids\n";
      while ( p != nullptr ) {
      	std::cout << "=> " << p->getId() << " ";
      	p = p->getPreviousEdge(FirstPtr);
      }
//
      cout << "\n=> next eids\n";
      p = NextEdge;
      while ( p != nullptr) {
      	std::cout << "=> " << p->getId() << " ";
      	p = p->getNextEdge(FirstPtr);
      }
//
      std::cout << "\n=> secondEdges\n";
      std::cout << "=> previous eids\n";
      p = NextEdge;
      while ( p != nullptr) {
      	std::cout << "=> " << p->getId() << " ";
	      p = p->getPreviousEdge(SecondPtr);
      }

      cout << "\n=> next eids\n";
      p = NextEdge;
      while ( p != nullptr) {
	      std::cout << "=> " << p->getId() << " ";
	      p = p->getNextEdge(SecondPtr);
      }
    }

    if (LastEdge != nullptr) {
      std::cout << "\n=> last edge id: " 
                << LastEdge->getId() << "\n";
    }

    VertexPropertyList.print();

  }

auto Vertex::deleteVertex() 
  -> void {
  VertexPropertyList.deletePropertyList();
}
