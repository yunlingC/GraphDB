//===-- storage/GraphType.cpp - Graph class type ----------------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the main class for the Graph structure.
///
//===----------------------------------------------------------------------===//

#include "GraphType.h"

auto GraphType::getVertexPointer(VertexDescriptor Vertex) 
  -> VertexPointer {
  if( VertexMap.find(Vertex) != VertexMap.end() ) {
    return VertexMap[Vertex];
  } else {
    return nullptr;
  }
}

auto GraphType::getEdgePointer(EdgeDescriptor Edge) 
  -> EdgePointer {
  if ( EdgeMap.find(Edge) != EdgeMap.end() ) {
    return EdgeMap[Edge];
  } else {
    return nullptr;
  }
}

auto GraphType::getAllVertices()
  -> VertexList {
  return Vertices;
}

auto GraphType::getAllEdges()
  -> EdgeList {
  return Edges;
}

auto GraphType::getVertexMap()
  ->  VertexMapType {
  return VertexMap;
}

auto GraphType::getEdgeMap()
  ->  EdgeMapType {
  return EdgeMap;
}

/// TODO attention nullptr
auto GraphType::getOutEdges(VertexDescriptor VertexId) 
-> EdgeList {
  auto CurrentVertex = GraphType::getVertexPointer(VertexId);
  return GraphType::getOutEdges(CurrentVertex);
}

auto GraphType::getOutEdges(VertexPointer CurrentVertex) 
  -> EdgeList {
  EdgeList OutEdges;

  auto NextEdge = CurrentVertex->getNextEdge();
  if (NextEdge != nullptr) {
    if (CurrentVertex == NextEdge->getFirstVertexPtr() ) {
      OutEdges.push_back(NextEdge);
    }
    auto EdgeIterator = NextEdge->getNextEdge(CurrentVertex);

    while (EdgeIterator != nullptr) {
      if (CurrentVertex == EdgeIterator->getFirstVertexPtr()) {
        OutEdges.push_back(EdgeIterator);
      }
      EdgeIterator = EdgeIterator->getNextEdge(CurrentVertex);
    }

    EdgeIterator = NextEdge->getPreviousEdge(CurrentVertex);
    while (EdgeIterator != nullptr) {
      if (CurrentVertex == EdgeIterator->getFirstVertexPtr()) {
        OutEdges.push_back(EdgeIterator);
      }
      EdgeIterator = EdgeIterator->getPreviousEdge(CurrentVertex);
    }
  } 
  return OutEdges;
}

auto GraphType::getInEdges(VertexDescriptor VertexId)
  -> EdgeList {
  return GraphType::getInEdges(GraphType::getVertexPointer(VertexId));
}

auto GraphType::getInEdges(VertexPointer CurrentVertex) 
  -> EdgeList {
  EdgeList InEdges;

  auto NextEdge = CurrentVertex->getNextEdge();
  if (NextEdge != nullptr) {

    if (CurrentVertex == NextEdge->getSecondVertexPtr() ) {
      InEdges.push_back(NextEdge);
    }
    auto EdgeIterator = NextEdge->getNextEdge(CurrentVertex);

    while (EdgeIterator != nullptr) {
      if (CurrentVertex == EdgeIterator->getSecondVertexPtr()) {
        InEdges.push_back(EdgeIterator);
      }
      EdgeIterator = EdgeIterator->getNextEdge(CurrentVertex);
    }

    EdgeIterator = NextEdge->getPreviousEdge(CurrentVertex);
    while (EdgeIterator != nullptr) {
      if (CurrentVertex == EdgeIterator->getSecondVertexPtr()) {
        InEdges.push_back(EdgeIterator);
      }
      EdgeIterator = EdgeIterator->getPreviousEdge(CurrentVertex);
    }
  }
  return InEdges;
}

auto GraphType::getOutNeighbors(VertexDescriptor VertexId) 
-> VertexIDList {
  return getOutNeighbors(GraphType::getVertexPointer(VertexId));
}

