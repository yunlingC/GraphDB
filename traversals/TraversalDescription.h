#ifndef _TRAVERSALDESCRIPTION_H_
#define _TRAVERSALDESCRIPTION_H_


#include <iostream>
#include "breadth_first_search.h"
#include "VertexVisitor.h"
#include "EdgeVisitor.h"
#include "Filter.h"
#include "Collector.h"
#include "Terminator.h"



class TraversalDescription {
public:
  typedef GraphType::VertexDescriptor VertexType;
 
public:
  typedef std::string KeyType;
  typedef std::string ValueType;
  typedef std::string Type;
//  typedef PathCollector<VeretxPointer, EdgePointer> PathType;
  typedef Collector<VertexType> VertexPath;
  typedef unsigned int IdType;
public:
    void breadth_first_search() { }

    void depth_first_search() { }

    void filtProperty(KeyType key, ValueType value ) {
      _f.setKey(key);
      _f.setValue(value);
    }

    void filtVertexId(IdType vid) {
      _f.setVertexId(vid);
    }

    void filtEdgeId (IdType eid) {
      _f.setEdgeId(eid);
    }

    void filtType (Type type) {
       _f.setType(type);
    }

    void goThroughType(Type type) {
      _f.setType(type);
    }

    void terminateAtDepth(unsigned int depth) {
      _t.atDepth(depth);
    }

    void terminateAtPathNumber(unsigned int pnum) {
      _t.atPathNumber(pnum);
    }

    void terminateAtTargetNumber(unsigned int tnum) {
      _t.atTargetNumber(tnum);
    }

private:
    /// to pass property/type as arguments to check/match 
    Filter _f;  
    /// to specify how should traversal stop
    Terminator _t;
    /// to record useful paths with all info(not only target vertices)
    PathType _paths;
    /// only have the scope of filters and set single flag for current vertexto be included/listed to be search/added as target/
    VertexVisitor _vv;
    /// the same as vv
    EdgeVisitor _ve;
     
    VertexType _startNode; 

    std::vector<VertexPath> _paths; 
    ///TO be added : subgraph, degree iterator, targetset

    /// graph to be traversed
    GraphType _graph;


};



#endif /*_TRAVERSALDESCRIPTION_H_*/
