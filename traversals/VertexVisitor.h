#ifndef _VERTEX_VISITOR_H_
#define _VERTEX_VISITOR_H_

#include "GraphType.h"
//#include "Filter.h"

template<typename CustomVisitor>
class VertexVisitor {
public:
  typedef FixedString KeyType;
  typedef FixedString ValueType;
  typedef GraphType::VertexPtr VertexPointer;
  typedef GraphType::EdgePtr   EdgePointer;
  typedef GraphType::VertexPropertyList VertexPropertyList;
public:
//  void setFilter(Filter & filter) {
//    _f = filter;
//  }

  bool visitVertex(VertexPointer vp) {
    return (customVisitor().visitVertex(vp));
  }


  bool scheduleVertex(VertexPointer vp, EdgePointer ep) {
    return customVisitor().scheduleVertex(vp, ep);
  }

private:
  CustomVisitor& customVisitor() {
    return *static_cast<CustomVisitor*>(this);
  }

//private:
//  Filter _f;
};
#endif /* _VERTEX_VISITOR_H_ */
