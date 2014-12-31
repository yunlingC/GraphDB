#include "PropertyList.h"
#include "Graph.h"
#include "Vertex.h"

#include <string>

int main(int argc, char * argv[]) {
  
  PropertyList<string,string> p;

  p.insert("a", "1");
  p.insert("b", "2");
  p.insert("c", "3");
  
  p.remove("b");

  p.print();


  PropertyList<string,string>::mitType mi = p.get("c");
  cout << "first: " << mi->first << ", second: " << mi->second << endl;

  /* Start to test the graph class */
  
  cout << "+ Start testing graph" << endl;
  Graph g;

  Graph::GraphType::vertex_descriptor v1 = g.insertVertex(p);
  Graph::GraphType::vertex_descriptor v2 = g.insertVertex();
  Graph::GraphType::vertex_descriptor v3 = g.insertVertex();
  g.insertEdge(v1, v2);
  g.insertEdge(v1, v3);
  g.print();

  cout << "+ Vertex range test" << endl;
  Graph::VertexRange vr = g.getVertexRange();
  
  for(Graph::GraphType::vertex_iterator vertexIterator = vr.first; 
      vertexIterator != vr.second; ++vertexIterator)      {
    Graph::GraphType::vertex_descriptor v = *vertexIterator;
    
    cout << v << ", " << endl;
    g[v].list.print();
  }

  return 0;
}
