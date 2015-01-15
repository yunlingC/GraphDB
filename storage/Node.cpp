#include "Node.h"
#include "Edge.h"


Node::Node(): _id(-1) {
    _nextEdge = NULL;
  }

void Node::setId(unsigned int id) {
    _id = id;
  }

unsigned int Node::getId() {
  return _id;
}

void Node::addEdge(Edge::EdgePtr ep) {
    _outEdges.push_back(ep);
  }

Edge::EdgePtr Node::getNextEdge(Edge::EdgeDescriptor ed) {
    // Use ed to index _outedges to retrieve pointer.
    // Next is ed + 1
    
    // No next edges then return NULL.
    if (_outEdges.size() < ed) {
      std::cout << "DBG: No next vertices" << std::endl;
      return NULL;

    }
    
    // Return the next  edge.
    return _outEdges[ed+1];
  }

Edge::EdgePtr Node::getPreviousEdge(Edge::EdgeDescriptor ed) {
    // Use ed to index _outedges to retrieve pointer.
    // Next is ed + 1
    
    // No next edges then return NULL.
    if (ed == 0) {
      return NULL;
      std::cout << "DBG: No previous edges" << std::endl;
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
    
    std::cout << std::endl;
    std::cout << " ========================================= " << std::endl;

  }

