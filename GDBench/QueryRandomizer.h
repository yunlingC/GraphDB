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

  const string & getName(const int index = -1 ) {
    return index == -1 ? std::move(createName()) : std::move(createName(index));
  }

  const string & getPid(int index = -1) {
    return index == 1 ? std::move(createPid()) : std::move(createPid(static_cast<unsigned int>(index)));
  }

  const VertexDescriptor & getAPersonIndex(int index = -1) {
    return index == -1 ? std::move(createPersonIndex()) : std::move(createPersonIndex(static_cast<unsigned int>(index)));
  }
  
  const VertexDescriptor & getAWebpageIndex(int index = -1) {
    return index == -1 ? std::move(createWebpageIndex()) : std::move(createWebpageIndex(static_cast<unsigned int>(index)));
  }

private:
  string createName(unsigned int index) {
    return _GDReader.getNameList().at(index);
  }

  /// Randomized output 
  /// But need srand(time(null)) as completely randmization if necessary
  string createName() {
    unsigned int nameCounter = _GDReader.getPeopleCounter();
    unsigned int nameId = rand() % (nameCounter/3);
    return _GDReader.getNameList().at(nameId);
  }

  string createPid(unsigned int index) {
    return _GDReader.getPidList().at(index);
  }

  string createPid() {
    unsigned int pidCounter = _GDReader.getPeopleCounter();
    unsigned int pidId = rand() % (pidCounter/3);
    return _GDReader.getPidList().at(pidId);
  }

  unsigned int createPersonIndex(unsigned int index) {
    return index;
  }

  unsigned int createPersonIndex() {
    unsigned int personCounter = _GDReader.getPeopleCounter();
    return rand() % (personCounter/3);
  }

  unsigned int createWebpageIndex(unsigned int index) {
    unsigned int personCounter = _GDReader.getPeopleCounter();
    return index + personCounter;
  }

  unsigned int createWebpageIndex() {
    unsigned int personCounter = _GDReader.getPeopleCounter();
    unsigned int webCounter = _GDReader.getWebpagesCounter();
    return rand() % (webCounter/3) + personCounter;
  } 

private:
  GDBReader & _GDReader;
};


#endif /*_QUERYRANDOMIZER_H*/
