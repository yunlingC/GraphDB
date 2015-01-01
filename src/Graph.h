#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <string>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>

#include "PropertyList.h"
#include "VertexProperty.h"
#include "EdgeProperty.h"

using namespace std;
using namespace boost;

// The interface we should follow is that defined by Blueprints (at least some standard)
// https://github.com/tinkerpop/blueprints/wiki

struct Graph {

public:
  
  // Vertex property types.
  typedef string VertexPropertyKeyType;
  typedef string VertexPropertyValueType;

  // Edge property types.
  typedef string EdgePropertyKeyType;
  typedef string EdgePropertyValueType;

  // Define the VertexProperty using the KeyType and ValueType.
  typedef VertexProperty<VertexPropertyKeyType, VertexPropertyValueType> VertexProp;
  // Define the VertexProperty using the KeyType and ValueType.
  typedef EdgeProperty<EdgePropertyKeyType, EdgePropertyValueType> EdgeProp;

  // Type for vertex and edge property list.
  typedef PropertyList<VertexPropertyKeyType, VertexPropertyValueType> VertexPropertyList;
  typedef PropertyList<EdgePropertyKeyType, EdgePropertyValueType> EdgePropertyList;

  // GraphType declaration
  typedef adjacency_list<vecS, vecS, directedS, 
			 VertexProp, EdgeProp> GraphType;

  // Vertex descriptors.
  typedef GraphType::vertex_descriptor VertexDescriptor;
  typedef GraphType::edge_descriptor EdgeDescriptor;

  // Iterators used for GraphType for convenience.
  typedef Graph::GraphType::vertex_iterator VertexIterator;
  typedef Graph::GraphType::edge_iterator EdgeIterator;

  typedef std::pair<Graph::GraphType::vertex_iterator, 
		    Graph::GraphType::vertex_iterator> VertexIteratorPair;

  typedef std::pair<Graph::GraphType::edge_iterator, 
		    Graph::GraphType::edge_iterator> EdgeIteratorPair;

  typedef std::pair<Graph::GraphType::out_edge_iterator, 
		    Graph::GraphType::out_edge_iterator> OutEdgeIteratorPair;
  
  
public:
  VertexDescriptor insertVertex() {
    VertexDescriptor vd = add_vertex(_g);
    return vd;
  }

  VertexDescriptor insertVertex(VertexPropertyList vlist) {
    VertexDescriptor vd = add_vertex(_g);
    _g[vd].list = vlist;
    return vd;
  }


  void removeVertex(VertexDescriptor v) {
    remove_vertex(v, _g);
  }

  void clearVertex(VertexDescriptor v) {
    clear_vertex(v, _g);
  }

  pair<EdgeDescriptor, bool> insertEdge(VertexDescriptor vs, VertexDescriptor vd) {
    pair<EdgeDescriptor, bool> ed = add_edge(vs, vd, _g);
    _g[ed.first].type = "default";
    return ed;
  }

  pair<EdgeDescriptor, bool> insertEdge(VertexDescriptor vs, VertexDescriptor vd, 
						   const string & type, EdgePropertyList elist) {
    pair<EdgeDescriptor, bool> ed = add_edge(vs, vd, _g);
    _g[ed.first].list = elist;
    _g[ed.first].type = type;
    
    return ed;
  }

  void removeEdge(VertexDescriptor vs, VertexDescriptor vd) {
    remove_edge(vs, vd, _g);
  }

  const VertexIteratorPair getVertexRange() {
    return boost::vertices(_g);
  }

  VertexProp & operator[](std::size_t i) {
    return _g[i];
  }

  OutEdgeIteratorPair outEdges(VertexDescriptor vd) {
    return out_edges(vd, _g);
  }

  void print() {

    // Output the properties of each vertex
    VertexIteratorPair vertexIteratorRange = vertices(_g);

    for(VertexIterator vertexIterator = vertexIteratorRange.first; 
	vertexIterator != vertexIteratorRange.second; ++vertexIterator)      {
      VertexDescriptor v = *vertexIterator;
      cout << v << endl;
    }
    // Output using edge properties.
    
    cout << "=========================== Graph print of edges ================================== \n";
    EdgeIteratorPair edgeIteratorRange = edges(_g);
    for(EdgeIterator edgeIterator = edgeIteratorRange.first; edgeIterator != edgeIteratorRange.second; ++edgeIterator)
    {
      cout << "= edge id: " << *edgeIterator << ", type: " << _g[*edgeIterator].type << endl;// ", source: " << source(*edgeIterator, _g) << ", target: " << target(*edgeIterator, _g) << endl;
      cout << "- Property list for edge" << endl;
      _g[*edgeIterator].list.print();

      cout << "- Property list for source vertex: " << source(*edgeIterator, _g) << endl;
      _g[source(*edgeIterator, _g)].list.print();
      cout << "- Property list for target vertex: " << target(*edgeIterator, _g) << endl;
      _g[source(*edgeIterator, _g)].list.print();

    }
  }

  Graph() {
  }

private:
  Graph::GraphType _g;

};
#endif /* _GRAPH_H_ */
