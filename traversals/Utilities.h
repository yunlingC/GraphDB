//===-- traversals/Utilities.h - functions to custom visitors---*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the function utilities for Graph visitors.
///
//===----------------------------------------------------------------------===//
#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "Filter.h"
#include "GraphType.h"

typedef std::string KeyType;
typedef std::string ValueType;
typedef std::string Type;
typedef unsigned int IdType;
typedef std::string Direction;
typedef GraphType::VertexPointer VertexPointer;
typedef GraphType::EdgePointer    EdgePointer;
typedef std::map<VertexPointer, unsigned int> DepthList;
typedef std::map<VertexPointer, unsigned int> DegreeList;
typedef std::vector<VertexPointer> VertexTargetSet;

void filtProperty(KeyType key, ValueType value, Filter & TraversalFilter){
  TraversalFilter.setKey(key);
  TraversalFilter.setValue(value);
//  GraphVisitor.setFilter(TraversalFilter);
}

void filtVertexId(IdType vid , Filter & TraversalFilter) {
  TraversalFilter.setVertexId(vid);
//  GraphVisitor.setFilter(TraversalFilter);
}

void filtEdgeId (IdType eid, Filter & TraversalFilter) {
  TraversalFilter.setEdgeId(eid);
//  GraphVisitor.setFilter(TraversalFilter);
}

//TODO set flag 
void filtType (Type type, Filter & TraversalFilter) {
  TraversalFilter.setType(type);
//  GraphVisitor.setFilter(TraversalFilter);
//  filtTypeFlag = true;
}

/**
std::vector<GraphType::VertexPtr> 
& getVertexTargetSet() {
  return VertexTargetSet;
}
*/   
void traverseThroughDirection(Direction direction, Filter & TraversalFilter) {
  TraversalFilter.setDirection(direction);
//  GraphVisitor.setFilter(TraversalFilter);
}

void traverseThroughType(Type type, Filter & TraversalFilter) {
  TraversalFilter.setType(type);
//  GraphVisitor.setFilter(TraversalFilter);
}

bool TerminateAtVertex(unsigned int targetNum, VertexTargetSet vertexSet) {

//      cout << "targetNum: " << targetNum << " targetSet size: " <<vertexSet.size() << endl;
    if(vertexSet.size() >= targetNum) 
    {
      return true;
    }
    else // not yet
      return false;
  }


/**
bool terminateAtDepth(unsigned int depth) {
  TraversalTerminator.atDepth(depth);
}

bool terminateAtPathNumber(unsigned int pnum) {
  TraversalTerminator.atPathNumber(pnum);
}

bool terminateAtTargetNumber(unsigned int tnum) {
  TraversalTerminator.atTargetNumber(tnum);
}

*/

void computeDepth(VertexPointer first, EdgePointer ep, VertexPointer second, DepthList &dl) {
  typedef pair<VertexPointer, unsigned int> DepthPair;

  if (dl.find(first) == dl.end()) {
    dl.insert(DepthPair(first, 0));
    dl.insert(DepthPair(second, 1));
//    cout << "vertex: " << first->getId() << " depth: 0" << endl;
//    cout << "vertex: " << second->getId() << " depth: 1" << endl;
   
  }
  else {
    unsigned int depth = dl[first] + 1;
    dl.insert(DepthPair(second, depth));
//    cout << "Vertex: " << second->getId() << "depth: " << depth << endl;
  }

}

template<typename ReturnValueType>
bool checkProperty(VertexPointer vertex, Filter &filter) {
  if ((filter.getValue() == "") || (filter.getKey() == "") )
      return false; 

  FixedString value(filter.getValue());
  FixedString key(filter.getKey());

  ReturnValueType rv = vertex->getPropertyValue(key); 
  if((rv.second != false) && (rv.first == value))
  {
//      cout << "++++Gotcha++++\n" << "Vertex:\t" << vp->getId() << endl;
      return true;
  }
}

void dumpDepthList(DepthList depthList){
//  cout<< "++++++++++++Depth List++++++++++++++\n";
  cout << "depth list\n";
  for (auto it = depthList.begin(); it != depthList.end(); ++it)
    cout << it->first->getId() << "\t" << it->second << endl;
}

void dumpVertexTarget(VertexTargetSet & VertexTargetList) {

//      cout << "+++++++++Vertex Target Set+++++++++\n";
  for (auto it = VertexTargetList.begin(); it != VertexTargetList.end(); ++it) {
    cout << "Vertex: "<< (*it)->getId() << endl;
//      it->getPropertyList();
  }
}


bool TerminateAtDepth(unsigned int depth, DepthList & depthlist) {
  for(auto it = depthlist.begin(); it != depthlist.end(); ++ it) {
    if(it->second > depth)
      return true;
  }
    return false;
}



bool checkType(EdgePointer edge, Filter &filter) {
    if (filter.getType() == "")
      return true;
    FixedString type(filter.getType());
    if (edge->getType() == type)
    {
//      cout << "Edge:\t" << ep->getId() << endl;
      return true;
    }
    else  {
//      cout << "type= " << ep->getType() << endl;
      return false;
    }
}


bool checkDirection(VertexPointer vertex, EdgePointer edge, Filter & filter ) {
    int direction = 0;

    if(filter.getDirection() == "" )
      return true;

    if (edge->getFirstId() == vertex->getId())
      direction = 1;     //first vertex == outEdge;
    else if ( edge->getSecondId() == vertex->getId())
      direction = 2;     //second vertex == inEdge;
    else 
      cout <<"Err: Irrelevant edges.\n";

    switch(direction) {
    case 1:
      if (filter.getDirection() == "out") {
//        cout << "one catch on vertex: " << vertex->getId() << endl;
        return true;
      }

    case 2:
      if (filter.getDirection() == "in") { 
        cout << "one catch on vertex: " << vertex->getId() << endl;
        return true;
      }
    default:
      return false;
    }
}


void computeOutDegree(VertexPointer vertex, EdgePointer edge, DegreeList &list) {
  typedef pair<VertexPointer, unsigned int> DegreePair; 

  if( list.find(vertex) == list.end() ) {
    list.insert(DegreePair(vertex, 0));
  }

  if ( edge->getFirstId() == vertex->getId() ) {
    list[vertex] = list[vertex] + 1;
  }

}

void computeInDegree(VertexPointer vertex, EdgePointer edge, DegreeList &list) {
  typedef pair<VertexPointer, unsigned int> DegreePair; 

  if( list.find(vertex) == list.end() ) {
    list.insert(DegreePair(vertex, 0));
  }

  if ( edge->getFirstId() == vertex->getId() ) {
    list[vertex] = list[vertex] + 1;
  }
}

#endif /**_UTILITIES_H_*/
