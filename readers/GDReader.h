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

#include "GraphType.h"

using namespace boost;
using namespace std;

enum readTypes  { people, webpages, friends, likes };

int  str2int(string str){
  readTypes retType;

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
public:
  typedef GraphType Graph;
  typedef Graph::VertexDescriptor VertexDescriptor;
  typedef Graph::VertexPropertyList VertexPropertyList;
  typedef Graph::EdgePropertyList EdgePropertyList;
private:
    Graph &_graph;
    unsigned int  _people;
    unsigned int  _webpages;
    unsigned int  _friends;
    unsigned int  _likes;
    
    vector<string> _nameList;
    vector<string> _pidList;
 
    map<string, VertexDescriptor> _vertexMap;
    ifstream _GDfile;

public:
    string line;
    GDReader(Graph &graph):_graph(graph){};

    void readFile(string filename) {
      try{
        _GDfile.open(filename.c_str());
        if (_GDfile.fail())
          throw 1;
       
        while(getline(_GDfile, line) != NULL){
          switch(str2int(line)) {
          
            case people:
              readPeople();
              break;
          
            case webpages:
              readWebpages();
              break;
         
            case friends:
              readFriends();
              break;
          
            case likes:
              readLikes();
              break;

            default:
              cout << "Error: " << "2" <<"\t Failed to recognize type" << endl;
          }//END_SWITCH
       } //END_WHILE
     }//END_TRY
    catch (int i){
      cout << "Error:"<< i <<"\tFailed to open file" <<endl;
      cerr << strerror(errno) << endl;
   }
      _GDfile.close();

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

  vector<string> & getPidList() {
    return _pidList;
  }

private:

  void readPeople(){

    _people = 0;
    string pid = "";
    string name = "";
    string age = "";
    string location = "";
    vector<string> attributes;

    while(getline(_GDfile,line) != NULL){

      VertexPropertyList peopleProp;

      if(line == "</PEOPLE>") {
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
       //insert vertex in Graph
      peopleProp.set("pid", pid);
      peopleProp.set("name",name);
      peopleProp.set("age", age);
      peopleProp.set("location",location);
      
      _nameList.push_back(name);
      _pidList.push_back(pid);
      VertexDescriptor vp = _graph.addVertex(peopleProp);
      _vertexMap.insert(pair<string, VertexDescriptor>(pid, vp));
      _people++;
    
    }
  }
  
  void readWebpages(){
    _webpages = 0;
    VertexPropertyList webpagesProp;
    string wpid = "";
    string wpurl = "";
    string wpdate = "";
    vector<string> attributes;

    while(getline(_GDfile, line) != NULL){
      VertexPropertyList webpagesProp;
      if(line == "</WEBPAGES>"){
        return;
      }
      split(attributes, line, is_any_of("-"));
      
      wpid = (attributes[0]);
      
      if(attributes[1] != "?" ){
        wpdate = (attributes[1]);
      }
       //insert vertex in Graph
      
     wpurl = "http://www.uwaterloo.ca/webpage" + wpid + ".html";

     webpagesProp.set("wpid",wpid);
     webpagesProp.set("wpurl",wpurl);
     webpagesProp.set("wpdate",wpdate);

     VertexDescriptor vw = _graph.addVertex(webpagesProp);
     _vertexMap.insert(pair<string, VertexDescriptor>(wpid, vw));
     _webpages++;
    }
  }

  void readFriends(){

    VertexDescriptor vs, vd;
    string eid  = "";
    string from = "";
    string to   = "";
    vector<string> attributes;

    while(getline(_GDfile, line) != NULL){
      EdgePropertyList friendsProp;
      if(line == "</FRIENDS>"){
        return;
      }
      split(attributes, line, is_any_of("-"));
      
      eid = (attributes[0]);
      from = (attributes[1]);
      to = (attributes[2]);
       //insert vertex in Graph
      friendsProp.set("eid", eid);
      friendsProp.set("endpts", from + "-" + to);

      if((_vertexMap.find(from) == _vertexMap.end()) || 
          (_vertexMap.find(to) == _vertexMap.end()) ) {
        cout << "Error: Failed to recognize " << from << "\t" << to << endl;
        exit(0);
      }
      vs = _vertexMap.at(from);
      vd = _vertexMap.at(to) ;
      
      _graph.addEdge(vs, vd, "FRIENDS", friendsProp);
      // _graph.addEdge(vs, vd, "FRIENDS");
      _friends++;
      }
  }

  void readLikes() {
    VertexDescriptor vs, vd;
    string eid  = "";
    string from = "";
    string to   = "";
    vector<string> attributes;
    
    while(getline(_GDfile, line) != NULL){
      EdgePropertyList likesProp;
      if(line == "</LIKES>"){
        return;
      }
      split(attributes, line, is_any_of("-"));
      
      eid = (attributes[0]);
      from = (attributes[1]);
      to = (attributes[2]);

      likesProp.set("eid", eid);
      likesProp.set("endpts", from + "-" + to);

      if((_vertexMap.find(from) == _vertexMap.end()) || 
          (_vertexMap.find(to) == _vertexMap.end()) ) {
        cout << "Error: Failed to recognize " << from << "\t" << to << endl;
        exit(0);
      }
      vs = _vertexMap.at(from);
      vd = _vertexMap.at(to);

      _graph.addEdge(vs, vd, "LIKES", likesProp);
      // _graph.addEdge(vs, vd, "LIKES");
      _likes++;
      }
  }
};




#endif  /*_FILEREADER_H_*/
