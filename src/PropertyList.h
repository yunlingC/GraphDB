#ifndef _PROPERTY_LIST_H_
#define _PROPERTY_LIST_H_

#if DEBUG
#include <iostream>
#endif 

#include "LinkedList.h"

using namespace std;

template <typename KeyType, typename ValueType>
class PropertyList {
public:  
  typedef LinkedList::iterator mapIteratorType;
  typedef pair<ValueType, bool> ReturnValueType;

public:
  PropertyList(): _maxSize(10) {
  }

  PropertyList(unsigned int s): _maxSize(0) {

  }

  PropertyList & operator=(const PropertyList & from) {
    if (this != &from) {
      _pl = from._pl;
      _maxSize = from._maxSize;
    }
    return *this;
  }

  void deletePropertyList() {
    // Must manually delete the linked list.
    if (_pl.size() > 0) 
      _pl.deleteList();

  }    

  ~PropertyList() {
    // Must explicitly call delete
  }

  bool set(const std::string & k, const std::string & v) {
    KeyType kk(k); ValueType vv(v);
    return set(kk, vv);
  }
 
  bool set(const KeyType & k, const ValueType & v) {
    if (_pl.size() >= _maxSize) {
      return false;
    }

    KeyType   kt(k);
    ValueType vt(v);
    _pl.insert(kt, vt);
    return true;
  }

  /// in support for update propertylist -Yunling
  // update the value with key == key to value
  bool update(const KeyType & key, const ValueType & value) {
    ReturnValueType returnValue = get(key);
    if (returnValue.second == false) {
      return false;
    } else {
      return _pl.update(key, value);
    }
  }

  ReturnValueType get(const string & k) {
    KeyType Key(k);
    return get(Key);
  }

  ReturnValueType get(const KeyType & k) {
    // Only initialize if known type parameters.
    // Assume <string, bool>
    ValueType str("null");
    ReturnValueType rv(str, true);

    mapIteratorType mi = _pl.find(k);
    // Check if it's not found.
    if (mi == nullptr) {
      // Set the bool flag if not found.
      rv.second = false;
      return rv;
    }
    // Copy the value.
    rv.first = mi->getValue();
    return rv;
  }

  // There is no need for the following.
  // - Copy constructor, assignment operator
  // This is only true if the key, value is assumed to be a simple type.

  // Copy constructor.
  // PropertyList(const PropertyList & from) {
    
  // }

  unsigned int size() {
    return _pl.size();
  }

  void print() {
    _pl.print();
  }

private:
  LinkedList _pl;
  unsigned int _maxSize;
};

#endif /* _PROPERTY_LIST_H_ */
