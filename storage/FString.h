#ifndef _F_STRING_H_
#define _F_STRING_H_

#include <iostream>
#include <string>

using namespace std;

class FString {
public:
  FString(const string & s) {
    _size = 64;
    _s = new string(s.c_str(), _size);
    cout << "_s : " << _s << ", size: " << _s->size() << endl;
    //    _s->replace(s.size(), 1, "\0");
    cout << "_s : " << _s << ", size: " << _s->size() << endl;
  }

  FString(): _s(nullptr), _size(64) {
  }

  ~FString() {
    if (_s != nullptr) {
      delete _s;
    }
  }

  FString & operator=(FString & from) {
    if ((this != &from) && isSafeString(from)) {
      *_s = from.getString();
    }
  }

  bool operator==(FString & rhs) {
    bool f =false;
    if (isSafeString(rhs)) {
      string s = rhs.getString();
      if (s == *_s)
	f = true;
    }   
    return f;
  }

  void setString(const string & s) {
    if (isSafeString(s)) {
      *_s = s;
    }
  }

  const string & getString() {
    return *_s;
  }

  bool isSafeString(FString & s) {
    return ( (s.getString().size() <= _size) && (_s != nullptr) );
  }

  bool isSafeString(const string & s) {
    return ( (s.size() <= _size) && (_s != nullptr) );
  }

  unsigned int getSize() {
    return _s->size();
  }
private:
  string * _s;
  unsigned int _size;
};

#endif /* _F_STRING _H_ */
