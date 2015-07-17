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
#include <time.h>
#include <boost/algorithm/string.hpp>

#include "Filter.h"
#include "GraphType.h"

typedef std::string KeyType;
typedef std::string ValueType;
typedef std::string Type;
typedef std::string RelType;
typedef unsigned int IdType;
typedef std::string Direction;
typedef std::vector<string> AttributesType;
typedef GraphType::VertexPointer VertexPointer;
typedef GraphType::EdgePointer    EdgePointer;
typedef std::map<VertexPointer, unsigned int> DepthList;
typedef std::multimap<VertexPointer, unsigned int> MultiDepthList;
typedef std::map<VertexPointer, unsigned int> DegreeList;
typedef std::vector<VertexPointer> VertexTargetSet;
typedef std::stack<unsigned int > LayerStack;
typedef std::map<unsigned int, VertexTargetSet> LayerMap;
typedef std::vector<ValueType> ValueListType;
typedef std::vector<ValueType> &  ValueListTypeReference;
typedef std::pair<bool, std::pair<std::string, GraphType::PropertyListType> > ReturnBranchType;

void filtProperty(KeyType key, ValueType value, Filter & TraversalFilter){
  TraversalFilter.setKey(key);
  TraversalFilter.setValue(value);
}

void filtPropertyList(KeyType key, ValueListTypeReference ValueList, Filter & TraversalFilter) {
  TraversalFilter.setKey(key);
  TraversalFilter.setValueList(key, ValueList);
}

void filtVertexId(IdType vid , Filter & TraversalFilter) {
  TraversalFilter.setVertexId(vid);
}

void filtEdgeId (IdType eid, Filter & TraversalFilter) {
  TraversalFilter.setEdgeId(eid);
}

//TODO set flag 
void filtType (Type type, Filter & TraversalFilter) {
  TraversalFilter.setType(type);
//  filtTypeFlag = true;
}

void traverseThroughDirection(Direction direction, Filter & TraversalFilter) {
  TraversalFilter.setDirection(direction);
}

void traverseThroughType(Type type, Filter & TraversalFilter) {
  TraversalFilter.setType(type);
}

void traverseThroughMultiRelType(RelType types, Filter & TypeFilter ) {
  AttributesType attributes;
  boost::split(attributes, types, boost::is_any_of("+"));
  for (auto it = attributes.begin(); it != attributes.end(); it++)
//    cout << "types " << *it ;
//  cout << endl;
  TypeFilter.setTypeList(attributes);
}

void traverseThroughTypeAndDirection(Type type, Direction direction, Filter & TraversalFilter) {
  TraversalFilter.setDirection(direction);
  TraversalFilter.setType(type);
}

bool terminateAtVertex(unsigned int targetNum, VertexTargetSet vertexSet) {

    if(vertexSet.size() >= targetNum) 
    {
      return true;
    }
    else // not yet
      return false;
  }


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
  }
  else {
    for( auto it = dl.equal_range(first).first; it != dl.equal_range(first).second; ++it ) {
        depth = (*it).second + 1;
        unsigned int unique = true;
        if(dl.count(second) != 0)
        for(auto iter = dl.equal_range(second).first; iter != dl.equal_range(second).second; ++iter) {
          if((*iter).second == depth) {
            unique = false;
            break;
          }
        }
          if(unique == true) {
            dl.insert(DepthPair(second, depth));
          }
    }
  }
  return ;
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
   
  }
  else {
    depth = dl[first] + 1;
    if(dl.find(second) == dl.end()) {
      dl.insert(DepthPair(second, depth));
    }
    else 
      dl[second] = depth;
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
  unsigned int depth = 0;
  for(auto it = dl.begin(); it != dl.end(); it ++) {
    if( (*it).second > depth)
      depth = (*it).second;
  }
  return depth;
}

template<typename ReturnValueType>
bool checkProperty(VertexPointer vertex, Filter &filter) {
  if ((filter.getValue() == "") || (filter.getKey() == "") )
      return true; 
  FixedString value(filter.getValue());
  FixedString key(filter.getKey());
  ReturnValueType rv = vertex->getPropertyValue(key); 
  std::cout << " checkProperty " << rv.first.std_str() << "\t" << filter.getValue() << "\n";
  if((rv.second == false) || (rv.first != value))
  {
      return false;
  }
  return true;
}

