//===-- traversals/breadth_first_search.h - BFsearch ----*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is a breadth_first search.
///
//===----------------------------------------------------------------------===//
#ifndef _BREADTH_FIRST_SEARCH_H_
#define _BREADTH_FIRST_SEARCH_H_

#include <queue>
#include <map>
#include <vector>
#include <iostream>

#include "GraphType.h"
#include "VertexVisitor.h"
#include "EdgeVisitor.h"
#include "CustomVisitor.h"
#include "TraversalDescription.h"

using namespace std;

//template<typename vertexVisitor,typename edgeVisitor>
//void breadth_first_search(GraphType::VertexDescriptor vs, GraphType & g, VertexVisitor<vertexVisitor> & vv, EdgeVisitor< edgeVisitor> & ve){
auto TraversalDescription::breadth_first_search() 
  -> void {
  typedef pair<GraphType::VertexPtr, bool> VisitPair;
  typedef vector<GraphType::VertexPtr> VertexPath;

  vector<VertexPath> Paths;
  bool vertexMatch = true; //
  bool typeMatch = true;
  bool directionMatch = true;
  bool edgeMatch = true;
  bool tflag = true;
  int  depth = 0;
  int  deg_in = 0;
  int  deg_out = 0;


//  cout << "================= BfS ===================== \n";

//  if(!_startNode.empty()) {
    auto np = Graph.getVertexPointer(StartVertex);
//    cout << "in bfs: start from vertex: " << np->getId() << endl;
//    _startNode.pop();
//  }
  // Start traversing the graph from here. 
  std::queue<GraphType::VertexPtr> Q;
  std::map<GraphType::VertexPtr, bool> C; // true = visited, false = not visited
/**
 * use VertexPath to record path info 
 * use queue to assist recording 
 * */
  queue<VertexPath> pathQueue;

  Q.push(np);
  C.insert(VisitPair(np,false));

  DepthList.insert(DepthPair(np, depth));
  VertexPath newPath;
  newPath.push_back(np);
  pathQueue.push(newPath);

  GraphType::VertexPtr targetVertex = nullptr;

  while (!Q.empty()) {
    np = Q.front();  Q.pop();

    if(!pathQueue.empty()) {
      newPath = pathQueue.front();
      pathQueue.pop();
    }

    vertexMatch = VVisitor.visitVertex(np); 
//      if (vertexPropertyMatch == false)   //if true, add  np to targetset
//        return ;  
   /// update path; 
   /// if true, increment counter of this path
   if (vertexMatch == true)
     VertexTargetSet.push_back(np); 

   tflag = TraversalTerminator.TerminateAtVertex(VertexTargetSet); 
   if(tflag == true)
     return;
//   if true, return all paths and terminate

   tflag = customTerminator();
   if (tflag == true)
     return ;
    // Set to visited.    
    C[np] = true;

    GraphType::EdgePtr nextEdge = np->getNextEdge();

    while (nextEdge != nullptr) {
      // Get the target
      edgeMatch= EVisitor.visitEdge(nextEdge);
      if (edgeMatch== true)
        EdgeTargetSet.push_back(nextEdge);

      /// update path;
      /// if true, increment counter of this path;
      /// if not, terminate this path;

      ///checkTerminator() 

      targetVertex = nextEdge->getTarget(np);
      depth = DepthList.at(np) + 1;
      DepthList.insert(DepthPair(targetVertex, depth));
      tflag = TraversalTerminator.TerminateAtDepth(DepthList);
      if (tflag == true)
        return;



      if (nextEdge->getFirstId() == np->getId()) //
        deg_out ++;
      else if (nextEdge->getSecondId() == np->getId())
        deg_in ++;

     
      if (C.find(targetVertex) == C.end()) {
	// queue the target for visitation
     
      	typeMatch = EVisitor.scheduleEdge(nextEdge);  //type selection
	      directionMatch = VVisitor.scheduleVertex(targetVertex, nextEdge); //direction selection
//        cout << "filtTypeFlag is: " << filtTypeFlag << endl;
        if((filtTypeFlag == true) && (typeMatch == true)) {
          VertexTargetSet.push_back(targetVertex);
         }

        if ((typeMatch== true) && (directionMatch == true)) {  
          Q.push(targetVertex);
//          cout << "push vertex: " << targetVertex->getId() << endl;
       
          VertexPath branchPath;
          branchPath = newPath;
          branchPath.push_back(targetVertex);
          pathQueue.push(branchPath);         
        }

        tflag = TraversalTerminator.TerminateAtEdge(EdgeTargetSet);
        if(tflag == true)
          return;


        /// if true, go through this edge/relationship
        /// if not, terminate this path

 	      C.insert(VisitPair(targetVertex,false));
      }//endif

      // Update nextEdge from np
      nextEdge = nextEdge->getNextEdge(np);
      if(nextEdge == nullptr) {
        InDegreeList.insert(DegreePair(np, deg_in));
        OutDegreeList.insert(DegreePair(np, deg_out));
        deg_in = 0;
        deg_out = 0;
        
 //       cout << "push new path of " << newPath.size() << " vertex\n";
        Paths.push_back(newPath);
      }

      tflag = TraversalTerminator.TerminateAtPath(Paths);
      if(tflag == true)
        return;
    }//endwhile(nextEdge)
  }//endwhile(Q)
  cout << "================= END BFS ===================== \n";
 }

#endif /* _BREADTH_FIRST_SEARCH_H_ */ 
