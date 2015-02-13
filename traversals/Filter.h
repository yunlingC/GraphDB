#ifndef _FILTER_H_
#define _FILTER_H_

//#include "PropertyList.h"
//#include "LinkedList.h"
#include "FixedString.h"

struct Filter {
public:
  ///TODO: enum every type

  typedef std::string KeyType;
  typedef std::string ValueType;
  typedef std::string Type;
  typedef std::string Direction; 
  typedef unsigned int IdType;
public:
  Filter() : _key(""), 
             _value(""), 
             _type(""), 
             _direction(""),
             _vertexId(0), 
             _edgeId(0)  { }

  void setKey(KeyType k) {
    _key = k;
  }
  
  void setValue(ValueType v)  {
    _value = v;
  }

  void setProperty(KeyType key, ValueType value) {
    setKey(key);
    setValue(value);
   }

  void setType(Type t)  {
    _type = t;
  }
 
  void setDirection(Direction d) {
    _direction = d;
  }

  void setVertexId(IdType vid) {
    _vertexId = vid;
  }
  
  void setEdgeId(IdType eid) {
    _edgeId = eid;
  }


  KeyType & getKey() {
    return _key;
  }

  ValueType & getValue() {
    return _value;
  }

  Type & getType() { 
    return _type; 
  }

  Direction & getDirection() {
    return _direction;
  }

  IdType & getVertexId() {
    return _vertexId;
  }

  IdType & getEdgeId() {
    return _edgeId;
  }

private:
  KeyType   _key;
  ValueType _value;
  Type      _type;
  Direction _direction;
  IdType    _vertexId;
  IdType    _edgeId;


};

/*
struct TypeFilter : public Filter {
public:
  typedef std::string Type;
public:

//  TypeFilter() {}
  TypeFilter(Type t = "") : _type(t) {}

  virtual void setType(Type t) {
    _type = t;
  }

 virtual Type & getType() {
    return _type;
  }

private:
  Type _type;
};


struct PropertyFilter : public Filter {
public:


private:

};
*/
#endif /*_FILTER_H_*/
