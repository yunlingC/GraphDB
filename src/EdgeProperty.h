#ifndef _EDGE_PROPERTY_H_
#define _EDGE_PROPERTY_H_

#include <string>
#include <iostream>

#include "PropertyList.h"

using namespace std;

struct EdgeProperty {
public:
  PropertyList<string, string > list;
};

#endif /* _EDGE_PROPERTY_H_ */
