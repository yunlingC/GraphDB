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

#include <vector>
#include <stdlib.h>
#include <algorithm>

#include "Vertex.h"
#include "Edge.h"
#include "PropertyList.h"
#include "util.h"

class GraphType {
public:
  /// Typedefs that are used to refer to within other classes.
  typedef unsigned int VertexDescriptor;
  typedef unsigned int EdgeDescriptor;
  typedef Vertex* VertexPointer;
  typedef Vertex* VertexPtr;
  typedef Edge* EdgePointer;
  typedef Edge* EdgePtr;
  typedef std::vector<EdgePointer> EdgeList;
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
    if( NextEdge != nullptr) {
      if ( CurrentVertex == NextEdge->getFirstVertexPtr() ) {
        OutEdges.push_back(NextEdge);
      }
      auto EdgeIterator = NextEdge->getNextEdge(CurrentVertex);

      while ( EdgeIterator != nullptr ) {
        if ( CurrentVertex == EdgeIterator->getFirstVertexPtr() ) {
          OutEdges.push_back(EdgeIterator);
        }
        EdgeIterator = EdgeIterator->getNextEdge(CurrentVertex);
      }

      EdgeIterator = NextEdge->getPreviousEdge(CurrentVertex);
      while ( EdgeIterator != nullptr ) {
        if ( CurrentVertex == EdgeIterator->getFirstVertexPtr() ) {
          OutEdges.push_back(EdgeIterator);
        }
        EdgeIterator = EdgeIterator->getPreviousEdge(CurrentVertex);
      }
    } 
    return OutEdges;
  }

  /// TODO: untested
  EdgeList getInEdges(VertexPointer CurrentVertex) {
    EdgeList InEdges;

    auto NextEdge = CurrentVertex->getNextEdge();
    if( NextEdge != nullptr) {

      if ( CurrentVertex == NextEdge->getSecondVertexPtr() ) {
        InEdges.push_back(NextEdge);
      }
      auto EdgeIterator = NextEdge->getNextEdge(CurrentVertex);

      while ( EdgeIterator != nullptr ) {
        if ( CurrentVertex == EdgeIterator->getSecondVertexPtr() ) {
          InEdges.push_back(EdgeIterator);
        }
        EdgeIterator = EdgeIterator->getNextEdge(CurrentVertex);
      }

      EdgeIterator = NextEdge->getPreviousEdge(CurrentVertex);
      while ( EdgeIterator != nullptr ) {
        if ( CurrentVertex == EdgeIterator->getSecondVertexPtr() ) {
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

  VertexDescriptor addVertex(string Label, PropertyListType & InitialPropertyList) {
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

    while ( NextEdge != nullptr ) {
      PreviousEdge = NextEdge;
      if ( NextEdge->getFirstVertexPtr()->getId() == Vertex->getId() ) {
	NextEdge = NextEdge->getFirstNextEdge();
      } else if ( NextEdge->getSecondVertexPtr()->getId() == Vertex->getId() ) {
	NextEdge = NextEdge->getSecondNextEdge();
      } else {
        std::cout << "+ \nERROR: no forward movement \n";
      }
    }

    /// The end of the chain is the new edge itself.
    /// Exit since it is already set.
    if ( PreviousEdge == NewEdge )  {
      return;
    }
    
    // Got to the end of the chain.
    /// Is the end of chain's first vertex the same as the Vertex.
    /// If it is then set the first edge pointers.
    /// Otherwise, use the second edge pointers.
    if ( PreviousEdge->getFirstVertexPtr() == Vertex ) {

      PreviousEdge->setFirstNextEdge(NewEdge);

      /// If new edge's first vertex is same as vertex then set its first
      /// previous and next edges.  Otherwise set second previous and next.
      if ( NewEdge->getFirstVertexPtr() == Vertex ) {
	NewEdge->setFirstPreviousEdge(PreviousEdge);
	NewEdge->setFirstNextEdge(nullptr); 
      } else if (NewEdge->getSecondVertexPtr() == Vertex ) {
	NewEdge->setSecondPreviousEdge(PreviousEdge);
	NewEdge->setSecondNextEdge(nullptr); 
      }

    } else if ( PreviousEdge->getSecondVertexPtr() == Vertex ) {
      PreviousEdge->setSecondNextEdge(NewEdge);
      if ( NewEdge->getFirstVertexPtr() == Vertex ) {
	NewEdge->setFirstPreviousEdge(PreviousEdge);
	NewEdge->setFirstNextEdge(nullptr);

      } else if ( NewEdge->getSecondVertexPtr() == Vertex ) {
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
    if ( FirstVertexPointer->getNextEdge() == nullptr ) {
      FirstVertexPointer->setNextEdge(NewEdge);
    }
    if ( SecondVertexPointer->getNextEdge() == nullptr ) {
      SecondVertexPointer->setNextEdge(NewEdge);
    }

    /// 2. Find the end of the chain for each first/second node.
    /// get the pointer of the last edge directly from node->getLastNode (no iteration)
    //TODO remove previous edge  
    auto FirstLastEdge = FirstVertexPointer->getLastEdge();
    auto SecondLastEdge = SecondVertexPointer->getLastEdge();

    if ( FirstLastEdge == nullptr ) {
      FirstVertexPointer->setLastEdge(NewEdge);
    } else {
      if ( FirstLastEdge->getFirstVertexPtr() == FirstVertexPointer ) {
        FirstLastEdge->setFirstNextEdge(NewEdge);
      }
      else if( FirstLastEdge->getSecondVertexPtr() == FirstVertexPointer) {
        FirstLastEdge->setSecondNextEdge(NewEdge);
      }
      NewEdge->setFirstPreviousEdge(FirstLastEdge);
      NewEdge->setFirstNextEdge(nullptr);
      FirstVertexPointer->setLastEdge(NewEdge);
    }

    if ( SecondLastEdge == nullptr) {
      SecondVertexPointer->setLastEdge(NewEdge);
    } else {
      if( SecondLastEdge->getFirstVertexPtr() == SecondVertexPointer ) {
        SecondLastEdge->setFirstNextEdge(NewEdge);
      } else if ( SecondLastEdge->getSecondVertexPtr() == SecondVertexPointer ) {
        SecondLastEdge->setSecondNextEdge(NewEdge);
      }
      NewEdge->setSecondPreviousEdge(SecondLastEdge);
      NewEdge->setSecondNextEdge(nullptr);
      SecondVertexPointer->setLastEdge(NewEdge);
    }
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
                         VertexDescriptor EndVertex, const string & Label) {
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

  EdgeDescriptor addEdge(VertexDescriptor StartVertex, VertexDescriptor EndVertex, 
                         const string & Label, 
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
      std::cout << "\n";
    }
  }

  GraphType(): NumberOfVertices(0), NumberOfEdges(0) {

  }
  
  ~GraphType() {
    /// Must manually delete the objects.  
    /// However, only one place is necessary since everywhere else, I am storing pointers.
    /// Thus, Vertices and _edges contain all newly created objects.

    for ( size_t i=0; i < Vertices.size(); i++ ) {
      Vertices[i]->deleteVertex();
      delete Vertices[i];
    }

    for ( size_t i=0; i < Edges.size(); i++ ) {
      Edges[i]->deleteEdge();
      delete Edges[i];
    }
  }

  vector<VertexPointer> getAllVertices(){
    return Vertices;
  }

  vector<EdgePointer> getAllEdges(){
    return Edges;
  }

  void handleAddr(){
    //g.dump();
    //vector<VertexPointer> Vertices = getAllVertices();
    //for(unsigned int i=0;i<Vertices.size();i++){
    //for(unsigned int i=0;i<50;i++){
    //  cout<<dec<<Vertices[i]->getId()<<" "<<hex<<Vertices[i]<<" ";
    //}
    //cout<<endl;

    VerticesSort = Vertices;
    EdgesSort = Edges;
    sort(VerticesSort.begin(),VerticesSort.end());
    sort(EdgesSort.begin(),EdgesSort.end());
  //start prefetching trigger
    passPtr(&VerticesSort,0,&EdgesSort,0,0,0);
    //cout<<"sof:"<<&Vertices<<endl;
    //cout<<"sof:"<<Vertices[1]<<endl;
    //cout<<"sof:"<<Edges[1]<<endl;
  }

  
protected:
  /// Hold pointers to all vertices.
  vector<VertexPointer> Vertices;
  vector<VertexPointer> VerticesSort;
  /// Hold pointers to all edges.
  vector<EdgePointer> Edges;
  vector<EdgePointer> EdgesSort;
  /// Keep a count of vertices and edges.
  unsigned int NumberOfVertices;
  unsigned int NumberOfEdges;
};

#endif /* _GRAPH_TYPE_H */

