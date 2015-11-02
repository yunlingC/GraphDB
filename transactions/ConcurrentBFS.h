//===-- transactions/BreathFirstSearch.h - BFS search ----*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is a breadth first search.
///
//===----------------------------------------------------------------------===//
#ifndef _BREATHFIRSTSEARCH_H_
#define _BREATHFIRSTSEARCH_H_

/// Standard includes.
#include <queue>
#include <map>
#include <vector>

/// Local includes.
#include "util.h"
#include "GraphType.h"
#include "Visitor.h"
#include "LocksManager.h"

//TODO
/// back-off one by one /read or write - stack
/// update graph->add/delete nodes/edges
/// remove std
/// Macros of spinning on locks
/// add comments

class ConcurrentBFS {
public:
  typedef LocksManager LockManagerType;
  typedef pair<GraphType::VertexPointer, bool> VisitPair;
  typedef pair<VertexPointer, pair<MutexType, LockType> >  VLockPair;
  typedef pair<EdgePointer, pair<MutexType, LockType> >  ELockPair;
  typedef vector<VLockPair> VLockListType; 
  typedef vector<ELockPair> ELockListType; 
public:
  void releaseAll(LockManagerType & LockManager) {
     LockManager.releaseAll(VertexLocks, EdgeLocks); 
  }

  /// get read locks one by one and store in locklists, 
  /// then release locks in reverse order one by one before return.
  void breadthFirstSearch(GraphType & Graph, 
                          const GraphType::VertexDescriptor & StartVertex,  
                          Visitor & GraphVisitor,
                          LockManagerType & LockManager) {
    auto ScheduledVertex = Graph.getVertexPointer(StartVertex);
  
    ///change 1
    if ( ScheduledVertex  == nullptr ) {
      cerr << "No such vertex in graph \n";
      exit(1); 
    }
    
    // Start traversing the graph from here. 
    std::queue<GraphType::VertexPointer> VertexQueue;
    /// True means visited and false means not visited.
    std::map<GraphType::VertexPointer, bool> ColorMap;
  
    VertexQueue.push(ScheduledVertex);               
    GraphVisitor.visitStartVertex(ScheduledVertex);
  
    ColorMap.insert(VisitPair(ScheduledVertex,false));
  
    GraphType::VertexPointer TargetVertex = nullptr;
  
    while ( !VertexQueue.empty() ) {
      ScheduledVertex = VertexQueue.front();  VertexQueue.pop();

#ifdef _SKIP_
      /// couldn't get the lock, skip it and visit the next one from queue
      if (!LockManager.getVertexLock(ScheduledVertex, Pp, SH))
        break;
#else 
      /// spin on it till it is available
      while ( !LockManager.getVertexLock(ScheduledVertex, Pp, SH));

#endif

      auto VPpPair = make_pair(ScheduledVertex, make_pair(Pp, SH));
      VertexLocks.push_back(VPpPair);
 
      if (GraphVisitor.visitVertex(ScheduledVertex))
        return;
        
      /// Set to visited.    
      ColorMap[ScheduledVertex] = true;
#ifdef _SKIP_
      if (!LockManager.getVertexLock(ScheduledVertex, NE, SH))
        break;
#else
      while (!LockManager.getVertexLock(ScheduledVertex, NE, SH));
#endif

      auto VNEPair = make_pair(ScheduledVertex, make_pair(NE, SH));
      VertexLocks.push_back(VNEPair);
  
      auto NextEdge = ScheduledVertex->getNextEdge();
      while ( NextEdge != nullptr ) {                  
        /// Get the target node.

#ifdef _SKIP_
        if (!LockManager.getEdgeLock(NextEdge, FV, SH)) {
          break;
        }
#else
        while (!LockManager.getEdgeLock(NextEdge, FV, SH));
#endif

#ifdef _SKIP_
        if (!LockManager.getEdgeLock(NextEdge, SV, SH)) {
          LockManager.releaseEdgeLock(NextEdge, FV, SH);
          break;
        }
#else
        while (!LockManager.getEdgeLock(NextEdge, SV, SH));
#endif
        auto EFVPair = make_pair(NextEdge, make_pair(FV, SH));
        auto ESVPair = make_pair(NextEdge, make_pair(SV, SH));
        EdgeLocks.push_back(EFVPair);
        EdgeLocks.push_back(ESVPair);

        TargetVertex = NextEdge->getTarget(ScheduledVertex);     
        bool RevisitFlag = GraphVisitor.discoverVertex(TargetVertex);             
        bool BranchMatch = GraphVisitor.scheduleBranch(ScheduledVertex, NextEdge, TargetVertex);
        bool TypeMatch =  GraphVisitor.scheduleEdge(NextEdge);
        bool DirectionMatch = GraphVisitor.visitDirection(TargetVertex, NextEdge);
  
        if( BranchMatch == true )
          return;
  
        if ( ColorMap.find(TargetVertex ) == ColorMap.end() || RevisitFlag ) {
          /// queue the target for visitation
          GraphVisitor.scheduleTree(ScheduledVertex, NextEdge, TargetVertex);
  
          if( TypeMatch && DirectionMatch )   {
           ///control the vertex to be visited filtered by type
  	        VertexQueue.push(TargetVertex);

  	        ColorMap.insert(VisitPair(TargetVertex,false));
          }
        } else {
          GraphVisitor.revisitVertex( TargetVertex );
        }
        
#ifdef _SKIP_
        if (!LockManager.getEdgeLock(NextEdge, FNE, SH)) {
          LockManager.releaseEdgeLock(NextEdge, FV, SH);
          LockManager.releaseEdgeLock(NextEdge, SV, SH);
          break;
        }
#else
        while (!LockManager.getEdgeLock(NextEdge, FNE, SH));
#endif

#ifdef _SKIP_
        if ( !LockManager.getEdgeLock(NextEdge, SNE, SH)) {
          LockManager.releaseEdgeLock(NextEdge, FV, SH);
          LockManager.releaseEdgeLock(NextEdge, SV, SH);
          LockManager.releaseEdgeLock(NextEdge, FNE, SH);
          break;
        }
#else
        while ( !LockManager.getEdgeLock(NextEdge, SNE, SH));
#endif
        auto EFNEPair = make_pair(NextEdge, make_pair(FNE, SH));
        auto ESNEPair = make_pair(NextEdge, make_pair(SNE, SH));
        EdgeLocks.push_back(EFNEPair);
        EdgeLocks.push_back(ESNEPair);

        /// Get the next edge from the scheduled vertex. 
        NextEdge = NextEdge->getNextEdge(ScheduledVertex);
      }
    }
    GraphVisitor.finishVisit();
  }
private:
  VLockListType VertexLocks;
  ELockListType EdgeLocks;
};

#endif /* _BREATHFIRSTSEARCH_H_*/
