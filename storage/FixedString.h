//===-- storage/FixedString.h - FixedString class type ----------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the base class for key and value type in linked list.
///
//===----------------------------------------------------------------------===//

#ifndef _FIXED_STRING_H_
#define _FIXED_STRING_H_

#include <stdio.h>
#include <string.h>
#if DEBUG
#include <iostream>
#endif

class FixedString {
private:
  static const int FIXSIZE = 32;
public:
  typedef char FixedStringType;
  typedef char* FixedStringPtrType;
public:
  FixedString(): Size(FIXSIZE) { }

  FixedString(const std::string & string): Size(FIXSIZE) { 
    strncpy(String, const_cast<char*>(string.c_str()), Size);
  }

  FixedString & operator=(const FixedString & From) {
    if (this != &From) {
      strncpy(this->String, From.String, Size);
    }
    return *this;
  }
  
  FixedString & operator+=(const FixedString & Appender){
    strcat(String, Appender.String);
    return *this;
  }

  friend std::ostream & operator<<(std::ostream & OutStream, const FixedString & string)  {
    OutStream << string.String ;
    return OutStream;
  }

  friend FixedString & operator+(FixedString Left, const FixedString & Right) {
    return Left += Right;
 }

  friend bool  operator<(const FixedString & Left, const FixedString & Right) {
    return strcmp(Left.String, Right.String) <= 0 ;
  }

  friend bool  operator>(const FixedString & Left, const FixedString & Right) {
    return strcmp(Left.String, Right.String) > 0;
  }

  bool operator==(const FixedString & right) const {
    return strcmp(String, right.String) == 0;
  }

  bool operator!=(const FixedString & right) const {
   return strcmp(String, right.String) != 0;
  }

  void setString(const std::string & str) {
    strncpy(String, const_cast<char*>(str.c_str()), Size);
  }

  FixedStringPtrType getString() {
    return &(String[0]);
  }

  std::string std_str() const {
    return std::string(String);
  }

  unsigned int size() {
    return Size;
  }

protected:
  FixedStringType String[FIXSIZE];
  unsigned int Size;
};

#endif /* _FIXED_STRING_H_ */
