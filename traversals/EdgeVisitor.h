#ifndef _EDGE_VISITOR_H_ 
#define _EDGE_VISITOR_H_ 

#include "GraphType.h"

class EdgeVisitor {
public:
  typedef GraphType::EdgePtr  EdgePointer;
  typedef GraphType::EdgePropertyList EdgePropertyList;
public:
//  void setFilter(Filter & filter) {
//    _f = filter;
//  }

  bool visitEdge(EdgePointer ep) {
    return customVisitor().visitEdge(ep);
  }
   bool scheduleEdge(EdgePointer ep) {
    return customVisitor().scheduleEdge(ep);
  }

private:
//private:
//   Filter _f;
};
#endif /* _EDGE_VISITOR_H_ */
