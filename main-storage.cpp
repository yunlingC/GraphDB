#include "Vertex.h"
#include "Edge.h"
#include "GraphType.h"

#include <iostream>
#include <queue>
#include <map>
using namespace std;

void bfs(GraphType::VertexDescriptor vs, GraphType & g) {
  typedef pair<GraphType::VertexPtr, bool> VisitPair;
  cout << "================= BfS ===================== \n";
  cout << "+ vertex: " << vs << "\n";

  GraphType::VertexPtr np = g.getVertexPointer(vs);

  // Start traversing the graph from here. 
  std::queue<GraphType::VertexPtr> Q;
  std::map<GraphType::VertexPtr, bool> C; // true = visited, false = not visited

  Q.push(np);
  C.insert(VisitPair(np,false));

  GraphType::VertexPtr targetVertex = NULL;

  while (!Q.empty()) {
    np = Q.front();  Q.pop();
    cout << "vid: " << np->getId() << "\n";

    // Set to visited.    
    C[np] = true;

    GraphType::EdgePtr nextEdge = np->getNextEdge();
    while (nextEdge != NULL) {
      // Get the target
      targetVertex = nextEdge->getTarget(np);
      if (C.find(targetVertex) == C.end()) {
	// queue the target for visitation
	Q.push(targetVertex);
	C.insert(VisitPair(targetVertex,false));
      }
      // Update nextEdge from np
      nextEdge = nextEdge->getNextEdge(np);
    }
  }
  cout << "================= END BFS ===================== \n";
};

int main() {

  typedef GraphType Graph;

  Graph g;
  g.allocVertexMemory(50);
  g.allocEdgeMemory(100);
  cout << "Begin testing\n";

  Graph::VertexDescriptor v0 = g.addVertex();
  cout << "v0: " << v0 << endl;
  Graph::VertexDescriptor v1 = g.addVertex();
  Graph::VertexDescriptor v2 = g.addVertex();
  Graph::VertexDescriptor v3 = g.addVertex();
  Graph::VertexDescriptor v4 = g.addVertex();
  Graph::VertexDescriptor v5 = g.addVertex();
  Graph::VertexDescriptor v6 = g.addVertex();
  cout << "start adding edges\n";
  Graph::EdgeDescriptor e0 = g.addEdge(v0, v1);
  Graph::EdgeDescriptor e1 = g.addEdge(v0, v2);
  Graph::EdgeDescriptor e2 = g.addEdge(v0, v4);

  Graph::EdgeDescriptor e3 = g.addEdge(v0, v6);
  Graph::EdgeDescriptor e4 = g.addEdge(v1, v3);
  Graph::EdgeDescriptor e5 = g.addEdge(v2, v3);
  Graph::EdgeDescriptor e6 = g.addEdge(v3, v5);
  Graph::EdgeDescriptor e7 = g.addEdge(v4, v5);
  cout << "Begin updating edges\n";
  g.updateEdges();
  g.dump();
  
  bfs(v0, g);


  return 0;
}