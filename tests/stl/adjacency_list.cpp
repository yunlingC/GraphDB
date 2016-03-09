#include <map>
#include <unordered_map>
#include <vector>
#include <list>
#include <set>
#include <queue>
#include <iostream>



class AdjacencyList {

public:
  typedef  unsigned long int VertexDescriptor;
  typedef  unsigned long int EdgeDescriptor;
  typedef  std::list<VertexDescriptor> EdgeList;
  typedef  std::pair<VertexDescriptor, EdgeList> Element;
  typedef  std::map<VertexDescriptor, EdgeList> AdjacencyListContainer;


  
  struct Debug {

    typedef std::list<VertexDescriptor> ListType;
    typedef std::_List_node<VertexDescriptor> _Node;
    typedef std::_List_iterator<VertexDescriptor> InternalListIterator;

    void dumpNext(EdgeList& Edges) {
      ListType::iterator ListIt = Edges.begin();
      if (ListIt == Edges.end()) {
        return;
      }
      
      InternalListIterator* bit = &ListIt;

      /// Cast it to a pointer to the internal _Node type. 
      InternalListIterator::_Node* bn =static_cast<InternalListIterator::_Node*>(bit->_M_node) ;
      std::cout << "# data: " << bn->_M_data << ", from cast: " << **bit << std::endl;
      std::cout << "# next data (using ptr): "<< static_cast< _Node* >(bn->_M_next)->_M_data << std::endl;


    };
  };

public:
  AdjacencyList() {

  }

  void addVertex(VertexDescriptor Source) {

  }
  
  void addEdge(VertexDescriptor Source, VertexDescriptor Target) {
    /// Check if Source is in the graph or not.

  
    AdjacencyListContainer::iterator SourceIterator = Graph.find(Source);
    if ( SourceIterator == Graph.end() ) {
      /// Source does not exist.
      EdgeList Edges;
      Edges.push_back(Target);

      Vertices.insert(Source);
      Vertices.insert(Target);

      /// Should insert a copy.
      Graph.insert(Element(Source, Edges));
      
    } else {
      /// Source is found.
      /// Insert Target in the edges list.
      EdgeList & Edges = SourceIterator->second;
      Edges.push_back(Target);

      Vertices.insert(Target);
    }
  }

  auto getAdjacentVertices(VertexDescriptor Vertex)
  -> EdgeList {
    return Graph[Vertex];
  }
  
  void dump() {
    for (auto Vertex: Vertices) {
      std::cout << Vertex << ": ";
      EdgeList Edges = Graph[Vertex];
      for (auto Target: Edges) {
        std::cout << Target << ", ";
      }
      std::cout << "\n";

      Debug DB;
      DB.dumpNext(Edges);
    }
  }


  void bfs(VertexDescriptor Source) {

    typedef std::pair<VertexDescriptor, bool> VisitPair;
  
    // Start traversing the graph from here. 
    std::queue<AdjacencyList::VertexDescriptor> VertexQueue;
    /// True means visited and false means not visited.
    std::unordered_map<AdjacencyList::VertexDescriptor, bool> ColorMap;
  
    auto ScheduledVertex = Source;
    
    VertexQueue.push(ScheduledVertex);               
    ColorMap.insert(VisitPair(ScheduledVertex, false));
    
    while ( !VertexQueue.empty() ) {
      ScheduledVertex = VertexQueue.front();  VertexQueue.pop();
      std::cout << "Visit: " << ScheduledVertex << "\n";
      /// Set to visited.    
      ColorMap[ScheduledVertex] = true;

      for ( auto TargetVertex: getAdjacentVertices(ScheduledVertex) ) {
        if (ColorMap.find(TargetVertex) == ColorMap.end()) {
          /// Queue up the target for visitation
          VertexQueue.push(TargetVertex);
  
          ColorMap.insert(VisitPair(TargetVertex,false));
        } 
          
      }

    }
  }

  void dfs(VertexDescriptor Source) {

    typedef std::pair<VertexDescriptor, bool> VisitPair;
  
    // Start traversing the graph from here. 
    std::vector<VertexDescriptor> VertexStack;
    /// True means visited and false means not visited.
    std::unordered_map<AdjacencyList::VertexDescriptor, bool> ColorMap;
  
    auto ScheduledVertex = Source;
    
    VertexStack.push_back(ScheduledVertex);               
    ColorMap.insert(VisitPair(ScheduledVertex, false));
    
    while ( !VertexStack.empty() ) {
      ScheduledVertex = VertexStack.back();  VertexStack.pop_back();
      std::cout << "Visit: " << ScheduledVertex << "\n";
      /// Set to visited.    
      ColorMap[ScheduledVertex] = true;

      for ( auto TargetVertex: getAdjacentVertices(ScheduledVertex) ) {
        if (ColorMap.find(TargetVertex) == ColorMap.end()) {
          /// Queue up the target for visitation
          VertexStack.push_back(TargetVertex);
  
          ColorMap.insert(VisitPair(TargetVertex,false));
        } 
          
      }

    }
  }
  
protected:

  /// Graph.
  AdjacencyListContainer Graph;

  /// Vertices.
  std::set<VertexDescriptor> Vertices;
      
};


int main() {

  AdjacencyList Graph;

  Graph.addEdge(0, 1);
  Graph.addEdge(0, 2);
  Graph.addEdge(0, 5);  
  Graph.addEdge(1, 3);
  Graph.addEdge(2, 3);  
  Graph.addEdge(2, 4);
  Graph.addEdge(5, 6);  
  

  Graph.dump();

  
  std::cout << "BFS\n";
  Graph.bfs(0);
  std::cout << "DFS\n";
  Graph.dfs(0);
  

  return 0;
}


