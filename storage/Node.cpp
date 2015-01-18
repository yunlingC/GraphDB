#include "Node.h"
#include "Edge.h"

using namespace std;

std::vector<Edge::EdgePtr> & Node::getEdges() {
  return _outEdges;
}

Node::Node(): _id(-1) {
    _nextEdge = NULL;
  }

void Node::setId(unsigned int id) {
    _id = id;
  }

unsigned int Node::getId() {
  return _id;
}

void Node::setNextEdge(Edge::EdgePtr ep) {
    _nextEdge = ep;
}
  
void Node::addEdge(Edge::EdgePtr ep) {
    _outEdges.push_back(ep);
  }

Edge::EdgePtr Node::getNextEdge() {
  return _nextEdge;
  }


void Node::dump() {
    std::cout << " ========================================= " << std::endl;
    std::cout << " Node: " << _id << ", addr:" << this << ", node size: " << sizeof(Node) << std::endl;
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

