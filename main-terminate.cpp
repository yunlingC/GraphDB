#include "Vertex.h"
#include "Edge.h"
#include "GraphType.h"
#include "FixedString.h"
#include "macros.h"
#include "GDReader.h"
#include "LinkedList.h"

#include <iostream>
#include <queue>

#include "breadth_first_search.h_"
/// Test
//#include "tests/gs/g1.h"

class VisitedVertex : public VertexVisitor<VisitedVertex> {
public:
  typedef pair<FixedString, bool> ReturnValueType;
public:
  VisitedVertex() { }
  void setFilter(Filter & f) {
    _f = f;
  }

  bool visitVertex(VertexPointer vp) {
//    cout << "visited vertex: " << vp->getId() << endl;
//    cout << "print property list\n";
//    Vertex::PropertyListType pl = vp->getPropertyList();
//    pl.print();
    FixedString value(_f.getValue());
    FixedString key(_f.getKey());

    ReturnValueType rv = vp->getPropertyValue(key); 
    if((rv.second != false) && (rv.first == value))
    {
      cout << "++++Gotcha++++\n" << "Vertex:\t" << vp->getId() << endl;
      return false;
    }
    return true;
  }


private:
  Filter _f;
};


class VisitedEdge: public EdgeVisitor<VisitedEdge>  {
public:
  VisitedEdge() { } 

  void setFilter(Filter & f) {
    _f = f;
  }

  bool  visitEdge(EdgePointer ep) {
    return true;
  }

  bool  scheduleEdge(EdgePointer ep ) {
    FixedString type(_f.getType());
    if (ep->getType() == type)
    {
      cout << "Edge:\t" << ep->getId() << endl;
      return true;
    }
    else  {
      cout << "type= " << ep->getType() << endl;
      return false;
//      return true;
    }
  }

private:
  Filter _f;
};



int main() {

  typedef GraphType Graph;

  Graph g;
  //  g.allocVertexMemory(50);
  //  g.allocEdgeMemory(100);
  cout << "Begin testing\n";

  GDReader reader(g);
  reader.readFile("../tests/gd/sndata.gd");

  //createGraph(g);

  Filter fv;
  fv.setProperty("pid", "2");
  Filter fe;
  fe.setType("LIKES");


  cout << "Find vertex with key= " << fv.getKey() << "\tvalue= " << fv.getValue() << endl;
  cout << "Find edge with type=" << fe.getKey() << fe.getValue() << fe.getType() <<endl;
  VisitedVertex VV;
  VV.setFilter(fv);

  VisitedEdge   VE;
  VE.setFilter(fe);
  Graph::VertexPtr vp0 = g.getVertexPointer(0);
  // Let sniper know fo source node.
  MAGIC_SOURCE_NODE(vp0);
  MAGIC_PREFETCH_TRIGGER;
  cout << "BFS start\n";
  breadth_first_search(vp0->getId(), g, VV, VE);


  return 0;
}
