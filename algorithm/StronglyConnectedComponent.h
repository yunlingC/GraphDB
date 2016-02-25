//===-- algorithm/StronglyConnectedComponent.h - conected graph -*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is for detecting strongly connected componets.
///
//===----------------------------------------------------------------------===//
#ifndef _STRONGLYCONNECTEDCOMPONENT_H_
#define _STRONGLYCONNECTEDCOMPONENT_H_

#include "GraphType.h"

#include <vector>
#include <iostream>

#define UNDEFINED -1 

namespace algorithm {

  typedef std::vector<unsigned int>  PathType;
  typedef std::vector<PathType> SubgraphType;
  
  /// Tarjan's SCC is a classic application of DFS(recursive DFS)
  void StrongConnect(GraphType & Graph
                    , unsigned int Vertex
                    , unsigned int Index[] 
                    , unsigned int LowLink[]
                    , std::vector<unsigned int> *Stack
                    , bool InStack[]
                    , SubgraphType & Components
      );
  
  void StronglyConnectedComponent(GraphType & Graph
                          , SubgraphType & Components 
                          ) {
  
    auto Vertices = Graph.getAllVertices();
    unsigned int NumberOfVertices = Vertices.size();
    unsigned int *Index = new unsigned int[NumberOfVertices];
    unsigned int *LowLink = new unsigned int[NumberOfVertices];
    bool *InStack = new bool[NumberOfVertices];
    std::vector<unsigned int> *Stack = new std::vector<unsigned int>(NumberOfVertices);
    
    /// Initialize vertex's info
    for (auto & VertexPtr : Vertices) {
      unsigned int Vertex = VertexPtr->getId();
      Index[Vertex] = UNDEFINED;
      LowLink[Vertex] = UNDEFINED;
      InStack[Vertex] = false;  }
  
    /// Call the recursive StrongConnect function to find strongly 
    /// connected componets in DFS tree with vertex Vertex
    for (auto & VertexPtr : Vertices) {
      unsigned int Vertex = VertexPtr->getId();
      unsigned int UNDEF = UNDEFINED;
      if (Index[Vertex] == UNDEF) {
        StrongConnect(Graph, Vertex, Index, LowLink, Stack, InStack, Components);
      }
    }
  }
  
  void StrongConnect(GraphType & Graph
                    /// Vertex is the Id in graph for each vertex, immutable
                    , unsigned int Vertex
                    , unsigned int Index[] 
                    , unsigned int LowLink[]
                    , std::vector<unsigned int> *Stack
                    , bool InStack[]
                    , SubgraphType & Components
                    ) {
    typedef std::vector<unsigned int> ComponentType;
  
    /// Id is the time id to record the order each vertex is visited
    static unsigned int Id = 0;
  
    /// Initialize the order (Id) the vertex is visited and lowLink value
    Id++;
    Index[Vertex] = Id;
    LowLink[Vertex] = Id;
    Stack->push_back(Vertex);
    InStack[Vertex] = true;
  
    /// Iterate through all adjacent vertice
    auto Neighbors = Graph.getOutNeighbors(Vertex);
    /// If the graph is undirected, need inNeighbors too
    //  auto InNeighbors = Graph.getInNeighbors(Vertex);
    //  Neighbors.insert(Neighbors.end(), InNeighbors.begin(), InNeighbors.end());
    for (auto & Neighbor : Neighbors) {
      /// Neighbor has NOT been visited, recur on it
      unsigned int UNDEF = UNDEFINED;
      if (Index[Neighbor] == UNDEF) {
        StrongConnect(Graph, Neighbor, Index, LowLink, Stack, InStack, Components); 
  
        /// Check if the subtree rooted with 'Neighbor' has 
        /// a connection to one of the ancestros of Vertex
        /// Case 1 tree edge
        LowLink[Vertex] = LowLink[Vertex] > LowLink[Neighbor] ? LowLink[Neighbor] : LowLink[Vertex]; 
      }
  
      /// Update LowLink of Vertex only if Neighbor is still in stack
      /// Case 2 back edge
      else if (InStack[Neighbor] == true) {
        LowLink[Vertex] = LowLink[Vertex] > Index[Neighbor] ? Index[Neighbor] : LowLink[Vertex];
      }
    }
  
    /// Found head node in the subgraph, pop out and record
    if (LowLink[Vertex] == Index[Vertex]) {
      auto Parent = 0;
      ComponentType Path;
      while (Stack->back() != Vertex){
        Parent = Stack->back(); 
        InStack[Parent] = false;
        Stack->pop_back();
        Path.push_back(Parent);
      }
  
      Parent = Stack->back(); 
      InStack[Parent] = false;
      Stack->pop_back();
      Path.push_back(Parent);
  
      Components.push_back(Path);
    }
  };
}

#endif 

