#include "iostream"
#include <algorithm>
#include<vector>
#include "GraphType.h"
#include "Vertex.h"
#include "BreadthFirstSearch.h"


#include<boost/python.hpp>
#include<boost/python/suite/indexing/vector_indexing_suite.hpp>


using namespace boost::python;

BOOST_PYTHON_MODULE(graph_python)
{
  //class graphType and the required member functions within the class
  class_<GraphType>("GraphType",init<>())
    .def("getAllVertices", &GraphType::getAllVertices)
    .def("addVertex",(GraphType::VertexDescriptor (GraphType::*)())&GraphType::addVertex)
    .def("addEdge",(GraphType::EdgeDescriptor (GraphType::*)(GraphType::VertexDescriptor, GraphType::VertexDescriptor))&GraphType::addEdge)
    .def("getVertexPointer",&GraphType::getVertexPointer,return_value_policy<return_by_value>())
    ;
  //getAllVertices returns a vector of vertexpointers; so the vector needs to be defined for python
  class_<std::vector<GraphType::VertexPointer> >("stl_vector_vertexpointer")
    .def(vector_indexing_suite<std::vector<GraphType::VertexPointer> >())
    ;
  //vector of vertex descriptor is needed to be given as an argument for breadthFirstSearch function
  class_<std::vector<GraphType::VertexDescriptor> >("vector_uint")
    .def(vector_indexing_suite<std::vector<GraphType::VertexDescriptor> >())
    ;
  //VertexPointer is Vertex*; so class Vertex also needs to be linked
  class_<Vertex,Vertex*>("Vertex")
    ;
  
  def("breadthFirstSearch",algorithm::breadthFirstSearch);
 
};
