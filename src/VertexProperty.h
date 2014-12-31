#ifndef _VERTEX_PROPERTY_H_
#define _VERTEX_PROPERTY_H_

#include <string>
#include <iostream>

#include "PropertyList.h"

using namespace std;

struct VertexProperty {
public:
  PropertyList<string, string > list;
};

#endif /* _VERTEX_PROPERTY_H_ */
