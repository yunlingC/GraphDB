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

#define NODE 0
#define EDGE 1

#define START 0
#define END   1

#include <vector>
#include <map>
#include <stdlib.h>
#include <algorithm>

#include "util.h"
#include "Vertex.h"
#include "Edge.h"
#include "PropertyList.h"

class GraphType {
public:
  /// Typedefs that are used to refer to within other classes.
  typedef unsigned int VertexDescriptor;
  typedef unsigned int EdgeDescriptor;
  typedef Vertex* VertexPointer;
  typedef Edge* EdgePointer;
  typedef std::vector<EdgePointer> EdgeList;
  typedef PropertyList<FixedString, FixedString> PropertyListType;
  typedef PropertyListType VertexPropertyList;
  typedef PropertyListType EdgePropertyList;
  typedef map<unsigned int, VertexPointer> VertexMapType;
  typedef map<unsigned int, EdgePointer> EdgeMapType;

public:
  /// id cannot be removed, otherwise return wrong pointers
  VertexPointer getVertexPointer(VertexDescriptor Vertex) {
    if( VertexMap.find(Vertex) != VertexMap.end() ) {
      return VertexMap[Vertex];
    } else {
      return nullptr;
    }
  }

  EdgePointer getEdgePointer(EdgeDescriptor Edge) {
    if ( EdgeMap.find(Edge) != EdgeMap.end() ) {
      return EdgeMap[Edge];
    } else {
      return nullptr;
    }
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

  VertexMapType getVertexMap() {
    return VertexMap;
  }

  EdgeMapType getEdgeMap() {
    return EdgeMap;
  }

  VertexDescriptor addVertex() {
  #ifdef _FIXALLOC_
    if (NodeMemory == NULL) {
      cerr << "ERROR: Edge space not allocated\n";
      exit(1);
    }

    char* PlacePtr = NodeMemory + NumberOfVertices*sizeof(Vertex);
//    cout << "Place node at: " << reinterpret_cast<int*>(PlacePtr) << endl;
       VertexPointer NewVertex = new(PlacePtr) Vertex();
  #else 
       VertexPointer NewVertex = new Vertex();
  #endif /* _FIXALLOC_ */

//    VertexPointer NewVertex = new Vertex();
    NewVertex->setId(NumberOfVertices); 
    //support for map
    VertexMap.insert(std::pair<unsigned int, VertexPointer>(NumberOfVertices, NewVertex));
    ++NumberOfVertices;
    //TODO delete
    Vertices.push_back(NewVertex);
    return NewVertex->getId();
  }

  VertexDescriptor addVertex(string Label, PropertyListType & InitialPropertyList) {
  #ifdef _FIXALLOC_
    if (NodeMemory == NULL) {
      cerr << "ERROR: Edge space not allocated\n";
      exit(1);
    }

    char* PlacePtr = NodeMemory + NumberOfVertices*sizeof(Vertex);
//    cout << "Place node at: " << reinterpret_cast<int*>(PlacePtr) << endl;
       VertexPointer NewVertex = new(PlacePtr) Vertex();
  #else 
       VertexPointer NewVertex = new Vertex();
  #endif /* _FIXALLOC_ */

//    VertexPointer NewVertex = new Vertex();
    NewVertex->setPropertyList(InitialPropertyList);
    NewVertex->setId(NumberOfVertices); 
    NewVertex->setType(Label);
    VertexMap.insert(std::pair<unsigned int, VertexPointer>(NumberOfVertices, NewVertex));
    ++NumberOfVertices;
    //TODO delete
    Vertices.push_back(NewVertex);
    return NewVertex->getId();
  }

  VertexDescriptor addVertex(PropertyListType & InitialPropertyList) {
  #ifdef _FIXALLOC_
    if (NodeMemory == NULL) {
      cerr << "ERROR: Edge space not allocated\n";
      exit(1);
    }

    char* PlacePtr = NodeMemory + NumberOfVertices*sizeof(Vertex);
//    cout << "Place node at: " << reinterpret_cast<int*>(PlacePtr) << endl;
       VertexPointer NewVertex = new(PlacePtr) Vertex();
  #else 
       VertexPointer NewVertex = new Vertex();
  #endif /* _FIXALLOC_ */

//    VertexPointer NewVertex = new Vertex();
    NewVertex->setPropertyList(InitialPropertyList);
    NewVertex->setId(NumberOfVertices); 
    VertexMap.insert(std::pair<unsigned int, VertexPointer>(NumberOfVertices, NewVertex));
    ++NumberOfVertices;
    //TODO delete
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
    ///: Is the end of chain's first vertex the same as the Vertex.
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
    /// The chain is going to iterate over first and second pointers based on who is source.
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

  EdgeDescriptor addEdge(VertexDescriptor StartVertex, 
                         VertexDescriptor EndVertex, const string & Label) {
  #ifdef _FIXALLOC_
    if (EdgeMemory == NULL) {
      cerr << "ERROR: Edge space not allocated\n";
      exit(1);
    }
    //    Create new edge by retrieving VertexPtr from vertices.
       char * PlacePtr = EdgeMemory + NumberOfEdges*sizeof(Edge);
//       cout << "Place edge at: " << reinterpret_cast<int*>(PlacePtr) << endl;
       EdgePointer NewEdge = new(PlacePtr) Edge(Vertices[StartVertex], Vertices[EndVertex]);
  #else
    EdgePointer NewEdge = new Edge(Vertices[StartVertex], Vertices[EndVertex]);
  #endif /* _FIXALLOC_ */

//    EdgePointer NewEdge = new Edge(Vertices[StartVertex], Vertices[EndVertex]);
    NewEdge->setId(NumberOfEdges);
    NewEdge->setType(Label);
    assignPointers(StartVertex, EndVertex, NewEdge);

    EdgeMap.insert(std::pair<unsigned int, EdgePointer>(NumberOfEdges, NewEdge));
    ++NumberOfEdges;
    Edges.push_back(NewEdge);
    return NewEdge->getId();
  }

  EdgeDescriptor addEdge(VertexDescriptor StartVertex, 
                         VertexDescriptor EndVertex, 
                         PropertyListType & InitialPropertyList) {
  #ifdef _FIXALLOC_
    if (EdgeMemory == NULL) {
      cerr << "ERROR: Edge space not allocated\n";
      exit(1);
    }
    //    Create new edge by retrieving VertexPtr from vertices.
       char * PlacePtr = EdgeMemory + NumberOfEdges*sizeof(Edge);
//       cout << "Place edge at: " << reinterpret_cast<int*>(PlacePtr) << endl;
       EdgePointer NewEdge = new(PlacePtr) Edge(Vertices[StartVertex], Vertices[EndVertex]);
  #else
    EdgePointer NewEdge = new Edge(Vertices[StartVertex], Vertices[EndVertex]);
  #endif /* _FIXALLOC_ */

//    EdgePointer NewEdge = new Edge(Vertices[StartVertex], Vertices[EndVertex]);

    NewEdge->setPropertyList(InitialPropertyList);
    NewEdge->setId(NumberOfEdges);    
    assignPointers(StartVertex, EndVertex, NewEdge);

    EdgeMap.insert(std::pair<unsigned int, EdgePointer>(NumberOfEdges, NewEdge));
    ++NumberOfEdges;
    Edges.push_back(NewEdge);
    return NewEdge->getId();
  }

  EdgeDescriptor addEdge(VertexDescriptor StartVertex, 
                         VertexDescriptor EndVertex, 
                         const string & Label, 
                         PropertyListType & InitialPropertyList) {
  #ifdef _FIXALLOC_
    if (EdgeMemory == NULL) {
      cerr << "ERROR: Edge space not allocated\n";
      exit(1);
    }
    //    Create new edge by retrieving VertexPtr from vertices.
       char * PlacePtr = EdgeMemory + NumberOfEdges*sizeof(Edge);
//       cout << "Place edge at: " << reinterpret_cast<int*>(PlacePtr) << endl;
       EdgePointer NewEdge = new(PlacePtr) Edge(Vertices[StartVertex], Vertices[EndVertex]);
  #else
    EdgePointer NewEdge = new Edge(Vertices[StartVertex], Vertices[EndVertex]);
  #endif /* _FIXALLOC_ */

//    EdgePointer NewEdge = new Edge(Vertices[StartVertex], Vertices[EndVertex]);

    NewEdge->setType(Label);
    NewEdge->setPropertyList(InitialPropertyList);
    NewEdge->setId(NumberOfEdges);    
    assignPointers(StartVertex, EndVertex, NewEdge);

    EdgeMap.insert(std::pair<unsigned int, EdgePointer>(NumberOfEdges, NewEdge));
    ++NumberOfEdges;
    Edges.push_back(NewEdge);
    return NewEdge->getId();
  }

  ///Support for update on graph
//  EdgeDescriptor removeEdgeChain(EdgePointer Edge) {
//    VertexPointer FirstVertex = Edge->getFirstVertexPtr();
//    VertexPointer SecondVertex = Edge->getSecondVertexPtr();
//    EdgePointer  FirstNextEdge = Edge->getFirstNextEdge();
//    EdgePointer  FirstPrevEdge = Edge->getFirstPreviousEdge();
//    EdgePointer  SecondNextEdge = Edge->getSecondNextEdge();
//    EdgePointer  SecondPrevEdge = Edge->getSecondPreviousEdge();
///**    
//    if(FirstVertex->getNextEdge() != nullptr)
//    std::cout << "FirstVertex->NextEdge " << FirstVertex->getNextEdge()->getId() << "\n";
//    if(SecondVertex->getNextEdge() != nullptr)
//    std::cout << "SecondVertex->NextEdge " << SecondVertex->getNextEdge()->getId() << "\n";
//    if(FirstNextEdge != nullptr)
//    std::cout << "FirstNextEdge " << FirstNextEdge->getId() << "\n";
//    if(FirstPrevEdge != nullptr)
//    std::cout << "FirstPreviousEdge " << FirstPrevEdge->getId() << "\n";
//    if(SecondNextEdge != nullptr)
//    std::cout << "SecondNextEdge " << SecondNextEdge->getId() << "\n";
//    if(SecondPrevEdge != nullptr)
//    std::cout << "SecondPrevEdge " << SecondPrevEdge->getId() << "\n";
//*/
//    if(Edge == FirstVertex->getNextEdge()) {
//      FirstVertex->setNextEdge(Edge->getNextEdge(FirstVertex));
//    }
//    // Edge is not the first edge of FirstVertex
//    if(FirstPrevEdge != nullptr) {
//      FirstPrevEdge->setFirstNextEdge(FirstNextEdge);
//    }
//    // Edge is not the last edge of FirstVertex
//    if(FirstNextEdge != nullptr) {
//      FirstNextEdge->setFirstPreviousEdge(FirstPrevEdge);
//    }
//    // Edge is the only next edge of FirstVertex
//    if((FirstPrevEdge == nullptr) && (FirstNextEdge == nullptr)) {
//      FirstVertex->setNextEdge(nullptr); 
//    }
//    
//    /// repeat for the second
//    if(Edge == SecondVertex->getNextEdge()) {
//      SecondVertex->setNextEdge(Edge->getNextEdge(SecondVertex));
//    }
//    if(SecondPrevEdge != nullptr) {
//      SecondPrevEdge->setSecondNextEdge(SecondNextEdge);
//    }
//    if(SecondNextEdge != nullptr) {
//      SecondNextEdge->setSecondPreviousEdge(SecondPrevEdge);
//    }
//    if((SecondPrevEdge == nullptr) && (SecondNextEdge == nullptr)) {
//      SecondVertex->setNextEdge(nullptr); 
//    }
//
//    return Edge->getId();
//  }
//

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

#ifdef _FIXALLOC_
    //    Delete the memory spaces.
    delete NodeMemory;
    delete EdgeMemory;
#else 
   std::cout << "GraphType: clean " << Vertices.size() << " vertices and " << Edges.size() << " edges\n";
    for ( size_t i=0; i < Vertices.size(); i++ ) {
      Vertices[i]->deleteVertex();
      delete Vertices[i];
    }
//    std::cout << "vertex cleaning is done\n";


    for ( size_t i=0; i < Edges.size(); i++ ) {
      Edges[i]->deleteEdge();
      delete Edges[i];
    }
#endif /* _FIXALLOC_ */

  }

#ifdef _FIXALLOC_
  void allocVertexMemory(unsigned int sz) {

    // Allocation sz number of Vertex objects.
    NodeMemory = new char[sizeof(Vertex)*sz];
    cout << "Vertex Memory\n + Starting address: " << reinterpret_cast<int*>(NodeMemory)
    	 << ", ending address: " << reinterpret_cast<int*>(NodeMemory + sizeof(Vertex)*sz) << "\n";

//    passAddr(NODE, NodeMemory, (NodeMemory + sizeof(Vertex)*sz));

  }

  void allocEdgeMemory(unsigned int sz) {
    // Allocation sz number of Vertex objects.
    cout << "Edge space: " << sizeof(Edge)*sz << "\n";
    EdgeMemory = new char[sizeof(Edge)*sz];
    cout << "Edge Memory\n + Starting address: " << reinterpret_cast<int*>(EdgeMemory) 
    	 << ", ending address: " << reinterpret_cast<int*>(EdgeMemory + sizeof(Edge)*sz) << "\n" << endl;
//    passAddr(EDGE, EdgeMemory, (EdgeMemory + sizeof(Edge)*sz));
  }
#endif /* _FIXALLOC_ */

  vector<VertexPointer> getAllVertices(){
    return Vertices;
  }

  vector<EdgePointer> getAllEdges(){
    return Edges;
  }

//  void handleAddr(){
//    VerticesSort = Vertices;
//    EdgesSort = Edges;
//    sort(VerticesSort.begin(),VerticesSort.end());
//  //start prefetching trigger
//    passPtr(&VerticesSort,0,&EdgesSort,0,0,0);
//  }

protected:
  /// Hold pointers to all vertices.
  vector<VertexPointer> Vertices;
  vector<VertexPointer> VerticesSort;
  VertexMapType VertexMap;
  /// Hold pointers to all edges.
  vector<EdgePointer> Edges;
  vector<EdgePointer> EdgesSort;
  EdgeMapType EdgeMap;
  /// Keep a count of vertices and edges.
  unsigned int NumberOfVertices;
  unsigned int NumberOfEdges;

#ifdef _FIXALLOC_
  char* NodeMemory;
  char* EdgeMemory;
#endif /* _FIXALLOC_ */
};

#endif /* _GRAPH_TYPE_H */
