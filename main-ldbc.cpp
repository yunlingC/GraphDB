#include <algorithm>
#include <iostream>
#include <thread>

#include "LDBCReader.h"
#include "LinkedList.h"


int main() {

  typedef GraphType Graph;
  typedef Graph::VertexDescriptor VertexDescriptor;

  Graph g;
  cout << "Begin testing\n";

  LDBCReader reader(g);
//  reader.readPerson("../tests/ldbc/social_network/person_0.csv");
//  reader.readFriends("../tests/ldbc/social_network/person_knows_person_0.csv");
//  reader.readProperty("../tests/ldbc/social_network/person_email_emailaddress_0.csv");
  reader.readDirectory("../tests/ldbc/social_network");

}

