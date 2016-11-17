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

#include "GraphType.h"
#include "FileTypes.h"

#include <fstream>
#include <boost/algorithm/string.hpp>
#include <map>
#include <dirent.h>

#define _DEBUG_ 1

#if _DEBUG_ 
#include <iostream>
#endif

class LDBCReader {
public:
  typedef GraphType::VertexPointer    VertexPointer;
  typedef GraphType::PropertyListType PropertyListType;
  typedef GraphType::VertexDescriptor VertexDescriptor;
  typedef std::vector<std::string> KeyListType;
  typedef std::vector<std::string> AttributeListType;
  typedef std::vector<VertexDescriptor, std::string> MapType;
  typedef std::vector<VertexDescriptor> ListTypeReference;
  typedef std::map<std::string, VertexDescriptor> VertexMapType;
private:
  GraphType & _Graph;
  std::ifstream _LDBCFile;
  std::map<std::string, std::map<std::string, VertexDescriptor> > _VertexLabelMap;
  std::vector<VertexDescriptor> _PersonList;
#if _DEBUG_
  unsigned int _NodeNum;
  unsigned int _EdgeNum;
#endif 

public:
  LDBCReader(GraphType & graph) : _Graph( graph ) { }

  auto readDirectory(std::string  DirName) 
    -> int {
    std::multimap<unsigned int, std::pair<std::string, RelLabel> > _FileTypeMap;
    DIR *_DirPointer;
    struct dirent *_DirEntry;

    _DirPointer = opendir( DirName.c_str() );
    if (_DirPointer == NULL) {
#if _DEBUG_
      std::cerr << "Error " << errno << " Cannot open " << DirName << "\n";
#endif
      exit(errno);
    }
    while ( (_DirEntry = readdir(_DirPointer)) != NULL ) {

      FileTypeReader TypeReader;
      TypeReader.readFileType(_DirEntry->d_name);
      auto TypeNum = TypeReader.getFileTypeNum();
      auto Label = TypeReader.getLabel();

      auto FileInfo = std::make_pair(_DirEntry->d_name, Label);
      _FileTypeMap.insert(std::pair<unsigned int, std::pair<std::string, RelLabel> >
                          (TypeNum, FileInfo)); 
    }
    for (auto it = _FileTypeMap.begin(), it_end = _FileTypeMap.end(); 
              it != it_end; ++it) {
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
#if _DEBUG_
          std::cerr << "Error : Cannot recognize file type "
                    << _DirEntry->d_name << "\n";
#endif
          exit(0);
      }//END_SWTICH
    } //END_FOR

    closedir(_DirPointer);
    std::cout << "\n+++++++++ Summary ++++++++++\n";
    std::cout << _NodeNum << " Nodes are read in total\n";
    std::cout << _EdgeNum << " Edges are read in total\n";
    return 0;
  }

  auto getPersonList() 
    -> ListTypeReference {
    return _PersonList;
  }

