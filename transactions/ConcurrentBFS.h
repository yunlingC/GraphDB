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

class ConcurrentBFS {
public:
  typedef LocksManager LockManagerType;
  typedef pair<GraphType::VertexPointer, bool> VisitPair;
  typedef pair<unsigned int, pair<MutexType, LockType> >  LockPair;
  typedef vector<pair<unsigned int, pair<MutexType, LockType> > > LockListType; 
public:
  void releaseAll(LockManagerType & LockManager) {
     LockManager.releaseAll(VertexLocks, EdgeLocks); 
  }

  ///get read locks one by one and store in locklists, then release locks in reverse order one by one before return.
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
      auto VPpLock = LockManager.getVertexLock(ScheduledVertex->getId(), Pp, SH);
      if (!VPpLock) {
//        std::cout << "Skip read lock on Pp vertex\t" << ScheduledVertex->getId() << std::endl;
//        releaseAll(LockManager);
        break;
      }
#else 
      auto VPpLock = false; 
      while ( !VPpLock ) {
        VPpLock = LockManager.getVertexLock(ScheduledVertex->getId(), Pp, SH);
      }
#endif
      auto VPpPair = make_pair(ScheduledVertex->getId(), make_pair(Pp, SH));
      VertexLocks.push_back(VPpPair);
 
      bool VertexMatch = GraphVisitor.visitVertex(ScheduledVertex);
      if( VertexMatch == true ) {
        return;
      }
        
      /// Set to visited.    
      ColorMap[ScheduledVertex] = true;
#ifdef _SKIP_
      auto VNELock = LockManager.getVertexLock(ScheduledVertex->getId(), NE, SH);
      if ( !VNELock ) {
//        std::cout << "Skip read lock on NE vertex\t" << ScheduledVertex->getId() << std::endl;
//        releaseAll(LockManager);
        break;
      }
#else
      auto VNELock = false;
      while ( !VNELock ) {
        VNELock = LockManager.getVertexLock(ScheduledVertex->getId(), NE, SH);
      }
#endif
      auto VNEPair = make_pair(ScheduledVertex->getId(), make_pair(NE, SH));
      VertexLocks.push_back(VNEPair);
  
      auto NextEdge = ScheduledVertex->getNextEdge();
      while ( NextEdge != nullptr ) {                  
        /// Get the target node.
        auto EFVLock = false;
        auto ESVLock = false;
#ifdef _SKIP_
        EFVLock = LockManager.getEdgeLock(NextEdge->getId(), FV, SH);
        if ( !EFVLock ) {
//          Fstd::cout << "Skip read lock on FV edge\t" << NextEdge->getId() << std::endl;
          break;
        }
#else
        while ( !EFVLock ) {
          EFVLock = LockManager.getEdgeLock(NextEdge->getId(), FV, SH);
        }
#endif

#ifdef _SKIP_
        ESVLock = LockManager.getEdgeLock(NextEdge->getId(), SV, SH);
        if ( !ESVLock ) {
//          std::cout << "Skip read lock on SV edge\t" << NextEdge->getId() << std::endl;
          LockManager.releaseEdgeLock(NextEdge->getId(), FV, SH);
          break;
        }
#else
        while ( !ESVLock ) {
          ESVLock = LockManager.getEdgeLock(NextEdge->getId(), SV, SH);
        }

#endif
        auto EFVPair = make_pair(NextEdge->getId(), make_pair(FV, SH));
        auto ESVPair = make_pair(NextEdge->getId(), make_pair(SV, SH));
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
        
        auto EFNELock = false;
        auto ESNELock = false;
#ifdef _SKIP_
        EFNELock = LockManager.getEdgeLock(NextEdge->getId(), FNE, SH);
        if ( !EFNELock ) {
//          std::cout << "Skip read lock on FNE edge\t" << NextEdge->getId() << std::endl;
          LockManager.releaseEdgeLock(NextEdge->getId(), FV, SH);
          LockManager.releaseEdgeLock(NextEdge->getId(), SV, SH);
          break;
        }
#else
        while ( !EFNELock ) {
          EFNELock = LockManager.getEdgeLock(NextEdge->getId(), FNE, SH);
        }
#endif

#ifdef _SKIP_
        ESNELock = LockManager.getEdgeLock(NextEdge->getId(), SNE, SH);
        if ( !ESNELock ) {
//          std::cout << "Skip read lock on SNE edge\t" << NextEdge->getId() << std::endl;
          LockManager.releaseEdgeLock(NextEdge->getId(), FV, SH);
          LockManager.releaseEdgeLock(NextEdge->getId(), SV, SH);
          LockManager.releaseEdgeLock(NextEdge->getId(), FNE, SH);
          break;
        }
#else
        while ( !ESNELock ) {
          ESNELock = LockManager.getEdgeLock(NextEdge->getId(), SNE, SH);
        }
#endif
        auto EFNEPair = make_pair(NextEdge->getId(), make_pair(FNE, SH));
        auto ESNEPair = make_pair(NextEdge->getId(), make_pair(SNE, SH));
        EdgeLocks.push_back(EFNEPair);
        EdgeLocks.push_back(ESNEPair);

        /// Get the next edge from the scheduled vertex. 
        NextEdge = NextEdge->getNextEdge(ScheduledVertex);
      }
    }
    GraphVisitor.finishVisit();
  }
private:
  LockListType VertexLocks;
  LockListType EdgeLocks;
};

#endif /* _BREATHFIRSTSEARCH_H_*/
