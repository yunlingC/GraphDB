#include "Node.h"
#include "Edge.h"
#include "BaseGraph.h"

#include <iostream>
using namespace std;

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
  //  Graph::EdgeDescriptor e1 = g.addEdge(v1, v2, "1-2", q);


  return 0;
}
