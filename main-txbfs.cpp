#include "LDBCReader.h"
#include "TranxBreadthFirstSearch.h"
#include "TransactionManager.h"

#include <iostream>

using namespace std;

#define _DEBUG_ENABLE_  true

int main() {

  typedef Visitor VisitorType;
  GraphType g;

  cout << "Begin testing\n"; 
  LDBCReader reader(g);
  reader.readDirectory("../tests/ldbc/social_network_100");

  auto persons = reader.getPersonList();
  cout << "Finish reading \n";

  LocksManager LkManager;
  TransactionManager TmManager;
  VisitorType Visitor;

//  auto TxId = TmManager.addTransaction();

  auto TxPtr = TmManager.addTransaction();

  tranxBreadthFirstSearch(g, 0, Visitor, TxPtr, LkManager);

}

