//===-- storage/GraphType.h - Graph class type ------------------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.  //
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the main class for the Graph structure.
///
//===----------------------------------------------------------------------===//
#ifndef _GRAPH_TYPE_H_ 
#define _GRAPH_TYPE_H_ 

#include <vector>
#include <unordered_map>
#include <stdlib.h>
#include <unordered_set>

#include "util.h"
#include "Vertex.h"
#include "Edge.h"
#include "PropertyList.h"

class VertexCopy {
public:
  unsigned int  VertexId;
  Vertex::LabelType VertexLabel;
  Vertex::PropertyListType VertexPropertyList;
public:

  VertexCopy() {}

  bool makeCopy(Vertex* VertexPtr) {
    if (VertexPtr == nullptr)
      return false;
    VertexId = VertexPtr->getId();
    VertexLabel = VertexPtr->getType();
    ///TODO problem here, only get a pointer
    VertexPropertyList = VertexPtr->getPropertyList(); 
    return true;
  }

};

class EdgeCopy {
public:
  unsigned int EdgeId;
  unsigned int FirstVertexId;
  unsigned int SecondVertexId;
  Edge::LabelType EdgeLabel;
  Edge::PropertyListType EdgePropertyList;
public:

  EdgeCopy() {}

  bool makeCopy(Edge* EdgePtr){
    if (EdgePtr == nullptr) 
      return false;
    EdgeId = EdgePtr->getId();
    FirstVertexId = EdgePtr->getFirstId();
    SecondVertexId = EdgePtr->getSecondId();
    EdgeLabel = EdgePtr->getType();
    EdgePropertyList = EdgePtr->getPropertyList();
    return true;
  }
};

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
  typedef unordered_map<unsigned int, VertexPointer> VertexMapType;
  typedef unordered_map<unsigned int, EdgePointer> EdgeMapType;
  
  ///temp 
  typedef EdgeCopy EdgeCopyType;
  typedef VertexCopy VertexCopyType;
  typedef EdgeCopy* EdgeCopyPointer;
  typedef VertexCopy* VertexCopyPointer;


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
    if ( NextEdge != nullptr) {
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
    if ( NextEdge != nullptr) {

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

    unsigned int VertexNumber = VertexMap.size();
    char* PlacePtr = NodeMemory + VertexNumber*sizeof(Vertex);
    VertexPointer NewVertex = new(PlacePtr) Vertex();
  #else 
       VertexPointer NewVertex = new Vertex();
  #endif /* _FIXALLOC_ */

//    VertexPointer NewVertex = new Vertex();
    NewVertex->setId(NumberOfVertices); 
    //support for map
    VertexMap.insert(std::pair<unsigned int, VertexPointer>(NumberOfVertices, NewVertex));
    ++NumberOfVertices;
    return NewVertex->getId();
  }

  VertexDescriptor addVertex(string Label, PropertyListType & InitialPropertyList) {
  #ifdef _FIXALLOC_
    if (NodeMemory == NULL) {
      cerr << "ERROR: Edge space not allocated\n";
      exit(1);
    }

    unsigned int VertexNumber = VertexMap.size();
    char* PlacePtr = NodeMemory + VertexNumber*sizeof(Vertex);
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
    return NewVertex->getId();
  }

  VertexDescriptor addVertex(PropertyListType & InitialPropertyList) {
  #ifdef _FIXALLOC_
    if (NodeMemory == NULL) {
      cerr << "ERROR: Edge space not allocated\n";
      exit(1);
    }

    unsigned int VertexNumber = VertexMap.size();
    char* PlacePtr = NodeMemory + VertexNumber*sizeof(Vertex);
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
    return NewVertex->getId();
  }

  VertexDescriptor addVertexBack(VertexCopyPointer VertexCopyPtr) {
  #ifdef _FIXALLOC_
    if (NodeMemory == NULL) {
      cerr << "ERROR: Edge space not allocated\n";
      exit(1);
    }

    unsigned int VertexNumber = VertexMap.size();
    char* PlacePtr = NodeMemory + VertexNumber*sizeof(Vertex);
    VertexPointer NewVertex = new(PlacePtr) Vertex();

  #else 
       VertexPointer NewVertex = new Vertex();
  #endif /* _FIXALLOC_ */

    NewVertex->setPropertyList(VertexCopyPtr->VertexPropertyList);
    NewVertex->setId(VertexCopyPtr->VertexId); 
    NewVertex->setType(VertexCopyPtr->VertexLabel);
    if (VertexMap.find(VertexCopyPtr->VertexId) != VertexMap.end()) {
      std::cerr << "Vertex " << VertexCopyPtr->VertexId << " exists in graph\n";
    } else {
      VertexMap.insert(std::pair<unsigned int, VertexPointer>(VertexCopyPtr->VertexId, NewVertex));
    }

//    std::cout << "Vertex " << NewVertex->getId() 
//              << " new address "  << NewVertex 
//              << "\n";

//    std::cout << "NewVeretx addback \n";
//    NewVertex->dump();
//
    return NewVertex->getId();
  }

  /// TODO Delete this function if we have LastEdge field 
  /// or insert edge to the head of the list
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

    NewEdge->setFirstPreviousEdge(FirstLastEdge);
    NewEdge->setFirstNextEdge(nullptr);

//    if ( FirstLastEdge == nullptr ) {
//      FirstVertexPointer->setLastEdge(NewEdge);
//    } else {

    if (FirstLastEdge != nullptr) {
      if ( FirstLastEdge->getFirstVertexPtr() == FirstVertexPointer ) {
        FirstLastEdge->setFirstNextEdge(NewEdge);
      }
      else if (FirstLastEdge->getSecondVertexPtr() == FirstVertexPointer) {
        FirstLastEdge->setSecondNextEdge(NewEdge);
      }
    }
//      NewEdge->setFirstPreviousEdge(FirstLastEdge);
//      NewEdge->setFirstNextEdge(nullptr);
      FirstVertexPointer->setLastEdge(NewEdge);
//    }

    NewEdge->setSecondPreviousEdge(SecondLastEdge);
    NewEdge->setSecondNextEdge(nullptr);

//    std::cout << "Edge " << NewEdge->getId() 
//              << " has SPE ";
//    if (SecondLastEdge != nullptr) {
//      std::cout << SecondLastEdge->getId() << "\n";
//    } else {
//      std::cout << "nullptr \n";
//    }
//

//    if ( SecondLastEdge == nullptr) {
//      SecondVertexPointer->setLastEdge(NewEdge);
//    } else {
    if (SecondLastEdge != nullptr) {
      if( SecondLastEdge->getFirstVertexPtr() == SecondVertexPointer ) {
        SecondLastEdge->setFirstNextEdge(NewEdge);
      } else if ( SecondLastEdge->getSecondVertexPtr() == SecondVertexPointer ) {
        SecondLastEdge->setSecondNextEdge(NewEdge);
      }
    }
//      NewEdge->setSecondPreviousEdge(SecondLastEdge);
//      NewEdge->setSecondNextEdge(nullptr);
      SecondVertexPointer->setLastEdge(NewEdge);
//    }
  }

  EdgeDescriptor addEdge(VertexDescriptor StartVertex, 
                         VertexDescriptor EndVertex, const string & Label) {
  #ifdef _FIXALLOC_
    if (EdgeMemory == NULL) {
      cerr << "ERROR: Edge space not allocated\n";
      exit(1);
    }
    //    Create new edge by retrieving VertexPtr from vertices.
       unsigned int EdgeNumber = EdgeMap.size();
       char * PlacePtr = EdgeMemory + EdgeNumber*sizeof(Edge);
//       cout << "Place edge at: " << reinterpret_cast<int*>(PlacePtr) << endl;
       EdgePointer NewEdge = new(PlacePtr) Edge(VertexMap[StartVertex]
                                              , VertexMap[EndVertex]);
  #else
       EdgePointer NewEdge = new Edge(VertexMap[StartVertex]
                                              , VertexMap[EndVertex]);
  #endif /* _FIXALLOC_ */

//    EdgePointer NewEdge = new Edge(Vertices[StartVertex], Vertices[EndVertex]);
    NewEdge->setId(NumberOfEdges);
    NewEdge->setType(Label);
    assignPointers(StartVertex, EndVertex, NewEdge);

    EdgeMap.insert(std::pair<unsigned int, EdgePointer>(NumberOfEdges, NewEdge));
    ++NumberOfEdges;
//    Edges.push_back(NewEdge);
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
       unsigned int EdgeNumber = EdgeMap.size();
       char * PlacePtr = EdgeMemory + EdgeNumber*sizeof(Edge);
//       cout << "Place edge at: " << reinterpret_cast<int*>(PlacePtr) << endl;
       EdgePointer NewEdge = new(PlacePtr) Edge(VertexMap[StartVertex]
                                              , VertexMap[EndVertex]);
  #else
       EdgePointer NewEdge = new Edge(VertexMap[StartVertex], VertexMap[EndVertex]);
  #endif /* _FIXALLOC_ */

//    EdgePointer NewEdge = new Edge(Vertices[StartVertex], Vertices[EndVertex]);

    NewEdge->setPropertyList(InitialPropertyList);
    NewEdge->setId(NumberOfEdges);    
    assignPointers(StartVertex, EndVertex, NewEdge);

    EdgeMap.insert(std::pair<unsigned int, EdgePointer>(NumberOfEdges, NewEdge));
    ++NumberOfEdges;
//    Edges.push_back(NewEdge);
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
       unsigned int EdgeNumber = EdgeMap.size();
       char * PlacePtr = EdgeMemory + EdgeNumber*sizeof(Edge);
       EdgePointer NewEdge = new(PlacePtr) Edge(VertexMap[StartVertex]
                                              , VertexMap[EndVertex]);
  #else
       EdgePointer NewEdge = new Edge(VertexMap[StartVertex]
                                              , VertexMap[EndVertex]);
  #endif /* _FIXALLOC_ */


    NewEdge->setType(Label);
    NewEdge->setPropertyList(InitialPropertyList);
    NewEdge->setId(NumberOfEdges);    
    assignPointers(StartVertex, EndVertex, NewEdge);

    EdgeMap.insert(std::pair<unsigned int, EdgePointer>(NumberOfEdges, NewEdge));
    ++NumberOfEdges;
    return NewEdge->getId();
  }

  EdgeDescriptor addEdgeBack(EdgeCopyPointer EdgeCopyPtr) {
//                            EdgeDescriptor EdgeId,
//                            VertexDescriptor StartVertex, 
//                            VertexDescriptor EndVertex, 
//                            const string & Label, 
//                            PropertyListType & InitialPropertyList) {
  #ifdef _FIXALLOC_
    if (EdgeMemory == NULL) {
      cerr << "ERROR: Edge space not allocated\n";
      exit(1);
    }
       unsigned int EdgeNumber = EdgeMap.size();
       char * PlacePtr = EdgeMemory + EdgeNumber*sizeof(Edge);

       EdgePointer NewEdge = new(PlacePtr) Edge(VertexMap[EdgeCopyPtr->FirstVertexId]
                                              , VertexMap[EdgeCopyPtr->SecondVertexId]);

//       std::cout << "NewEdge " << NewEdge->getFirstVertexPtr()->getId()
//                               << "\t" 
//                               << NewEdge->getSecondVertexPtr()->getId()
//                               << "\n";
  #else
       EdgePointer NewEdge = new Edge(VertexMap[EdgeCopyPtr->FirstVertexId]
                                    , VertexMap[EdgeCopyPtr->SecondVertexId]);
  #endif /* _FIXALLOC_ */

    NewEdge->setId(EdgeCopyPtr->EdgeId);    
    NewEdge->setType(EdgeCopyPtr->EdgeLabel);
    NewEdge->setPropertyList(EdgeCopyPtr->EdgePropertyList);
    assignPointers(EdgeCopyPtr->FirstVertexId, EdgeCopyPtr->SecondVertexId, NewEdge);

    if (EdgeMap.find(EdgeCopyPtr->EdgeId) != EdgeMap.end()) {
      std::cerr <<"Edge " << EdgeCopyPtr->EdgeId <<" exists in graph\n";
    } else {
      EdgeMap.insert(std::pair<unsigned int, EdgePointer>(NewEdge->getId(), NewEdge));
    }

//    std::cout << "NewEdge addback \n";
//    NewEdge->dump();

    return NewEdge->getId();
  }

  ///Support for update on graph
  EdgeDescriptor removeEdgeChains(EdgePointer Edge) {
    VertexPointer FirstVertex = Edge->getFirstVertexPtr();
    VertexPointer SecondVertex = Edge->getSecondVertexPtr();
    EdgePointer  FirstNextEdge = Edge->getFirstNextEdge();
    EdgePointer  FirstPrevEdge = Edge->getFirstPreviousEdge();
    EdgePointer  SecondNextEdge = Edge->getSecondNextEdge();
    EdgePointer  SecondPrevEdge = Edge->getSecondPreviousEdge();

    if(Edge == FirstVertex->getNextEdge()) {
      FirstVertex->setNextEdge(FirstNextEdge);
    }
    // Edge is the last edge of FirstVertex
    if (Edge == FirstVertex->getLastEdge()) {
      /// TODO bug 
///      FirstVertex->setLastEdge(nullptr);
  
      FirstVertex->setLastEdge(FirstPrevEdge);
    }
    // Edge is not the first edge of FirstVertex
    if(FirstPrevEdge != nullptr) {
      if (FirstPrevEdge->getFirstVertexPtr() == FirstVertex) {
        FirstPrevEdge->setFirstNextEdge(FirstNextEdge);
      } else if (FirstPrevEdge->getSecondVertexPtr() == FirstVertex) {
        FirstPrevEdge->setSecondNextEdge(FirstNextEdge);
      }
    }
    // Edge is not the last edge of FirstVertex
    if (FirstNextEdge != nullptr) {
      if (FirstNextEdge->getFirstVertexPtr() == FirstVertex) {
        FirstNextEdge->setFirstPreviousEdge(FirstPrevEdge);
      } else if (FirstNextEdge->getSecondVertexPtr() == FirstVertex) {
        FirstNextEdge->setSecondPreviousEdge(FirstPrevEdge);
      }
    }

    /// Edge is the only next edge of FirstVertex
//    if ((FirstPrevEdge == nullptr) && (FirstNextEdge == nullptr)) {
//      FirstVertex->setNextEdge(nullptr); 
//    }
    
    /// repeat for the second
    if (Edge == SecondVertex->getNextEdge()) {
      SecondVertex->setNextEdge(SecondNextEdge);
    }
    if (Edge == SecondVertex->getLastEdge()) {
      SecondVertex->setLastEdge(SecondPrevEdge);
    }
    if(SecondPrevEdge != nullptr) {
      if (SecondPrevEdge->getFirstVertexPtr() == SecondVertex) {
        SecondPrevEdge->setFirstNextEdge(SecondNextEdge);
      } else if (SecondPrevEdge->getSecondVertexPtr() == SecondVertex) {
        SecondPrevEdge->setSecondNextEdge(SecondNextEdge);
      }
    }
    if(SecondNextEdge != nullptr) {
      if (SecondNextEdge->getFirstVertexPtr() == SecondVertex){
        SecondNextEdge->setFirstPreviousEdge(SecondPrevEdge);
      } else if (SecondNextEdge->getSecondVertexPtr() == SecondVertex) {
        SecondNextEdge->setSecondPreviousEdge(SecondPrevEdge);
      }
    }

//   if((SecondPrevEdge == nullptr) && (SecondNextEdge == nullptr)) {
//      SecondVertex->setNextEdge(nullptr); 
//    }
//
    return Edge->getId();
  }

  /// This function removes edges connecting to the current vertex
  /// Basically just call removeEdge on all the edges
  VertexDescriptor removeVertexChains(VertexPointer VertexPtr) {
    auto NextEdge =  VertexPtr->getNextEdge();
    auto CurrEdge = NextEdge;
    while (NextEdge != nullptr) {
      /// TODO Bug here       //--- check
      /// removeEdge on CurrEdge could change the NextEdge (pointers)
      CurrEdge = NextEdge; 
      NextEdge =  CurrEdge->getNextEdge(VertexPtr);
//      std::cout <<"CurrEdge " << CurrEdge->getId() << CurrEdge << "\n";
//      CurrEdge->dump();
//      std::cout <<"NextEdge "  << NextEdge << "\n";
     if (NextEdge == nullptr) {
        removeEdge(CurrEdge);
        break;
      } else {
        auto NextEdgeId = NextEdge->getId();
        removeEdge(CurrEdge);
        NextEdge = EdgeMap[NextEdgeId];
//        std::cout <<"New NextEdge "  << NextEdgeId << NextEdge << "\n";
      }
    }
    return VertexPtr->getId();
  }

  VertexDescriptor removeVertex(VertexDescriptor VertexId) {
    VertexPointer VertexPtr = getVertexPointer(VertexId);
    return removeVertex(VertexPtr);
  }

  EdgeDescriptor removeEdge(EdgeDescriptor EdgeId) {
    EdgePointer EdgePtr = getEdgePointer(EdgeId);
    return removeEdge(EdgePtr);
  } 
  
  VertexDescriptor removeVertex(VertexPointer VertexPtr) {
    if (VertexPtr == nullptr) {
      std::cerr << "Vertex does NOT exist \n";
      return -1;
    }

    Vertices.insert(VertexPtr->getId());
    VertexCopyPointer DeletedVertex = new VertexCopy();
    DeletedVertex->makeCopy(VertexPtr);
    VertexBackup.push_back(DeletedVertex);
    /// Remove the edges that connect to this vertex
    /// Basically use removeEdge() function; 
    
    auto VertexId = VertexPtr->getId();
    removeVertexChains(VertexPtr);

    /// Call PropertyList.delete() explicitly
//    VertexPtr->deleteVertex();

#ifdef _FIXALLOC_
    /// move the last vertex in the memory layout to current location
    reLocateVertex(VertexPtr);
#endif 

    /// Attention! VertexPtr points to the new vertex now
    ///TODO Need a lock guarding the VertexMap in multithread
    VertexMap.erase(VertexId);

    return VertexId;
  }


  EdgeDescriptor removeEdge(EdgePointer EdgePtr) {
    if (EdgePtr == nullptr) {
//      std::cerr << "Edge does NOT exist\n";
//      exit(0);
    return -1;
    }

    Edges.insert(EdgePtr->getId());
    EdgeCopyPointer  DeletedEdge = new EdgeCopy(); 
    DeletedEdge->makeCopy(EdgePtr);
    EdgeBackup.push_back(DeletedEdge);
    /// Reset the fields in edges that have pointers to this edge
    auto EdgeId = removeEdgeChains(EdgePtr);


    /// Only call deletePropertyList()
//    EdgePtr->deleteEdge();

#ifdef _FIXALLOC_
    /// Move the last edge in the memory layout to current location
    reLocateEdge(EdgePtr);
#endif

    /// Should remove the entry in the map
    /// TODO With Multithread, should have a lock guarding the map
    EdgeMap.erase(EdgeId);

//    std::cout << "remove edge " << EdgeId << "\n";
    return EdgeId;
  }

  /// Call removeEdge and check if any endpoint is isolated 
  /// If yes, remove the vertex too.
  EdgeDescriptor removeBranch(EdgePointer EdgePtr) {
    auto FirstVertex = EdgePtr->getFirstVertexPtr();
    auto SecondVertex = EdgePtr->getSecondVertexPtr();
    auto EdgeId = removeEdge(EdgePtr);
    checkIsolatedVertex(FirstVertex);
    checkIsolatedVertex(SecondVertex);
    return EdgeId;
  }

  EdgeDescriptor removeBranch(EdgeDescriptor EdgeId) {
    EdgePointer EdgePtr = getEdgePointer(EdgeId);
    return removeBranch(EdgePtr);
  }

  void checkIsolatedVertex(VertexPointer VertexPtr) {

    /// Only need to check LastEdge if LastEdge is set properly
    if (VertexPtr->getLastEdge() == nullptr &&
        VertexPtr->getNextEdge() == nullptr) {
//        std::cout << "Vertex " << VertexPtr->getId()
//                  << " becomes isolated\n";
        removeVertex(VertexPtr);
    }
  }