template<typename ReturnValueType>
bool checkPropertyList(VertexPointer vertex, Filter & filter) {
  auto itend = filter.getValueList().end();
  if ( filter.getKey() == "") 
    return true;
  for ( auto it = filter.getValueList().begin(); it != itend; it++) {
    if ((*it == ""))
      ///TODO should be an exception here?
      return true; 
    FixedString value(*it);
    FixedString key(filter.getKey());
    ReturnValueType rv = vertex->getPropertyValue(key); 
    if(rv.first == value) {
      ///erase this pair so we don't have to check it later
      filter.getValueList().erase(it);
      return true;
    }
  }
    return false;
}

template<typename ReturnValueType>
bool checkPropertyValue(VertexPointer vertex, Filter & filter) {
  auto itend = filter.getPropertyMap().end();
  for ( auto it = filter.getPropertyMap().begin(); it != itend; it++) {
    FixedString Label((*it).first);
    FixedString key("id");
    FixedString value((*it).second);

    if ( Label == vertex->getType() ) {
      ReturnValueType rv = vertex->getPropertyValue(key); 
      if(rv.first == value) {
        return true;
      }
    }
  }
    return false;
}

template<typename ReturnValueType>
bool checkBranchProperty(VertexPointer vertex, EdgePointer edge, Filter & filter) {
  std::string EdgeType = edge->getType().std_str();
  if ( filter.getBranchPropertyMap().find(EdgeType) 
      == filter.getBranchPropertyMap().end() ) { 
    return false; 
  }
  std::pair<KeyType, ValueType> VertexPropertyPair = 
    filter.getBranchPropertyMap()[EdgeType];
 
  FixedString Key(VertexPropertyPair.first);
  FixedString Value(VertexPropertyPair.second);
  ReturnValueType rv = vertex->getPropertyValue(Key); 
  if(rv.first == Value) {
      ///erase this pair so we don't have to check it later
    filter.getBranchPropertyMap().erase(EdgeType);
    return true;
  }
  return false;
}

///checking vertex property to see if a new branch could grow here
///Vertex property format pair<Vertex->getType(), Vertex->getId()>
//template<typename ReturnValueType>
//bool checkBranch(VertexPointer vertex, Filter & filter) {
//  auto rv = vertex->getPropertyValue("id");
//  if(rv.second == false) {
//    ///TODO exception here
//    return false;
//  }
//  std::cout << "find vertex type " << vertex->getType() << " id " << rv.first << "\n";
//  auto Identity = std::pair<std::string, std::string>(vertex->getType().std_str(), rv.first);
//  if ( filter.getBranchMap().find(Identity) != filter.getBranchMap().end()) {
//    return true; 
//  }
//
//  return false;
//}

template<typename ReturnValueType>
ReturnBranchType checkBranch(VertexPointer vertex, Filter & filter) {
  GraphType::PropertyListType EdgeProp;
  EdgeProp.set("null", "null");
  ReturnBranchType rb = std::make_pair(false, std::make_pair("", EdgeProp));
  auto rv = vertex->getPropertyValue("id");
  if(rv.second == false) {
    ///TODO exception here
    return rb;
  }
//  std::cout << "find vertex type " << vertex->getType() << " id " << rv.first << "\n";
  auto Identity = std::pair<std::string, std::string>(vertex->getType().std_str(), rv.first.std_str());
  if ( filter.getBranchMap().find(Identity) != filter.getBranchMap().end()) {
    rb.first = true;
    rb.second = filter.getBranchMap()[Identity];
    return rb;
  }

  return rb;
}

int compareTime(time_t time1, time_t time2) {
  auto second = difftime(time1, time2);
  if (second >= 0.0000001) 
    return 1; 
  else  if(second < -0.0000001)
    return -1;
  else 
    return 0;
}

