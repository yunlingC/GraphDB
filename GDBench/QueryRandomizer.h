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

#include <time.h>

class QueryRandomizer {
public:
  typedef GraphType::VertexDescriptor VertexDescriptor;
  typedef std::string string;
public:
  QueryRandomizer (GDBReader & reader) : _GDReader(reader) { };

  string & getName(unsigned int index ) {
    createName(_Name, index);
    return _Name;
  }

  string & getPid(unsigned int index) {
    createPid(_PID, index);
    return _PID;
  }

  VertexDescriptor & getAPersonIndex(unsigned int index) {
    createPersonIndex(_Person, index);
    return _Person;
  }
  
  VertexDescriptor & getAWebpageIndex(unsigned int index) {
    createWebpageIndex(_Webpage, index);
    return _Webpage;
  }

public:
  void createName(string & name, unsigned int index) {
    name = _GDReader.getNameList().at(index);
  }

  /// Randomized output 
  /// But need srand(time(null)) as completely randmization if necessary
  void createName(string & name) {
    unsigned int nameCounter = _GDReader.getPeopleCounter();
    unsigned int nameId = rand() % (nameCounter/3);
    name = _GDReader.getNameList().at(nameId);
  }

  void createPid(string & pid, unsigned int index) {
    pid = _GDReader.getPidList().at(index);
  }

  void createPid(string & pid) {
    unsigned int pidCounter = _GDReader.getPeopleCounter();
    unsigned int pidId = rand() % (pidCounter/3);
    pid = _GDReader.getPidList().at(pidId);
  }

  void createPersonIndex(VertexDescriptor & person, unsigned int index) {
    person = index;
  }

  void createPersonIndex(VertexDescriptor & person) {
    unsigned int personCounter = _GDReader.getPeopleCounter();
    person = rand() % (personCounter/3);
  }

  void createWebpageIndex(VertexDescriptor & webpage, unsigned int index) {
    unsigned int personCounter = _GDReader.getPeopleCounter();
    webpage = index + personCounter ;
  }

  void createWebpageIndex(VertexDescriptor & webpage) {
    unsigned int personCounter = _GDReader.getPeopleCounter();
    unsigned int webCounter = _GDReader.getWebpagesCounter();
    webpage = rand() % (webCounter/3) + personCounter ;
  } 

private:
  string _Name;
  string _PID;
  VertexDescriptor _Person;
  VertexDescriptor _Webpage;
  GDBReader & _GDReader;
};


#endif /*_QUERYRANDOMIZER_H*/
