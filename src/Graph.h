#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <string>

#include <boost/graph/adjacency_list.hpp>

#include "PropertyList.h"

using namespace std;

struct VertexProperties {
public:
  PropertyList<string, string > vpl;
};

struct EdgeProperties {
public:
  PropertyList<string, string > vpl;
};

struct Graph {

public:
  typedef int VertexType;
  typedef int EdgeType;

private:
  typedef boost::adjacency_list<boost::vecS, boost::vecS, 
				boost::undirectedS, 
				VertexProperties, 
				EdgeProperties> GraphType;

public:
  Graph();

  bool insertVertex();
  //  bool removeVertex();

};
#endif /* _GRAPH_H_ */
