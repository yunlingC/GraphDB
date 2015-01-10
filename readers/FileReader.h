#ifndef _GDREADER_H_
#define _GDREADER_H_

#include <fstream>
#include <boost/algorithm/string.hpp>
#include <map>
#include <exception>
#include <errno.h>
#include "Graph.h"

#define TRUE (1 == 1)
#define FALSE (!TRUE)

using namespace boost;
using namespace std;

struct GDReader {

  private:
    Graph &_graph;
    string _line;
    unsigned int  _people;
    unsigned int  _webpages;
    unsigned int  _friends;
    unsigned int  _likes;
    
    map<string, Graph::VertexDescriptor> _vertexMap;
    ifstream _GDfile;

  public:
    GDReader(Graph &graph):_graph(graph){};

    void readFile(string filename) {

      try{
        _GDfile.open(filename.c_str());
        if (_GDfile.fail())
          throw 1;

        while(getline(_GDfile, _line) != NULL){
          if (_line == "<PEOPLE>" ) { 
              readPeople();
              continue;
          }
          if (_line == "<WEBPAGES>") {
              readWebpages();
              continue;
          } 
          if (_line == "<FRIENDS>") {
              readFriends();
              continue;
          } 
          if (_line ==  "<LIKES>") {
              readLikes();
              continue;
          }
       } 
     }
    catch (int i){
      cout << "Error:"<< i <<"\tFailed to open file" <<endl;
      cerr << strerror(errno) << endl;
   }

      _GDfile.close();

     // _graph.print();

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

private:

  void readPeople(){

    _people = 0;
    string pid = "";
    string name = "";
    string age = "";
    string location = "";
    vector<string> attributes;

    while(getline(_GDfile,_line) != NULL){

      Graph::VertexPropertyList peopleProp;

      if(_line == "</PEOPLE>") {
        return;
      }

      split(attributes, _line, is_any_of("-"));

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
      
      Graph::VertexDescriptor vp = _graph.addVertex(peopleProp);
      _vertexMap.insert(pair<string, Graph::VertexDescriptor>(pid, vp));
      _people++;
    
    }
  }
  

  void readWebpages(){

    _webpages = 0;
    Graph::VertexPropertyList webpagesProp;
    string wpid = "";
    string wpurl = "";
    string wpdate = "";
    vector<string> attributes;

    while(getline(_GDfile, _line) != NULL){
      Graph::VertexPropertyList webpagesProp;
      if(_line == "</WEBPAGES>"){
        return;
      }
      split(attributes, _line, is_any_of("-"));
      
      wpid = (attributes[0]);
      
      if(attributes[1] != "?" ){
        wpdate = (attributes[1]);
      }
       //insert vertex in Graph
      
     wpurl = "http://www.uwaterloo.ca/webpage" + wpid + ".html";

     webpagesProp.set("wpid",wpid);
     webpagesProp.set("wpurl",wpurl);
     webpagesProp.set("wpdate",wpdate);

     Graph::VertexDescriptor vw = _graph.addVertex(webpagesProp);
     _vertexMap.insert(pair<string, Graph::VertexDescriptor>(wpid, vw));
     _webpages++;
    }
  }

  void readFriends(){

    Graph::VertexDescriptor vs, vd;
    string eid  = "";
    string from = "";
    string to   = "";
    vector<string> attributes;

    while(getline(_GDfile, _line) != NULL){
      Graph::EdgePropertyList friendsProp;
      if(_line == "</FRIENDS>"){
        return;
      }
      split(attributes, _line, is_any_of("-"));
      
      eid = (attributes[0]);
      from = (attributes[1]);
      to = (attributes[2]);
       //insert vertex in Graph
      friendsProp.set("eid", eid);
      friendsProp.set("endpts", from + "-" + to);

      vs = _vertexMap.at(from);
      vd = _vertexMap.at(to) ;
      
      _graph.addEdge(vs, vd, "FRIENDS", friendsProp);
      _friends++;
      }
  }

  void readLikes() {

    Graph::VertexDescriptor vs, vd;
    string eid  = "";
    string from = "";
    string to   = "";
    vector<string> attributes;
    //property_visitor<string, string, Graph::VertexDescriptor> vis;
    //property_visitor vis;
    
    while(getline(_GDfile, _line) != NULL){
      Graph::EdgePropertyList likesProp;
      if(_line == "</LIKES>"){
        return;
      }
      split(attributes, _line, is_any_of("-"));
      
      eid = (attributes[0]);
      from = (attributes[1]);
      to = (attributes[2]);

      likesProp.set("eid", eid);
      likesProp.set("endpts", from + "-" + to);

      vs = _vertexMap.at(from);
      vd = _vertexMap.at(to);

      _graph.addEdge(vs, vd, "LIKES", likesProp);
      _likes++;
      }
  }
};




#endif  /*_FILEREADER_H_*/
