//===-- readers/LDBCReader.h - class to read from LDBC DATA =---*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the class to read output files from ldbc_snb_datagen.
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
  vector<VertexDescriptor> _PersonList;
public:
  LDBCReader(GraphType & graph) : _Graph( graph ) { }

  auto readDirectory(string  DirName) 
    -> int {
    multimap<unsigned int, pair<string, RelLabel> > _FileTypeMap;
    DIR *_DirPointer;
    struct dirent *_DirEntry;
//    struct stat _FileStat;

    cout << "==========Read Directory " << DirName << "==========\n";
    _DirPointer = opendir( DirName.c_str() );
    if (_DirPointer == NULL) {
      cout << "Error " << errno << " Cannot open " << DirName << endl;
      return errno;
    }
    while( (_DirEntry = readdir(_DirPointer)) != NULL ) {
//      auto filePath = DirName + "/" + _DirEntry->d_name;
//      if(stat(filePath.c_str(), & _FileStat)) continue;
//      if(S_ISDIR(_FileStat.st_mode))          continue;

      FileTypeReader TypeReader;
      TypeReader.readFileType(_DirEntry->d_name);
      auto TypeNum = TypeReader.getFileTypeNum();
      auto Label = TypeReader.getLabel();

      cout << "file to be read " << _DirEntry->d_name << TypeNum << endl;
      auto FileInfo = make_pair(_DirEntry->d_name, Label);
      _FileTypeMap.insert(pair<unsigned int, pair<string, RelLabel>>(TypeNum, FileInfo)); 
    }
    for (auto it = _FileTypeMap.begin(); it != _FileTypeMap.end(); ++it) {
      auto TypeNum = (*it).first;
      auto FileName = DirName + "/" + (*it).second.first;
      auto Label = (*it).second.second;
//      cout << "FileName " << FileName <<" FileTypeNum " << TypeNum << endl;
      switch (TypeNum ) {
        case 1:
          readNodeInfo(FileName, Label);
          break;
        case 2:
          readEdgeInfo(FileName, Label); 
          break;
        case 3:
          readPropertyInfo(FileName, Label);
          break;
        case 0:
//          cout <<"Error : Type is only initialized\n";
          break;
        default:
          cout << "Error : Cannot recognize file type " << _DirEntry->d_name << endl;

      }//END_SWTICH

    } //END_FOR

    closedir(_DirPointer);
    return 0;
  }

  auto getPersonList() 
    -> ListTypeReference {
    return _PersonList;
  }

  auto readPerson(string FileName) 
    -> void {
//    readNodeInfo(FileName, "PERSON");
  }

  auto readFriends(string FileName) 
    -> void {
//    readEdgeInfo(FileName, "KNOWS");
  }

  auto readProperty(string FileName)
    -> void {
//    readPropertyInfo(FileName);
  }
private:
  auto readNodeInfo(string FileName, RelLabel & Label) 
    -> void {
    auto counter= 0;
    string line;
    KeyListType keys;
    AttributeListType attributes;

    VertexMapType VertexMap;
    _VertexLabelMap.insert(pair<string, VertexMapType>(Label.First, VertexMap));

    try{
      _LDBCFile.open(FileName.c_str());
      if (_LDBCFile.fail())
        throw 1;

    cout << "---------->Start reading file: " << FileName << "<----------\n";
    cout << "Node type " << Label.First << endl;
    auto Counter = 0;
      if(getline(_LDBCFile, line) != NULL) {
        boost::split(keys, line, boost::is_any_of("|"));
      }

      for (unsigned int j = 0; j < keys.size()-1; j++) {
       cout << "key " << keys[j] << endl;
      } 
      while(getline(_LDBCFile, line) != NULL){ 
        PropertyListType PropertyList; 
        boost::split(attributes, line, boost::is_any_of("|"));
        for(unsigned int i = 0; i < attributes.size()-1; i++) {
          PropertyList.set(keys[i], attributes[i]); 
        }//END_FOR

        GraphType::VertexDescriptor vd = _Graph.addVertex(Label.First, PropertyList);
//        PropertyList.print();
        _VertexLabelMap[Label.First].insert(pair<string, VertexDescriptor>(attributes[0], vd));
        if((Counter ++) < 5)
          if (Label.First == "PERSON")
            _PersonList.push_back(vd);
        counter++;
      } //END_WHILE
      cout << counter << " Nodes are read in this file\n";
    }//END_TRY
    catch (int i){
      cout << "Error:"<< i <<"\tFailed to open file" <<endl;
      cerr << strerror(errno) << endl;
    }

    _LDBCFile.close();

    cout << "---------->Finish reading file: " << FileName << "<----------\n";
    cout << endl;
  }//END_READNODEINFO_


  auto readEdgeInfo(string FileName, RelLabel & EdgeLabel) 
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
    cout << "relationship label " << EdgeLabel.Edge << endl;
      if(getline(_LDBCFile, line) != NULL) {
        boost::split(keys, line, boost::is_any_of("|"));
      }

      for (unsigned int j = 0; j < keys.size()-1; j++) {
        cout << "key " << keys[j] << endl;
      } 
      while(getline(_LDBCFile, line) != NULL){ 
        PropertyListType PropertyList; 
        boost::split(attributes, line, boost::is_any_of("|"));

        for(unsigned int i = 2; i < attributes.size()-1; i++) {
          PropertyList.set(keys[i], attributes[i]); 
        }//END_FOR
        //auto flag = false;
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
        _Graph.addEdge(vs,vd, EdgeLabel.Edge, PropertyList);
//        cout << "vs " << attributes[0] << " vd " << attributes[1] << endl;
//        PropertyList.print();
        counter++;
      } //END_WHILE
      cout << counter << " Rels are read in this file\n";
    }//END_TRY
    catch (int i){
      cout << "Error:"<< i <<"\tFailed to open file" <<endl;
      cerr << strerror(errno) << endl;
    }

    _LDBCFile.close();

    cout << "---------->Finish reading file: " << FileName << "<----------\n";
    cout << endl;
  }//END_READNODEINFO_

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




};

#endif /*_LDBCREADER_H_*/