#ifdef _FIXALLOC_

  /// Move the last Vertex to the memory location that gets free()
  /// VertexAddr is the address 
  void reLocateVertex(VertexPointer VertexAddr) {

    /// get the last vertex in the memory layout
    char *LastAddr = (VertexMap.size()-1)*sizeof(Vertex) + NodeMemory;
    VertexPointer LastVertex = reinterpret_cast<VertexPointer>(LastAddr);

    /// If the deleted vertex is the last one in the memory layout, then don't move
    if (VertexAddr == LastVertex) {
 //     std::cout <<"No relocating \n";
      return;
    }
//    std::cout <<"Memory hole " << VertexAddr 
//              <<" , LastVertex " << LastVertex 
//              << "\n";

    VertexPointer NewVertex = new(VertexAddr) Vertex();
    NewVertex->setId(LastVertex->getId());
    NewVertex->setNextEdge(LastVertex->getNextEdge());
    NewVertex->setLastEdge(LastVertex->getLastEdge());
    NewVertex->setType(LastVertex->getType());
    NewVertex->setPropertyList(LastVertex->getPropertyList());

//    LastVertex->dump();
//
//    NewVertex->dump();


//    auto NextEdge = LastVertex->getNextEdge();
//    while (NextEdge != nullptr) {
//      auto CurrEdge = NextEdge;
//      NextEdge = CurrEdge->getNextEdge(LastVertex);
//      if (NextEdge->getFirstVertexPtr() == LastVertex) {
//        NextEdge
//      }
//    }


    /// change the FV pointer from the edges that originate from this vertex
    /// change the SV pointer;
    /// Tricky here 
    EdgeList AllEdges = getOutEdges(LastVertex);
    EdgeList InEdges = getInEdges(LastVertex);
    AllEdges.insert( AllEdges.end(), InEdges.begin(), InEdges.end());

    for (auto & Edge : AllEdges){
      if (Edge->getFirstVertexPtr() == LastVertex)
        Edge->setFirstVertexPtr(NewVertex);
      else if (Edge->getSecondVertexPtr() == LastVertex)
        Edge->setSecondVertexPtr(NewVertex);
    }

    /// change the SV pointer;
//    for (auto & Edge: getInEdges(LastVertex)) {
//      Edge->setSecondVertexPtr(NewVertex);
//    }
//
    /// Change the entry<VertexId, LastVertex> into <VertexId, NewVertex>
    VertexMap[NewVertex->getId()] = NewVertex;

    /// Just delete this vertex pointer, not the PropertyList
    /// TODO trouble here, need fix
//    delete LastVertex;

  }

  void reLocateEdge(EdgePointer EdgeAddr) {

    char *LastAddr = (EdgeMap.size()-1)*sizeof(Edge) + EdgeMemory;
    EdgePointer LastEdge = reinterpret_cast<EdgePointer>(LastAddr);
    EdgePointer NewEdge = new(EdgeAddr) Edge(nullptr, nullptr);

    if (EdgeAddr == LastEdge) {
//      std::cout <<"No relocating \n";
      return;
    }

//    std::cout <<"Memory hole " << EdgeAddr 
//              <<" , LastVertex " << LastEdge
//              << "\n";
                                      
    NewEdge->setId(LastEdge->getId());
    NewEdge->setFirstVertexPtr(LastEdge->getFirstVertexPtr());
    NewEdge->setSecondVertexPtr(LastEdge->getSecondVertexPtr());
    NewEdge->setFirstNextEdge(LastEdge->getFirstNextEdge());
    NewEdge->setSecondNextEdge(LastEdge->getSecondNextEdge());
    NewEdge->setFirstPreviousEdge(LastEdge->getFirstPreviousEdge());
    NewEdge->setSecondPreviousEdge(LastEdge->getSecondPreviousEdge());
    NewEdge->setPropertyList(LastEdge->getPropertyList());
    NewEdge->setType(LastEdge->getType());

//    LastEdge->dump();
//
//    NewEdge->dump();

    auto FirstVertex = NewEdge->getFirstVertexPtr();
    auto SecondVertex = NewEdge->getSecondVertexPtr();
 
    /// Reset all pointers that point to LastEdge/NewEdge
    auto FirstNextEdge = NewEdge->getFirstNextEdge();
    auto FirstPrevEdge = NewEdge->getFirstPreviousEdge();
    auto SecondNextEdge = NewEdge->getSecondNextEdge();
    auto SecondPrevEdge = NewEdge->getSecondPreviousEdge();
   
    if (FirstNextEdge != nullptr) {
      if (FirstNextEdge->getFirstVertexPtr() == FirstVertex) {
        FirstNextEdge->setFirstPreviousEdge(NewEdge);
      } else if (FirstNextEdge->getSecondVertexPtr() == FirstVertex) {
        FirstNextEdge->setSecondPreviousEdge(NewEdge);
      }
    }

    if (FirstPrevEdge != nullptr) {
      if (FirstPrevEdge->getFirstVertexPtr() == FirstVertex) {
        FirstPrevEdge->setFirstNextEdge(NewEdge);
      } else if (FirstPrevEdge->getSecondVertexPtr() == FirstVertex) {
        FirstPrevEdge->setSecondNextEdge(NewEdge);
      } 
    }

    if (SecondNextEdge != nullptr) {
      if (SecondNextEdge->getFirstVertexPtr() == SecondVertex) {
        SecondNextEdge->setFirstPreviousEdge(NewEdge);
      } else if (SecondNextEdge->getSecondVertexPtr() == SecondVertex) {
        SecondNextEdge->setSecondPreviousEdge(NewEdge);
      }
    }

    if (SecondPrevEdge != nullptr) {
      if (SecondPrevEdge->getFirstVertexPtr() == SecondVertex) {
        SecondPrevEdge->setFirstNextEdge(NewEdge);
      } else if (SecondPrevEdge->getSecondVertexPtr() == SecondVertex) {
        SecondPrevEdge->setSecondNextEdge(NewEdge);
      }
    }

    ///Change the NextEdge  field
    if (FirstVertex->getNextEdge() == LastEdge) {
      FirstVertex->setNextEdge(NewEdge);
    }

    if (SecondVertex->getNextEdge() == LastEdge) {
      SecondVertex->setNextEdge(NewEdge);
    }

    /// Change LastEdge field 
    if (FirstVertex->getLastEdge() == LastEdge) {
      FirstVertex->setLastEdge(NewEdge);
    }

    if (SecondVertex->getLastEdge() == LastEdge) {
      SecondVertex->setLastEdge(NewEdge);
    }

    /// Change the entry in EdgeMap

//    std::cout << "change the value in entry " << NewEdge->getId() 
//              << "to " << NewEdge
//              << "\n";
//
    EdgeMap[NewEdge->getId()] = NewEdge;

    /// Just delete Pointer, not the content
    /// TODO trouble here, need fix
//    delete LastEdge;

  }

