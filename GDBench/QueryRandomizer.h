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

#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <time.h>

#include "GDReader.h"

class QueryRandomizer {
public:
  typedef GraphType::VertexDescriptor VertexDescriptor;
public:
  QueryRandomizer (GDReader & reader) : _gdReader(reader) { };

  string & getName() {
    createName(_name);
    return _name;
  }

  string & getPid() {
    createPid(_pid);
    return _pid;
  }

  VertexDescriptor & getAPersonIndex() {
    createPersonIndex(_person);
    return _person;
  }
  
  VertexDescriptor & getAWebpageIndex() {
    createWebpageIndex(_webpage);
    return _webpage;
  }

private:
  void createName(string & name) {
    unsigned int nameCounter = _gdReader.getPeopleCounter();
    unsigned int nameId = rand() % (nameCounter);
    name = _gdReader.getNameList().at(nameId);
//    std::cout << "Random name is " << name << std::endl;
  }

  void createPid(string & pid) {
    unsigned int pidCounter = _gdReader.getPeopleCounter();
    unsigned int pidId = rand() % (pidCounter);
    pid = _gdReader.getPidList().at(pidId);
//    std::cout << "Random pid is " << pid << std::endl;
  }

  void createPersonIndex(VertexDescriptor & person) {
    unsigned int personCounter = _gdReader.getPeopleCounter();
    person = rand() % (personCounter);
//    std::cout << "Random person is " << person << std::endl;
  }

  void createWebpageIndex(VertexDescriptor & webpage) {
    unsigned int personCounter = _gdReader.getPeopleCounter();
    unsigned int webCounter = _gdReader.getWebpagesCounter();
    webpage = rand() % (webCounter) + personCounter ;
//    std::cout << "Random webpage is " << webpage << std::endl;
  }
  
private:
  string _name;
  string _pid;
  VertexDescriptor _person;
  VertexDescriptor _webpage;
  GDReader & _gdReader;
};


#endif /*_QUERYRANDOMIZER_H*/
