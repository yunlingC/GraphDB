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

  // Iterator for GraphType.
  typedef std::pair<Graph::GraphType::vertex_iterator, 
		    Graph::GraphType::vertex_iterator> VertexRange;
  
public:
  GraphType::vertex_descriptor insertVertex() {
    GraphType::vertex_descriptor vd = add_vertex(_g);
    return vd;
    //    _g[vd].list.insert("a", "B");
  }

  GraphType::vertex_descriptor  insertVertex(VertexPropertyList vlist) {
    GraphType::vertex_descriptor vd = add_vertex(_g);
    _g[vd].list = vlist;
    return vd;
  }

  void removeVertex(GraphType::vertex_descriptor v) {
    remove_vertex(v, _g);
  }

  void clearVertex(GraphType::vertex_descriptor v) {
    clear_vertex(v, _g);
  }

  void insertEdge(GraphType::vertex_descriptor vs, GraphType::vertex_descriptor vd) {
    add_edge(vs, vd, _g);
  }

  void insertEdge(GraphType::vertex_descriptor vs, GraphType::vertex_descriptor vd, 
		  EdgePropertyList elist) {
    pair<GraphType::edge_descriptor,bool> ed = add_edge(vs, vd, _g);
    _g[ed.first].list = elist;
  }

  void removeEdge(GraphType::vertex_descriptor vs, GraphType::vertex_descriptor vd) {
    remove_edge(vs, vd, _g);
  }

  const VertexRange getVertexRange() {
    return boost::vertices(_g);
  }

  Graph::VertexProp & operator[](std::size_t i) {
    return _g[i];
  }

  void print() {

    // Output the properties of each vertex
    std::pair<Graph::GraphType::vertex_iterator, Graph::GraphType::vertex_iterator> vertexIteratorRange = boost::vertices(_g);

    for(Graph::GraphType::vertex_iterator vertexIterator = vertexIteratorRange.first; 
	vertexIterator != vertexIteratorRange.second; ++vertexIterator)      {
      GraphType::vertex_descriptor v = *vertexIterator;
      cout << v << endl;
    }
    // Output using edge properties.
    
    cout << "=========================== Graph print of edges ================================== \n";
    std::pair<Graph::GraphType::edge_iterator, Graph::GraphType::edge_iterator> edgeIteratorRange = boost::edges(_g);
    for(Graph::GraphType::edge_iterator edgeIterator = edgeIteratorRange.first; edgeIterator != edgeIteratorRange.second; ++edgeIterator)
    {
      cout << "= edge id: " << *edgeIterator << endl;// ", source: " << source(*edgeIterator, _g) << ", target: " << target(*edgeIterator, _g) << endl;
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
