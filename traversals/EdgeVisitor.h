#ifndef _EDGE_VISITOR_H_ 
#define _EDGE_VISITOR_H_ 

#include "GraphType.h"
template <typename CustomVisitor>
class EdgeVisitor {
public:
  typedef GraphType::EdgePtr  EdgePointer;
  typedef GraphType::EdgePropertyList EdgePropertyList;
public:
  bool visitEdge(EdgePointer ep) {
    return customVisitor().visitEdge(ep);
  }
   bool scheduleEdge(EdgePointer ep) {
    return customVisitor().scheduleEdge(ep);
  }

private:
   CustomVisitor& customVisitor() {
     return *static_cast<CustomVisitor*>(this);
   }
};
#endif /* _EDGE_VISITOR_H_ */
