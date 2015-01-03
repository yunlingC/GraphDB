#include "PropertyList.h"
#include "Graph.h"

#include "Utility.h"

#include <string>

int main(int argc, char * argv[]) {

  typedef PropertyList<string, string> PropertyListType;
  PropertyListType p;
  PropertyListType q;

  p.set("a", "1");
  p.set("b", "2");
  p.set("c", "3");

  q = p;
  p.remove("b");

  q.print();
  p.print();
  
  cout << "value for 'c': " << p.get("c") << endl;

  /* Start to test the graph class */
  
  cout << "+ Start testing graph" << endl;
  Graph g;

  Graph::VertexDescriptor v1 = g.addVertex(p);
  Graph::VertexDescriptor v2 = g.addVertex();
  Graph::VertexDescriptor v3 = g.addVertex();
  Graph::EdgeDescriptor e1 = g.addEdge(v1, v2, "1-2", q);
  g.addEdge(v2, v1, "2-1", q);
  g.addEdge(v1, v3); // 1-3
  g.addEdge(v3, v2, "3-2", p); // 3-2

  // Set individual properties.
  g[e1].setProperty("name", "edge1");
  g[v2].setProperty("z", "lkjadsf");
  g.print();

  // Check for interface methods from blueprint

  cout << "\n= getEdges() test" << endl;
  Graph::EdgeIteratorPair ei = g.getEdges();
  print(ei.first, ei.second);

  cout << "\n= getOutEdges() test" << endl;
  Graph::OutEdgeIteratorPair oei = g.getOutEdges(v1);
  print(oei.first, oei.second);

  cout << "\n= getInEdges() test" << endl;
  Graph::InEdgeIteratorPair iei = g.getInEdges(v2);
  print(iei.first, iei.second);

  cout << "\n= getVertices() test" << endl;
  Graph::VertexIteratorPair vr = g.getVertices();
  print(vr.first, vr.second);

  return 0;
}
