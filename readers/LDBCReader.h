//===-- readers/LDBCReader.h - class to read LDBC DATA =---*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the class to read processed data by LDBCTranslator.h .
///
//===----------------------------------------------------------------------===//

#ifndef _LDBCREADER_H_
#define _LDBCREADER_H_

#include <fstream>
#include <boost/algorithm/string.hpp>
#include <map>
#include <exception>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "GraphType.h"
#include "FileTypes.h"

using namespace std;

class LDBCReader {
public:
  typedef GraphType::VertexPointer    VertexPointer;
  typedef GraphType::PropertyListType PropertyListType;
  typedef GraphType::VertexDescriptor VertexDescriptor;
  typedef vector<string> KeyListType;
  typedef vector<string> AttributeListType;
  typedef vector<VertexDescriptor, string> MapType;
  typedef vector<VertexDescriptor> ListTypeReference;
  typedef map<string, VertexDescriptor> VertexMapType;
private:
  GraphType & _Graph;
  ifstream _LDBCFile;
  map<string, map<string, VertexDescriptor>> _VertexLabelMap;
public:
  LDBCReader(GraphType & graph) : _Graph( graph ) { }

  auto readDirectory(string  DirName) 
    -> int {
    multimap<unsigned int, pair<string, RelLabel> > _FileTypeMap;
    DIR *_DirPointer;
    struct dirent *_DirEntry;
   
    _DirPointer = opendir( DirName.c_str() );
    if (_DirPointer == NULL) {
      cout << "Error " << errno << " Cannot open " << DirName << endl;
      exit(0);
    }
    while( (_DirEntry = readdir(_DirPointer)) != NULL ) {

      FileTypeReader TypeReader;
      TypeReader.readFileType(_DirEntry->d_name);
      auto TypeNum = TypeReader.getFileTypeNum();
      auto Label = TypeReader.getLabel();

      auto FileInfo = make_pair(_DirEntry->d_name, Label);
      _FileTypeMap.insert(pair<unsigned int, pair<string, RelLabel>>(TypeNum, FileInfo)); 
    }
    for (auto it = _FileTypeMap.begin(); it != _FileTypeMap.end(); ++it) {
      auto TypeNum = (*it).first;
      auto FileName = DirName + "/" + (*it).second.first;
      auto Label = (*it).second.second;

      switch (TypeNum ) {
        case 1:
          readNodeInfo(FileName, Label);
          break;
        case 2:
          readEdgeInfo(FileName, Label); 
          break;
        case 3:
          break;
        case 0:
          break;
        default:
          cout << "Error : Cannot recognize file type " << _DirEntry->d_name << endl;
      }//END_SWTICH
    } //END_FOR

    closedir(_DirPointer);
    return 0;
  }

private:
  auto readNodeInfo(string FileName, RelLabel & Label) 
    -> void {
    string line;
    KeyListType keys;
    AttributeListType attributes;

    try{
      _LDBCFile.open(FileName.c_str());
      if (_LDBCFile.fail())
        throw 1;

      if(getline(_LDBCFile, line) != NULL) {
        boost::split(keys, line, boost::is_any_of("|"));
      }

      while(getline(_LDBCFile, line) != NULL){ 
        PropertyListType PropertyList; 
        boost::split(attributes, line, boost::is_any_of("|"));

        ///Do not read the first attr(ID in graph)
        for(unsigned int i = 1; i < attributes.size()-1; i++) {
          PropertyList.set(keys[i], attributes[i]); 
        }//END_FOR

        _Graph.addVertex(Label.First, PropertyList);
      } //END_WHILE

      _LDBCFile.close();
    }//END_TRY
    catch (int i){
      cout << "Error:"<< i <<"\tFailed to open file" <<endl;
      cerr << strerror(errno) << endl;
      exit(0);
    }
  }//END_READNODEINFO_

  auto readEdgeInfo(string FileName, RelLabel & EdgeLabel) 
    -> void {
    string line;
    KeyListType keys;
    AttributeListType attributes;

    try{
      _LDBCFile.open(FileName.c_str());
      if (_LDBCFile.fail())
        throw 1;

      if(getline(_LDBCFile, line) != NULL) {
        boost::split(keys, line, boost::is_any_of("|"));
       }

      while(getline(_LDBCFile, line) != NULL){ 
        PropertyListType PropertyList; 
        boost::split(attributes, line, boost::is_any_of("|"));

        for(unsigned int i = 2; i < attributes.size()-1; i++) {
          PropertyList.set(keys[i], attributes[i]); 
        }//END_FOR

        unsigned int vs = stoi(attributes[0]);
        unsigned int vd = stoi(attributes[1]);
        _Graph.addEdge(vs,vd, EdgeLabel.Edge, PropertyList);

      } //END_WHILE

    _LDBCFile.close();
 
    }//END_TRY
    catch (int i){
      cout << "Error:"<< i <<"\tFailed to open file" <<endl;
      cerr << strerror(errno) << endl;
      exit(0);
    }
  }//END_READNODEINFO_

  auto readPropertyInfo(string FileName, RelLabel & PropertyLabel) 
    -> void {
    string line;
    KeyListType keys;
    AttributeListType attributes;

    try{
      _LDBCFile.open(FileName.c_str());
      if (_LDBCFile.fail())
        throw 1;

      if(getline(_LDBCFile, line) != NULL) {
        boost::split(keys, line, boost::is_any_of("|"));
      }

      while(getline(_LDBCFile, line) != NULL){ 
        boost::split(attributes, line, boost::is_any_of("|"));
        if(_VertexLabelMap.find(PropertyLabel.First) == _VertexLabelMap.end()) {
          cout << "Error: Cannot not recognize label " << PropertyLabel.First << endl;
          exit(0);
        }

        if (_VertexLabelMap[PropertyLabel.First].find(attributes[0]) == _VertexLabelMap[PropertyLabel.First].end() ) {
          cout << "Error: Cannot find vertex " << attributes[0] << endl;
          exit(0); // should not break

        } else {
          auto vs = _VertexLabelMap[PropertyLabel.First].at(attributes[0]);
          auto vp = _Graph.getVertexPointer(vs);
          auto PropertyList = vp->getPropertyList();
          PropertyList.set(keys[1], attributes[1]); 
          PropertyList.print();
        }
      } //END_WHILE
    }//END_TRY
    catch (int i){
      cout << "Error:"<< i <<"\tFailed to open file" <<endl;
      cerr << strerror(errno) << endl;
      exit(0);
    }
    _LDBCFile.close();
  }//END_READNODEINFO_
};

#endif /*_LDBCREADER_H_*/

