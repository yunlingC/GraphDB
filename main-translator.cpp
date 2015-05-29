#include <iostream>

#include "LDBCTranslator.h"

///This translator is used on every new dataset from LDBC
int main() {

  cout << "Begin reading\n";

  LDBCTranslator translator;
  translator.readDirectory("../tests/ldbc/social_network_part");

  cout << "New folder called New is inside original one \n";
  cout << "Finish translating\n";

}
