#ifndef _PROPERTY_LIST_H_
#define _PROPERTY_LIST_H_

#include <map>

// TODO: This should be replaced with debug function.
#include <iostream>
//#include <string>
//#include "FixedString.h"

using namespace std;

template <typename KeyType, typename ValueType>
struct PropertyList {

public:  
  typedef typename map<KeyType, ValueType>::iterator mapIteratorType;
  typedef pair<ValueType, bool> ReturnValueType;

public:
PropertyList(): _maxSize(10) {
}

PropertyList(unsigned int s): _maxSize(0) {

}

  bool set(const std::string & k, const std::string & v) {
    KeyType kk(k); ValueType vv(v);
    set(kk, vv);
  }
 
  bool set(const KeyType & k, const ValueType & v) {
    if (_pl.size() >= _maxSize) {
      return false;
    }

    KeyType   kt(k);
    ValueType vt(v);
    pair<KeyType, ValueType> p(kt,vt);
    _pl.insert(p);
    return true;
  }

  void remove(const KeyType & k) {
    KeyType kt(k);
    _pl.erase(kt);
  }

  ReturnValueType get(const KeyType & k) {
    // Only initialize if known type parameters.
    // Assume <string, bool>
    ValueType str("null");
    ReturnValueType rv(str, true);

    KeyType kt(k);
    mapIteratorType mi = _pl.find(kt);
    // Check if it's not found.
    if (mi == _pl.end()) {
      // Set the bool flag if not found.
      rv.second = false;
      return rv;
    }
    //    cout << "copy value: " << mi->first << ", " << mi->second << endl;
    // Copy the value.
    rv.first = mi->second;
    return rv;
  }

  // There is no need for the following.
  // - Copy constructor, assignment operator
  // This is only true if the key, value is assumed to be a simple type.

  // Copy constructor.
  // PropertyList(const PropertyList & from) {
    
  // }

  void print() {
    mapIteratorType mbeg, mend;
    for (mbeg = _pl.begin(), mend = _pl.end(); mbeg != mend; mbeg++) {
      cout << "[" << mbeg->first.std_str() << ", " << mbeg->second.std_str() << "]" << endl;
    }
  }
private:
  map<KeyType, ValueType> _pl;
  unsigned int _maxSize;
};

#endif /* _PROPERTY_LIST_H_ */
