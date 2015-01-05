#ifndef _GRAPHREADERJSON_H_
#define _GRAPHREADERJSON_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost;

#include <string>
#include <iostream>

#include "Graph.h"


class GraphReaderJSON {
public:
  // Parse tree type.
  typedef property_tree::ptree jsonTreeType;
  // Node type.
  typedef property_tree::ptree::value_type jsonTreeNodeType;
  // Constant iterator.
  typedef jsonTreeType::const_iterator const_iterator;
  
public:
  GraphReaderJSON(const string & f) {
    read_json(f.c_str(), _jt);
  }

  GraphReaderJSON(const string & f, Graph & g) {
    read_json(f.c_str(), _jt);
    // Begin parsing and population of g
  }    

  void parseJSON() {
    
  }

  void print() {
    // This format will be only 3 levels deep as it is defined.
    jsonTreeType & g = _jt.get_child("graph");
    //    jsonTreeType & mode = _jt.get_child("graph.mode");
    jsonTreeType & vertices = _jt.get_child("graph.vertices");
    jsonTreeType & edges = _jt.get_child("graph.edges");

    // Check the mode of the graph.
    // Should be NORMAL or EXTENDED.
    for (jsonTreeType::iterator m = g.begin(); m != g.end(); m++) {
      if ( (!m->first.empty() ) && 
	   ( (m->second.data() == "NORMAL")  
	     || (m->second.data() == "EXTENDED") )
	   )
      cout << "mode: " << m->first.data() << ", " << m->second.data() << endl;
    }



    // Vertices
    for (jsonTreeType::iterator i = vertices.begin();  i != vertices.end(); i++) {
      
      cout << i->first.data() << ", " << i->second.data() << " , size: " << i->second.size() << endl;
      if (i->second.size() > 0 ) {
	for(jsonTreeType::iterator j = i->second.begin(); j!=i->second.end(); j++) {
	  cout << "\t" << j->first.data() << ", " << j->second.data() << endl;
	}

      }
    }

    // Edges
    for (jsonTreeType::iterator i = edges.begin();  i != edges.end(); i++) {
      
      cout << i->first.data() << ", " << i->second.data() << " , size: " << i->second.size() << endl;
      if (i->second.size() > 0 ) {
	for(jsonTreeType::iterator j = i->second.begin(); j!=i->second.end(); j++) {
	  cout << "\t" << j->first.data() << ", " << j->second.data() << endl;
	}

      }
    }


  }
protected:
  jsonTreeType _jt;

};

#endif /* _GRAPHREADERJSON_H_ */
