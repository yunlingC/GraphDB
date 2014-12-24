#include "PropertyList.h"

#include <string>

int main(int argc, char * argv[]) {
  
  PropertyList<string,string> p;

  p.insert("a", "1");
  p.insert("b", "2");
  p.insert("c", "3");
  
  p.remove("b");

  p.print();


  PropertyList<string,string>::mitType mi = p.get("c");
  cout << "first: " << mi->first << ", second: " << mi->second << endl;

  return 0;
}
