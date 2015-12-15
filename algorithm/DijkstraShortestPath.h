//===-- algorithm/DijkstraShortestPath.h - SSSP --------------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is a shortest path algorithm 
///
//===----------------------------------------------------------------------===//
#ifndef _DIJKSTRASHORTESTPATH_
#define _DIJKSTRASHORTESTPATH_

#include "GraphType.h"

#include <queue>
#include <unordered_map>
#include <iostream>

/// INF largest distance from source vertex
/// Realistic for social network
/// Use with caution
#define INF -1 
#define EdgeWeight 1

namespace algorithm {

  class CompareDistance {
  public:
    typedef std::pair<unsigned int, GraphType::VertexDescriptor> DistPair;
  public:
    bool operator()(DistPair LeftPair, DistPair RightPair) {
      return LeftPair.second > RightPair.second;
    }
  };
  /// For non-negative weights only
  void DijkstraShortestPath(GraphType & Graph
                            , const GraphType::VertexDescriptor & Source 
                            , std::vector<unsigned int> & DistMap
                            ) {
    
    typedef GraphType::VertexDescriptor VertexDescriptor;
    typedef CompareDistance::DistPair DistPair;
  
    std::priority_queue<DistPair, std::vector<DistPair>, CompareDistance> DistQueue;
    std::unordered_map<VertexDescriptor, bool> VisitMap;
    
  //  DistMap.reserve(Graph.getAllVertices().size());
    /// Initialize DistMap so that the default value is INF
    for (unsigned int i = 0; i < DistMap.size(); i++) {
      DistMap[i] = INF;
    }
  
    /// Start from the source
    DistQueue.push({Source, 0});
    VisitMap.insert({Source, true});
    DistMap[Source] = 0;
    
    while (!DistQueue.empty()) {
      auto VertexDist = DistQueue.top();
      auto Vertex = VertexDist.first;
      auto Dist = VertexDist.second;
      DistQueue.pop();
  
  //    std::cout << "Vertex " << Vertex << "\tDist " << Dist << "\n";
      /// Get neighbor list
      /// Options here: directed or undirected(bidirected)
  
      auto OutNeighbors = Graph.getOutNeighbors(Vertex); 
      auto InNeighbors = Graph.getInNeighbors(Vertex);
      OutNeighbors.insert(OutNeighbors.end(), InNeighbors.begin(), InNeighbors.end());
  
      for (auto & Neighbor : OutNeighbors) {
        auto NewDist = EdgeWeight + Dist;
  
          /// Neighbor has never been visited before
        if (VisitMap.find(Neighbor) == VisitMap.end()) {
          DistQueue.push({Neighbor, NewDist});
          DistMap[Neighbor] = NewDist;
          VisitMap.insert({Neighbor, true});
        } else {
          if (NewDist < DistMap[Neighbor]) {
            // TODO support updatable priority queue
  //          DistQueue.DecreaseKey({Neighbor, NewDist});
            DistMap[Neighbor] = NewDist;
          }
        }
      }
    }//End While
  };
}
#endif
