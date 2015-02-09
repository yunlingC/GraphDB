#include "Vertex.h"
#include "Edge.h"

using namespace std;

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
//Vertex::PropertyListType &  
auto Vertex::getPropertyList() -> PropertyListTypeReference  {
  return VertexPropertyList;
}

///std::vector<Edge::EdgePtr> & 
auto Vertex::getEdges() -> EdgeListReference {
  return AllEdges;
}

//std::vector<Edge::EdgePtr> & 
auto Vertex::getOutEdges() -> EdgeListReference {
  return OutEdges;
}

Vertex::Vertex(): _id(-1) {
  _nextEdge = nullptr;
}

void Vertex::setId(unsigned int id) {
  _id = id;
}

unsigned int Vertex::getId() {
  return _id;
}

void Vertex::setNextEdge(Edge::EdgePtr ep) {
    _nextEdge = ep;
}
  
void Vertex::addEdge(Edge::EdgePtr NewEdge) {
    AllEdges.push_back(NewEdge);

    if ( this == NewEdge->getFirstVertexPtr() ) {
      OutEdges.push_back(NewEdge);
    }
}

void Vertex::addOutEdge(Edge::EdgePtr ep) {

}

auto Vertex::getNextEdge() -> EdgePtr{
  return _nextEdge;
}


void Vertex::dump() {
    std::cout << " ========================================= " << std::endl;
    std::cout << " Vertex: " << _id << ", addr:" << this << ", node size: " << sizeof(Vertex) << std::endl;
    std::cout << " Outedge ids: ";
    for ( auto i = 0; i < AllEdges.size(); i++ ) {
      EdgePtr ep = AllEdges[i];
      std::cout << ep->getId() << ": (" << ep->getFirstId() << ", " << ep->getSecondId() << ") ";
    }
    
    // For this node's next first edge
    if ( _nextEdge != nullptr ) {
      std::cout << "\nPrint edge traversals:" << _nextEdge << std::endl;
      cout << "=> current edge id: " << _nextEdge->getId() << ", previous edge ids: \n";
      EdgePtr p = _nextEdge;
      std::cout << "\n=> firstEdges\n";
      std::cout << "=> previous eids\n";
      while ( p != nullptr ) {
	std::cout << "=> " << p->getId() << " ";
	p = p->getFirstPreviousEdge();
      }

      cout << "\n= next eids\n";
      p = _nextEdge;
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
      p = _nextEdge;
      while ( p != nullptr) {
	std::cout << "=> " << p->getId() << " ";
	p = p->getSecondNextEdge();
      }
    }

    std::cout << std::endl;

    std::cout << "\nVertex Propertylist:\n";
    VertexPropertyList.print();
    std::cout << " ========================================= \n";

  }

void Vertex::deleteVertex() {
  VertexPropertyList.deletePropertyList();
}
