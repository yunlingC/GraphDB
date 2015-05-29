//===-- readers/LDBCTranslator.h - class to pro-precess LDBC DATA =---*- C++ -*-===//
// //                     CAESR Graph Database //
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the class to process raw data: add id field to vertex and edge.
///
//===----------------------------------------------------------------------===//

#ifndef _LDBCTRANSLATOR_H_
#define _LDBCTRANSLATOR_H_

#include <fstream>
#include <boost/algorithm/string.hpp>
#include <map>
#include <exception>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>

#include "FileTypes.h"

using namespace std;

class LDBCTranslator {
public:
  typedef vector<string> KeyListType;
  typedef vector<string> AttributeListType;
  typedef vector<unsigned int, string> MapType;
  typedef vector<unsigned int> ListTypeReference;
  typedef map<string, unsigned int> VertexMapType;

private:
  ifstream _LDBCFile;
  string NewDirName ;
  map<string, map< string, unsigned int > > _VertexLabelMap;
  unsigned int NodeNum = 0;

public:
  auto readDirectory(string  DirName) 
    -> int {
    multimap<unsigned int, pair<string, RelLabel> > _FileTypeMap;
    DIR *_DirPointer;
    struct dirent *_DirEntry;
    NewDirName = DirName + "/New";

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
    auto EndIter = _FileTypeMap.end();
    mkdir(NewDirName.c_str(), 0777);
    for ( auto it = _FileTypeMap.begin(); it != EndIter; ++it ) {
      auto TypeNum = (*it).first;
      auto FileName = DirName + "/" + (*it).second.first;
      auto ShortName = (*it).second.first;
      auto Label = (*it).second.second;
      switch (TypeNum ) {
        case 1:
          readNodeInfo(FileName, ShortName, Label);
          break;
        case 2:
          readEdgeInfo(FileName, ShortName, Label); 
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
  auto readNodeInfo(string FileName, string ShortName, RelLabel& Label) 
    -> void {
    string line;
    KeyListType keys;
    AttributeListType attributes;

    VertexMapType VertexMap;
    _VertexLabelMap.insert(pair<string, VertexMapType>(Label.First, VertexMap));

    try{
      _LDBCFile.open(FileName.c_str(), ios::in);
      if (_LDBCFile.fail())
        throw 1;

      auto Counter = 0;
      string NewName = NewDirName + "/"+ ShortName;
      ofstream OutFile(NewName.c_str());
      if(OutFile.fail()) {
        cout << "Fail to write to file " << NewName << "\n";
        exit(0);
      }

      if(getline(_LDBCFile, line) != NULL) {
        boost::split(keys, line, boost::is_any_of("|"));
      }

      string NewLine = "ID|" + line; 
      OutFile << NewLine << "\n";

      while(getline(_LDBCFile, line) != NULL){ 
        boost::split(attributes, line, boost::is_any_of("|"));
        _VertexLabelMap[Label.First].insert (pair< string, unsigned int > (attributes[0], NodeNum));
        Counter++;
        NewLine = to_string(NodeNum) + '|'+ line;
        NodeNum++;
        OutFile << NewLine << "\n";
      } //END_WHILE

      cout << Counter << "\tNodes are read in " << ShortName << "\n";
      OutFile.close(); 
      _LDBCFile.close();

    }//END_TRY
    catch (int i){
      cout << "Error:"<< i <<"\tFailed to open file" <<endl;
      cerr << strerror(errno) << endl;
      exit(0);
    }

  }//END_READNODEINFO_

  auto readEdgeInfo(string FileName, string ShortName, RelLabel & EdgeLabel) 
    -> void {
    auto Counter= 0;
    string line;
    KeyListType keys;
    AttributeListType attributes;

    try{
      _LDBCFile.open(FileName.c_str());
      if (_LDBCFile.fail())
        throw 1;

      string NewName = NewDirName + "/"+ ShortName;
      ofstream OutFile(NewName.c_str());
      if(OutFile.fail()) {
        cout << "Fail to write to file " << NewName << "\n";
        exit(0);
      }

      if(getline(_LDBCFile, line) != NULL) {
        boost::split(keys, line, boost::is_any_of("|"));
      }

      string NewLine = "ID1|ID2|" + line; 
      OutFile << NewLine << "\n";

      while(getline(_LDBCFile, line) != NULL){ 
        boost::split(attributes, line, boost::is_any_of("|"));

        if( (_VertexLabelMap.find(EdgeLabel.First) == _VertexLabelMap.end()) ||
            (_VertexLabelMap.find(EdgeLabel.Second) == _VertexLabelMap.end()) ) {
          cout << "Error: Cannot recognize vertex label\n";
          exit(0);
        }
         if ((_VertexLabelMap[EdgeLabel.First].find(attributes[0]) == _VertexLabelMap[EdgeLabel.First].end()) ||
             (_VertexLabelMap[EdgeLabel.Second].find(attributes[1]) == _VertexLabelMap[EdgeLabel.First].end())) {
           cout << "Error: Cannot find vertex " << attributes[0] << attributes[1]  << endl;
           exit(0);
         }

        auto vs = _VertexLabelMap[EdgeLabel.First].at(attributes[0]);
        auto vd = _VertexLabelMap[EdgeLabel.Second].at(attributes[1]);

        OutFile << to_string(vs) + '|' + to_string(vd) + '|'+ line << "\n";
        Counter++;
      } //END_WHILE

    cout << Counter << "\tRels are read in " << ShortName << "\n";
    OutFile.close();
    _LDBCFile.close();

    }//END_TRY
    catch (int i){
      cout << "Error:"<< i <<"\tFailed to open file" <<endl;
      cerr << strerror(errno) << endl;
      exit(0);
    }

  }//END_READNODEINFO_
/**
  auto readPropertyInfo(string FileName, RelLabel & PropertyLabel) 
    -> void {
    auto counter= 0;
    string line;
    KeyListType keys;
    AttributeListType attributes;

    try{
      _LDBCFile.open(FileName.c_str());
      if (_LDBCFile.fail())
        throw 1;

    cout << "---------->Start reading file: " << FileName << "<----------\n";
      if(getline(_LDBCFile, line) != NULL) {
        boost::split(keys, line, boost::is_any_of("|"));
      }

      for (unsigned int j = 0; j < keys.size()-1; j++) {
        cout << "key " << keys[j] << endl;
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
          counter++;
        }
      } //END_WHILE
      cout << counter << " new properties are read in this file\n";
    }//END_TRY
    catch (int i){
      cout << "Error:"<< i <<"\tFailed to open file" <<endl;
      cerr << strerror(errno) << endl;
    }

    _LDBCFile.close();

    cout << "---------->Finish reading file: " << FileName << "<----------\n";
    cout << endl;
  }//END_READNODEINFO_
*/
};

#endif /*_LDBCREADER_H_*/

