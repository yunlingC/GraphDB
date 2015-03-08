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
#include <string>

#include "GraphType.h"

using namespace std;

class FileTypeReader {
public:
  auto readFileType(string FileName )
    -> void { 
    if (FileName == COMMENT) {
      _FileTypeNum = 1; 
    } else 
    if (FileName == POST) {
      _FileTypeNum = 1; 
    } else 
    if (FileName == PERSON) {
      _FileTypeNum = 1; 
    } else 
    if (FileName == FORUM) {
      _FileTypeNum = 1; 
    } else 
    if (FileName == TAG) {
      _FileTypeNum = 1; 
    } else 
    if (FileName == TAGCLASS) {
      _FileTypeNum = 1; 
    } else 
    if (FileName == ORGANIZATION) {
      _FileTypeNum = 1; 
    } else 
    if (FileName == PLACE) {
      _FileTypeNum = 1; 
    } else 
    if (FileName == COMMENT_REPLY_OF_COMMENT) {
      _FileTypeNum = 2;
      _Label = "REPLY_OF_COMMENT";
    } else  
    if (FileName == COMMENT_REPLY_OF_POST) {
      _FileTypeNum = 2;
      _Label = "REPLY_OF_POST";
    } else  
    if (FileName == COMMENT_LOCATED_IN_PLACE) {
      _FileTypeNum = 2;
      _Label = "COMMENT_LOCATED_IN_PLACE";
    } else  
    if (FileName == PLACE_IS_PART_OF_PLACE) {
      _FileTypeNum = 2;
      _Label = "IS_PART_OF";
    } else  
    if (FileName == PERSON_KNOWS_PERSON) {
      _FileTypeNum = 2;
      _Label = "KNOWS";
    } else  
    if (FileName == PERSON_STUDIES_AT_ORGANISATION) {
      _FileTypeNum = 2;
      _Label = "STUDY_AT";
    } else  
    if (FileName == COMMENT_HAS_CREATOR_PERSON) {
      _FileTypeNum = 2;
      _Label = "COMMENT_HAS_CREATOR";
    } else  
    if (FileName == POST_HAS_CREATOR_PERSON) {
      _FileTypeNum = 2;
      _Label = "POST_HAS_CREATOR";
    } else  
    if (FileName == FORUM_HAS_MODERATOR_PERSON) {
      _FileTypeNum = 2;
      _Label = "HAS_MODERATOR";
    } else  
    if (FileName == PERSON_WORKS_AT_ORGANISATION) {
      _FileTypeNum = 2;
      _Label = "WORKS_AT";
    } else  
    if (FileName == PERSON_IS_LOCATED_IN_PLACE) {
      _FileTypeNum = 2;
      _Label = "PERSON_IS_LOCATED_IN";
    } else  
    if (FileName == PERSON_HAS_INTEREST_TAG) {
      _FileTypeNum = 2;
      _Label = "HAS_INTEREST";
    } else  
    if (FileName == POST_HAS_INTEREST_TAG) {
      _FileTypeNum = 2;
      _Label = "POST_HAS_TAG";
    } else  
    if (FileName == PERSON_LIKES_POST) {
      _FileTypeNum = 2;
      _Label = "LIKES_POST";
    } else  
    if (FileName == POST_IS_LOCATED_IN_PLACE) {
      _FileTypeNum = 2;
      _Label = "POST_IS_LOCATED_IN";
    } else  
    if (FileName == FORUM_HAS_MEMBER_PERSON) {
      _FileTypeNum = 2;
      _Label = "HAS_MEMBER";
    } else  
    if (FileName == FORUM_HAS_TAG_TAG) {
      _FileTypeNum = 2;
      _Label = "FORUM_HAS_TAG";
    } else  
    if (FileName == TAG_HAS_TYPE_TAGCLASS) {
      _FileTypeNum = 2;
      _Label = "HAS_TYPE";
    } else  
    if (FileName == TAGCLASS_IS_SUBCLASS_OF_TAGCLASS) {
      _FileTypeNum = 2;
      _Label = "IS_SUBCLASS_OF";
    } else  
    if (FileName == ORGANISATION_IS_LOCATED_IN_PLACE) {
      _FileTypeNum = 2;
      _Label = "ORGANISATION_IS_LOCATED_IN";
    } else  
    if (FileName == PERSON_LIKES_COMMENT) {
      _FileTypeNum = 2;
      _Label = "LIKES_COMMENT";
    } else  
    if (FileName == COMMENT_HAS_TAG_TAG) {
      _FileTypeNum = 2;
      _Label = "COMMENT_HAS_TAG";
    }   
    if (FileName == FORUMS_CONTAINER_OF_POST) {
      _FileTypeNum = 2;
      _Label = "CONTAINER_OF";
    }   
  }

  auto getFileTypeNum() 
  -> unsigned int {
    return _FileTypeNum;
  }

