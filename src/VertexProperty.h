#ifndef _VERTEX_PROPERTY_H_
#define _VERTEX_PROPERTY_H_

#include "PropertyList.h"

using namespace std;

template <typename KeyType, typename ValueType>
struct VertexProperty {
public:
  typedef PropertyList<KeyType, ValueType> PropertyListType;

  void setId(unsigned int id) {
    _id = id;
  }

  void setPropertyList(PropertyListType l) {
    _list = l;
  }

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