auto GraphType::getOutNeighbors(VertexPointer CurrentVertex) 
-> VertexIDList {
  VertexIDList OutNeighbors;

  auto NextEdge = CurrentVertex->getNextEdge();

  if (NextEdge != nullptr) {
    if (CurrentVertex == NextEdge->getFirstVertexPtr()) {
      OutNeighbors.push_back(NextEdge->getSecondVertexPtr()->getId());
    }
    auto EdgeIterator = NextEdge->getNextEdge(CurrentVertex);
    while (EdgeIterator != nullptr) {
      if (CurrentVertex == EdgeIterator->getFirstVertexPtr()) {
        OutNeighbors.push_back(EdgeIterator->getSecondVertexPtr()->getId());
      }
      EdgeIterator = EdgeIterator->getNextEdge(CurrentVertex);
    }
  
    EdgeIterator = NextEdge->getPreviousEdge(CurrentVertex);
    while (EdgeIterator != nullptr) {
      if (CurrentVertex == EdgeIterator->getFirstVertexPtr()) {
        OutNeighbors.push_back(EdgeIterator->getSecondVertexPtr()->getId());
      }
      EdgeIterator = EdgeIterator->getPreviousEdge(CurrentVertex);
    }
  }
  return OutNeighbors;
}

auto GraphType::getInNeighbors(VertexDescriptor VertexId) 
-> VertexIDList {
  return GraphType::getInNeighbors(GraphType::getVertexPointer(VertexId));
}

auto GraphType::getInNeighbors(VertexPointer CurrentVertex)
  -> VertexIDList {
  VertexIDList InNeighbors;

  auto NextEdge = CurrentVertex->getNextEdge();
  if (NextEdge != nullptr) {
    if (CurrentVertex == NextEdge->getSecondVertexPtr()) {
      InNeighbors.push_back(NextEdge->getFirstVertexPtr()->getId());
    } 
    auto EdgeIterator = NextEdge->getNextEdge(CurrentVertex);
    while (EdgeIterator != nullptr) {
      if (CurrentVertex == EdgeIterator->getSecondVertexPtr()) {
        InNeighbors.push_back(EdgeIterator->getFirstVertexPtr()->getId());
      }
      EdgeIterator = EdgeIterator->getNextEdge(CurrentVertex);
    }
  
    EdgeIterator = NextEdge->getPreviousEdge(CurrentVertex);
    while (EdgeIterator != nullptr) {
      if (CurrentVertex == EdgeIterator->getSecondVertexPtr()) {
        InNeighbors.push_back(EdgeIterator->getFirstVertexPtr()->getId());
      }
      EdgeIterator = EdgeIterator->getPreviousEdge(CurrentVertex);
    }
  }
  return InNeighbors;
}

auto  GraphType::addVertex() 
  -> VertexDescriptor {
  VertexPointer NewVertex = new Vertex();

  NewVertex->setId(NumberOfVertices); 
  VertexMap.insert(VertexEntryType(NumberOfVertices, NewVertex));
  ++NumberOfVertices;
  Vertices.push_back(NewVertex);
  return NewVertex->getId();
}

auto GraphType::addVertex(std::string Label, 
                          PropertyListType & InitialPropertyList) 
  -> VertexDescriptor {
  VertexPointer NewVertex = new Vertex();
  NewVertex->setPropertyList(InitialPropertyList);
  NewVertex->setId(NumberOfVertices); 
  NewVertex->setType(Label);
  VertexMap.insert(VertexEntryType(NumberOfVertices, NewVertex));
  ++NumberOfVertices;
  Vertices.push_back(NewVertex);
  return NewVertex->getId();
}

auto GraphType::addVertex(PropertyListType & InitialPropertyList) 
  -> VertexDescriptor { 
  VertexPointer NewVertex = new Vertex(); 
  NewVertex->setPropertyList(InitialPropertyList);
  NewVertex->setId(NumberOfVertices); 
  VertexMap.insert(VertexEntryType(NumberOfVertices, NewVertex));
  ++NumberOfVertices;
  Vertices.push_back(NewVertex);
  return NewVertex->getId();
}

auto GraphType::addVertex(VertexPointer NewVertex) 
  ->  VertexDescriptor {
  NewVertex->setId(NumberOfVertices); 
  VertexMap.insert(VertexEntryType(NumberOfVertices, NewVertex));
  ++NumberOfVertices;
  Vertices.push_back(NewVertex);
  return NewVertex->getId();
}

