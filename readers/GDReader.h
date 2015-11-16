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
#include "PropertyList.h"

using namespace boost;
using namespace std;

//#define INTERVAL 100000

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
    unsigned int  ReadState;
    unsigned int INTERVAL;

    vector<string> _nameList;
    ifstream _GDfile;

public:
    string line;
    GDReader(Graph &graph):_graph(graph),
                           _people(0),
                           _webpages(0),
                           _friends(0),
                           _likes(0)  {};

    void setPrintInterval(unsigned int interv) {
      INTERVAL = interv;
    }

    void readFile(string filename) {
      try{
        _GDfile.open(filename.c_str());
        if (_GDfile.fail())
          throw 1;
       
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
      cout << "\n";
      cout << "-- people " << _people << "\n";
      cout << "-- webpages" << _webpages << "\n";
      cout << "-- friends " << _friends << "\n";
      cout << "-- likes " << _likes << "\n";
      cout << "\n";
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

    string age = "";
    string location = "";
    vector<string> attributes;

    VertexPropertyList peopleProp;

    if((line == "</PEOPLE>") || (line == "<PEOPLE>")) {
      return;
    }

    split(attributes, line, is_any_of("-"));

    if(attributes[2] != "?" ){
      age = (attributes[2]);
    }
    if(attributes[3] != "?" ){
      location = (attributes[3]);
    }
    //insert vertex in Graph
    peopleProp.set("pid", attributes[0]);
    peopleProp.set("name",attributes[1]);
    peopleProp.set("age", age);
    peopleProp.set("location",location);
      
    _nameList.push_back(attributes[1]);
    _graph.addVertex(peopleProp);
    _people++;
//    if ( _people % INTERVAL == 0)
//      cout << "# people " <<_people << "\n";
  }
  
  void readWebpages(string line){

    VertexPropertyList webpagesProp;
    string wpurl = "";
    string wpdate = "";
    vector<string> attributes;

    if((line == "</WEBPAGES>") || (line == "<WEBPAGES>")){
      return;
    }
    split(attributes, line, is_any_of("-"));
    
    if(attributes[1] != "?" ){
      wpdate = (attributes[1]);
    }
      
    wpurl = "http://www.uwaterloo.ca/webpage" + attributes[0] + ".html";

    webpagesProp.set("wpid",attributes[0]);
    webpagesProp.set("wpurl",wpurl);
    webpagesProp.set("wpdate",wpdate);

    _graph.addVertex(webpagesProp);
    _webpages++;
//    if( _webpages % INTERVAL == 0)
//      cout << "# webpages " << _webpages << "\n";
  }

  void readFriends(string line){

    vector<string> attributes;

    EdgePropertyList friendsProp;
    if((line == "</FRIENDS>") || (line == "<FRIENDS>")){
      return;
    }
    split(attributes, line, is_any_of("-"));
    
       //insert vertex in Graph
    friendsProp.set("eid", attributes[0]);
    friendsProp.set("endpts", attributes[1] + "-" + attributes[2]);

    _graph.addEdge(stoi(attributes[1])-1, stoi(attributes[2])-1, "FRIENDS", friendsProp);
    _friends++;
//    if(_friends % INTERVAL == 0) 
//      cout << "# friend " << _friends << "\n";
  }

  void readLikes(string line) {

    vector<string> attributes;
    
    EdgePropertyList likesProp;
    if(( line == "</LIKES>") || ( line == "<LIKES>")){
      return;
    }
    split(attributes, line, is_any_of("-"));
      

    likesProp.set("eid", attributes[0]);
    likesProp.set("endpts", attributes[1]+ "-" + attributes[2]);


    _graph.addEdge(stoi(attributes[1])-1, stoi(attributes[2])-1, "LIKES", likesProp);
    // _graph.addEdge(vs, vd, "LIKES");
    _likes++;
//     if(_likes % INTERVAL == 0) 
 //     cout <<"# likes " << _likes  << "\n";
  }

};




#endif  /*_FILEREADER_H_*/
