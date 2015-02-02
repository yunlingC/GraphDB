#include "Vertex.h"
#include "Edge.h"

using namespace std;

Vertex::ReturnPropertyValueType Vertex::getPropertyValue(KeyType k) {
  return _list.get(k);
}

void Vertex::setProperty(Vertex::KeyType k, Vertex::ValueType v) {
  _list.set(k,v);
}

void Vertex::setPropertyList(Vertex::PropertyListType & l) {
  _list = l;
}

// Remember you can change this when returned.
Vertex::PropertyListType &  Vertex::getPropertyList() {
  return _list;
}

std::vector<Edge::EdgePtr> & Vertex::getEdges() {
  return _outEdges;
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
  
void Vertex::addEdge(Edge::EdgePtr ep) {
    _outEdges.push_back(ep);
  }

Edge::EdgePtr Vertex::getNextEdge() {
  return _nextEdge;
  }


void Vertex::dump() {
    std::cout << " ========================================= " << std::endl;
    std::cout << " Vertex: " << _id << ", addr:" << this << ", node size: " << sizeof(Vertex) << std::endl;
    std::cout << " Outedge ids: ";
    for (int i = 0; i < _outEdges.size(); i++) {
      EdgePtr ep = _outEdges[i];
      std::cout << ep->getId() << ": (" << ep->getFirstId() << ", " << ep->getSecondId() << ") ";
      //      std::cout << "\nEdge property: \n";
      //      ep->dump();
    }
    
    // For this node's next first edge

    if (_nextEdge != nullptr) {
      std::cout << "\nPrint edge traversals:" << _nextEdge << std::endl;
      cout << "=> current edge id: " << _nextEdge->getId() << ", previous edge ids: \n";
      EdgePtr p = _nextEdge;
      cout << "\n=> firstEdges\n";
      cout << "=> previous eids\n";
      while ( p != nullptr) {
	std::cout << "=> " << p->getId() << " ";
	p = p->getFirstPreviousEdge();
      }

      cout << "\n= next eids\n";
      p = _nextEdge;
      while ( p != nullptr) {
	std::cout << "=> " << p->getId() << " ";
	p = p->getFirstNextEdge();
      }

      cout << "\n\n=> secondEdges\n";
      cout << "=> previous eids\n";
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

    // Print property list.
    std::cout << "\nVertex Propertylist:\n";
    _list.print();
    std::cout << " ========================================= " << std::endl;

  }

void Vertex::deleteVertex() {
  cout << "- vertex: " << _id << "\n";
  _list.deletePropertyList();
}