auto GraphType::chainEdges(VertexPointer Vertex, EdgePointer FirstNextEdge, EdgePointer NewEdge) 
  -> void {

  EdgePointer NextEdge = FirstNextEdge;
  EdgePointer PreviousEdge = nullptr;

  while (NextEdge != nullptr) {
    PreviousEdge = NextEdge;
    if (NextEdge->getFirstVertexPtr()->getId() == Vertex->getId()) {
      NextEdge = NextEdge->getFirstNextEdge();
    } else if (NextEdge->getSecondVertexPtr()->getId() == Vertex->getId()) {
      NextEdge = NextEdge->getSecondNextEdge();
    } 
  }

  /// The end of the chain is the new edge itself.
  /// Exit since it is already set.
  if (PreviousEdge == NewEdge)  {
    return;
  }
  
  // Got to the end of the chain.
  /// Is the end of chain's first vertex the same as the Vertex.
  /// If it is then set the first edge pointers.
  /// Otherwise, use the second edge pointers.
  if (PreviousEdge->getFirstVertexPtr() == Vertex) {

    PreviousEdge->setFirstNextEdge(NewEdge);

    /// If new edge's first vertex is same as vertex then set its first
    /// previous and next edges.  Otherwise set second previous and next.
    if (NewEdge->getFirstVertexPtr() == Vertex) {
NewEdge->setFirstPreviousEdge(PreviousEdge);
NewEdge->setFirstNextEdge(nullptr); 
    } else if (NewEdge->getSecondVertexPtr() == Vertex ) {
NewEdge->setSecondPreviousEdge(PreviousEdge);
NewEdge->setSecondNextEdge(nullptr); 
    }

  } else if (PreviousEdge->getSecondVertexPtr() == Vertex) {
    PreviousEdge->setSecondNextEdge(NewEdge);
    if (NewEdge->getFirstVertexPtr() == Vertex) {
NewEdge->setFirstPreviousEdge(PreviousEdge);
NewEdge->setFirstNextEdge(nullptr);

    } else if (NewEdge->getSecondVertexPtr() == Vertex) {
NewEdge->setSecondPreviousEdge(PreviousEdge);
NewEdge->setSecondNextEdge(nullptr);
    }
  }
}

auto GraphType::assignPointers(VertexDescriptor vs, VertexDescriptor vd, 
                    EdgePointer NewEdge) 
  -> void {
  
  VertexPointer FirstVertexPointer = NewEdge->getFirstVertexPtr();
  VertexPointer SecondVertexPointer = NewEdge->getSecondVertexPtr();

  /// 1. See if first's and second's nextEdge is set or not.
  /// If it is not set then set it. Doesn't matter who the next really is.

  NewEdge->setFirstPreviousEdge(nullptr);
  NewEdge->setSecondPreviousEdge(nullptr);

  EdgePointer FirstNextEdge = FirstVertexPointer->getNextEdge();
  EdgePointer SecondNextEdge = SecondVertexPointer->getNextEdge();

  NewEdge->setFirstNextEdge(FirstNextEdge);
  NewEdge->setSecondNextEdge(SecondNextEdge);

  if (FirstNextEdge != nullptr) { 
    if (FirstNextEdge->getFirstVertexPtr() == FirstVertexPointer)  {
      FirstNextEdge->setFirstPreviousEdge(NewEdge);
    } else if (FirstNextEdge->getSecondVertexPtr() == FirstVertexPointer) {
      FirstNextEdge->setSecondPreviousEdge(NewEdge);
    }
  }

  if (SecondNextEdge != nullptr)  {
    if (SecondNextEdge->getFirstVertexPtr() == SecondVertexPointer) {
      SecondNextEdge->setFirstPreviousEdge(NewEdge);
    } else if (SecondNextEdge->getSecondVertexPtr() == SecondVertexPointer) {
      SecondNextEdge->setSecondPreviousEdge(NewEdge);
    }
  }

  FirstVertexPointer->setNextEdge(NewEdge);
  SecondVertexPointer->setNextEdge(NewEdge);

}

auto GraphType::addEdge(VertexDescriptor StartVertex, 
                        VertexDescriptor EndVertex) 
  -> EdgeDescriptor {
  EdgePointer NewEdge = new Edge(VertexMap[StartVertex], VertexMap[EndVertex]);
  NewEdge->setId(NumberOfEdges);
  assignPointers(StartVertex, EndVertex, NewEdge);
  EdgeMap.insert(EdgeEntryType(NumberOfEdges, NewEdge));
  ++NumberOfEdges;
  Edges.push_back(NewEdge);
  return NewEdge->getId();
}

auto GraphType::addEdge(VertexDescriptor StartVertex, 
                                      VertexDescriptor EndVertex, 
                                      const std::string & Label) 
  -> EdgeDescriptor {
  EdgePointer NewEdge = new Edge(VertexMap[StartVertex], VertexMap[EndVertex]);
  
  NewEdge->setId(NumberOfEdges);
  NewEdge->setType(Label);
  assignPointers(StartVertex, EndVertex, NewEdge);
  EdgeMap.insert(EdgeEntryType(NumberOfEdges, NewEdge));
  ++NumberOfEdges;
  Edges.push_back(NewEdge);
  return NewEdge->getId();
}

