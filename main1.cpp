#include "PropertyList.h"
#include "Graph.h"

#include "Utility.h"

#include <string>
#include "readers/FileReader.h"

int main(int argc, char * argv[]) {

  Graph gf;
  GDReader reader(gf);
  reader.readFile("sndata.gd");
  gf.print();

  return 0;
}