#endif


  /// For recovery from VertexBackup and EdgeBackup

  void recoverVertex(VertexCopyPointer VertexCopyPtr) {
    addVertexBack(VertexCopyPtr);  
    delete VertexCopyPtr;
  }

  void recoverEdge(EdgeCopyPointer EdgeCopyPtr) {
    addEdgeBack(EdgeCopyPtr);
    delete EdgeCopyPtr;

  }

  void recoverVertices() {
 
    std::cout << "Recover " << VertexBackup.size() << " vertices \n";
    for (auto & VertexCopyPtr : VertexBackup) {
      recoverVertex(VertexCopyPtr);
    }

    VertexBackup.clear();
    
    /// TODO erase the entry from VertexBackup
  }

  void recoverEdges() {

    std::cout << "Recover " << EdgeBackup.size() << " edges \n";
    for (auto & EdgeCopyPtr : EdgeBackup) {
      recoverEdge(EdgeCopyPtr);
    }

    EdgeBackup.clear();

    /// TODO erase the entry from EdgeBackup
  }

  void recoverAll() {
    recoverVertices();
    recoverEdges();
  }

  void dump() {
    std::cout << "\n-- Total number of Vertices: " << VertexMap.size()
              << "\n-- Total number of Edges: " << EdgeMap.size() 
              << "\n";
  
    std::cout << "----- Vertex \n";

    for (auto & Vertex : VertexMap) {
      Vertex.second->dump();
    }

    std::cout << "----- Edge \n";

    for (auto & Edge: EdgeMap) {
        Edge.second->dump();
    }

  }

  GraphType(): NumberOfVertices(0), NumberOfEdges(0) { }
  
  ~GraphType() {
    /// Must manually delete the objects.  
    /// However, only one place is necessary since everywhere else, I am storing pointers.

#ifdef _FIXALLOC_
    //    Delete the memory spaces.
    delete NodeMemory;
    delete EdgeMemory;
#else 

    for (auto & Vertex : VertexMap) {
      Vertex.second->deleteVertex();
      delete Vertex.second;
    }

    for (auto & Edge : EdgeMap) {
      Edge.second->deleteEdge();
      delete Edge.second;
    }
#endif /* _FIXALLOC_ */

  }