auto  GraphType::addEdge(VertexDescriptor StartVertex, 
                       VertexDescriptor EndVertex, 
                       PropertyListType & InitialPropertyList) 
  -> EdgeDescriptor {

  EdgePointer NewEdge = new Edge(VertexMap[StartVertex], VertexMap[EndVertex]);

  NewEdge->setPropertyList(InitialPropertyList);
  NewEdge->setId(NumberOfEdges);    
  assignPointers(StartVertex, EndVertex, NewEdge);
  EdgeMap.insert(EdgeEntryType(NumberOfEdges, NewEdge));
  ++NumberOfEdges;
  Edges.push_back(NewEdge);
  return NewEdge->getId();
}

auto GraphType::addEdge(VertexDescriptor StartVertex, 
                       VertexDescriptor EndVertex, 
                       const std::string & Label, 
                       PropertyListType & InitialPropertyList) 
  -> EdgeDescriptor {

  EdgePointer NewEdge = new Edge(VertexMap[StartVertex], VertexMap[EndVertex]);

  NewEdge->setType(Label);
  NewEdge->setPropertyList(InitialPropertyList);
  NewEdge->setId(NumberOfEdges);    
  assignPointers(StartVertex, EndVertex, NewEdge);
  EdgeMap.insert(EdgeEntryType(NumberOfEdges, NewEdge));
  ++NumberOfEdges;
  Edges.push_back(NewEdge);
  return NewEdge->getId();
}

auto GraphType::addEdge(EdgePointer NewEdge) 
  -> EdgeDescriptor {
  NewEdge->setId(NumberOfEdges);    
  assignPointers(NewEdge->getFirstId(), NewEdge->getSecondId(), NewEdge);
  EdgeMap.insert(EdgeEntryType(NumberOfEdges, NewEdge));
  ++NumberOfEdges;
  Edges.push_back(NewEdge);
  return NewEdge->getId();

  }

#ifdef _DEBUG_
auto GraphType::dump() 
  -> void {
  for ( size_t  i = 0; i < VertexMap.size(); i++ ) {
    VertexMap[i]->dump();
  }

  for ( size_t i = 0; i <EdgeMap.size() ; i++ ) {
    EdgeMap[i]->dump();
  }
}
#endif 

GraphType::GraphType(): NumberOfVertices(0), NumberOfEdges(0) {}

GraphType::~GraphType() {
  /// Must manually delete the objects.  
  /// However, only one place is necessary since everywhere else, 
  /// I am storing pointers.
  /// Thus, Vertices and Edges contain all newly created objects.

  
  for ( size_t i= 0; i < Vertices.size(); i++ ) {
    VertexMap[i]->deleteVertex();
    delete VertexMap[i];
  }


  for ( size_t i=0; i < Edges.size(); i++ ) {
    EdgeMap[i]->deleteEdge();
    delete EdgeMap[i];
  }

  #ifdef _FIXALLOC_
    ///    Delete the memory spaces.
    delete NodeMemory;
    delete EdgeMemory;
  #endif /* _FIXALLOC_ */

}

#ifdef _FIXALLOC_
  auto GraphType::allocVertexMemory(unsigned int sz) 
  -> void {
    // Allocation sz number of Vertex objects.
    NodeMemory = new char[sizeof(Vertex)*sz];
  }

  auto GraphType::allocEdgeMemory(unsigned int sz) 
  -> void {
    // Allocation sz number of Vertex objects.
    EdgeMemory = new char[sizeof(Edge)*sz];
  }

  auto GraphType::allocVertexMemory(char *StartAddress, unsigned int sz) 
  -> void {
    // Allocation sz number of Vertex objects.
    NodeMemory = StartAddress;
///    std::cout << "Vertex Memory\n + Starting address: " 
///              << reinterpret_cast<int*>(NodeMemory)
///    	        << ", ending address: " 
///              << reinterpret_cast<int*>(NodeMemory + sizeof(Vertex)*sz) 
///              << "\n";
  }

  auto GraphType::allocEdgeMemory(char *StartAddress, unsigned int sz) 
  -> void {
    // Allocation sz number of Vertex objects.
    EdgeMemory = StartAddress;
///    std::cout << "Edge Memory\n + Starting address: " 
///              << reinterpret_cast<int*>(EdgeMemory) 
///    	        << ", ending address: " 
///              << reinterpret_cast<int*>(EdgeMemory + sizeof(Edge)*sz) 
///              << "\n";
  }
#endif /* _FIXALLOC_ */



