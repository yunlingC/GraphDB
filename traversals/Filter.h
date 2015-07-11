#ifndef _FILTER_H_
#define _FILTER_H_

#include <map>
#include "FixedString.h"

struct Filter {
public:
  ///TODO: enum every type
  typedef std::string KeyType;
  typedef std::string ValueType;
  typedef std::string Type;
  typedef std::string Direction; 
  typedef unsigned int IdType;
  typedef std::vector<Type> TypeList;
  typedef std::vector<ValueType> ValueListType;
  typedef std::vector<ValueType> &  ValueListTypeReference;
  typedef std::map<KeyType, ValueType> PropertyMapType;
  typedef PropertyMapType &  PropertyMapTypeReference;
  typedef std::pair<std::string, GraphType::PropertyListType> EdgePairType;
  typedef std::map<Type, std::pair<KeyType, ValueType> > BranchPropertyType;
  typedef std::map<std::pair<std::string, std::string>, EdgePairType> BranchMapType;
  typedef BranchPropertyType & BranchPropertyTypeReference;
  typedef BranchMapType & BranchMapTypeReference;
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

  void setTypeList(TypeList & typeList) {
    _typeList = typeList;
  }

  auto setValueRange(KeyType key, ValueType valueMin, ValueType valueMax) 
    -> void {
    _key = key;
    ValueListType ValueList;
    ValueList.push_back(valueMin);
    ValueList.push_back(valueMax);
    _valueRange = ValueList;
  }

  auto setValueList(KeyType key, ValueListType & ValueList) 
    -> void {
    _key = key;
    _ValueList = ValueList;
  }

  auto setPropertyMap(PropertyMapType & pm)
    -> void {
    _PropertyMap = pm;
  }

  auto setBranchPropertyMap(BranchPropertyType & bm) 
    -> void {
   _BranchPropertyMap = bm;
  }

  auto setBranchMap(BranchMapType & bm) 
    -> void {
   _BranchMap = bm;
  }


  void setDefault() {
    _key = "";
    _value = "";
    _type = "";
    _direction = "";
    _vertexId = 0;
    _edgeId = 0;
    _typeList.push_back("");
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

  TypeList & getTypeList() {
    return _typeList;
  }

  auto getValueRange() 
    -> ValueListTypeReference {
    return _valueRange;
  }

  auto getValueList()
    -> ValueListTypeReference {
    return _ValueList;
  }

  auto getPropertyMap() 
    -> PropertyMapTypeReference {
    return _PropertyMap;
  }

  auto getBranchPropertyMap() 
    -> BranchPropertyTypeReference {
    return _BranchPropertyMap;
  }

  auto getBranchMap() 
    -> BranchMapTypeReference {
    return _BranchMap;
  }

private:
  KeyType   _key;
  ValueType _value;
  Type      _type;
  Direction _direction;
  IdType    _vertexId;
  IdType    _edgeId;
  TypeList  _typeList;
  ValueListType _valueRange;
  ValueListType _ValueList;
  PropertyMapType _PropertyMap;
  BranchMapType _BranchMap;
  BranchPropertyType _BranchPropertyMap;
};

#endif /*_FILTER_H_*/
