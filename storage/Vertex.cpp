#include "Vertex.h"
#include "Edge.h"

using namespace std;

std::vector<Edge::EdgePtr> & Vertex::getEdges() {
  return _outEdges;
}

Vertex::Vertex(): _id(-1) {
    _nextEdge = NULL;
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
    }
    
    // For this node's next first edge
    std::cout << "\nPrint edge traversals:" << _nextEdge << std::endl;
    cout << "=> previous edge\n";
    EdgePtr p = _nextEdge;
    while ( p != NULL) {
      std::cout << "=> " << p->getId() << " ";
      p = p->getFirstPreviousEdge();
    }

    cout << "\n= next edge\n";
    p = _nextEdge;
    while ( p != NULL) {
      std::cout << "=> " << p->getId() << " ";
      p = p->getFirstNextEdge();
    }

    std::cout << std::endl;
    std::cout << " ========================================= " << std::endl;

  }
