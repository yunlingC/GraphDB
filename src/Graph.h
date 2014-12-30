#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <string>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>

#include "PropertyList.h"

using namespace std;
using namespace boost;

struct VertexProperties {
public:
  PropertyList<string, string > vpl;
};

struct EdgeProperties {
public:
  PropertyList<string, string > epl;
};

struct Graph {

public:
  typedef int VertexType;
  typedef int EdgeType;
  typedef VertexProperties VertexProp;
  typedef EdgeProperties EdgeProp;
  typedef adjacency_list<vecS, vecS, undirectedS, 
			 VertexProp, EdgeProp> GraphType;
  typedef std::pair<Graph::GraphType::vertex_iterator, Graph::GraphType::vertex_iterator> VertexRange;
  
public:
  void insertVertex() {
    add_vertex(_g);
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

  void removeEdge(GraphType::vertex_descriptor vs, GraphType::vertex_descriptor vd) {
    remove_edge(vs, vd, _g);
  }

  VertexProp const & getVertexProperty(GraphType::vertex_descriptor v) {
    VertexProp const & vp =_g[v];
    return vp;
  }

  const VertexRange getVertexRange() {
    return boost::vertices(_g);
  }

  void print() {

    // Output the properties of each vertex
    std::pair<Graph::GraphType::vertex_iterator, Graph::GraphType::vertex_iterator> vertexIteratorRange = boost::vertices(_g);

    for(Graph::GraphType::vertex_iterator vertexIterator = vertexIteratorRange.first; 
	vertexIterator != vertexIteratorRange.second; ++vertexIterator)      {
      GraphType::vertex_descriptor v = *vertexIterator;
      cout << v << endl;
    }
  }

  Graph() {
  }

private:
  Graph::GraphType _g;
  

};
#endif /* _GRAPH_H_ */
