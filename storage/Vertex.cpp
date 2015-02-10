#include "Vertex.h"
#include "Edge.h"

//using namespace std;

/// New format for return types
auto Vertex::getPropertyValue(KeyType Key) -> ReturnPropertyValueType {
  return VertexPropertyList.get(Key);
}

void Vertex::setProperty(Vertex::KeyType Key, Vertex::ValueType Value) {
  VertexPropertyList.set(Key,Value);
}

void Vertex::setPropertyList(Vertex::PropertyListType & InitialList) {
  VertexPropertyList = InitialList;
}

// Remember you can change this when returned.
auto Vertex::getPropertyList() -> PropertyListTypeReference  {
  return VertexPropertyList;
}

Vertex::Vertex(): 
  VertexId(-1) {
  NextEdge = nullptr;
}

void Vertex::setId(unsigned int id) {
  VertexId = id;
}

unsigned int Vertex::getId() {
  return VertexId;
}

void Vertex::setNextEdge(Edge::EdgePtr ep) {
    NextEdge = ep;
}
  
auto Vertex::getNextEdge() -> EdgePtr {
  return NextEdge;
}


void Vertex::dump() {
    std::cout << " ========================================= " << std::endl;
    std::cout << " Vertex: " << VertexId << ", addr:" << this << ", node size: " << sizeof(Vertex) << std::endl;
    std::cout << " Outedge ids: ";
    // for ( auto i = 0; i < AllEdges.size(); i++ ) {
    //   EdgePtr ep = AllEdges[i];
    //   std::cout << ep->getId() << ": (" << ep->getFirstId() << ", " << ep->getSecondId() << ") ";
    // }
    
    // For this node's next first edge
    if ( NextEdge != nullptr ) {
      std::cout << "\nPrint edge traversals:" << NextEdge << std::endl;
      cout << "=> current edge id: " << NextEdge->getId() << ", previous edge ids: \n";
      EdgePtr p = NextEdge;
      std::cout << "\n=> firstEdges\n";
      std::cout << "=> previous eids\n";
      while ( p != nullptr ) {
	std::cout << "=> " << p->getId() << " ";
	p = p->getFirstPreviousEdge();
      }

      cout << "\n= next eids\n";
      p = NextEdge;
      while ( p != nullptr) {
	std::cout << "=> " << p->getId() << " ";
	p = p->getFirstNextEdge();
      }

      std::cout << "\n\n=> secondEdges\n";
      std::cout << "=> previous eids\n";
      while ( p != nullptr) {
	std::cout << "=> " << p->getId() << " ";
	p = p->getSecondPreviousEdge();
      }

      cout << "\n= next eids\n";
      p = NextEdge;
      while ( p != nullptr) {
	std::cout << "=> " << p->getId() << " ";
	p = p->getSecondNextEdge();
      }
    }

    std::cout << "\n"; //std::endl;

    std::cout << "\nVertex Propertylist:\n";
    VertexPropertyList.print();
    std::cout << " ========================================= \n";

  }

void Vertex::deleteVertex() {
  VertexPropertyList.deletePropertyList();
}
