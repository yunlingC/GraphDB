#include "Vertex.h"
#include "Edge.h"
#include "GraphType.h"
#include "FixedString.h"
#include "macros.h"
#include "GDReader.h"
#include "LinkedList.h"

#include <iostream>
#include <queue>

#include "breadth_first_search.h"
#include "CustomVisitor.h"


int main() {

  typedef GraphType Graph;

  Graph g;
  //  g.allocVertexMemory(50);
  //  g.allocEdgeMemory(100);
  cout << "Begin testing\n";

  GDReader reader(g);
  reader.readFile("../tests/gd/sndata.gd");

  //createGraph(g);
  Graph::VertexPtr vp0 = g.getVertexPointer(0);
  MAGIC_SOURCE_NODE(vp0);
  MAGIC_PREFETCH_TRIGGER;
  cout << "BFS start\n";

  cout << "Query 1\n";
  TraversalDescription Q1(g);
//  Q1.traverseGraph(g);
  Q1.traverseFromVertex(vp0->getId());
  Q1.filtProperty("name", "KIRA VERLATO");
  Q1.traverseThroughType("FRIENDS");
  Q1.terminateAtTargetNumber(1);
  Q1.breadth_first_search();
  Q1.dumpVertexTarget();
  Q1.dumpDepthList();

 
  cout << "===============================\n";
  cout << "Query 2\n";
  TraversalDescription Q2(g);
  Q2.traverseFromVertex(vp0->getId());
  Q2.filtProperty("wpurl", "http://www.uwaterloo.ca/webpage15.html");
//  Q2.filtProperty("wpid", "15");
//  Q2.traverseThroughType("LIKES");
//  Q2.filtProperty("name", "KIRA VERLATO");
//  Q2.traverseThroughType("FRIENDS");
  Q2.terminateAtTargetNumber(1);
  Q2.breadth_first_search();
//  Q2.startRecording();
  Q2.dumpVertexTarget();
  
  auto targets2 = Q2.getVertexTargetSet();
 
  for(auto it = targets2.begin(); it != targets2.end(); ++it) {
    TraversalDescription q(g);
    cout << "start from vertex: " << (*it)->getId() << endl;
    q.traverseFromVertex((*it)->getId());
    q.traverseThroughType("LIKES");
    q.filtType("LIKES");
//    q.traverseThroughDirection("in");
    q.terminateAtDepth(1);
    q.breadth_first_search();
    q.dumpVertexTarget();
  }


  cout << "===============================\n";
  cout << "Query 3\n";
  TraversalDescription Q3(g);
  Q3.traverseFromVertex(vp0->getId());
  Q3.filtProperty("name", "KIRA VERLATO");
  Q3.traverseThroughType("FRIENDS");
  Q3.terminateAtTargetNumber(1);
  Q3.breadth_first_search();
//  Q2.startRecording();
  Q3.dumpVertexTarget();
  
  auto targets3 = Q3.getVertexTargetSet();
 
  for(auto it = targets3.begin(); it != targets3.end(); ++it) {
    TraversalDescription q(g);
    cout << "start from vertex: " << (*it)->getId() << endl;
    q.traverseFromVertex((*it)->getId());
    q.traverseThroughType("LIKES");
    q.filtType("LIKES");
//    q.traverseThroughDirection("in");
    q.terminateAtDepth(1);
    q.breadth_first_search();
    q.dumpVertexTarget();
  }


  cout << "===============================\n";
  cout << "Query 4\n";
  TraversalDescription Q4(g);
  Q4.traverseFromVertex(vp0->getId());
  Q4.filtProperty("pid", "5");
  Q4.traverseThroughType("FRIENDS");
  Q4.terminateAtTargetNumber(1);
  Q4.breadth_first_search();
  Q4.dumpVertexTarget();
  
  auto targets4 = Q4.getVertexTargetSet();
 
  for(auto it = targets4.begin(); it != targets4.end(); ++it) {
    FixedString key("name");
    cout << (*it)->getPropertyValue(key).first;
    cout << endl;
  }


  return 0;
}
