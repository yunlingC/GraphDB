#ifndef _VERTEX_VISITOR_H_
#define _VERTEX_VISITOR_H_

#include "GraphType.h"

class VertexVisitor {
public:
  typedef GraphType::VertexPtr VertexPointer;
public:
  virtual bool visitVertex(VertexPointer vp) {
    return true;
  }

  virtual bool scheduleVertex(VertexPointer vp) {
    return true;
  }
};
#endif /* _VERTEX_VISITOR_H_ */
