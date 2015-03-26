#ifndef _FIXED_STRING_H_
#define _FIXED_STRING_H_

#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

class FixedString {
public:
  typedef char FixedStringType;
  typedef char* FixedStringPtrType;
public:
  FixedString(): _size(64) { 

  }

  FixedString(const string & s): _size(64) { 
//    cout << "==> s: " << s <<endl;
    strncpy(_string, const_cast<char*>(s.c_str()), _size);
//    cout << "==> s: " << _string <<endl;
  }

  FixedString & operator=(const FixedString & from) {
//    cout << "FixedString:: assignment operator\n";
    if (this != &from) {
      strncpy(this->_string, from._string, _size);
    }
    return *this;
  }
  
  FixedString & operator+=(const FixedString & appender){
    strcat(_string, appender._string);
    return *this;
  }

  friend std::ostream & operator<<(std::ostream & os, const FixedString & s)  {
    os << s._string ;
    return os;
  }

  friend FixedString & operator+(FixedString lhs, const FixedString &rhs) {
    return lhs += rhs;
 }

  friend bool  operator<(const FixedString & lhs, const FixedString & rhs) {
    if(strcmp(lhs._string, rhs._string) <= 0)
      return true;
    else 
      return false;
  }

  friend bool  operator>(const FixedString & lhs, const FixedString & rhs) {
    if(strcmp(lhs._string, rhs._string) > 0)
      return true;
    else 
      return false;
  }

  bool operator==(const FixedString & right) const {
    if(strcmp(_string, right._string) == 0)
      return true;
    else 
      return false;
  }

  bool operator!=(const FixedString & right) const {
   if(strcmp(_string, right._string) != 0)
     return true;
   else 
     return false;
  }

  void setString(const string& s) {
    strncpy(_string, const_cast<char*>(s.c_str()), _size);
  }

  FixedStringPtrType getString() {
    return &(_string[0]);
  }

  std::string std_str() const {
    return string(_string);
  }

  unsigned int size() {
    return _size;
  }

protected:
  FixedStringType _string[64];
  unsigned int _size;
};

#endif /* _FIXED_STRING_H_ */
