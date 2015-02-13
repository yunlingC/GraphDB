#ifndef _CUSTOMVISITOR_H_
#define _CUSTOMVISITOR_H_

#include "VertexVisitor.h"
#include "EdgeVisitor.h"
#include "Filter.h"
using namespace std;

class VisitedVertex : public VertexVisitor<VisitedVertex> {
public:
  typedef pair<FixedString, bool> ReturnValueType;
public:
  VisitedVertex() { }
  void setFilter(Filter & f) {
    _f = f;
  }

  bool visitVertex(VertexPointer vp) {
    if ((_f.getValue() == "") || (_f.getKey() == "") )
      return false; 

    FixedString value(_f.getValue());
    FixedString key(_f.getKey());

    ReturnValueType rv = vp->getPropertyValue(key); 
    if((rv.second != false) && (rv.first == value))
    {
//      cout << "++++Gotcha++++\n" << "Vertex:\t" << vp->getId() << endl;
      return true;
    }
    return false;
  }
 /// to select specified direction
  bool scheduleVertex(VertexPointer vp, EdgePointer ep) {
    int direction = 0;

    if(_f.getDirection() == "" )
      return true;

    if (ep->getFirstId() == vp->getId())
      direction = 1;     //first vertex == outEdge;
    else if ( ep->getSecondId() == vp->getId())
      direction = 2;     //second vertex == inEdge;
    else 
      cout <<"Err: Irrelevant edges.\n";

    switch(direction) {
    case 1:
      if (_f.getDirection() == "out") {
//        cout << "one catch on vertex: " << vp->getId() << endl;
        return true;
      }

    case 2:
      if (_f.getDirection() == "in") { 
        cout << "one catch on vertex: " << vp->getId() << endl;
        return true;
      }
    default:
      return false;
    }
    return false;
  }

private:
  Filter _f;
};

class VisitedEdge: public EdgeVisitor<VisitedEdge>  {
public:
  VisitedEdge() { } 

  void setFilter(Filter & f) {
    _f = f;
  }

  bool  visitEdge(EdgePointer ep) {
    return false;
  }

  bool  scheduleEdge(EdgePointer ep ) {
    if (_f.getType() == "")
      return true;
    FixedString type(_f.getType());
    if (ep->getType() == type)
    {
//      cout << "Edge:\t" << ep->getId() << endl;
      return true;
    }
    else  {
//      cout << "type= " << ep->getType() << endl;
      return false;
    }
  }

private:
  Filter _f;
};

#endif /**_CUSOTMVISITOR_H_*/
