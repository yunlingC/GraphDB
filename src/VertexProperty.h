#ifndef _VERTEX_PROPERTY_H_
#define _VERTEX_PROPERTY_H_

#include "PropertyList.h"

using namespace std;

template <typename KeyType, typename ValueType>
struct VertexProperty {
public:
  PropertyList<KeyType, ValueType > list;
};

#endif /* _VERTEX_PROPERTY_H_ */
