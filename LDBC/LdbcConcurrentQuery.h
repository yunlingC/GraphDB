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
#include "LdbcUpdateVisitor.h"
#include "BreadthFirstSearch.h"
#include "ConcurrentBFS.h"
#include "QueryDescription.h"

#define CLOCK_ID  CLOCK_THREAD_CPUTIME_ID
#define MILLION 100000

/*This is the base class for LdbcQuery*/
class LdbcQuery : public Query{
public:
  typedef std::pair<string, string> ParamPairType;
  typedef pair<string, pair<string, string> > RangePairType;
  typedef LocksManager LockManagerType;
public:
  LdbcQuery(unsigned int id) : _QueryId(id) {
    LdbcFile.open("ldbc"+std::to_string(id)+".log", ios_base::out| ios_base::app);
  }

  virtual void runQuery(Graph & graph, VertexDescriptor startVertex ) { }

  virtual void runQuery(Graph & graph, VertexDescriptor startVertex,
      LockManagerType & LockManager) {}

  void setParam(const string & Key, const string & Value) {
    ParamPair.first = Key;
    ParamPair.second = Value;
  }
  
  void setParam(ParamPairType & param) {
    ParamPair = param;
  }

  void setRange(string Key, string Min, string Max) {
    ValueRange.first = Key;
    ValueRange.second.first = Min;
    ValueRange.second.second = Max;
  }

  void setPropertyRange(string Key, string A, string B) {
    PropRange.first = Key;
    PropRange.second.first = A;
    PropRange.second.second = B;
  }
  
  void getExecTime() {
    _End = (struct timespec){ 0 };
    if ( clock_gettime( CLOCK_ID, &_End) == -1) {
      exit(0);
    }

    double execTime = ( _End.tv_sec - _Start.tv_sec )*1000
                  + ( _End.tv_nsec - _Start.tv_nsec)/MILLION;

    LdbcFile << "Query\t" << _QueryId << "\t" << execTime << "\n"; 
  }

  //TODO: throw exception or exit
  void getStartTime() { 
    _Start = (struct timespec){ 0 };
    if ( clock_gettime( CLOCK_ID, &_Start ) == -1 ) {
      LdbcFile << "Fail to get start time\n";
      exit(0);
    } 
//    LdbcFile << "Start time\t" << _Start.tv_sec << "\t" << _Start.tv_nsec <<"\n";
  }

protected:
  struct timespec _Start;
  struct timespec _End;
  unsigned int _QueryId;
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
  auto setPropertyList(PropertyListTypeReference pl) 
    -> void {
    VertexPropertyList = pl; 
  }
protected:
  PropertyListType VertexPropertyList;
};

#endif /*_LDBCQUERYDESCRIPTION_H_*/
