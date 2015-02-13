#ifndef _TRAVERSALDESCRIPTION_H_
#define _TRAVERSALDESCRIPTION_H_

#include <iostream>
//#include "GraphType.h"
#include "breadth_first_search.h"
//#include "VertexVisitor.h"
//#include "EdgeVisitor.h"
//#include "Filter.h"
#include "Terminator.h"

class TraversalDescription {
public:
  typedef std::string KeyType;
  typedef std::string ValueType;
  typedef std::string Type;
  typedef std::string Direction;
  typedef GraphType::VertexDescriptor VertexType;  
  typedef vector<GraphType::VertexPtr> VertexPath;
  typedef pair<GraphType::VertexPtr, int > DegreePair;
  typedef pair<GraphType::VertexPtr, int > DepthPair;
  typedef vector<VertexPath> PathType;
  typedef unsigned int IdType;
public:

//    TraversalDescription() { }
    TraversalDescription(GraphType & graph) : Graph(graph) { }
    void traverseGraph(GraphType & graph) {
      Graph = graph;
    }

    void traverseFromVertex(GraphType::VertexDescriptor startvertex) {
      StartVertex = startvertex;
    }

    void breadth_first_search(); 

    void depth_first_search();

    void filtProperty(KeyType key, ValueType value ) {
      TraversalFilter.setKey(key);
      TraversalFilter.setValue(value);
      VVisitor.setFilter(TraversalFilter);
    }

    void filtVertexId(IdType vid) {
      TraversalFilter.setVertexId(vid);
      VVisitor.setFilter(TraversalFilter);
    }

    void filtEdgeId (IdType eid) {
      TraversalFilter.setEdgeId(eid);
      EVisitor.setFilter(TraversalFilter);
    }

    void filtType (Type type) {
      TraversalFilter.setType(type);
      EVisitor.setFilter(TraversalFilter);
      filtTypeFlag = true;
    }

    std::vector<GraphType::VertexPtr> 
    getVertexTargetSet() {
      return VertexTargetSet;
    }
    
    void traverseThroughDirection(Direction direction) {
      TraversalFilter.setDirection(direction);
      VVisitor.setFilter(TraversalFilter);
    }

    void traverseThroughType(Type type) {
      TraversalFilter.setType(type);
      EVisitor.setFilter(TraversalFilter);
    }

    bool terminateAtDepth(unsigned int depth) {
      TraversalTerminator.atDepth(depth);
    }

    bool terminateAtPathNumber(unsigned int pnum) {
      TraversalTerminator.atPathNumber(pnum);
    }

    bool terminateAtTargetNumber(unsigned int tnum) {
      TraversalTerminator.atTargetNumber(tnum);
    }

    bool customTerminator (){
      return TraversalTerminator.customTerminator() ;
      return false;
    }

    void dumpVertexTarget() {

//      cout << "+++++++++Vertex Target Set+++++++++\n";
      for (auto it = VertexTargetSet.begin(); it != VertexTargetSet.end(); ++it) {
        cout << "Vertex: "<< (*it)->getId() << endl;
//      it->getPropertyList();
      }
    }

    void dumpPath() {
      cout << "+++++++++++Path  Set+++++++++\n";
      cout << "print out " << ReturnPaths.size() << "paths"<< endl;
      int i = 1;
      for(vector<VertexPath>::iterator it = ReturnPaths.begin(); it != ReturnPaths.end(); ++it) {
        cout << "path  " << i ;
        i++;
        for(VertexPath::iterator iter = it->begin(); iter != it->end(); ++iter) {
          GraphType::VertexPtr vp = *iter;
          cout << "\tvertex id  "<< vp->getId() << "\t";
        }
        cout << "\n";
     }
   }

    void dumpDegreeList() {
      cout <<"++++++++++Degree list++++++++++++\n";
      cout <<  "indegree list\n";
      for (map<GraphType::VertexPtr, int>::iterator it = InDegreeList.begin(); it != InDegreeList.end(); ++it) {
        cout << it->first->getId() << "\t" << it->second << endl;
      }

      cout <<  "outDegree list\n";
      for (map<GraphType::VertexPtr, int>::iterator it = OutDegreeList.begin(); it != OutDegreeList.end(); ++it) {
    cout << it->first->getId() << "\t" << it->second << endl;
      }
    }


    void dumpDepthList() {
      cout<< "++++++++++++Depth List++++++++++++++\n";
      cout << "depth list\n";
      for (map<GraphType::VertexPtr, int>::iterator it = DepthList.begin(); it != DepthList.end(); ++it)
        cout << it->first->getId() << "\t" << it->second << endl;

    }

protected:
    /// to pass property/type as arguments to check/match 
    Filter TraversalFilter;  
    /// to specify how should traversal stop
    Terminator TraversalTerminator;
    /// to record useful paths with all info(not only target vertices)
    PathType ReturnPaths;
    /// only have the scope of filters and set single flag for current vertexto be included/listed to be search/added as target/
//    VertexVisitor<VisitedVertex> VVisitor;
    VisitedVertex VVisitor;
    /// the same as vv
//    VertexVisitor<ScheduleVertex> SVisitor;
    VisitedEdge   EVisitor;
//    EdgeVisitor<VisitedEdge> EVisitor;

    unsigned int Depth;
    
    unsigned int filtTypeFlag = false;
    
//    std::queue<VertexType> _startNode;
    GraphType::VertexDescriptor  StartVertex;
//    GraphType::VertexPtr StartVertex;

//    std::vector<VertexPath> _paths; 
    std::map<GraphType::VertexPtr, int> OutDegreeList;
    std::map<GraphType::VertexPtr, int> InDegreeList;
    std::map<GraphType::VertexPtr, int> DepthList;
    std::vector<GraphType::VertexPtr> VertexTargetSet;
    std::vector<GraphType::EdgePtr> EdgeTargetSet;

    ///TO be added : subgraph

    /// graph to be traversed
    GraphType &Graph;

};



#endif /*_TRAVERSALDESCRIPTION_H_*/
