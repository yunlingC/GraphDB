#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <iostream>
using namespace std;

// General algorithm to print from iterators.
template <typename I>
void print(I b, I e) {
  for (I i = b; i != e; i++) {
    cout << *i << " ";
  }
  cout << endl;
}
#endif /* _UTILITY_H_ */
