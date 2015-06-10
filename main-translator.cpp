#include <iostream>

#include "LDBCTranslator.h"

///This translator is used on every new dataset from LDBC
int main(int argc, char *argv[]) {

  cout << "Begin reading\n";
  string DirName = "../tests/ldbc/";
  string FileName = "social_network_10";

  if( argc > 1)
    FileName  = argv[1];

  LDBCTranslator translator;
  translator.readDirectory((DirName + FileName).c_str());

  cout << "New folder  is inside original one \n";
  cout << "Finish translating\n";

}