#ifdef _FIXALLOC_
  void allocVertexMemory(unsigned int sz) {

    // Allocation sz number of Vertex objects.
    std::cout << "Vertex space: " << sizeof(Vertex)*sz << "\n";
    NodeMemory = new char[sizeof(Vertex)*sz];
    std::cout << "Vertex Memory\n + Starting address: " 
              << reinterpret_cast<int*>(NodeMemory)
    	        << ", ending address: " 
              << reinterpret_cast<int*>(NodeMemory + sizeof(Vertex)*sz) 
              << "\n";
  }

  void allocEdgeMemory(unsigned int sz) {

    // Allocation sz number of Edge objects.
    std::cout << "Edge space: " << sizeof(Edge)*sz << "\n";
    EdgeMemory = new char[sizeof(Edge)*sz];
    std::cout << "Edge Memory\n + Starting address: " 
              << reinterpret_cast<int*>(EdgeMemory) 
    	        << ", ending address: " 
              << reinterpret_cast<int*>(EdgeMemory + sizeof(Edge)*sz) 
              << "\n" << endl;
  }

  /// For experiments on separate graphs
  /// Memory allocated from a fixed address -startMemory
  void allocVertexMemory(char * StartMemory, unsigned int sz) {

    // Allocation sz number of Vertex objects.
    std::cout << "Vertex space: " << sizeof(Vertex)*sz << "\n";
//    NodeMemory = new char[sizeof(Vertex)*sz];
    NodeMemory = StartMemory;
    std::cout << "Vertex Memory\n + Starting address: " 
              << reinterpret_cast<int*>(NodeMemory)
    	        << ", ending address: " 
              << reinterpret_cast<int*>(NodeMemory + sizeof(Vertex)*sz) 
              << "\n";
  }

  void allocEdgeMemory(char * StartMemory, unsigned int sz) {

    // Allocation sz number of Edge objects.
    std::cout << "Edge space: " << sizeof(Edge)*sz << "\n";
    EdgeMemory = StartMemory;
//    EdgeMemory = new char[sizeof(Edge)*sz];
    std::cout << "Edge Memory\n + Starting address: " 
              << reinterpret_cast<int*>(EdgeMemory) 
    	        << ", ending address: " 
              << reinterpret_cast<int*>(EdgeMemory + sizeof(Edge)*sz) 
              << "\n" << endl;
  }


#endif /* _FIXALLOC_ */

protected:
  /// Hold pointers to all vertices.
  VertexMapType VertexMap;
  /// Hold pointers to all edges.
  EdgeMapType EdgeMap;
  /// Keep a count of vertices and edges.
  unsigned int NumberOfVertices;
  unsigned int NumberOfEdges;

  /// The two vector below just for experiments
  std::vector<VertexCopyPointer> VertexBackup;
  std::vector<EdgeCopyPointer> EdgeBackup;

#ifdef _FIXALLOC_
  char* NodeMemory;
  char* EdgeMemory;
#endif /* _FIXALLOC_ */

public:
  /// Temporal experiments
  std::unordered_set<unsigned int> Vertices;
  std::unordered_set<unsigned int> Edges;
};

#endif /* _GRAPH_TYPE_H */
