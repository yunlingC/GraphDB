#ifndef _VERTEX_PROPERTY_H_
#define _VERTEX_PROPERTY_H_

#include "PropertyList.h"

using namespace std;

template <typename KeyType, typename ValueType>
struct VertexProperty {
public:
  typedef PropertyList< KeyType, ValueType > PropertyListType;

  typedef pair<ValueType, bool> ReturnPropertyValueType;
public:
  unsigned int getId() {
    return _id;
  }

  ReturnPropertyValueType getPropertyValue(KeyType k) {
    return _list.get(k);
  }

  void setId(unsigned int id) {
    _id = id;
  }

  void setProperty(KeyType k, ValueType v) {
    _list.set(k,v);
  }

  void setPropertyList(PropertyListType l) {
    _list = l;
  }

  // Remember you can change this when returned.
  PropertyListType &  getPropertyList() {
    return _list;
  }

protected:
  // Vertex id.
  unsigned int _id;
  // Property list.
  PropertyListType _list;
};

#endif /* _VERTEX_PROPERTY_H_ */
