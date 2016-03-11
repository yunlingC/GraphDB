//===-- pybind/GraphPrototype.h - Graph class type ------------------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.  //
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the class of prototypes to integrate Gremlin interfaces.
///
//===----------------------------------------------------------------------===//
#ifndef _GRAPH_PROTOTYPE_H_ 
#define _GRAPH_PROTOTYPE_H_ 

#include "GraphType.h"

namespace GraphPrototypes {

template<typename ReturnType>
ReturnType addVertex();

template<typename VertexType, typename ReturnType>
ReturnType addVertex(VertexType Vertex);

template<typename Pipe>
Pipe out(std::string label);

};

#endif /* _GRAPH_RPOTOTYPE_H */
