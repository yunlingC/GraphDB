#include <new>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <boost/graph/breadth_first_search.hpp>

#include "PropertyList.h"
#include "Graph.h"

#include "readers/GraphReaderJSON.h"
#include "Utility.h"

#include <string>

void printmem (char* p) {
  char* q = p;
  cout << "Trying to print the contents of memory" << endl;
  cout << "\n===============================================" << endl;
  for (int i = 0; i < sizeof(char)*200/(sizeof(int)); i++) {
    int fourbyte = (*q);
    cout << hex << setfill('0')<< fourbyte << " ";
    q = q++;
  }
  cout << "\n===============================================" << endl;
}

class custom_bfs_visitor : public boost::default_bfs_visitor
{
public:
     
  template < typename Vertex, typename Gr >
  void discover_vertex(Vertex u, Gr & g) 
  {
    std::cout << u << std::endl;
    VertexProperty<string,string> v = g[u];
    //VertexProperty<string,string>::ReturnPropertyValueType vt = v.getPropertyValue("a");
    //cout << ", " << vt.first << endl;
    VertexProperty<string,string>::PropertyListType & p = v.getPropertyList();
    p.print();
  }
};
 
int main(int argc, char * argv[]) {

  char memory[sizeof(char)*400];
  void* place = memory;
  for (int i = 0; i < (sizeof(char)*400); i++) {
    memory[i] = 'x';
  }

  cout << "Initialized memory\n";
  cout << "\n===============================================" << endl;
  for (int i = 0; i < (sizeof(char)*200); i++) {
    cout << hex << setfill('0') << memory[i] << " " ;
  }
  cout << "\n===============================================" << endl;

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
  
  cout << "value for 'c': " << p.get("c").first << endl;

  /* Start to test the graph class */
  
  cout << "+ Start testing graph" << endl;
  //  Graph g;

  // Create a graph object
  Graph* gr = new(place) Graph();
  Graph & g = *gr;



  Graph::VertexDescriptor v1 = g.addVertex(p);
  Graph::VertexDescriptor v2 = g.addVertex(p);
  Graph::VertexDescriptor v3 = g.addVertex(p);
  Graph::VertexDescriptor v4 = g.addVertex();
  Graph::VertexDescriptor v5 = g.addVertex();
  Graph::EdgeDescriptor e1 = g.addEdge(v1, v2, "1-2", q);
  g.addEdge(v2, v1, "2-1", q);
  g.addEdge(v1, v3); // 1-3
  g.addEdge(v3, v2, "3-2", p); // 3-2
  g.addEdge(v2, v4, "2-4", p); // 3-2
  g.addEdge(v1, v5, "1-5", p); // 3-2

  // Set individual properties.
  g.setProperty(e1, "name", "edge1");
  g.setProperty(v2, "z", "lkjadsf");
  cout << "edge: prop: " << g.getProperty(e1, "name").first << endl;
  cout << "vertex: prop: " << g.getProperty(v2, "z").first << endl;
  // Failed error checking.
  if (g.getProperty(v2, "66").second == true) {
    cout << "property exists in vertex " << endl;
   }
  else {
     cout << "property does not exist in vertex " << endl;
  }

  // Failed error checking.
  if (g.getProperty(e1, "name2").second == true) {
    cout << "property exists in edge " << endl;
  }
  else {
    cout << "property does not exist in edge " << endl;
  }

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


  /* PLEASE ignore this code.  It's the basic to allow for JSON parsing */
  // TODO: Still need to implement the parsing method.
  //GraphReaderJSON json("t.json");
  //json.print();

  cout << "\n= ==========================================\n";
  custom_bfs_visitor vis;
  breadth_first_search(g._g, v1, visitor(vis));
 
  // Call destructor
  // Must do this manually as the place is used.
  gr->~Graph();

  return 0;
}
