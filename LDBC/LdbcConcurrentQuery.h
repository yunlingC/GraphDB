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

#include "LdbcCustomVisitor.h"
#include "LdbcUpdateVisitor.h"
#include "ConcurrentBFS.h"
#include "QueryDescription.h"
//#ifdef _INDEXING_
//#include "Index.h"
//#endif

#include <vector>
#include <string>
#include <time.h>
#include <unordered_map>

#define CLOCK_ID  CLOCK_THREAD_CPUTIME_ID
#define MILLION 100000

class Transaction;
/// This is the base class for LdbcQuery
class LdbcQuery : public Query{
public:
  typedef std::pair<std::string, std::string> ParamPairType;
  typedef std::pair<std::string, std::pair<std::string, std::string> > RangePairType;
  typedef LocksManager LockManagerType;
  typedef Transaction TransactionType;

public:
  LdbcQuery(unsigned int Id) : QueryId(Id) {
    LdbcFile.open("ldbc"+std::to_string(Id)+".log", std::ios_base::out| std::ios_base::app);
  }

  virtual void runQuery(Graph & graph, VertexDescriptor StartVertex ) { }

  virtual void runQuery(Graph & graph, LockManagerType & LockManager) { }

  virtual void runQuery(Graph & graph, VertexDescriptor StartVertex,
      LockManagerType & LockManager) {}

  virtual void runQuery(Graph & graph, VertexDescriptor StartVertex,
      VertexDescriptor EndVertex, LockManagerType & LockManager) {}

	virtual void runQuery(Graph & graph
						, VertexDescriptor StartVertex
						, TransactionType &tranx
		);


  void setParam(const std::string & Key, const std::string & Value) {
    ParamPair.first = Key;
    ParamPair.second = Value;
  }
  
  void setParam(ParamPairType & Param) {
    ParamPair = Param;
  }

  void setRange(std::string Key, std::string Min, std::string Max) {
    ValueRange.first = Key;
    ValueRange.second.first = Min;
    ValueRange.second.second = Max;
  }

  void setPropertyRange(std::string Key, std::string MinValue, std::string MaxValue) {
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
  auto setPropertyList(PropertyListTypeReference PropList) 
    -> void {
    PropertyList = PropList; 
  }
protected:
  PropertyListType PropertyList;
};


class LdbcAddEdgeQuery : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef std::string LabelType;
  typedef std::string KeyType;
  typedef std::string ValueType;
public:
  void initEdge(LabelType label) {
    NewEdge = new Edge(); 
    NewEdge->setType(label);
  }

  void initEdge(LabelType label, PropertyListType PropList) {
    initEdge(label);
    NewEdge->setPropertyList(PropList);
  }

  void initEdge(LabelType label, std::string Key, std::string Value)  {
    PropertyListType PropList;
    PropertyList.set(Key, Value);
    initEdge(label, PropList);
  }

  void getVertexId(ValueType firstId
                  , LabelType firstLabel
                  , ValueType secondId
                  , LabelType secondLabel)  {
    FirstId = firstId;
    FirstLabel = firstLabel;
    SecondId = secondId;
    SecondLabel = secondLabel;
  }

  virtual void runQuery(Graph & graph
	  					, VertexDescriptor StartVertex
	  					, TransactionType &tranx
	  	);

protected:
  EdgePointer NewEdge;
//#ifdef _INDEXING_
  VertexDescriptor FirstId;
  VertexDescriptor SecondId;
  LabelType   FirstLabel;
  LabelType   SecondLabel;
//#endif
};


class LdbcAddVertexQuery : public LdbcAddEdgeQuery {
  using LdbcAddEdgeQuery::LdbcAddEdgeQuery;
public:
//  typedef std::pair<std::string, std::string> VertexPairType;
//  typedef std::vector<VertexPairType>  EdgeListType;
  typedef std::vector<EdgePointer> EdgeListType;
  typedef std::string LabelType;
public:
  void initVertex(LabelType label, PropertyListType PropList) {
    NewVertex = new Vertex();  
    NewVertex->setType(label);
    NewVertex->setPropertyList(PropList);
  }

//  void initEdge(ValueType first
//              , ValueType second
//              , LabelType label
//              , std::string Key
//              , std::string Value)  {
//
//    EdgePointer NewEdge;
//    PropertyListType PropList;
//    PropertyList.set(Key, Value);
//    initEdge(label, PropList);
//  }

  virtual void runQuery(Graph & graph
	  					, VertexDescriptor StartVertex
	  					, TransactionType &tranx
  );

protected:
  VertexPointer NewVertex;
  EdgeListType EdgeList;
};


/// to return function pointer to pthread_create();
/// pointer to non-static functions could NOT be functor for "this" pointer
/// in member functions of class

class QueryHelper {
public:
  typedef GraphType::VertexDescriptor Vertex;
public:
  QueryHelper(GraphType & graph, 
            Vertex  StartV,
            Vertex  EndV,
            LocksManager &LockM,
            LdbcQuery &query):
            Graph (graph),
            StartVertex(StartV),
            EndVertex(EndV),
            LockManager(LockM),
            Query (query){}

public:
  GraphType &Graph;
  Vertex StartVertex;
  Vertex EndVertex;
  LocksManager &LockManager;
  LdbcQuery &Query;
};


#endif /*_LDBCQUERYDESCRIPTION_H_*/
