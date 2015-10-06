#include <algorithm>
#include <iostream>
#include <thread>

#include "LDBCReader.h"
#include "LocksManager.h"
#include "LdbcConcurrentQuery.h"

int main( int argc, char * argv[]) {

  typedef GraphType Graph;

  Graph g;
  LDBCReader reader(g);

#ifdef _FIXALLOC_
  g.allocVertexMemory(400000);
  g.allocEdgeMemory(2000000);
#endif /* _FIXALLOC */

  reader.readDirectory("../tests/ldbc/social_network_1500/New");

}
