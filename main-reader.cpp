//#include "macros.h"
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <queue>
#include <thread>
#include <string>

#include "GDBench/util.h"
#include "FreeReader.h"

int main(int argc, char *argv[]) {

//  GraphType g;
  cout << "Begin testing\n";

  GDReader reader;
  reader.readFile("../tests/gd/sndata50000.gd");

  SimRoiStart();

  SimRoiEnd();

  cout << "finish testing\n";
  return 0;
}
