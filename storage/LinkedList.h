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

#if DEBUG
#include <iostream>
#endif

#include "FixedString.h"
// This class is necessary to have fixed size linked lists.
// These will serve as PropertyLists.

class ListNode {
public:
  typedef FixedString KeyType;
  typedef FixedString ValueType;


  ListNode(KeyType k, ValueType v): _key(k), _value(v), _next(nullptr), _previous(nullptr) {
  }

  ListNode(const ListNode & from) {
    _key = from._key;
    _value = from._value;
    _next = from._next;
    _previous = from._previous;
  }

  ListNode &  operator=(const ListNode & from) {
    if (this != &from) {
      _key = from._key;
      _value = from._value;
      _next = from._next;
      _previous = from._previous;
    }
    return *this;
  }
    
  const KeyType & getKey() {
    return _key;
  }

  const ValueType & getValue() {
    return _value;
  }

  ListNode * getNext() {
    return _next;
  }

  ListNode * getPrevious() {
    return _previous;
  }

  void setNext(ListNode * n) {
    _next = n;
  }

  void setPrevious(ListNode * p) {
    _previous = p;
  }

private:
  KeyType _key;
  ValueType _value;
  ListNode * _next;
  ListNode * _previous;

};

class LinkedList {
public:
  typedef FixedString KeyType;
  typedef FixedString ValueType;

  // Make it work with iterator
  typedef ListNode* iterator;

public:
  LinkedList(): _head(nullptr), _tail(nullptr), _size(0) {
  }

  LinkedList & operator=(const LinkedList & from) {
    if (this != &from) {
      _head = from._head;
      _tail = from._tail;
      _size = from._size;
    }
  }

  void insert(const char * k, const char * v) {
    KeyType kk(k); ValueType vv(v);
    insert(kk, vv);
  }

  void insert(const KeyType & k, const ValueType & v) {
    // This should be a placement new, but we'll allow it for now and change later.
    ListNode * l = new ListNode(k, v);

    // Set head only once.
    if (_head == nullptr) {
      _head = l;
    }

    // Add to end of linked list.
    // Set current node
    l->setNext(nullptr);
    l->setPrevious(_tail);
    // Set previous node
    if (_tail != nullptr) {
      _tail->setNext(l);
    }
    _tail = l;
    ++_size;
  }

  ListNode * find(const KeyType & k) {
    ListNode * it = _head;
    ListNode * fit = nullptr;
    bool found = false;
    while ( it != nullptr && found == false ) {
      if ( it->getKey() == k ) {
	fit = it;
	found = true;
      }
      it = it->getNext();
    }
    return fit;
  }

  void print() {
    // Iterate over the linked list and delete it.
    ListNode * it = _head;
    ListNode * del = nullptr;
    
    while (it != nullptr) {
      cout << "k: " << it->getKey() << ", " << "v: " << it->getValue() << endl;
      it = it->getNext();

    }
  }

  unsigned int size() {
    return _size;
  }

  ListNode* begin() {
    return _head;
  }

  ListNode* end() {
    return _tail;
  }

  void deleteList() {
    // Iterate over the linked list and delete it.
    ListNode * it = _head;
    ListNode * del = nullptr;
    
    while (it != nullptr) {
      del = it;
      it = it->getNext();
      delete del;
    }
  }

  

private:
  ListNode * _head;
  ListNode * _tail;
  unsigned int _size;
};

#endif /* _LINKED_LIST_H_ */
