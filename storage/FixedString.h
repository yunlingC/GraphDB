#ifndef _FIXED_STRING_H_
#define _FIXED_STRING_H_

#include <stdio.h>
#include <string.h>

#include <array>
using namespace std;

class FixedString {
public:
  typedef char FixedStringType;
  typedef char* FixedStringPtrType;
public:
  FixedString() { }

  FixedString(const string & s): _size(64) { 
    cout << "==> s: " << s <<endl;
    strncpy(_string, const_cast<char*>(s.c_str()), _size);
    cout << "==> s: " << _string <<endl;
  }


  FixedString & operator=(FixedString & from) {
    if (this != &from) {
      strncpy(_string, from._string, _size);
    }
  }
  
  FixedString & operator+=(const FixedString & appender){
    strcat(_string, appender._string);
    return *this;
  }

  friend FixedString & operator+(FixedString lhs, const FixedString &rhs) {
    return lhs += rhs;
 }

  int operator==(const FixedString& right) {
    return strcmp(_string, right._string);
  }

  void setString(const string& s) {
    strncpy(_string, const_cast<char*>(s.c_str()), _size);
  }

  FixedStringPtrType getString() {
    return &(_string[0]);
  }

  string std_str() {
    return string(_string);
  }

protected:
  FixedStringType  _string[64];
  unsigned int _size;
};

#endif /* _FIXED_STRING_H_ */
