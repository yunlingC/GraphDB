//===-- storage/GraphType.h - Graph class type ------------------*- C++ -*-===//
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
#ifndef _GRAPH_TYPE_H_ 
#define _GRAPH_TYPE_H_ 

#include "Vertex.h"
#include "Edge.h"

class GraphType {
public:
  /// Typedefs that are used to refer to within other classes.
  typedef unsigned int VertexDescriptor;
  typedef unsigned int EdgeDescriptor;
  typedef Vertex* VertexPointer;
  typedef Edge* EdgePointer;
  typedef std::vector<EdgePointer> EdgeList;
  typedef std::vector<VertexPointer> VertexList;
  typedef PropertyList<FixedString, FixedString> PropertyListType;
  typedef PropertyListType VertexPropertyList;
  typedef PropertyListType EdgePropertyList;

public:
  VertexPointer getVertexPointer(VertexDescriptor Vertex) {
    if ( (Vertex > Vertices.size() - 1) || (Vertex < 0) ) {
      return nullptr;
    }
    return Vertices[Vertex];
  }

  EdgeList getOutEdges(VertexPointer CurrentVertex) {
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

  EdgeList getInEdges(VertexPointer CurrentVertex) {
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

  VertexDescriptor addVertex() {
    VertexPointer NewVertex = new Vertex();

    NewVertex->setId(NumberOfVertices); 
    ++NumberOfVertices;
    Vertices.push_back(NewVertex);
    return NewVertex->getId();
  }

  VertexDescriptor addVertex(std::string Label, 
                            PropertyListType & InitialPropertyList) {
    VertexPointer NewVertex = new Vertex();
    NewVertex->setPropertyList(InitialPropertyList);
    NewVertex->setId(NumberOfVertices); 
    NewVertex->setType(Label);
    ++NumberOfVertices;
    Vertices.push_back(NewVertex);
    return NewVertex->getId();
  }

  VertexDescriptor addVertex(PropertyListType & InitialPropertyList) {
    VertexPointer NewVertex = new Vertex();
    NewVertex->setPropertyList(InitialPropertyList);
    NewVertex->setId(NumberOfVertices); 
    ++NumberOfVertices;
    Vertices.push_back(NewVertex);
    return NewVertex->getId();
  }

  void chainEdges(VertexPointer Vertex, EdgePointer FirstNextEdge, 
                  EdgePointer NewEdge) {

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

  void assignPointers(VertexDescriptor vs, VertexDescriptor vd, 
                      EdgePointer NewEdge) {
    
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

  EdgeDescriptor addEdge(VertexDescriptor StartVertex, VertexDescriptor EndVertex) {
    EdgePointer NewEdge = new Edge(Vertices[StartVertex], Vertices[EndVertex]);
    NewEdge->setId(NumberOfEdges);
    assignPointers(StartVertex, EndVertex, NewEdge);
    ++NumberOfEdges;
    Edges.push_back(NewEdge);
    return NewEdge->getId();
  }

  EdgeDescriptor addEdge(VertexDescriptor StartVertex, 
                         VertexDescriptor EndVertex, 
                         const std::string & Label) {
    EdgePointer NewEdge = new Edge(Vertices[StartVertex], Vertices[EndVertex]);
    
    NewEdge->setId(NumberOfEdges);
    NewEdge->setType(Label);
    assignPointers(StartVertex, EndVertex, NewEdge);
    ++NumberOfEdges;
    Edges.push_back(NewEdge);
    return NewEdge->getId();
  }

  EdgeDescriptor addEdge(VertexDescriptor StartVertex, 
                         VertexDescriptor EndVertex, 
                         PropertyListType & InitialPropertyList) {

    EdgePointer NewEdge = new Edge(Vertices[StartVertex], Vertices[EndVertex]);

    NewEdge->setPropertyList(InitialPropertyList);
    NewEdge->setId(NumberOfEdges);    
    assignPointers(StartVertex, EndVertex, NewEdge);
    ++NumberOfEdges;
    Edges.push_back(NewEdge);
    return NewEdge->getId();
  }

  EdgeDescriptor addEdge(VertexDescriptor StartVertex, 
                         VertexDescriptor EndVertex, 
                         const std::string & Label, 
                         PropertyListType & InitialPropertyList) {

    EdgePointer NewEdge = new Edge(Vertices[StartVertex], Vertices[EndVertex]);

    NewEdge->setType(Label);
    NewEdge->setPropertyList(InitialPropertyList);
    NewEdge->setId(NumberOfEdges);    
    assignPointers(StartVertex, EndVertex, NewEdge);
    ++NumberOfEdges;
    Edges.push_back(NewEdge);
    return NewEdge->getId();
  }

  void dump() {
    for ( size_t  i = 0; i < Vertices.size(); i++ ) {
      Vertices[i]->dump();
    }

    for ( size_t i = 0; i <Edges.size() ; i++ ) {
      Edges[i]->dump();
    }
  }

  GraphType(): NumberOfVertices(0), NumberOfEdges(0) {

  }
  
  ~GraphType() {
    /// Must manually delete the objects.  
    /// However, only one place is necessary since everywhere else, 
    /// since I am storing pointers.
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

  VertexList getAllVertices(){
    return Vertices;
  }

  EdgeList getAllEdges(){
    return Edges;
  }

  
protected:
  /// Hold pointers to all vertices.
  std::vector<VertexPointer> Vertices;
  /// Hold pointers to all edges.
  std::vector<EdgePointer> Edges;
  /// Keep a count of vertices and edges.
  unsigned int NumberOfVertices;
  unsigned int NumberOfEdges;
};

#endif /* _GRAPH_TYPE_H */

