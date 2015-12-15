#include "GDBReader.h"
#include "DijkstraShortestPath.h"
#include "algorithm/BreadthFirstSearch.h"
#include "algorithm/DepthFirstSearch.h"
#include "algorithm/RecursiveDepthFirstSearch.h"
#include "StronglyConnectedComponent.h"

#include <iostream>

using namespace std;

void Print_Map(std::vector<unsigned int> Dist) {

  for (unsigned int i = 0; i < Dist.size(); i++) {
    std::cout << "Vertex  " << i ;
    if (Dist[i] == -1)
      std::cout << "\tNIL" << "\n";
    else 
      std::cout << "\t" << Dist[i] << "\n";
  }
}

void Print_Subgraph(algorithm::SubgraphType & Subgraph) {
  for (auto  Path : Subgraph) {
    cout << "Component ";
    for (auto Node : Path) {
      cout << Node << " ";
    }
    cout << "\n";
  }
}

void Print_Pagerank();

int main() {
  typedef GraphType Graph;
  typedef Graph::VertexDescriptor VertexDescriptor;
  typedef std::vector<unsigned int> VertexMapType;
  typedef std::vector<std::vector<VertexDescriptor> > ComponentType;

  Graph g;
  cout << "Begin testing\n";

//  GDBReader reader(g);
//  reader.readFile("../tests/gd/sndata1000.gd");
//
//  cout << "Finish reading \n";

  auto v0 = g.addVertex();
  auto v1 = g.addVertex();
  auto v2 = g.addVertex();
  auto v3 = g.addVertex();
  auto v4 = g.addVertex();
  auto v5 = g.addVertex();

  g.addEdge(v1, v0);
  g.addEdge(v1, v2);
  g.addEdge(v2, v3);
  g.addEdge(v3, v4);
  g.addEdge(v4, v1);
  g.addEdge(v5, v1);
  g.addEdge(v0, v5);

  /// Allocate space for VertexMap here
  /// Otherwise, reallocation make all iterators, 
  /// pointers and references invalidated
 
  cout << "Dijkstra sssp \n";
  VertexMapType DistMap(g.getAllVertices().size());
  algorithm::DijkstraShortestPath(g, 0, DistMap);
  Print_Map(DistMap);

  cout << "BFS \n";
  VertexMapType BFSTree(g.getAllVertices().size());
  algorithm::breadthFirstSearch(g, 0, BFSTree);
  Print_Map(BFSTree);

  cout << "DFS \n";
  VertexMapType DFSTree(g.getAllVertices().size());
  algorithm::depthFirstSearch(g, 0, DFSTree);
  Print_Map(DFSTree);

  cout << "Recursive DFS \n";
  algorithm::recursiveDFS(g, 0, DFSTree);
  Print_Map(DFSTree);

  cout <<"SCC \n";
  algorithm::SubgraphType Subgraph;
  algorithm::StronglyConnectedComponent(g, Subgraph);
  Print_Subgraph(Subgraph);

}
