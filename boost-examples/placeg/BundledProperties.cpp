#include <new>
#include <iostream>
#include <iomanip>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>

using namespace std;


class custom_bfs_visitor : public boost::default_bfs_visitor
{
public:
     
  template < typename Vertex, typename Graph >
  void discover_vertex(Vertex u, const Graph & g) const
  {
    cout << u << endl;
   
  }
};
 
// Create a struct to hold properties for each vertex
struct VertexProperties
{
  int VertexIntProperty;
  string VertexStringProperty;

};
 
// Create a struct to hold properties for each edge
struct EdgeProperties
{
  int EdgeIntProperty;
  string EdgeStringProperty;
};
 
// Define the type of the graph
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, VertexProperties, EdgeProperties> Graph;
 

void printmem (char* p) {
  char* q = p;
  cout << "Trying to print the contents of memory" << endl;
  cout << "\n===============================================" << endl;
  for (int i = 0; i < sizeof(char)*200/(sizeof(int)); i++) {
    int fourbyte = (*q);
    cout << hex << setfill('0')<< fourbyte << " ";
    q = q++;
  }
  cout << "\n===============================================" << endl;
}

int main(int,char*[])
{

  char memory[sizeof(char)*200];
  void* place = memory;
  for (int i = 0; i < (sizeof(char)*200); i++) {
    memory[i] = 'x';
  }

  cout << "Initialized memory\n";
  cout << "\n===============================================" << endl;
  for (int i = 0; i < (sizeof(char)*200); i++) {
    cout << hex << setfill('0') << memory[i] << " " ;
  }
  cout << "\n===============================================" << endl;

  // Create a graph object
  Graph* g = new(place) Graph();

  cout << "Address for graph: " << g << endl;
  cout << "Address for memory: " << place << endl;

  // Add vertices
  Graph::vertex_descriptor v0 = boost::add_vertex(*g);
  Graph::vertex_descriptor v1 = boost::add_vertex(*g);
  Graph::vertex_descriptor v2 = boost::add_vertex(*g);
  Graph::vertex_descriptor v3 = boost::add_vertex(*g);
  Graph::vertex_descriptor v4 = boost::add_vertex(*g);    
  //  Graph::vertex_descriptor v5 = boost::add_vertex(*g);    
  //  Graph::vertex_descriptor v6 = boost::add_vertex(*g);    
         
  // Add an edge
  std::pair<Graph::edge_descriptor, bool> e01 = boost::add_edge(v0, v1, *g);
  std::pair<Graph::edge_descriptor, bool> e02 = boost::add_edge(v0, v2, *g);
  std::pair<Graph::edge_descriptor, bool> e03 = boost::add_edge(v1, v3, *g);
  
  std::pair<Graph::edge_descriptor, bool> e04 = boost::add_edge(v0, v4, *g);    
  
  // Set and output the properties of a single vertex
  (*g)[v0].VertexIntProperty = 0;  (*g)[v0].VertexStringProperty = "zero";
  (*g)[v1].VertexIntProperty = 1;  (*g)[v1].VertexStringProperty = "one";
  (*g)[v2].VertexIntProperty = 2;  (*g)[v2].VertexStringProperty = "two";
  (*g)[v3].VertexIntProperty = 3;  (*g)[v3].VertexStringProperty = "three";
  (*g)[v4].VertexIntProperty = 4;  (*g)[v4].VertexStringProperty = "four";      
  
  // Set and output the properties of a single edge
  (*g)[e01.first].EdgeIntProperty = 01;  (*g)[e01.first].EdgeStringProperty = "0-1";
  (*g)[e02.first].EdgeIntProperty = 02;  (*g)[e02.first].EdgeStringProperty = "0-2";
  (*g)[e03.first].EdgeIntProperty = 13;  (*g)[e03.first].EdgeStringProperty = "1-3";
  (*g)[e04.first].EdgeIntProperty = 04;  (*g)[e04.first].EdgeStringProperty = "0-3";      
                       
  // Output the properties of each vertex
  std::pair<Graph::vertex_iterator, Graph::vertex_iterator> vertexIteratorRange = boost::vertices(*g);

  for(Graph::vertex_iterator vertexIterator = vertexIteratorRange.first; vertexIterator != vertexIteratorRange.second; ++vertexIterator)
    {
      //     g[*vertexIterator].VertexIntProperty = 5;
      //     g[*vertexIterator].VertexStringProperty = "test";
      std::cout << (*g)[*vertexIterator].VertexIntProperty <<  " : " << (*g)[*vertexIterator].VertexStringProperty <<  std::endl;
    }
                           
  // Output the properties of each edge
  std::pair<Graph::edge_iterator, Graph::edge_iterator> edgeIteratorRange = boost::edges(*g);
  for(Graph::edge_iterator edgeIterator = edgeIteratorRange.first; edgeIterator != edgeIteratorRange.second; ++edgeIterator)
    {
  //     g[*edgeIterator].EdgeIntProperty = 5;
  //     g[*edgeIterator].EdgeStringProperty = "test";
      //      std::cout << (*g)[*edgeIterator].EdgeIntProperty <<  " : " << (*g)[*edgeIterator].EdgeStringProperty <<  std::endl;
      cout << source(*edgeIterator, _g) << endl;
    }

  custom_bfs_visitor vis;
  breadth_first_search(*g, vertex(0, *g), visitor(vis));

  cout << "\n===============================================" << endl;
  for (int i = 0; i < (sizeof(char)*200); i++) {
    cout << hex << setfill('0') << memory[i] << " " ;
  }
  cout << "\n===============================================" << endl;

  // Call destructor
  // Must do this manually as the place is used.
  g->~Graph();

  
  return 0;
}
