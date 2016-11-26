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
//#include "LdbcUpdateVisitor.h"
//#include "ConcurrentBFS.h"
#include "QueryDescription.h"
#ifdef _INDEXING_
#include "Index.h"
#endif

#include <vector>
#include <string>
#include <time.h>
#include <unordered_map>

#ifdef _PRINTLOG_
#include <iostream>
#endif

#define CLOCK_ID  CLOCK_THREAD_CPUTIME_ID
#define MILLION 1000000
#define NANO 1000000000
#define SCALE 1000

class Transaction;
/// This is the base class for LdbcQuery
class LdbcQuery : public Query{
public:
  typedef std::pair<std::string, std::string> ParamPairType;
  typedef std::pair<std::string, std::pair<std::string, std::string> > RangePairType;
  typedef LocksManager LockManagerType;
  typedef Transaction* TransactionPointerType;
  typedef GraphType::PropertyListType PropertyListType;
  typedef std::string LabelType;
  typedef std::string KeyType;
  typedef std::string ValueType;
  typedef Index IndexType;

public:
  LdbcQuery(unsigned int Id) : QueryId(Id) {
    LdbcFile.open("ldbc"+std::to_string(Id)+".log", std::ios_base::out| std::ios_base::app);
    std::cout <<"LdbcQuery\t" << QueryId << "\tconstructor opened file\n";
  }

  ~LdbcQuery() {
    LdbcFile.close();
    std::cout <<"LdbcQuery\t" << QueryId << "\tdestructor closed file\n";
  }

  virtual void runQuery(Graph & graph\
              , VertexDescriptor StartVertex\
              ) { }

  virtual void runQuery(Graph & graph\
              , LockManagerType & LockManager\
              ) { }

  virtual void runQuery(Graph & graph\
              , VertexDescriptor StartVertex\
              , LockManagerType & LockManager\
              ) {}

  virtual void runQuery(Graph & graph\
              , VertexDescriptor StartVertex\
              , VertexDescriptor EndVertex\
              , LockManagerType & LockManager\
              ) {}

	virtual void runQuery(Graph & graph\
						, VertexDescriptor StartVertex\
						, TransactionPointerType tranx\
		) {}

	virtual void runQuery(Graph & graph\
						, VertexDescriptor StartVertex\
            , Visitor  & GraphVisitor\
						, TransactionPointerType tranx\
            , LockManagerType & LockManager\
		){}

	virtual void runQuery(Graph & graph\
						, VertexDescriptor StartVertex\
            , Visitor  & GraphVisitor\
						, TransactionPointerType tranx\
            , LockManagerType & LockManager\
            , IndexType & Index\
		){}

//  virtual void initEdge(LabelType label) {}
//
//  virtual void initEdge(LabelType label, PropertyListType PropList) {}
//
//  virtual void initEdge(LabelType label, std::string Key, std::string Value);
//
//  virtual void initEdge(bool isNewVertexFirst
//              , LabelType edgeLabel
//              , LabelType vertexLabel
//              , ValueType existVertexId
//              , std::string Key
//              , std::string Value){}
  
  ///This function is to support transaction/Query aborts
  //Clear all member storage for a restart
  virtual void clearStorage() { }

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

    uint64_t execTime = (( End.tv_sec - Start.tv_sec )*NANO
                  + ( End.tv_nsec - Start.tv_nsec))/SCALE;

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
/* **** Inactive ******/
/**
class LDBCQuery : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
  typedef FixedString KeyType;
  typedef FixedString ValueType;
  typedef PropertyList< FixedString, FixedString> PropertyListType;
  typedef PropertyListType & PropertyListTypeReference;
public:
  auto setPropertyList(PropertyListTypeReference PropList) 
    -> void {
    PropertyList = PropList; 
  }
protected:
  PropertyListType PropertyList;
};
*/

class LdbcAddEdgeQuery : public LdbcQuery {
  using LdbcQuery::LdbcQuery;
public:
//  typedef std::string LabelType;
//  typedef std::string KeyType;
//  typedef std::string ValueType;
//public:

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
    PropList.set(Key, Value);
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

//  virtual void runQuery(Graph & graph
//	  					, VertexDescriptor StartVertex
//              , Visitor & GraphVisitor
//	  					, TransactionPointerType &tranx
//              , LockManagerType LockManager
//	  	);
//
protected:
  EdgePointer NewEdge;
  ValueType FirstId;
  ValueType SecondId;
  LabelType   FirstLabel;
  LabelType   SecondLabel;
};


class LdbcAddVertexQuery : public LdbcAddEdgeQuery {
  using LdbcAddEdgeQuery::LdbcAddEdgeQuery;
public:
//  typedef std::pair<std::string, std::string> VertexPairType;
//  typedef std::vector<VertexPairType>  EdgeListType;
  typedef std::string ValueType;
  typedef std::string LabelType;
  typedef std::pair<ValueType, LabelType> VertexInfoPairType;
  typedef std::unordered_map<EdgePointer, VertexInfoPairType> EdgeMapType;
  typedef std::pair<EdgePointer, VertexInfoPairType> EdgeMapEntryType;
//  typedef std::pair<ValueType, ValueType, LabelType, std::string, std::string> EdgeInitPairType;
//  typedef std::vector<EdgeInitPairType> EdgeListType;
public:

  void initVertex(LabelType label, PropertyListType PropList) {
    NewVertex = new Vertex();  
    NewVertex->setType(label);
    NewVertex->setPropertyList(PropList);
  }

  void initEdge(bool isNewVertexFirst
              , LabelType edgeLabel
              , LabelType vertexLabel
              , ValueType existVertexId
              , std::string Key
              , std::string Value)  {

    EdgePointer NewEdge = new Edge(); 
    NewEdge->setType(edgeLabel);
    PropertyListType PropList;
    PropList.set(Key, Value);
    NewEdge->setPropertyList(PropList);
    if (isNewVertexFirst) {
      NewEdge->setFirstVertexPtr(NewVertex);
    } 
    else {
      NewEdge->setSecondVertexPtr(NewVertex);
    }
    EdgeMap.insert(EdgeMapEntryType(NewEdge, VertexInfoPairType(vertexLabel,existVertexId)));
  }

//  virtual void runQuery(Graph & graph
//	  					, VertexDescriptor StartVertex
//	  					, TransactionPointerType &tranx
//  );

protected:
  ValueType NewVertexIndex;
  VertexPointer NewVertex;
  EdgeMapType EdgeMap;
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
