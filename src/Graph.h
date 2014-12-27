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

public:
  void insertVertex() {
    add_vertex(_g);
  }

  void insertEdge(GraphType::vertex_descriptor vs, GraphType::vertex_descriptor vd) {
    add_edge(vs, vd, _g);
  }

  void print() {

    // typedef graph_traits<Graph>::vertex_iterator vertex_iter;
    // std::pair<vertex_iter, vertex_iter> vp;

    // Output the properties of each vertex
    std::pair<Graph::GraphType::vertex_iterator, Graph::GraphType::vertex_iterator> vertexIteratorRange = boost::vertices(_g);

    for(Graph::GraphType::vertex_iterator vertexIterator = vertexIteratorRange.first; vertexIterator != vertexIteratorRange.second; ++vertexIterator)
      {
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
