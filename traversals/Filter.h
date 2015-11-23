//===-- traversals/Filter.h - Filter class ----------------------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the base class for Filter used in visitor.
///
//===----------------------------------------------------------------------===//
#ifndef _FILTER_H_
#define _FILTER_H_

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
public:
  Filter() : _Key(""), 
             _Value(""), 
             _Type(""), 
             _Direction(""),
             _VertexId(0), 
             _EdgeId(0)  { }

  void setKey(KeyType key) {
    _Key = key;
  }
  
  void setValue(ValueType value)  {
    _Value = value;
  }

  void setProperty(KeyType key, ValueType value) {
    setKey(key);
    setValue(value);
   }

  void setType(Type type)  {
    _Type = type;
  }
 
  void setDirection(Direction direct) {
    _Direction = direct;
  }

  void setVertexId(IdType VertexId) {
    _VertexId = VertexId;
  }
  
  void setEdgeId(IdType EdgeId) {
    _EdgeId = EdgeId;
  }

  void setTypeList(TypeList & typeList) {
    _TypeList = typeList;
  }

  auto setValueRange(KeyType key, ValueType valueMin, ValueType valueMax) 
    -> void {
    _Key = key;
    ValueListType ValueList;
    ValueList.push_back(valueMin);
    ValueList.push_back(valueMax);
    _ValueRange = ValueList;
  }

  auto setValueList(KeyType key, ValueListType & ValueList) 
    -> void {
    _Key = key;
    _ValueRange = ValueList;
  }

  void setDefault() {
    _Key = "";
    _Value = "";
    _Type = "";
    _Direction = "";
    _VertexId = 0;
    _EdgeId = 0;
    _TypeList.push_back("");
  }

  KeyType & getKey() {
    return _Key;
  }

  ValueType & getValue() {
    return _Value;
  }

  Type & getType() { 
    return _Type; 
  }

  Direction & getDirection() {
    return _Direction;
  }

  IdType & getVertexId() {
    return _VertexId;
  }

  IdType & getEdgeId() {
    return _EdgeId;
  }

  TypeList & getTypeList() {
    return _TypeList;
  }

  auto getValueRange() 
    -> ValueListTypeReference {
    return _ValueRange;
  }

private:
  KeyType   _Key;
  ValueType _Value;
  Type      _Type;
  Direction _Direction;
  IdType    _VertexId;
  IdType    _EdgeId;
  TypeList  _TypeList;
  ValueListType _ValueRange;
};

#endif /*_FILTER_H_*/
