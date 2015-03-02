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

#include <stack>

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
typedef std::multimap<VertexPointer, unsigned int> MultiDepthList;
typedef std::map<VertexPointer, unsigned int> DegreeList;
typedef std::vector<VertexPointer> VertexTargetSet;
typedef std::stack<unsigned int > LayerStack;
typedef std::map<unsigned int, VertexTargetSet> LayerMap;


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

void traverseThroughTypeAndDirection(Type type, Direction direction, Filter & TraversalFilter) {
  TraversalFilter.setDirection(direction);
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
/// compute and return the depth for second
unsigned int computeDepth(VertexPointer first, EdgePointer ep, VertexPointer second, DepthList &dl) {
  typedef pair<VertexPointer, unsigned int> DepthPair;

  unsigned int depth = 1;
  if (dl.find(first) == dl.end()) {
    dl.insert(DepthPair(first, 0));
    if(dl.find(second) == dl.end())
      dl.insert(DepthPair(second, 1));
    else 
      dl[second] = 1;
//    cout << "vertex: " << first->getId() << " depth: 0" << endl;
//    cout << "vertex: " << second->getId() << " depth: 1" << endl;
  }
  else {
    depth = dl[first] + 1;
    if(dl.find(second) == dl.end()) {
      dl.insert(DepthPair(second, depth));
    }
    else {
      if(depth < dl[second])
        dl[second] = depth;
    }
  }
  return  depth;
}

void recordDepth(VertexPointer first, EdgePointer ep, VertexPointer second, MultiDepthList &dl) {
  typedef pair<VertexPointer, unsigned int> DepthPair;

  unsigned int depth = 1;
  if (dl.find(first) == dl.end()) {
    dl.insert(DepthPair(first, 0));
    dl.insert(DepthPair(second, 1));
//    cout << "vertex: " << first->getId() << " depth: 0" << endl;
//    cout << "vertex: " << second->getId() << " depth: 1" << endl;
  }
  else {
    auto range = dl.equal_range(second);
    for( auto it = dl.equal_range(first).first; it != dl.equal_range(first).second; ++it ) {
//        cout << "it " << (*it).first->getId() << " depth " << (*it).second << endl;
        depth = (*it).second + 1;
        unsigned int unique = true;
        if(dl.count(second) != 0)
        for(auto iter = dl.equal_range(second).first; iter != dl.equal_range(second).second; ++iter) {
//          cout << "iter " << (*iter).first->getId() << " depth " << (*iter).second << endl;
          if((*iter).second == depth) {
            unique = false;
            break;
          }
        }
          if(unique == true) {
//            cout << "vertex " << second->getId() << " depth " << depth << endl;
            dl.insert(DepthPair(second, depth));
          }
    }
  }

  return ;
}


void recordLayer(VertexPointer first, EdgePointer edge, VertexPointer second, LayerMap &lm, LayerStack& ls) {
/**   unsigned int layerNum ;
    if(_layerMap.empty()) {
      VertexTarget newLayer;
      newLayer.push_back(first);
      layerNum = 0;
      _layerMap.insert(LayerPair(layerNum, newLayer));
      _layerStack.push(layerNum);
    }// else {
      layerNum = _layerStack.top();
      cout << "Now come to layer: " << layerNum << endl;
      auto layerList = _layerMap.find(layerNum);
      if(layerList != _layerMap.end()) {
        for(auto it = layerList->second.begin(); it != layerList->second.end(); ++it) {
//          cout << "traverse list id " << (*it)->getId() << "first id " << first->getId() << endl;
          if( (*it)->getId() == first->getId() ) {
            if(_layerMap.find(layerNum+1) == _layerMap.end()) {
              VertexTarget newLayer;
              newLayer.push_back(second);
              cout <<"vertex " << second->getId() << " is pushed into layer\n";
              _layerMap.insert(LayerPair(layerNum+1, newLayer));
            }//endif
            else  {
              _layerMap[layerNum+1].push_back(second);
              cout <<"vertex " << second->getId() << " is pushed into layer\n";
            }//end_else
          }//end_if
//          else 
//            cout << "Error: Failed to match child vertex with parent\n";
        }//end_for
        if((edge->getNextEdge(first) == nullptr) && (first == layerList->second.back())) {
          layerNum ++;
          _layerStack.push(layerNum);
          cout << "first " << first->getId() << " second " << second->getId() << " to layernum " << layerNum << endl;
        }//end_if
      }//end_if
      else
        cout << "Error: Failed to match the layerNum with layerList\n";
//    }
*/
}

void updateDepth(VertexPointer first, EdgePointer ep, VertexPointer second, DepthList &dl) {
  typedef pair<VertexPointer, unsigned int> DepthPair;

  unsigned int depth = 1;
  if (dl.find(first) == dl.end()) {
    dl.insert(DepthPair(first, 0));
    if(dl.find(second) == dl.end())
      dl.insert(DepthPair(second, 1));
    else 
      dl[second] = 1;
//    cout << "vertex: " << first->getId() << " depth: 0" << endl;
//    cout << "vertex: " << second->getId() << " depth: 1" << endl;
   
  }
  else {
    depth = dl[first] + 1;
    if(dl.find(second) == dl.end()) {
      dl.insert(DepthPair(second, depth));
    }
    else 
      dl[second] = depth;
//    cout << "Vertex: " << second->getId() << "depth: " << depth << endl;
  }
  return  ;

}

/// check if the vertex's depth == given depth
bool checkDepth(unsigned int depth, VertexPointer vertex, DepthList & depthList) {
  if (depthList.find(vertex) != depthList.end())
    if(depthList[vertex]  == depth)
      return true;
  return false;
}

unsigned int  checkMaxDepth(MultiDepthList & dl) {
  int depth = 0;
  for(auto it = dl.begin(); it != dl.end(); it ++) {
    if( (*it).second > depth)
      depth = (*it).second;
  }
  return depth;
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


void dumpVertexTarget(VertexTargetSet & VertexTargetList) {

  for (auto it = VertexTargetList.begin(); it != VertexTargetList.end(); ++it) {
    cout << "Vertex: "<< (*it)->getId() << endl;
//      (*it)->getPropertyList();
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

/// check if the direction of vertex to edge is 
bool checkDirection(VertexPointer vertex, EdgePointer edge, Filter & filter ) {
    int direction = 0;

    if(filter.getDirection() == "" )
      return true;

    if ( filter.getDirection() == "out")
      direction = 1;     //first vertex => outEdge for vertex;
    else if ( filter.getDirection() == "in")
      direction = 2;     //second vertex => inEdge for vertex;
    else 
      cout <<"Err: Irrelevant edges.\n";

    switch(direction) {
    case 1:
      if (edge->getFirstId() == vertex->getId()) {
//        cout << "one catch on vertex: " << vertex->getId() << endl;
        return false;
      }
      break;

    case 2:
      if (edge->getSecondId() == vertex->getId()) { 
//        cout << "one catch on vertex: " << vertex->getId() << endl;
        return false;
      }
      break;

    default:
      return true;
    }
    return true;
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
