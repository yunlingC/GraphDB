#include "Vertex.h"
#include "Edge.h"
#include "GraphType.h"

void createGraph(GraphType & g) {
  typedef GraphType Graph;

  std::cout << "start adding vertices\n";
  Graph::VertexDescriptor v0 = g.addVertex();
  Graph::VertexDescriptor v1 = g.addVertex();
  Graph::VertexDescriptor v2 = g.addVertex();
  Graph::VertexDescriptor v3 = g.addVertex();
  Graph::VertexDescriptor v4 = g.addVertex();
  Graph::VertexDescriptor v5 = g.addVertex();
  Graph::VertexDescriptor v6 = g.addVertex();

  g.addEdge(v0, v1);
  g.addEdge(v0, v2);
  g.addEdge(v0, v5);
  g.addEdge(v1, v3);
  g.addEdge(v2, v3);
  g.addEdge(v2, v4);
  g.addEdge(v5, v6);
  g.addEdge(v6, v3);
  g.addEdge(v2, v4);
  g.addEdge(v3, v5);
  g.addEdge(v4, v5);
}
