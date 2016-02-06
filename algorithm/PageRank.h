//===-- algorithm/PageRank.h - PageRank algorithm ---------------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is a PageRank implementation.
/// 
//===----------------------------------------------------------------------===// 

#ifndef _PAGE_RANK_H_
#define _PAGE_RANK_H_

#include "GraphType.h"

#include <unordered_map>

#define UNDEFINE -1

namespace algorithm {

  /// Recursive DFS with stack
  void PageRank(GraphType & Graph
                , const double & DampingFactor
                , const unsigned int & MaxIteration
                , const double & Convergence
                , std::unordered_map<unsigned int, double> & PageRankTable
                ) {

    /// Declare and initialize all the varibles needed for PR
    double Diff = 1.0;
    double SumPR;
    double DanglingPR;
    const double Alpha = 1 - DampingFactor;
    unsigned int NumIteration = 0;
    unsigned int NumberOfVertices = Graph.getAllVertices().size();
    std::unordered_map<unsigned int, double> OldPR(NumberOfVertices);
    std::vector<GraphType::VertexIDList> InNeighborsList(NumberOfVertices);

    if (NumberOfVertices == 0)
      return;

    /// Build outdegree array for graph
    std::vector<unsigned int> OutDegree(NumberOfVertices);
    for (unsigned int Vertex = 0; Vertex < NumberOfVertices; 
        Vertex++) {
      InNeighborsList[Vertex] = Graph.getInNeighbors(Vertex);
      OutDegree[Vertex] = Graph.getOutNeighbors(Vertex).size();  
    }

    PageRankTable.insert({0, 1});

    /// iterative calculating pagerank value for each node
    while (Diff > Convergence && NumIteration < MaxIteration) {

      SumPR = 0;
      DanglingPR = 0;

      for (unsigned int Index = 0; Index < NumberOfVertices; Index++) {
        double CPR = PageRankTable[Index];
        SumPR += CPR;
        if (OutDegree[Index] == 0){
          DanglingPR += CPR;
        }
      } //END_FOR

      if (NumIteration == 0) {
          OldPR = PageRankTable;
      } else {
        /// Normalize so that we start with sum equal to one
        for (unsigned int Index = 0; Index < NumberOfVertices; Index++) {
          OldPR[Index] = PageRankTable[Index]/SumPR;
        }
      }

      /// After normalization, PR value for vertex sums to one
      SumPR = 1;
      
      double GMatrix = (1 - Alpha) * SumPR / NumberOfVertices;
      double DMatrix = Alpha * DanglingPR / NumberOfVertices;
      
      /// Now calculate the diff and decide whetherto continue 
      Diff = 0;
      for (unsigned int Index = 0; Index < NumberOfVertices; Index++) {

        /// H is the coresponding value in H multiplication
        double PR = 0.0;
        for (unsigned int & Neighbor : InNeighborsList[Index] ) {
          double WeightedH = (OutDegree[Neighbor] == 0) 
                              ? 0.0 : 1.0/OutDegree[Neighbor];
          PR += WeightedH* OldPR[Index];
        }

        PR *= Alpha;
        PageRankTable[Index] = PR + GMatrix + DMatrix;
        Diff += fabs(PageRankTable[Index] - OldPR[Index]);
      }

    NumIteration++;

    } //END_WEHILE
  };
}

#endif /* _PAGE_RANK_H_ */
