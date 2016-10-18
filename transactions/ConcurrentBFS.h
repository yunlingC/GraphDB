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
#ifndef _CONCURRENTBFS_H_
#define _CONCURRENTBFS_H_

/// Local includes.
//#include "util.h"
#include "GraphType.h"
#include "Visitor.h"
#include "LocksManager.h"

/// Standard includes.
#include <queue>
#include <map>
#include <vector>

//TODO
/// back-off one by one /read or write - stack
/// update graph->add/delete nodes/edges
/// remove std
/// Macros of spinning on locks
/// add comments

class ConcurrentBFS {
public:
  typedef LocksManager LockManagerType;
  typedef std::pair<GraphType::VertexPointer, bool> VisitPair;
  typedef std::pair<VertexPointer, std::pair<MutexType, LockType> >  VLockPair;
  typedef std::pair<EdgePointer, std::pair<MutexType, LockType> >  ELockPair;
  typedef std::vector<VLockPair> VLockListType; 
  typedef std::vector<ELockPair> ELockListType; 

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
      std::cerr << "No such vertex in graph \n";
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
      if (!LockManager.getVertexLock(ScheduledVertex, T_Property, T_SH))
        break;
#else 
      /// spin on it till it is available
      while ( !LockManager.getVertexLock(ScheduledVertex, T_Property, T_SH));

#endif

      auto VT_PropertyPair = std::make_pair(ScheduledVertex, std::make_pair(T_Property, T_SH));
      VertexLocks.push_back(VPpPair);
 
      if (GraphVisitor.visitVertex(ScheduledVertex))
        return;
        
      /// Set to visited.    
      ColorMap[ScheduledVertex] = true;
#ifdef _SKIP_
      if (!LockManager.getVertexLock(ScheduledVertex, T_NextEdge, T_SH))
        break;
#else
      while (!LockManager.getVertexLock(ScheduledVertex, T_NextEdge, T_SH));
#endif

      auto VT_NextEdgePair = std::make_pair(ScheduledVertex, std::make_pair(T_NextEdge, T_SH));
      VertexLocks.push_back(VNEPair);
  
      auto NextEdge = ScheduledVertex->getNextEdge();
      while ( NextEdge != nullptr ) {                  
        /// Get the target node.

#ifdef _SKIP_
        if (!LockManager.getEdgeLock(NextEdge, T_FirstVertex, T_SH)) {
          break;
        }
#else
        while (!LockManager.getEdgeLock(NextEdge, T_FirstVertex, T_SH));
#endif

#ifdef _SKIP_
        if (!LockManager.getEdgeLock(NextEdge, T_SecondVertex, T_SH)) {
          LockManager.releaseEdgeLock(NextEdge, T_FirstVertex, T_SH);
          break;
        }
#else
        while (!LockManager.getEdgeLock(NextEdge, T_SecondVertex, T_SH));
#endif
        auto ET_FirstVertexPair = std::make_pair(NextEdge, std::make_pair(T_FirstVertex, T_SH));
        auto ET_SecondVertexPair = std::make_pair(NextEdge, std::make_pair(T_SecondVertex, T_SH));
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
        if (!LockManager.getEdgeLock(NextEdge, T_FirstNextEdge, T_SH)) {
          LockManager.releaseEdgeLock(NextEdge, T_FirstVertex, T_SH);
          LockManager.releaseEdgeLock(NextEdge, T_SecondVertex, T_SH);
          break;
        }
#else
        while (!LockManager.getEdgeLock(NextEdge, T_FirstNextEdge, T_SH));
#endif

#ifdef _SKIP_
        if (!LockManager.getEdgeLock(NextEdge, T_SecondNextEdge, T_SH)) {
          LockManager.releaseEdgeLock(NextEdge, T_FirstVertex, T_SH);
          LockManager.releaseEdgeLock(NextEdge, T_SecondVertex, T_SH);
          LockManager.releaseEdgeLock(NextEdge, T_FirstNextEdge, T_SH);
          break;
        }
#else
        while (!LockManager.getEdgeLock(NextEdge, T_SecondNextEdge, T_SH));
#endif
        auto ET_FirstNextEdgePair = std::make_pair(NextEdge, std::make_pair(T_FirstNextEdge, T_SH));
        auto ET_SecondNextEdgePair = std::make_pair(NextEdge, std::make_pair(T_SecondNextEdge, T_SH));
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

#endif /* _CONCURRENTBFS_H_*/
