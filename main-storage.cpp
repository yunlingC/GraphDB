#include "Vertex.h"
#include "Edge.h"
#include "GraphType.h"
#include "FixedString.h"
#include "macros.h"
#include "GDReader.h"

#include <iostream>
#include <queue>

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
	//	cout << "push: " << targetVertex->getId() << "\n";
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

  // typedef PropertyList<string, string> PropertyListType;
  // PropertyListType p;
  // PropertyListType q;

  // p.set("a", "1");
  // p.set("b", "2");
  // p.set("c", "3");

  // q = p;
  // p.remove("b");

  // cout << "Print property list q\n";
  // q.print();
  // cout << "Print property list p\n";
  // p.print();

  typedef GraphType Graph;

  Graph g;
  //  g.allocVertexMemory(50);
  //  g.allocEdgeMemory(100);
  cout << "Begin testing\n";

  GDReader reader(g);
  reader.readFile("../tests/gd/sndata-big.gd");
  //g.print();

  /*

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

  cout << "add edge e1\n";
  Graph::EdgeDescriptor e1 = g.addEdge(v0, v2);
  Graph::EdgeDescriptor e2 = g.addEdge(v0, v4);

  Graph::EdgeDescriptor e3 = g.addEdge(v0, v6);
  Graph::EdgeDescriptor e4 = g.addEdge(v1, v3);
  Graph::EdgeDescriptor e5 = g.addEdge(v2, v3);
  Graph::EdgeDescriptor e6 = g.addEdge(v3, v5);
  Graph::EdgeDescriptor e7 = g.addEdge(v4, v5);
  */
  cout << "Begin updating edges\n";
  g.dump();
  

  Graph::VertexPtr vp0 = g.getVertexPointer(0);
  // Let sniper know fo source node.
  MAGIC_SOURCE_NODE(vp0);
  MAGIC_PREFETCH_TRIGGER;
  //bfs(vp0->getId(), g);

  // FixedString fs("hello");
  // FixedString ft = fs;
  
  // cout << "s: " << fs.std_str() << endl;
  // cout << "s: " << ft.getString() << endl;
  
  // ft.setString("World");
  // cout << "s: " << ft.getString() << endl;
  // if (ft == fs)
  //   cout << "Yes!" << endl;

  // fs += ft;
  // cout << "s: " << fs.getString() << endl;

  // fs = fs + ft;
  // cout << "s: " << fs.getString() << endl;

  // Linked list test
  // LinkedList ll;

  // ll.insert("1", "abc");
  // ll.insert("2", "def");
  // ll.insert("3", "ghi");
  // ListNode * l = ll.find("2");
  // cout << "l: " << l->getValue() << endl;
  // l = ll.find("3");
  // cout << "l: " << l->getValue() << endl;
  // ll.print();
  return 0;
}
