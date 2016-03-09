#include <map>
#include <unordered_map>
#include <vector>
#include <list>
#include <set>
#include <queue>
#include <iostream>



class EdgeList {

public:
  typedef  unsigned long int VertexDescriptor;
  typedef  unsigned long int EdgeDescriptor;
  typedef  std::pair<VertexDescriptor, VertexDescriptor> Element;  
  typedef  std::list<Element> EdgeListContainer;
public:
  EdgeList() {

  }

  void addVertex(VertexDescriptor Source) {

  }

  bool findEdge(Element NewEdge) {
    bool Found = false;
    for ( auto Edge: Graph ) {
      /// Check if NewEdge is already there.
      if ( ( Edge.first == NewEdge.first ) &&
           ( Edge.second == NewEdge.second ) ) {
        Found = true;
      }
    }
    return Found;
  }
  
  void addEdge(VertexDescriptor Source, VertexDescriptor Target) {
    /// Check if Source is in the graph or not.

    Element NewEdge(Source, Target);
    
    if ( findEdge(NewEdge) == false ) {
      /// Should insert a copy.
      Graph.push_back(NewEdge); 
   }
  }

  auto getAdjacentVertices(VertexDescriptor Vertex)
  -> EdgeListContainer {

    EdgeListContainer Edges; 
    for ( auto Edge: Graph ) {
      /// Check if NewEdge is already there.
      if ( ( Edge.first == Vertex ) ) {
        Edges.push_back(Edge);
      }
    }
    return Edges;
  }
  
  void dump() {
    for (auto Edge: Graph) {
      std::cout << "[ " << Edge.first << ", " << Edge.second << "]";
      std::cout << "\n";
    }
  }


  void bfs(VertexDescriptor Source) {

    typedef std::pair<VertexDescriptor, bool> VisitPair;
  
    // Start traversing the graph from here. 
    std::queue<VertexDescriptor> VertexQueue;
    /// True means visited and false means not visited.
    std::unordered_map<VertexDescriptor, bool> ColorMap;
  
    auto ScheduledVertex = Source;
    
    VertexQueue.push(ScheduledVertex);               
    ColorMap.insert(VisitPair(ScheduledVertex, false));
    
    while ( !VertexQueue.empty() ) {
      ScheduledVertex = VertexQueue.front();  VertexQueue.pop();
      std::cout << "Visit: " << ScheduledVertex << "\n";
      /// Set to visited.    
      ColorMap[ScheduledVertex] = true;

      for ( auto TargetEdge: getAdjacentVertices(ScheduledVertex) ) {
        VertexDescriptor TargetVertex = TargetEdge.second;
        if (ColorMap.find(TargetVertex) == ColorMap.end()) {
          /// Queue up the target for visitation
          VertexQueue.push(TargetVertex);
  
          ColorMap.insert(VisitPair(TargetVertex,false));
        } 
          
      }

    }
  }
  
protected:

  /// Graph.
  EdgeListContainer Graph;

};


int main() {

  EdgeList Graph;

  Graph.addEdge(0, 1);
  Graph.addEdge(0, 2);
  Graph.addEdge(0, 5);  
  Graph.addEdge(1, 3);
  Graph.addEdge(2, 3);  
  Graph.addEdge(2, 4);
  Graph.addEdge(5, 6);  
  

  Graph.dump();

  Graph.bfs(0);
  return 0;
}


