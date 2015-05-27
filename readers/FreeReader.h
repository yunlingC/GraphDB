//===-- readers/GDReader.h - class to read from GDbench output ---*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the class to read output files from GDBench sndata.gd.
///
//===----------------------------------------------------------------------===//


#ifndef _GDREADER_H_
#define _GDREADER_H_

#include <fstream>
#include <boost/algorithm/string.hpp>
#include <map>
#include <exception>
#include <errno.h>

using namespace boost;
using namespace std;

#define INTERVAL 100000

enum readTypes  { people, webpages, friends, likes, defaultValue};

int  str2int(string str){
  readTypes retType = defaultValue;

  if (str == "<PEOPLE>")
    retType = people;

  else if (str == "<WEBPAGES>")
    retType = webpages;

  else if (str == "<FRIENDS>")
    retType = friends;

  else if (str == "<LIKES>")
    retType = likes;

  return retType;
}

struct GDReader {
private:
    unsigned int  _people;
    unsigned int  _webpages;
    unsigned int  _friends;
    unsigned int  _likes;
    
    unsigned int ReadState;

    vector<string> _nameList;
    map<string, unsigned int> _vertexMap;
    ifstream _GDfile;

public:
    string line;
    GDReader():_people(0),
                           _webpages(0),
                           _friends(0),
                           _likes(0)  {};

    void readFile(string filename) {
      try{
        _GDfile.open(filename.c_str());
        if (_GDfile.fail())
          throw 1;
       
        cout << "FreeReader: No graph building\n";
        while(getline(_GDfile, line) != NULL){

          int State = str2int(line);
          if(State != defaultValue) {
            ReadState = State;
          }

          switch(ReadState) {

          case people:
            readPeople(line);
            break;
          
          case webpages:
            readWebpages(line);
            break;
         
          case friends:
            readFriends(line);
            break;
          
          case likes:
            readLikes(line);
            break;

          case defaultValue:
            break;

          }//END_SWITCH
       } //END_WHILE

      _GDfile.close();
      cout << " people " << _people << "\n";
      cout << " webpages" << _webpages << "\n";
      cout << " friends " << _friends << "\n";
      cout << " likes " << _likes << "\n";
     }//END_TRY
    catch (int i){
      cout << "Error:"<< i <<"\tFailed to open file" <<endl;
      cerr << strerror(errno) << endl;
   }
  }//END_READFILE

  unsigned int  getPeopleCounter(){
    return _people;
  } 
 
  unsigned int  getWebpagesCounter(){
    return _webpages;
  }

  unsigned int  getFriendsCounter(){
    return _friends;
  }

  unsigned int  getLikesCounter(){
    return _likes;
  }

  vector<string> & getNameList() {
    return _nameList;
  }


private:

  void readPeople(string & line){

    string pid = "";
    string name = "";
    string age = "";
    string location = "";
    vector<string> attributes;

      if((line == "</PEOPLE>") || (line == "<PEOPLE>")) {
        return;
      }

      split(attributes, line, is_any_of("-"));

      pid = (attributes[0]);
      name = (attributes[1]);

      if(attributes[2] != "?" ){
        age = (attributes[2]);
      }
      if(attributes[3] != "?" ){
        location = (attributes[3]);
      }
      
      _nameList.push_back(name);
      unsigned int vp = _people;
      _vertexMap.insert(pair<string, unsigned int>(pid, vp));
      _people++;
      if ( _people % INTERVAL == 0)
        cout << "# people " <<_people << "\n";
  }
  
  void readWebpages(string line){
    string wpid = "";
    string wpurl = "";
    string wpdate = "";
    vector<string> attributes;

      if((line == "</WEBPAGES>") || (line == "<WEBPAGES>")){
        return;
      }
      split(attributes, line, is_any_of("-"));
      
      wpid = (attributes[0]);
      
      if(attributes[1] != "?" ){
        wpdate = (attributes[1]);
      }
       //insert vertex in Graph
      
     wpurl = "http://www.uwaterloo.ca/webpage" + wpid + ".html";

     unsigned int vw = _webpages + _people;
     _vertexMap.insert(pair<string, unsigned int>(wpid, vw));
     _webpages++;
     if( _webpages % INTERVAL == 0)
       cout << "# webpages " << _webpages << "\n";
//    }
  }

  void readFriends(string line){

    string eid  = "";
    string from = "";
    string to   = "";
    vector<string> attributes;

      if((line == "</FRIENDS>") || (line == "<FRIENDS>")){
        return;
      }
      split(attributes, line, is_any_of("-"));
      
      eid = (attributes[0]);
      from = (attributes[1]);
      to = (attributes[2]);

      if((_vertexMap.find(from) == _vertexMap.end()) || 
          (_vertexMap.find(to) == _vertexMap.end()) ) {
        cout << "Error: Failed to recognize " << from << "\t" << to << endl;
        exit(0);
      }

      _friends++;
      if(_friends % INTERVAL == 0) 
        cout << "# friend " << _friends << "\n";
  }

  void readLikes(string line) {
    string eid  = "";
    string from = "";
    string to   = "";
    vector<string> attributes;
    
      if(( line == "</LIKES>") || ( line == "<LIKES>")){
        return;
      }
      split(attributes, line, is_any_of("-"));
      
      eid = (attributes[0]);
      from = (attributes[1]);
      to = (attributes[2]);

      if((_vertexMap.find(from) == _vertexMap.end()) || 
          (_vertexMap.find(to) == _vertexMap.end()) ) {
        cout << "Error: Failed to recognize " << from << "\t" << to << endl;
        exit(0);
      }

      _likes++;
      if(_likes % INTERVAL == 0) 
        cout <<"# likes " << _likes  << "\n";
//      }
  }

};




#endif  /*_FILEREADER_H_*/
