//===-- storage/PropertyList.h - PropertyList type ---------------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the main class for the Property list.
///
//===----------------------------------------------------------------------===//

#ifndef _PROPERTY_LIST_H_
#define _PROPERTY_LIST_H_

#include "LinkedList.h"

#if DEBUG
#include <iostream>
#endif 

template <typename KeyType, typename ValueType>
class PropertyList {
private:
  static const int MAXSIZE = 10;
public:
  typedef LinkedList::iterator mapIteratorType;
  typedef std::pair<ValueType, bool> ReturnValueType;
public:
  PropertyList(): _MaxSize(MAXSIZE) { }

  PropertyList(unsigned int size): _MaxSize(size) { }

  PropertyList & operator=(const PropertyList & From) {
    if (this != &From) {
      _LinkedList = From._LinkedList;
      _MaxSize = From._MaxSize;
    }
    return *this;
  }

  void deletePropertyList() {
    // Must manually delete the linked list.
    if (_LinkedList.size() > 0) 
      _LinkedList.deleteList();
  }    

  ~PropertyList() {
    // Must explicitly call delete    
  }

  bool set(const std::string & key, const std::string & value) {
    KeyType Key(key); ValueType Value(value);
    return set(Key, Value);
  }
 
  bool set(const KeyType & key, const ValueType & value) {
    if (_LinkedList.size() >= _MaxSize) {
      return false;
    }

    KeyType   Key(key);
    ValueType Value(value);
    _LinkedList.insert(Key, Value);
    return true;
  }

  /// in support for update propertylist -Yunling
  /// update the value with key == key to value
  bool update(const KeyType & key, const ValueType & value) {
    ReturnValueType returnValue = get(key);
    if (returnValue.second == false) {
      return false;
    } else {
      return _LinkedList.update(key, value);
    }
  }

  ReturnValueType get(const std::string & key) {
    KeyType Key(key);
    return get(Key);
  }

  ReturnValueType get(const KeyType & key) {
    // Only initialize if known type parameters.
    // Assume <string, bool>
    ValueType value("null");
    ReturnValueType rv(value, true);

    mapIteratorType mi = _LinkedList.find(key);
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

  unsigned int size() {
    return _LinkedList.size();
  }

#if DEBUG
  void print() {
    _LinkedList.print();
  }
#endif

private:
  LinkedList _LinkedList;
  unsigned int _MaxSize;
};

#endif /* _PROPERTY_LIST_H_ */
