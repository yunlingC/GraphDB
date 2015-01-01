#include "PropertyList.h"
#include "Graph.h"

#include "Utility.h"

#include <string>

int main(int argc, char * argv[]) {

  typedef PropertyList<string, string> PropertyListType;
  PropertyListType p;
  PropertyListType q;

  p.insert("a", "1");
  p.insert("b", "2");
  p.insert("c", "3");

  q = p;
  p.remove("b");

  q.print();
  p.print();
  
  PropertyListType::mitType mi = p.get("c");
  cout << "first: " << mi->first << ", second: " << mi->second << endl;

  /* Start to test the graph class */
  
  cout << "+ Start testing graph" << endl;
  Graph g;

  Graph::VertexDescriptor v1 = g.insertVertex(p);
  Graph::VertexDescriptor v2 = g.insertVertex();
  Graph::VertexDescriptor v3 = g.insertVertex();
  g.insertEdge(v1, v2, "1-2", q);
  g.insertEdge(v2, v1, "2-1", q);
  g.insertEdge(v1, v3); // 1-3
  g.insertEdge(v3, v2, "3-2", p); // 3-2
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
