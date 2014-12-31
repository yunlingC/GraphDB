#ifndef _EDGE_PROPERTY_H_
#define _EDGE_PROPERTY_H_

#include "PropertyList.h"

using namespace std;

template <typename KeyType, typename ValueType>
struct EdgeProperty {
public:
  PropertyList<KeyType, ValueType > list;
};

#endif /* _EDGE_PROPERTY_H_ */