template<typename ReturnValueType, typename GraphElemPointer>
bool checkTimeRange(GraphElemPointer GraphElem, Filter &filter, bool & EqualFlag) {
  char buf[32];
  string times;  
  struct tm  time[2];
  bool cmpResult[2] = {false, false};
  EqualFlag = false;
  vector<string> attributes;
  for (auto i = 0; i < 2; i++) {
    times = filter.getValueRange().at(i);
    if(times == "")
      cmpResult[i] = true; 
    else {
      boost::split(attributes, times, boost::is_any_of("T"));
      boost::split(attributes, attributes[1], boost::is_any_of("."));
      memset(&time[i], 0, sizeof(struct tm));
      strptime(attributes[0].c_str(), "%H:%M:%S", &time[i]); 
      strftime(buf, sizeof(buf), "%H:%M:%S", &time[i]);
    } //END_ELSE
  } //END_FOR
  auto timeProp = GraphElem->getPropertyValue(filter.getKey());                          
  if(timeProp.second == false) {
    cout << "Error: Failed to find time property\n";
    return false;   /// cannot find time property ;
  }
  else  {
    struct tm TimeToCheck;
    auto timeString = timeProp.first.std_str();
    boost::split(attributes, timeString, boost::is_any_of("T"));
    boost::split(attributes, attributes[1], boost::is_any_of("."));
      memset(&TimeToCheck, 0, sizeof(struct tm));
    strptime(attributes[0].c_str(), "%H:%M:%S", &TimeToCheck);
    strftime(buf, sizeof(buf), "%H:%M:%S", &TimeToCheck);
    auto t = mktime(&TimeToCheck); 
    if (cmpResult[0] != true) {
      auto t1 = mktime(&time[0]); 
      auto cmp = compareTime(t, t1);
      if( cmp >=  0) {
        if ( cmp == 0) 
          EqualFlag = true;
        cmpResult[0] = true; 
      }
    }
    if (cmpResult[1] != true) {
      auto t2 = mktime(&time[1]);
      auto cmp2 = compareTime(t2, t);
      if(cmp2 >= 0) {
        if (cmp2 == 0)
          EqualFlag = true;
        cmpResult[1] = true; 
      }
    }
    return (cmpResult[0] && cmpResult[1]); 
  }
 }

template<typename GraphElemType>
bool checkYearRange(GraphElemType elem, Filter & filter, bool & EqualFlag) {
  EqualFlag = false;
  bool cmpResult[2] = {false, false};
  unsigned int Year[2] = {0, 0};
  for(auto i = 0; i < 2; i++){
    if(filter.getValueRange()[i] == "")
      cmpResult[i] = true;
    else  {
      Year[i] = stoi(filter.getValueRange()[i]);
    }
  }
  if(elem->getPropertyValue(filter.getKey()).second == false)
    return false;
  unsigned int YearProp = stoi(elem->getPropertyValue(filter.getKey()).first.std_str());
  if(cmpResult[0] == false)
    if( YearProp >= Year[0]) {
      if (YearProp == Year[0])
        EqualFlag = true;
      cmpResult[0] = true;
    }
  if(cmpResult[1] == false) {
    if( YearProp <= Year[1])
      cmpResult[1] = true;
  }
  return (cmpResult[0] && cmpResult[1]);

  }

template<typename ReturnValueType, typename GraphElemPointer>
bool checkDateRange(GraphElemPointer GraphElem, Filter &filter, bool & EqualFlag) {
  char buf[32];
  string times;  
  struct tm date[2];
  EqualFlag = false;
  bool cmpResult[2] = {false, false};
  vector<string> attributes;
  for (auto i = 0; i < 2; i++) {
    times = filter.getValueRange().at(i);
    if ( times == "" ) 
      cmpResult[i] = true;
    else {
      boost::split(attributes, times, boost::is_any_of("T"));
      memset(&date[i], 0, sizeof(struct tm));
      strptime(attributes[0].c_str(), "%Y-%m-%d", &date[i]);
      strftime(buf, sizeof(buf), "%Y-%m-%d", &date[i]);
    }
  }
  auto timeProp = GraphElem->getPropertyValue(filter.getKey());                          
  if(timeProp.second == false)
    return false;   /// cannot find time property ;
  else  {
    struct tm DateToCheck;
    auto dateString = timeProp.first.std_str();
    boost::split(attributes, dateString, boost::is_any_of("T"));
    memset(&DateToCheck, 0, sizeof(struct tm));
    strptime(attributes[0].c_str(), "%Y-%m-%d", &DateToCheck);
    strftime(buf, sizeof(buf), "%Y-%m-%d", &DateToCheck);
    auto d = mktime(&DateToCheck);
    if(cmpResult[0] != true) {
      auto d1 = mktime(&date[0]);
      unsigned int cmp = compareTime(d, d1);
      if (cmp >= 0) {
        if(cmp == 0) 
          EqualFlag = true;
        cmpResult[0] = true; 
      }
    }
    if(cmpResult[1] != true) {
      auto d2 = mktime(&date[1]);
      unsigned int cmp2 = compareTime(d2, d);
      if(cmp2 >= true) {
        if(cmp2 == true)
          EqualFlag = true;
        cmpResult[1] = true; 
      }
    }
    return (cmpResult[0] && cmpResult[1]); 
  }
 }

