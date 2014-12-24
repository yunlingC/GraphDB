#ifndef _PROPERTY_LIST_H_
#define _PROPERTY_LIST_H_

#include <map>

// TODO: This should be replaced with debug function.
#include <iostream>

using namespace std;

template <typename KeyType, typename ValueType>
struct PropertyList {
  
  //  typedef K KeyType;
  //  typedef V ValueType;
  typedef typename map<KeyType, ValueType>::iterator mitType;

  bool insert(const KeyType & k, const ValueType & v) {
    pair<KeyType, ValueType> p(k,v);
    _pl.insert(p);
    return true;
  }

  void remove(const KeyType & k) {
    _pl.erase(k);
  }

  mitType  get(const KeyType & k) {
    return _pl.find(k);
  }

  void print() {
    mitType mbeg, mend;
    for (mbeg = _pl.begin(), mend = _pl.end(); mbeg != mend; mbeg++) {
      cout << "[" << mbeg->first << ", " << mbeg->second << "]" << endl;
    }
  }
private:
  map<KeyType, ValueType> _pl;
};

#endif /* _PROPERTY_LIST_H_ */
