#include "PropertyList.h"
#include "Graph.h"

#include "Utility.h"

#include <string>
#include "GDBReader.h"

int main(int argc, char * argv[]) {

  Graph gf;
  GDBReader reader(gf);
  reader.readFile("../tests/gd/sndata.gd");
  gf.print();

  return 0;
}
