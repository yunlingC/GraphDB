//===-- storage/LinkedList.h - Linked List class type -----------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is a basic linked list implementation.
///
//===----------------------------------------------------------------------===//
#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include "FixedString.h"

#if DEBUG
#include <iostream>
#endif

// This class is necessary to have fixed size linked lists.
// These will serve as PropertyLists.

class ListNode {
public:
  typedef FixedString KeyType;
  typedef FixedString ValueType;
public:
  ListNode(KeyType key, ValueType value): _Key(key)
                                        , _Value(value)
                                        , _Next(nullptr)
                                        , _Previous(nullptr) {}

  ListNode(const ListNode & From) {
    _Key = From._Key;
    _Value = From._Value;
    _Next = From._Next;
    _Previous = From._Previous;
  }

  ListNode &  operator=(const ListNode & From) {
    if (this != &From) {
      _Key = From._Key;
      _Value = From._Value;
      _Next = From._Next;
      _Previous = From._Previous;
    }
    return *this;
  }
    
  const KeyType & getKey() {
    return _Key;
  }

  const ValueType & getValue() {
    return _Value;
  }

  void updateValue(const ValueType & value) {
    _Value = value;
  }

  ListNode * getNext() {
    return _Next;
  }

  ListNode * getPrevious() {
    return _Previous;
  }

  void setNext(ListNode * next) {
    _Next = next;
  }

  void setPrevious(ListNode * prev) {
    _Previous = prev;
  }

private:
  KeyType _Key;
  ValueType _Value;
  ListNode * _Next;
  ListNode * _Previous;
};

class LinkedList {
public:
  typedef FixedString KeyType;
  typedef FixedString ValueType;
  // Make it work with iterator
  typedef ListNode* iterator;

public:
  LinkedList(): _Head(nullptr)
              , _Tail(nullptr)
              , _Size(0) {}

  LinkedList & operator=(const LinkedList & From) {
    if (this != &From) {
      _Head = From._Head;
      _Tail = From._Tail;
      _Size = From._Size;
    }
    return *this;
  }

  void insert(const char * key, const char * value) {
    KeyType Key(key); 
    ValueType Value(value);
    insert(Key, Value);
  }

  void insert(const KeyType & key, const ValueType & value) {
    /// TODO This should be a placement new, 
    /// but we'll allow it for now and change later if necessary
    ListNode * NewNode = new ListNode(key, value);

    // Set head only once.
    if (_Head == nullptr) {
      _Head = NewNode;
    }

    // Add to end of linked list.
    // Set current node
    NewNode->setNext(nullptr);
    NewNode->setPrevious(_Tail);

    // Set previous node
    if (_Tail != nullptr) {
      _Tail->setNext(NewNode);
    }
    _Tail = NewNode;

    ++_Size;
  }

  ListNode * find(const KeyType & key) {
    ListNode * it = _Head;
    ListNode * fit = nullptr;
    bool found = false;
    while ( it != nullptr && found == false ) {
      if ( it->getKey() == key ) {
      	fit = it;
      	found = true;
      }
      it = it->getNext();
    }
    return fit;
  }
  
  bool update(const KeyType & key, const ValueType & value) {
    ListNode * fit = find(key);
    if (fit == nullptr) 
      return false;
    else {
      fit->updateValue(value);
      return true;
    }
  }

#if DEBUG
  void print() {
    // Iterate over the linked list and delete it.
    ListNode * it = _Head;
    
    while (it != nullptr) {
      std::cout << "key: " << it->getKey() << ", " 
                << "value: " << it->getValue() 
                << std::endl;
      it = it->getNext();
    }
  }
#endif

  unsigned int size() {
    return _Size;
  }

  ListNode* begin() {
    return _Head;
  }

  ListNode* end() {
    return _Tail;
  }

  void deleteList() {
    // Iterate over the linked list and delete it.
    ListNode * it = _Head;
    ListNode * del = nullptr;
    
    while (it != nullptr) {
      del = it;
      it = it->getNext();
      delete del;
    }
  }

private:
  ListNode * _Head;
  ListNode * _Tail;
  unsigned int _Size;
};

#endif /* _LINKED_LIST_H_ */