///apply to birthday within given ranges: xxxx-0m-0d : xxxx-0m'-0d'
//DOES NOT apply to single range  xxxx-0m-0d : now
template<typename ReturnValueType, typename GraphElemPointer>
bool checkBirthdayRange(GraphElemPointer GraphElem, Filter &filter, bool & EqualFlag) {
  char buf[32];
  string times;  
  struct tm date[2];
  EqualFlag = false;
  bool cmpResult[2] = {false, false};
  vector<string> attributes;
  for (auto i = 0; i < 2; i++) {
    times = filter.getValueRange().at(i);
    if ( times == "" ) 
      cmpResult[i] = true;
    else {
      boost::split(attributes, times, boost::is_any_of("T"));
      memset(&date[i], 0, sizeof(struct tm));
      strptime(attributes[0].c_str(), "%Y-%m-%d", &date[i]);
      strftime(buf, sizeof(buf), "%Y-%m-%d", &date[i]);
    }
  }
  auto timeProp = GraphElem->getPropertyValue(filter.getKey());                          
  if(timeProp.second == false)
    return false;   /// cannot find time property ;
  else  {
    struct tm DateToCheck;
    auto dateString = timeProp.first.std_str();
    boost::split(attributes, dateString, boost::is_any_of("T"));
    memset(&DateToCheck, 0, sizeof(struct tm));
    strptime(attributes[0].c_str(), "%Y-%m-%d", &DateToCheck);
    strftime(buf, sizeof(buf), "%Y-%m-%d", &DateToCheck);
//    std::cout << "date " << DateToCheck.tm_mon << "\n";
//    std::cout << "left date " << date[0].tm_mon << "\n";
//    std::cout << "right date " << date[1].tm_mon << "\n";
    if ( (date[0].tm_mon - DateToCheck.tm_mon <= 0)
         && (date[1].tm_mon - DateToCheck.tm_mon >= 0) ) {
      if ( ((date[0].tm_mday- DateToCheck.tm_mday <= 0) && 
            (date[0].tm_mon - DateToCheck.tm_mon == 0))
         || ((date[1].tm_mday- DateToCheck.tm_mday > 0) &&
            (date[1].tm_mon - DateToCheck.tm_mon == 0)) ) {
         cmpResult[0] = true;
         cmpResult[1] = true;
      }
    }
  }
    return (cmpResult[0] && cmpResult[1]); 
 }


template<typename GraphElemType>
bool checkRange(unsigned int opt, GraphElemType elem, Filter & RangeFilter, bool & EqualFlag) {
  typedef std::pair<FixedString, bool> ReturnValueType;
  switch(opt) {
          case 1:
            return checkTimeRange<ReturnValueType, GraphElemType>(elem, RangeFilter, EqualFlag);
          case 2:
            return checkDateRange<ReturnValueType, GraphElemType>(elem, RangeFilter, EqualFlag);
          case 3:
            return checkYearRange<GraphElemType>(elem, RangeFilter, EqualFlag);

          case 4: 
            return checkBirthdayRange<ReturnValueType, GraphElemType>(elem, RangeFilter, EqualFlag);

          case 0:
            return true;
          default:
            return true;
        }//switch 
}

void dumpVertexTarget(VertexTargetSet & VertexTargetList) {

  for (auto it = VertexTargetList.begin(); it != VertexTargetList.end(); ++it) {
    cout << "Vertex: "<< (*it)->getId() << endl;
  }
}

bool TerminateAtDepth(unsigned int depth, DepthList & depthlist) {
  for(auto it = depthlist.begin(); it != depthlist.end(); ++ it) {
    if(it->second > depth)
      return true;
  }
    return false;
}

bool checkEdgeType(EdgePointer edge, Type type) {
    if (type == "")
      return true;
    FixedString FixType(type);
    if (edge->getType() == FixType)
      return true;
    else  
      return false;
}

bool checkType(EdgePointer edge, Filter &filter) {
  return checkEdgeType(edge, filter.getType());
}

//bool checkTypes(EdgePointer edge, Filter & filter) {
//  auto itend = filter.getBranchMap().end();
//  auto IsMatch = false;
//  for ( auto it = filter.getBranchMap().begin(); it != itend; it++ ) {
////    std::cout  << "Utils: Type : " << (*it).first << "\n";
//    IsMatch = checkType(edge, (*it).first);
//    if ( IsMatch ) 
//      break;
//  }
//  return IsMatch;
//}

///TODO could be changed
bool checkMultiRelType(EdgePointer edge, Filter & filter) {
  auto typeMatch = false;   
  if(filter.getTypeList().empty())
    return true;
  for (auto it = filter.getTypeList().begin(); it != filter.getTypeList().end(); ++it) {
    if (*it == "") {
      return true;
    }
    FixedString type(*it);
    if (edge->getType() == type)
    {
      return true;
    }
    else  {
      typeMatch = false;
    }
  }
  return typeMatch;
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
        return false;
      }
      break;

    case 2:
      if (edge->getSecondId() == vertex->getId()) { 
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
