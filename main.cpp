#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>


#include "PropertyList.h"
#include "Graph.h"

#include "readers/GraphReaderJSON.h"
#include "Utility.h"

#include <string>

int main(int argc, char * argv[]) {

  typedef PropertyList<string, string> PropertyListType;
  PropertyListType p;
  PropertyListType q;

  p.set("a", "1");
  p.set("b", "2");
  p.set("c", "3");

  q = p;
  p.remove("b");

  q.print();
  p.print();
  
  cout << "value for 'c': " << p.get("c") << endl;

  /* Start to test the graph class */
  
  cout << "+ Start testing graph" << endl;
  Graph g;

  Graph::VertexDescriptor v1 = g.addVertex(p);
  Graph::VertexDescriptor v2 = g.addVertex();
  Graph::VertexDescriptor v3 = g.addVertex();
  Graph::EdgeDescriptor e1 = g.addEdge(v1, v2, "1-2", q);
  g.addEdge(v2, v1, "2-1", q);
  g.addEdge(v1, v3); // 1-3
  g.addEdge(v3, v2, "3-2", p); // 3-2

  // Set individual properties.
  g[e1].setProperty("name", "edge1");
  g[v2].setProperty("z", "lkjadsf");
  g.print();

  // Check for interface methods from blueprint

  cout << "\n= getEdges() test" << endl;
  Graph::EdgeIteratorPair ei = g.getEdges();
  print(ei.first, ei.second);

  cout << "\n= getOutEdges() test" << endl;
  Graph::OutEdgeIteratorPair oei = g.getOutEdges(v1);
  print(oei.first, oei.second);

  cout << "\n= getInEdges() test" << endl;
  Graph::InEdgeIteratorPair iei = g.getInEdges(v2);
  print(iei.first, iei.second);

  cout << "\n= getVertices() test" << endl;
  Graph::VertexIteratorPair vr = g.getVertices();
  print(vr.first, vr.second);


  /* PLEASE ignore this code.  It's the basic to allow for JSON parsing */
  GraphReaderJSON json("t.json");
  json.print();
  /* 
  cout << "\n= JSON parsing" << endl;
  try    {
    std::stringstream ss;
    ss << "{ \"root\": { \"values\": [1, 2, 3, 4, 5 ] } }"; 

    boost::property_tree::ptree pt;
    boost::property_tree::read_json("t.json", pt);

    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, 
		  pt.get_child("graph.vertices") )      {
      assert(v.first.empty()); // array elements have no names
      
      cout << v.second.size() << endl;
      property_tree::ptree & pt2 = v.second;
      
      property_tree::ptree::const_iterator ci;
      for (ci = pt2.begin(); ci != pt2.end(); ci++) {
	cout << "k: " << ci->first.data() << ", v: " << ci->second.data() << endl;
      }
    }
    return EXIT_SUCCESS;
    }
  catch (std::exception const& e)    {
      std::cerr << e.what() << std::endl;
    }
  */

  return 0;
}
