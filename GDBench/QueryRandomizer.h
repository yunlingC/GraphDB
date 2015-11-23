//===-- readers/QueryRandomizer.h - GDbench Query input ---------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
// 
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the class to randomize input to query for GDBench.
///
//===----------------------------------------------------------------------===//

#ifndef _QUERYRANDOMIZER_H_
#define _QUERYRANDOMIZER_H_

#include "GDBReader.h"

#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <time.h>

class QueryRandomizer {
public:
  typedef GraphType::VertexDescriptor VertexDescriptor;
  typedef std::string string;
public:
  QueryRandomizer (GDBReader & reader) : _gdReader(reader) { };

  string & getName(unsigned int i ) {
    createName(_name, i);
    return _name;
  }

  string & getPid(unsigned int i) {
    createPid(_pid, i);
    return _pid;
  }

  VertexDescriptor & getAPersonIndex(unsigned int i) {
    createPersonIndex(_person, i);
    return _person;
  }
  
  VertexDescriptor & getAWebpageIndex(unsigned int i) {
    createWebpageIndex(_webpage, i);
    return _webpage;
  }

public:
//  void createName(string & name) {
  void createName(string & name, unsigned int i) {
//    unsigned int nameCounter = _gdReader.getPeopleCounter();
//    unsigned int nameId = rand() % (nameCounter/3);
//    name = _gdReader.getNameList().at(nameId);
    name = _gdReader.getNameList().at(i);
    //std::cout << "Random name is " << name << std::endl;
  }

//  void createPid(string & pid) {
  void createPid(string & pid, unsigned int i) {
//    unsigned int pidCounter = _gdReader.getPeopleCounter();
//    unsigned int pidId = rand() % (pidCounter/3);
//    pid = _gdReader.getPidList().at(pidId);
    pid = _gdReader.getPidList().at(i);
    //std::cout << "Random pid is " << pid << std::endl;
  }

//  void createPersonIndex(VertexDescriptor & person) {
  void createPersonIndex(VertexDescriptor & person, unsigned int i) {
//    unsigned int personCounter = _gdReader.getPeopleCounter();
//    person = rand() % (personCounter/3);
    person = i;
    //std::cout << "Random person is " << person << std::endl;
  }

//  void createWebpageIndex(VertexDescriptor & webpage) {
  void createWebpageIndex(VertexDescriptor & webpage, unsigned int i) {
    unsigned int personCounter = _gdReader.getPeopleCounter();
//    unsigned int webCounter = _gdReader.getWebpagesCounter();
//    webpage = rand() % (webCounter/3) + personCounter ;
    webpage = i + personCounter ;
    //std::cout << "Random webpage is " << webpage << std::endl;
  }
  
private:
  string _name;
  string _pid;
  VertexDescriptor _person;
  VertexDescriptor _webpage;
  GDBReader & _gdReader;
};


#endif /*_QUERYRANDOMIZER_H*/
