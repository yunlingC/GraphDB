#include "Node.h"
#include "Edge.h"
#include "BaseGraph.h"

#include <iostream>
#include <queue>
#include <map>
using namespace std;

void bfs(BaseGraph::VertexDescriptor vs, BaseGraph & g) {
  typedef pair<BaseGraph::NodePtr, bool> VisitPair;
  cout << "================= BfS ===================== \n";
  cout << "+ vertex: " << vs << "\n";

  BaseGraph::NodePtr np = g.getNodePointer(vs);

  // Start traversing the graph from here. 
  std::queue<BaseGraph::NodePtr> Q;
  std::map<BaseGraph::NodePtr, bool> C; // true = visited, false = not visited

  Q.push(np);
  C.insert(VisitPair(np,false));

  BaseGraph::NodePtr targetNode = NULL;

  while (!Q.empty()) {
    np = Q.front();  Q.pop();
    cout << "vid: " << np->getId() << "\n";

    // Set to visited.    
    C[np] = true;

    BaseGraph::EdgePtr nextEdge = np->getNextEdge();
    while (nextEdge != NULL) {
      // Get the target
      targetNode = nextEdge->getTarget(np);
      if (C.find(targetNode) == C.end()) {
	// queue the target for visitation
	Q.push(targetNode);
	C.insert(VisitPair(targetNode,false));
      }
      // Update nextEdge from np
      nextEdge = nextEdge->getNextEdge(np);
    }
  }
  cout << "================= END BFS ===================== \n";
};

int main() {

  typedef BaseGraph Graph;

  Graph g;

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