  auto getEdgeLabel() 
  -> string {
    return _Label;
  }
private:
  unsigned int _FileTypeNum = 0; /// 0. default 1. vertex 2. relationship 3. property
  string _Label = "";
private:
  /// Nodes:
  const string COMMENT = "comment_0.csv";
  const string POST = "post_0.csv";
  const string PERSON = "person_0.csv";
  const string FORUM = "forum_0.csv";
  const string TAG = "tag_0.csv";
  const string TAGCLASS = "tagclass_0.csv";
  const string ORGANIZATION = "organisation_0.csv";
  const string PLACE = "place_0.csv";

  /// Nodes properties:
  const string PERSON_SPEAKS_LANGUAGE = "person_speaks_language_0.csv";
  const string PERSON_EMAIL_ADDRESS = "person_email_emailaddress_0.csv";
 
  /// Relationships
  const string COMMENT_REPLY_OF_COMMENT = "comment_replyOf_comment_0.csv";
  const string COMMENT_REPLY_OF_POST = "comment_replyOf_post_0.csv";
  const string COMMENT_LOCATED_IN_PLACE = "comment_isLocatedIn_place_0.csv";
  const string PLACE_IS_PART_OF_PLACE = "place_isPartOf_place_0.csv";
  const string PERSON_KNOWS_PERSON = "person_knows_person_0.csv";
  const string PERSON_STUDIES_AT_ORGANISATION = "person_studyAt_organisation_0.csv";
  const string COMMENT_HAS_CREATOR_PERSON = "comment_hasCreator_person_0.csv";
  const string POST_HAS_CREATOR_PERSON = "post_hasCreator_person_0.csv";
  const string FORUM_HAS_MODERATOR_PERSON = "forum_hasModerator_person_0.csv";
  const string PERSON_IS_LOCATED_IN_PLACE = "person_isLocatedIn_place_0.csv";
  const string PERSON_WORKS_AT_ORGANISATION = "person_workAt_organisation_0.csv";
  const string PERSON_HAS_INTEREST_TAG = "person_hasInterest_tag_0.csv";
  const string POST_HAS_TAG_TAG = "post_hasTag_tag_0.csv";
  const string PERSON_LIKES_POST = "person_likes_post_0.csv";
  const string POST_IS_LOCATED_IN_PLACE = "post_isLocatedIn_place_0.csv";
  const string FORUM_HAS_MEMBER_PERSON = "forum_hasMember_person_0.csv";
  const string FORUMS_CONTAINER_OF_POST = "forum_containerOf_post_0.csv";
  const string FORUM_HAS_TAG_TAG = "forum_hasTag_tag_0.csv";
  const string TAG_HAS_TYPE_TAGCLASS = "tag_hasType_tagclass_0.csv";
  const string TAGCLASS_IS_SUBCLASS_OF_TAGCLASS = "tagclass_isSubclassOf_tagclass_0.csv";
  const string ORGANISATION_IS_LOCATED_IN_PLACE = "organisation_isLocatedIn_place_0.csv";
  const string PERSON_LIKES_COMMENT = "person_likes_comment_0.csv";
  const string COMMENT_HAS_TAG_TAG = "comment_hasTag_tag_0.csv";

} ;

/**
class RelationshipTypes {
  const string string STUDY_AT,
        REPLY_OF_COMMENT,
        REPLY_OF_POST,
        PERSON_IS_LOCATED_IN,
        ORGANISATION_IS_LOCATED_IN,
        POST_IS_LOCATED_IN,
        COMMENT_IS_LOCATED_IN,
        IS_PART_OF,
        KNOWS,
        HAS_MODERATOR,
        POST_HAS_CREATOR,
        COMMENT_HAS_CREATOR,
        WORKS_AT,
        HAS_INTEREST,
        LIKES_POST,
        LIKES_COMMENT,
        HAS_MEMBER,
        CONTAINER_OF,
        FORUM_HAS_TAG,
        POST_HAS_TAG,
        COMMENT_HAS_TAG,
        HAS_TYPE,
        IS_SUBCLASS_OF
    }

    public enum Nodes implements Label {
        Comment,
        Post,
        Person,
        Forum,
        Tag,
        TagClass
    }


}
*/
class LDBCReader {
public:
  typedef GraphType::VertexPointer    VertexPointer;
  typedef GraphType::PropertyListType PropertyListType;
  typedef GraphType::VertexDescriptor VertexDescriptor;
  typedef vector<string> KeyListType;
  typedef vector<string> AttributeListType;
private:
  GraphType & _Graph;
  ifstream _LDBCFile;
  string  _EdgeLabel;
//  string line;
//  string DirName;
  map<string, GraphType::VertexDescriptor> _VertexMap;
public:
  LDBCReader(GraphType & graph) : _Graph( graph ) { }

