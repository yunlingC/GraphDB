//===-- LDBC/LdbcQueryDescription.h - Query class type ----------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the class for LDBC queries.
///
//===----------------------------------------------------------------------===//

#ifndef _LDBCQUERYDESCRIPTION_H_
#define _LDBCQUERYDESCRIPTION_H_

#include <vector>
#include <string>
#include <time.h>
#include <unordered_map>

#include "LdbcCustomVisitor.h"
//#include "LdbcUpdateVisitor.h"
#include "BreadthFirstSearch.h"
#include "QueryDescription.h"

#define CLOCK_ID  CLOCK_THREAD_CPUTIME_ID
#define MILLION 100000

/// This is the base class for LdbcQuery
class LdbcQuery : public Query{
public:
  typedef std::pair<string, string> ParamPairType;
  typedef std::pair<string, pair<string, string> > RangePairType;
//  typedef LocksManager LockManagerType;
public:
  LdbcQuery(unsigned int Id) : QueryId(Id) {
    LdbcFile.open("ldbc"+std::to_string(Id)+".log", ios_base::out| ios_base::app);
  }

  virtual void runQuery(Graph & graph, VertexDescriptor StartVertex ) { }

//  virtual void runQuery(Graph & graph, LockManagerType & LockManager) { }

  virtual void runQuery(Graph & graph) { }

//  virtual void runQuery(Graph & graph, VertexDescriptor StartVertex,
//      LockManagerType & LockManager) {}

//  virtual void runQuery(Graph & graph, VertexDescriptor StartVertex,
//      VertexDescriptor EndVertex, LockManagerType & LockManager) {}

  virtual void runQuery(Graph & graph, VertexDescriptor StartVertex, 
      VertexDescriptor EndVertex) {}

  void setParam(const string & Key, const string & Value) {
    ParamPair.first = Key;
    ParamPair.second = Value;
  }
  
  void setParam(ParamPairType & Param) {
    ParamPair = Param;
  }

  void setRange(string Key, string Min, string Max) {
    ValueRange.first = Key;
    ValueRange.second.first = Min;
    ValueRange.second.second = Max;
  }

  void setPropertyRange(string Key, string MinValue, string MaxValue) {
    PropRange.first = Key;
    PropRange.second.first = MinValue;
    PropRange.second.second = MaxValue;
  }
  
  void getExecTime() {
    End = (struct timespec){ 0 };
    if ( clock_gettime( CLOCK_ID, &End) == -1) {
      LdbcFile << "Query\t" << QueryId << "\tCould NOT get exec time\n"; 
      exit(0);
    }

    double execTime = ( End.tv_sec - Start.tv_sec )*1000
                  + ( End.tv_nsec - Start.tv_nsec)/MILLION;

    LdbcFile << "Query\t" << QueryId << "\t" << execTime << "\n"; 
  }

  void getStartTime() { 
    Start = (struct timespec){ 0 };
    if ( clock_gettime( CLOCK_ID, &Start ) == -1 ) {
      LdbcFile << "Fail to get start time\n";
      exit(0);
    } 
  }

protected:
  struct timespec Start;
  struct timespec End;
  unsigned int QueryId;
  ParamPairType ParamPair;
  RangePairType ValueRange;
  RangePairType PropRange;
  std::ofstream LdbcFile;
};

class LDBCQuery : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef FixedString KeyType;
  typedef FixedString ValueType;
  typedef PropertyList< KeyType, ValueType > PropertyListType;
  typedef PropertyListType & PropertyListTypeReference;
public:
  auto setPropertyList(PropertyListTypeReference PropertyList) 
    -> void {
    VertexPropertyList = PropertyList; 
  }
protected:
  PropertyListType VertexPropertyList;
};


#endif /*_LDBCQUERYDESCRIPTION_H_*/
