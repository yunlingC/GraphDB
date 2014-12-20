#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
 
#include <iostream>
 
class custom_bfs_visitor : public boost::default_bfs_visitor
{
public:
     
  template < typename Vertex, typename Graph >
  void discover_vertex(Vertex u, const Graph & g) const
  {
    std::cout << u << std::endl;
  }
};
 
int main()
{
  typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS > graph_t;
     
  graph_t g;
       
  add_edge(0, 1, g);
  add_edge(0, 2, g);
  add_edge(1, 3, g);
  add_edge(0, 4, g);
               
  custom_bfs_visitor vis;
  breadth_first_search(g, vertex(0, g), visitor(vis));
                   
  // You should expect to see 0 1 2 4 3
  return EXIT_SUCCESS;
}
