#ifndef _CUSTOMVISITOR_H_
#define _CUSTOMVISITOR_H_

#include "VertexVisitor.h"
#include "EdgeVisitor.h"

class VisitedVertex : public VertexVisitor<VisitedVertex> {
public:
  typedef pair<FixedString, bool> ReturnValueType;
public:
  VisitedVertex() { }
  void setFilter(Filter f) {
    _f = f;
  }

  bool visitVertex(VertexPointer vp) {
    FixedString value(_f.getValue());
    FixedString key(_f.getKey());

  cout << "Find vertex with key= " << key << "\tvalue= " << value << endl;
    ReturnValueType rv = vp->getPropertyValue(key); 
    if((rv.second != false) && (rv.first == value))
    {
      cout << "++++Gotcha++++\n" << "Vertex:\t" << vp->getId() << endl;
      return false;
    }
    return true;
  }

private:
  Filter _f;
};


class VisitedEdge: public EdgeVisitor<VisitedEdge>  {
public:
  VisitedEdge() { } 

  void setFilter(Filter f) {
    _f = f;
  }

  bool  visitEdge(EdgePointer ep) {
    return true;
  }

  bool  scheduleEdge(EdgePointer ep ) {
    FixedString type(_f.getType());
    if (ep->getType() == type)
    {
      cout << "Edge:\t" << ep->getId() << endl;
      return true;
    }
    else  {
      cout << "type= " << ep->getType() << endl;
      return false;
    }
  }

private:
  Filter _f;
};

#endif /**_CUSOTMVISITOR_H_*/
