#ifndef _FIXED_STRING_H_
#define _FIXED_STRING_H_

#include <stdio.h>
#include <string.h>

using namespace std;

class FixedString {
public:
  typedef char FixedStringType;
public:
  FixedString() { }
  FixedString & operator=(FixedString & from) {
    if (this != &from) {

    }
  }
  
  string str() {
    return string(_string);
  }

protected:
  FixedStringType  _string[64];
};
#endif /* _FIXED_STRING_H_ */
