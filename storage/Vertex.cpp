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

auto Vertex::updateProperty(KeyType Key, ValueType Value)
  -> bool {
  return VertexPropertyList.update(Key, Value);
  }

auto Vertex::updateProperty(std::string Key, std::string Value)
  -> bool {
  KeyType key(Key); ValueType value(Value);
  return updateProperty(key, value);
  }

// Remember you can change this when returned.
auto Vertex::getPropertyList() 
  -> PropertyListTypeReference  {
  return VertexPropertyList;
}

Vertex::Vertex(): 
  VertexId(-1) {
  NextEdge = nullptr;
//  LastEdge = nullptr;
}

auto Vertex::setId(unsigned int Id) 
  -> void {
  VertexId = Id;
}

auto Vertex::getId() 
  -> unsigned int {
  return VertexId;
}

auto Vertex::setType (std::string Label)
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

/*
 * These two functions are not needed if 
 * LastEdge is not set in Vertex
 * NewEdge will be inserted to the head
 *
auto Vertex::setLastEdge(EdgePtr NewEdge) 
  -> void {
   LastEdge = NewEdge;
}

auto Vertex::getLastEdge() -> EdgePtr {
  return LastEdge;
}
*/

#ifdef _LOCKING_STORAGE_
auto Vertex::getVertexLock() ->VertexLockPointer {
  return VertexLock;
}

auto Vertex::setVertexLock(VertexLockPointer NewLock)
  -> void {
  VertexLock = NewLock;
  }
#endif

#ifdef _DEBUG_
auto Vertex::dump() 
  -> void {

    std::cout << " ========================================= \n"; 
    std::cout << " Vertex: " << VertexId 
              << ", addr:" << this 
              << ", node size: " << sizeof(Vertex) << "\n";
    std::cout << " Outedge ids: ";
    
    // For this node's next first edge
    if ( NextEdge != nullptr ) {
      std::cout << "\nPrint edge traversals:" << NextEdge << "\n";
      std::cout << "=> current edge id: " << NextEdge->getId() << ", previous edge ids: \n";
      EdgePtr TempEdgePtr = NextEdge;
      std::cout << "\n=> firstEdges\n";
      std::cout << "=> previous eids\n";
      while ( TempEdgePtr != nullptr ) {
	std::cout << "=> " << TempEdgePtr->getId() << " ";
	TempEdgePtr = TempEdgePtr->getFirstPreviousEdge();
      }

      std::cout << "\n= next eids\n";
      TempEdgePtr = NextEdge;
      while ( TempEdgePtr != nullptr) {
	std::cout << "=> " << TempEdgePtr->getId() << " ";
	TempEdgePtr = TempEdgePtr->getFirstNextEdge();
      }

      std::cout << "\n=> secondEdges\n";
      std::cout << "=> previous eids\n";

      while ( TempEdgePtr != nullptr) {
	std::cout << "=> " << TempEdgePtr->getId() << " ";
	TempEdgePtr = TempEdgePtr->getSecondPreviousEdge();
      }

      std::cout << "\n= next eids\n";
      TempEdgePtr = NextEdge;
      while ( TempEdgePtr != nullptr) {
	std::cout << "=> " << TempEdgePtr->getId() << " ";
	TempEdgePtr = TempEdgePtr->getSecondNextEdge();
      }
    }

    std::cout << "\n"; 

    VertexPropertyList.print();

    std::cout << " ========================================= \n";
  }
#endif

auto Vertex::deleteVertex() 
  -> void {
  VertexPropertyList.deletePropertyList();
}
