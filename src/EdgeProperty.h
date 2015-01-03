#ifndef _EDGE_PROPERTY_H_
#define _EDGE_PROPERTY_H_

#include <string>
#include "PropertyList.h"

using namespace std;

template <typename KeyType, typename ValueType>
struct EdgeProperty {

public:
  typedef PropertyList<KeyType, ValueType > PropertyListType;

  unsigned int getId() {
    return _id;
  }

  string getType() {
    return _type;
  }

  PropertyListType & getPropertyList() {
    return _list;
  }

  void setId(unsigned int id) {
    _id = id;
  }

  void setPropertyList(PropertyListType l) {
    _list = l;
  }

  void setType(string t) {
    _type = t;
  }

protected:
  // Edge id.
  unsigned int _id;

  // Label to denote type.
  string _type;

  // Property list/map.
  PropertyListType _list;
};

#endif /* _EDGE_PROPERTY_H_ */