private:
  auto readNodeInfo(std::string FileName, RelLabel & Label) 
    -> void {
    auto counter= 0;
    std::string line;
    KeyListType keys;
    AttributeListType attributes;

    VertexMapType VertexMap;
    _VertexLabelMap.insert(std::pair<std::string, VertexMapType>
                          (Label.First, VertexMap));

    try{
      _LDBCFile.open(FileName.c_str());
      if (_LDBCFile.fail())
        throw 1;

      auto Counter = 0;
      if (getline(_LDBCFile, line)) {
        boost::split(keys, line, boost::is_any_of("|"));
      }

      while (getline(_LDBCFile, line)){ 
        PropertyListType PropertyList; 
        boost::split(attributes, line, boost::is_any_of("|"));
        for (unsigned int i = 0; i < attributes.size()-1; i++) {
          PropertyList.set(keys[i], attributes[i]); 
        }//END_FOR

        GraphType::VertexDescriptor vd = _Graph.addVertex(Label.First, PropertyList);
//        PropertyList.print();
        _VertexLabelMap[Label.First].insert(std::pair<std::string, VertexDescriptor>
                                            (attributes[0], vd));
        if ((Counter ++) < 5)
          if (Label.First == "PERSON")
            _PersonList.push_back(vd);

        counter++;
      } //END_WHILE
#if  _DEBUG_
      std::cout << counter << " Nodes are read in this file\n";
      _NodeNum += counter;
#endif
    }//END_TRY
    catch (int i){
#if  _DEBUG_
      std::cerr << "Error:"<< i <<"\tFailed to open file" <<"\n";
#endif
      exit(0);
    }
    _LDBCFile.close();
  }//END_READNODEINFO_


  auto readEdgeInfo(std::string FileName, RelLabel & EdgeLabel) 
    -> void {
    auto counter= 0;
    std::string line;
    KeyListType keys;
    AttributeListType attributes;

    try{
      _LDBCFile.open(FileName.c_str());
      if (_LDBCFile.fail())
        throw 1;

      if (getline(_LDBCFile, line)) {
        boost::split(keys, line, boost::is_any_of("|"));
       }

      while (getline(_LDBCFile, line)){ 
        PropertyListType PropertyList; 
        boost::split(attributes, line, boost::is_any_of("|"));

        for (unsigned int i = 2; i < attributes.size()-1; i++) {
          PropertyList.set(keys[i], attributes[i]); 
        }//END_FOR

        if ((_VertexLabelMap.find(EdgeLabel.First) == _VertexLabelMap.end()) ||
            (_VertexLabelMap.find(EdgeLabel.Second) == _VertexLabelMap.end()) ) {
#if  _DEBUG_
          std::cerr << "Error: Cannot recognize vertex label\n";
#endif
          exit(0);
        }
         if ((_VertexLabelMap[EdgeLabel.First].find(attributes[0]) == _VertexLabelMap[EdgeLabel.First].end()) ||
             (_VertexLabelMap[EdgeLabel.Second].find(attributes[1]) == _VertexLabelMap[EdgeLabel.First].end())) {
#if  _DEBUG_
           std::cerr << "Error: Cannot find vertex " 
                      << attributes[0] 
                      << attributes[1]  << "\n";
#endif
           exit(0);
         }

        auto vs = _VertexLabelMap[EdgeLabel.First].at(attributes[0]);
        auto vd = _VertexLabelMap[EdgeLabel.Second].at(attributes[1]);
        _Graph.addEdge(vs,vd, EdgeLabel.Edge, PropertyList);
        counter++;
      } //END_WHILE
#if _DEBUG_
      std::cout << counter << " Rels are read in this file\n";
      _EdgeNum += counter;
#endif
    }//END_TRY
    catch (int i){
#if _DEBUG_
      std::cerr << "Error:"<< i <<"\tFailed to open file" <<"\n";
#endif
      exit(0);
    }

    _LDBCFile.close();

  }//END_READNODEINFO_

  auto readPropertyInfo(std::string FileName, RelLabel & PropertyLabel) 
    -> void {
  /**
    auto counter= 0;
    std::string line;
    KeyListType keys;
    AttributeListType attributes;

    try{
      _LDBCFile.open(FileName.c_str());
      if (_LDBCFile.fail())
        throw 1;

      if (!getline(_LDBCFile, line) ) {
        boost::split(keys, line, boost::is_any_of("|"));
      }

      while (!getline(_LDBCFile, line)){ 
        boost::split(attributes, line, boost::is_any_of("|"));
        if (_VertexLabelMap.find(PropertyLabel.First) == _VertexLabelMap.end()) {
#if _DEBUG_
          std::cerr << "Error: Cannot recognize label " 
                    << PropertyLabel.First << "\n";
#endif
          exit(0);
        }

        if (_VertexLabelMap[PropertyLabel.First].find(attributes[0]) == _VertexLabelMap[PropertyLabel.First].end() ) {
#if _DEBUG_
          std::cerr << "Error: Cannot find vertex " << attributes[0] << "\n";
#endif
          exit(0); // should not break

        } else {
          auto vs = _VertexLabelMap[PropertyLabel.First].at(attributes[0]);
          auto vp = _Graph.getVertexPointer(vs);
          auto PropertyList = vp->getPropertyList();
          PropertyList.set(keys[1], attributes[1]); 
//          PropertyList.print();
          counter++;
        }
      } //END_WHILE
#if _DEBUG_
      std::cout << counter << " new properties are read in this file\n";
#endif
    }//END_TRY
    catch (int i){
#if  _DEBUG_
      std::cerr << "Error:"<< i <<"\tFailed to open file" <<"\n";
      exit(0);
#endif 
    }
    _LDBCFile.close();
 */

  }//END_READNODEINFO_
};

#endif /*_LDBCREADER_H_*/

