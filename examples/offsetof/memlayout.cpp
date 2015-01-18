#include <iostream>
#include <stddef.h>     /* offsetof */

using namespace std;

class A {
public:
  int x;
  int y[10];
  char z;
};

int main() {

  cout << "A.x: " << offsetof(struct A, x) << " bytes" << endl;
  cout << "A.y: " << offsetof(struct A, y) << " bytes" << endl;
  cout << "A.z: " << offsetof(struct A, z) << " bytes" << endl;
  
  return 0;
};
