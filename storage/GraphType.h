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

///TODO : GraphType for immutable graphs

class GraphType {
public:
  /// Typedefs that are used to refer to within other classes.
  typedef unsigned int VertexDescriptor;
  typedef unsigned int EdgeDescriptor;
  typedef Vertex* VertexPointer;
  typedef Edge* EdgePointer;
  typedef std::vector<EdgePointer> EdgeList;
  typedef std::vector<VertexPointer> VertexList;
  typedef std::vector<VertexDescriptor> VertexIDList;
  typedef PropertyList<FixedString, FixedString> PropertyListType;
  typedef PropertyListType VertexPropertyList;
  typedef PropertyListType EdgePropertyList;
//  typedef std::unordered_map<VertexDescriptor, VertexPointer> VertexMapType;
//  typedef std::unordered_map<EdgeDescriptor, EdgePointer> EdgeMapType;

public:
  /// Index <VertexDescriptor, VertexPointer>
  VertexPointer getVertexPointer(VertexDescriptor Vertex);

  /// All edges and vertices
  VertexList getAllVertices();

  EdgeList getAllEdges();

  /// OutEdges and InEdges
  EdgeList getOutEdges(VertexDescriptor VertexId);

  EdgeList getOutEdges(VertexPointer CurrentVertex);

  EdgeList getInEdges(VertexDescriptor VertexId);

  EdgeList getInEdges(VertexPointer CurrentVertex);

  /// OutNeighbors and InNeighbors
  VertexIDList getOutNeighbors(VertexDescriptor VertexId);

  VertexIDList getOutNeighbors(VertexPointer CurrentVertex);
  
  VertexIDList getInNeighbors(VertexDescriptor VertexId);

  VertexIDList getInNeighbors(VertexPointer CurrentVertex);

  /// Update operations to the graph
  VertexDescriptor addVertex();

  VertexDescriptor addVertex(std::string Label, PropertyListType & InitialPropertyList);

  VertexDescriptor addVertex(PropertyListType & InitialPropertyList);

  void chainEdges(VertexPointer Vertex, EdgePointer FirstNextEdge, 
                  EdgePointer NewEdge);

  void assignPointers(VertexDescriptor vs, VertexDescriptor vd, 
                      EdgePointer NewEdge);

  EdgeDescriptor addEdge(VertexDescriptor StartVertex, 
                         VertexDescriptor EndVertex);

  EdgeDescriptor addEdge(VertexDescriptor StartVertex, 
                         VertexDescriptor EndVertex, 
                         const std::string & Label);

  EdgeDescriptor addEdge(VertexDescriptor StartVertex, 
                         VertexDescriptor EndVertex, 
                         PropertyListType & InitialPropertyList);

  EdgeDescriptor addEdge(VertexDescriptor StartVertex, 
                         VertexDescriptor EndVertex, 
                         const std::string & Label, 
                         PropertyListType & InitialPropertyList);

  /// Constructor and destructor

  GraphType();

  ~GraphType();

  /// Print graph
  
#ifdef _DEBUG_
  void dump();
#endif
  
  
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

