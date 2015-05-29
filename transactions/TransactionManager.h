//===-- transactions/TransactionManager.h - transaction class --*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the main class for managing transactions
//.
///
//===----------------------------------------------------------------------===//

#ifndef _TRANSACTIONMANAGER_H_
#define _TRANSACTIONMANAGER_H_

#include <map>

#include "GraphType.h"

enum OperationType { NOOP, UPDATE, INSERT, DELETE };

class LogRecord {
public:
  typedef GraphType::VertexPointer Vertex;
  typedef GraphType::EdgePointer   Edge;
  typedef GraphType::VertexPropertyList VProp;
  typedef GraphType::EdgePropertyList EProp;
  typedef std::pair<Vertex, Vertex> VertexPair;
  typedef std::pair<Edge,   Edge> EdgePair;
  typedef std::pair<VProp,  VProp> VPropPair;
  typedef std::pair<EProp,  EProp> EPropPair;
  typedef std::vector<VertexPair> VertexPairListType;
  typedef std::vector<EdgePair>   EdgePairListType;
  typedef std::vector<VPropPair> VertexPropPairListType;
  typedef std::vector<EPropPair> EdgePropPairListType;
  
public:
  LogRecord () : Commit(false), Abort(false), OpType(NOOP) { }

  auto setOperationType (OperationType opT)
    -> void {
      OpType = opT; 
  }

  auto setVertexPropertyPair(const VPropPair & vp) 
    -> void {
      VertexPropList.push_back(vp);

    }
  auto setCommit() 
    -> void {
      Commit = true;
  }

  auto setAbort() 
    -> void {
      Abort = true;
  }

  auto checkStatus() 
    -> bool {
    /// no need to rollback
    if(Commit)  
      return true;
    /// must rollback
    if(Abort)
      return false;
    /// could be not finished, no rollback
    return true;
  }

  auto checkOperationType()
    -> OperationType {
    return OpType;
  }
  ~LogRecord() { }

protected:
  bool Commit ;
  bool Abort ;
  OperationType  OpType;
  VertexPairListType VertexList;
  EdgePairListType   EdgeList;
  VertexPropPairListType VertexPropList;
  EdgePropPairListType EdgePropList;

};

class TransactionManager {
public:
  typedef LogRecord *  LogRecordPointer;
  typedef std::map<unsigned int, LogRecordPointer> TransactionTableType;
public:
  TransactionManager() : TransNumber(0) {}

  auto initLogRecord() 
    -> LogRecordPointer {
      LogRecordPointer log = new LogRecord;
      return log;
  }

  auto addLogRecord(unsigned int tid, LogRecordPointer log) 
    -> bool  {
      if(TransTable.find(tid) != TransTable.end())
        return false;
      TransTable.insert(std::pair<unsigned int, LogRecordPointer>(tid, log));
      return true;
  }

  auto addTransaction()
    -> LogRecordPointer {
      LogRecordPointer log = new LogRecord;
      TransTable.insert(std::pair<unsigned int, LogRecordPointer>(TransNumber, log));
      TransNumber++;
      return log;
  }
  
  auto rollBack(GraphType & graph) 
    -> bool {
    for (auto it = TransTable.begin(); 
          it != TransTable.end(); it++) {
      if(!(*it).second->checkStatus()) {
        switch ((*it).second->checkOperationType()) {
          case NOOP:
            break;
          case UPDATE:
            undoUpdate(graph, (*it).second);
            break;
          case INSERT:
            undoInsert(graph, (*it).second);
            break;
          case DELETE:
            undoDelete(graph, (*it).second);
            break;
          default:
            break;
        }//switch
      }//if
    }
    return true;
  }

  ~TransactionManager() { 
    
    for(auto it = TransTable.begin(); 
        it != TransTable.end(); it++) {
      delete (*it).second;
    }
  }

private:
  auto undoUpdate(GraphType & graph, LogRecordPointer log) 
    -> void {
  }

  auto undoInsert(GraphType & graph, LogRecordPointer log) 
    -> void {
    
  }

  auto undoDelete(GraphType & graph, LogRecordPointer log) 
    -> void {
    
  }

protected:
  unsigned int TransNumber;
  TransactionTableType TransTable; 
};

#endif /**_TRANSACTIONMANAGER_H_*/
