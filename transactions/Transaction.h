//===-- transactions/Transaction.h - Log class --*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the main class for managing log records for transactions
//.
///
//===----------------------------------------------------------------------===//

#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include <map>

#include "GraphType.h"

enum OperationType { NOOP, READ, UPDATE, INSERT, DELETE };

class Transaction {
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
  Transaction () : Commit(false), Abort(false), OpType(NOOP) { }

  auto setOperationType (OperationType opT)
    -> void {
      OpType = opT; 
  }

  auto setTxId(unsigned int id) 
    -> void {
      Id = id;
  }

  auto getTxId() 
    -> unsigned int {
    return Id;
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

  ~Transaction() { }

protected:
  unsigned int Id;
  bool Commit ;
  bool Abort ;
  OperationType  OpType;
  VertexPairListType VertexList;
  EdgePairListType   EdgeList;
  VertexPropPairListType VertexPropList;
  EdgePropPairListType EdgePropList;

};

#endif /**_TRANSACTION_H_*/
