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
  if ( (Vertex > Vertices.size() - 1) || (Vertex < 0) ) {
    return nullptr;
  }
  return Vertices[Vertex];
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

auto  GraphType::addVertex() 
  -> VertexDescriptor {
  VertexPointer NewVertex = new Vertex();

  NewVertex->setId(NumberOfVertices); 
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
  ++NumberOfVertices;
  Vertices.push_back(NewVertex);
  return NewVertex->getId();
}

auto GraphType::addVertex(PropertyListType & InitialPropertyList) 
  -> VertexDescriptor {
  VertexPointer NewVertex = new Vertex();
  NewVertex->setPropertyList(InitialPropertyList);
  NewVertex->setId(NumberOfVertices); 
  ++NumberOfVertices;
  Vertices.push_back(NewVertex);
  return NewVertex->getId();
}

auto GraphType::chainEdges(VertexPointer Vertex, EdgePointer FirstNextEdge, 
                EdgePointer NewEdge) 
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
  if (FirstVertexPointer->getNextEdge() == nullptr) {
    FirstVertexPointer->setNextEdge(NewEdge);
  }
  if (SecondVertexPointer->getNextEdge() == nullptr) {
    SecondVertexPointer->setNextEdge(NewEdge);
  }

  /// 2. Find the end of the chain for each first/second node.
  /// The chain is going to iterate over first and second pointers based on who is source.
  EdgePointer FirstNextEdge = FirstVertexPointer->getNextEdge();
  chainEdges(FirstVertexPointer, FirstNextEdge, NewEdge);
  /// Chain the edges for the second vertex.
  FirstNextEdge = SecondVertexPointer->getNextEdge();
  chainEdges(SecondVertexPointer, FirstNextEdge, NewEdge);

}

auto GraphType::addEdge(VertexDescriptor StartVertex, 
                        VertexDescriptor EndVertex) 
  -> EdgeDescriptor {
  EdgePointer NewEdge = new Edge(Vertices[StartVertex], Vertices[EndVertex]);
  NewEdge->setId(NumberOfEdges);
  assignPointers(StartVertex, EndVertex, NewEdge);
  ++NumberOfEdges;
  Edges.push_back(NewEdge);
  return NewEdge->getId();
}

auto GraphType::addEdge(VertexDescriptor StartVertex, 
                                      VertexDescriptor EndVertex, 
                                      const std::string & Label) 
  -> EdgeDescriptor {
  EdgePointer NewEdge = new Edge(Vertices[StartVertex], Vertices[EndVertex]);
  
  NewEdge->setId(NumberOfEdges);
  NewEdge->setType(Label);
  assignPointers(StartVertex, EndVertex, NewEdge);
  ++NumberOfEdges;
  Edges.push_back(NewEdge);
  return NewEdge->getId();
}

auto  GraphType::addEdge(VertexDescriptor StartVertex, 
                       VertexDescriptor EndVertex, 
                       PropertyListType & InitialPropertyList) 
  -> EdgeDescriptor {

  EdgePointer NewEdge = new Edge(Vertices[StartVertex], Vertices[EndVertex]);

  NewEdge->setPropertyList(InitialPropertyList);
  NewEdge->setId(NumberOfEdges);    
  assignPointers(StartVertex, EndVertex, NewEdge);
  ++NumberOfEdges;
  Edges.push_back(NewEdge);
  return NewEdge->getId();
}

auto GraphType::addEdge(VertexDescriptor StartVertex, 
                       VertexDescriptor EndVertex, 
                       const std::string & Label, 
                       PropertyListType & InitialPropertyList) 
  -> EdgeDescriptor {

  EdgePointer NewEdge = new Edge(Vertices[StartVertex], Vertices[EndVertex]);

  NewEdge->setType(Label);
  NewEdge->setPropertyList(InitialPropertyList);
  NewEdge->setId(NumberOfEdges);    
  assignPointers(StartVertex, EndVertex, NewEdge);
  ++NumberOfEdges;
  Edges.push_back(NewEdge);
  return NewEdge->getId();
}

#ifdef _DEBUG_
auto GraphType::dump() 
  -> void {
  for ( size_t  i = 0; i < Vertices.size(); i++ ) {
    Vertices[i]->dump();
  }

  for ( size_t i = 0; i <Edges.size() ; i++ ) {
    Edges[i]->dump();
  }
}
#endif 

auto GraphType::getAllVertices()
  -> VertexList {
  return Vertices;
}

auto GraphType::getAllEdges()
  -> EdgeList {
  return Edges;
}

GraphType::GraphType(): NumberOfVertices(0), NumberOfEdges(0) {}

GraphType::~GraphType() {
  /// Must manually delete the objects.  
  /// However, only one place is necessary since everywhere else, 
  /// I am storing pointers.
  /// Thus, Vertices and _edges contain all newly created objects.

  for ( size_t i= 0; i < Vertices.size(); i++ ) {
    Vertices[i]->deleteVertex();
    delete Vertices[i];
  }

  for ( size_t i=0; i < Edges.size(); i++ ) {
    Edges[i]->deleteEdge();
    delete Edges[i];
  }
}