  auto readDirectory(string  DirName) 
    -> int {
    multimap<unsigned int, string> _FileTypeMap;
    DIR *_DirPointer;
    struct dirent *_DirEntry;
    struct stat _FileStat;

    cout << "==========Read Directory " << DirName << "==========\n";
    _DirPointer = opendir( DirName.c_str() );
    if (_DirPointer == NULL) {
      cout << "Error " << errno << " Cannot open " << DirName << endl;
      return errno;
    }
    while( (_DirEntry = readdir(_DirPointer)) != NULL ) {
      auto filePath = DirName + "/" + _DirEntry->d_name;
      if(stat(filePath.c_str(), & _FileStat)) continue;
      if(S_ISDIR(_FileStat.st_mode))          continue;

      FileTypeReader TypeReader;
      TypeReader.readFileType(_DirEntry->d_name);
      auto TypeNum = TypeReader.getFileTypeNum();
      cout << "file to be read " << _DirEntry->d_name << TypeNum << endl;
      _FileTypeMap.insert(pair<unsigned int, string>(TypeNum, _DirEntry->d_name));
    }
    for (auto it = _FileTypeMap.begin(); it != _FileTypeMap.end(); ++it) {
      auto TypeNum = (*it).first;
      auto FileName = DirName + "/" + (*it).second;
      cout << "FileName " << FileName <<" FileTypeNum " << TypeNum << endl;
      switch (TypeNum ) {
        case 1:
          readNodeInfo(FileName);
          break;
        case 2:
          readEdgeInfo(FileName, "KNOWS");
          break;
        case 3:
          readPropertyInfo(FileName);
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


  auto readPerson(string FileName) 
    -> void {
    readNodeInfo(FileName);
  }

  auto readFriends(string FileName) 
    -> void {
    readEdgeInfo(FileName, "KNOWS");
  }

  auto readProperty(string FileName)
    -> void {
    readPropertyInfo(FileName);
    }
private:
  auto readNodeInfo(string FileName) 
    -> void {
    unsigned int counter= 0;
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

      for (auto j = 0; j < keys.size()-1; j++) {
       cout << "key " << keys[j] << endl;
      } 
      while(getline(_LDBCFile, line) != NULL){ 
        PropertyListType PropertyList; 
        boost::split(attributes, line, boost::is_any_of("|"));
        for(auto i = 0; i < attributes.size()-1; i++) {
          PropertyList.set(keys[i], attributes[i]); 
        }//END_FOR
        GraphType::VertexDescriptor vd = _Graph.addVertex(PropertyList);
//        PropertyList.print();
        _VertexMap.insert(pair<string, VertexDescriptor>(attributes[0], vd));
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
  }//END_READNODEINFO_


  auto readEdgeInfo(string FileName, const string & EdgeLabel) 
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

      for (auto j = 0; j < keys.size()-1; j++) {
        cout << "key " << keys[j] << endl;
      } 
      while(getline(_LDBCFile, line) != NULL){ 
        PropertyListType PropertyList; 
        boost::split(attributes, line, boost::is_any_of("|"));
        // TODO :: to be deleted 
        for(auto i = 2; i < attributes.size()-1; i++) {
          PropertyList.set(keys[i], attributes[i]); 
        }//END_FOR
        auto flag = false;
        for (auto j = 0; j < 2; j++) {
         if (_VertexMap.find(attributes[j]) == _VertexMap.end() ) {
           cout << "Error: Cannot find vertex " << attributes[j] << endl;
           flag = true;
           break;
         }
        }
        if(flag == false) {
          auto vs = _VertexMap.at(attributes[0]);
          auto vd = _VertexMap.at(attributes[1]);
          _Graph.addEdge(vs,vd, EdgeLabel, PropertyList);
          PropertyList.print();
          counter++;
        } else {
          cout << "Error: Cannot find source or dest vertex\n";
          break;
        }
      } //END_WHILE
      cout << counter << " Nodes are read in this file\n";
    }//END_TRY
    catch (int i){
      cout << "Error:"<< i <<"\tFailed to open file" <<endl;
      cerr << strerror(errno) << endl;
    }

    _LDBCFile.close();

    cout << "---------->Finish reading file: " << FileName << "<----------\n";
  }//END_READNODEINFO_

  auto readPropertyInfo(string FileName) 
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

      for (auto j = 0; j < keys.size()-1; j++) {
        cout << "key " << keys[j] << endl;
      } 
      while(getline(_LDBCFile, line) != NULL){ 
        boost::split(attributes, line, boost::is_any_of("|"));
        if (_VertexMap.find(attributes[0]) == _VertexMap.end() ) {
          cout << "Error: Cannot find vertex " << attributes[0] << endl;
          break; // TODO should not break
        } else {
          auto vs = _VertexMap.at(attributes[0]);
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
  }//END_READNODEINFO_




};

#endif /*_LDBCREADER_H_*/

