#ifndef _BREADTH_FIRST_SEARCH_H_
#define _BREADTH_FIRST_SEARCH_H_

#include <queue>
#include <map>


#include "GraphType.h"

using namespace std;

void breadth_first_search(GraphType::VertexDescriptor vs, GraphType & g) {
  typedef pair<GraphType::VertexPtr, bool> VisitPair;
  cout << "================= BfS ===================== \n";
  cout << "+ vertex: " << vs << "\n";

  GraphType::VertexPtr np = g.getVertexPointer(vs);

  // Start traversing the graph from here. 
  std::queue<GraphType::VertexPtr> Q;
  std::map<GraphType::VertexPtr, bool> C; // true = visited, false = not visited

  Q.push(np);
  C.insert(VisitPair(np,false));

  GraphType::VertexPtr targetVertex = nullptr;

  while (!Q.empty()) {
    np = Q.front();  Q.pop();
    cout << "vid: " << np->getId() << "\n";

    // Set to visited.    
    C[np] = true;

    GraphType::EdgePtr nextEdge = np->getNextEdge();
    while (nextEdge != nullptr) {
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



#endif /* _BREADTH_FIRST_SEARCH_H_ */
