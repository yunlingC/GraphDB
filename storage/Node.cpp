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

Edge::EdgePtr Node::getPreviousEdge(Edge::EdgeDescriptor ed) {
    // Use ed to index _outedges to retrieve pointer.
    // Next is ed + 1
    
    // No next edges then return NULL.
    if (ed == 0) {
      std::cout << "DBG: No previous edges" << std::endl;
      return NULL;
    }
    
    // Return the next  edge.
    return _outEdges[ed-1];
  }

void Node::dump() {
    std::cout << " ========================================= " << std::endl;
    std::cout << " Node: " << _id << std::endl;
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

