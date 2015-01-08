#ifndef _GDREADER_H_
#define _GDREADER_H_

//#include <string>
//#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/graph/breadth_first_search.hpp>
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
    Graph graph;
    string line;
    long people;
    long webpages;
    long friends;
    long likes;
    map<string, Graph::VertexDescriptor> vertexMap;
    ifstream GDfile;
  public:
    GDReader(Graph _graph){
    graph = _graph;
  }

    void readFile() {

      try{

      GDfile.open("sndata.gd");
      if (GDfile.fail())
        throw 1;

      while(getline(GDfile, line) != NULL){
        if(line == "<PEOPLE>" ){
          readPeople();
          continue;
        }  
         if(line == "<WEBPAGES>") {
          readWebpages();
          continue;
        }
         if(line == "<FRIENDS>") {
          readFriends();
          continue;
        } 
         if(line == "<LIKES>") { 
          readLikes();
          continue;
         }
      }
     } catch (int i){
       cout << "Error:"<< i <<"\tFailed to open file" <<endl;
       cerr << strerror(errno) << endl;
     }

    GDfile.close();

    graph.print();

  }//END_READFILE

  long getPeopleCounter(){
    return people;
  } 
  long getWebpagesCounter(){
    return webpages;
  }

  long getFriendsCounter(){
    return friends;
  }

  long getLikesCounter(){
    return likes;
  }

private:

  void readPeople(){

    people = 0;
    string pid = "";
    string name = "";
    string age = "";
    string location = "";
    vector<string> attributes;


    while(getline(GDfile,line) != NULL){

      Graph::VertexPropertyList peopleProp;
      if(line == "</PEOPLE>") {
        return;
      }
      split(attributes, line, is_any_of("-"));
      if(attributes[0] != "?"){
        pid = (attributes[0]);
      }
      if(attributes[1] != "?" ){
        name = (attributes[1]);
      }
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
      
      Graph::VertexDescriptor vp = graph.addVertex(peopleProp);
      vertexMap.insert(pair<string, Graph::VertexDescriptor>(pid, vp));
      people++;
      }
  }


  
  void readWebpages(){

    webpages = 0;
    Graph::VertexPropertyList webpagesProp;
    string wpid = "";
    string wpurl = "";
    string wpdate = "";
    vector<string> attributes;

    while(getline(GDfile,line) != NULL){
      Graph::VertexPropertyList webpagesProp;
      if(line == "</WEBPAGES>"){
        return;
      }
      split(attributes, line, is_any_of("-"));
      if(attributes[0] != "?" ){
        wpid = (attributes[0]);
      }
      if(attributes[1] != "?" ){
        wpdate = (attributes[1]);
      }
       //insert vertex in Graph
      
     wpurl = "http://www.uwaterloo.ca/webpage" + wpid + ".html";

     webpagesProp.set("wpid",wpid);
     webpagesProp.set("wpurl",wpurl);
     webpagesProp.set("wpdate",wpdate);

     Graph::VertexDescriptor vw = graph.addVertex(webpagesProp);
     vertexMap.insert(pair<string, Graph::VertexDescriptor>(wpid, vw));
     webpages++;
    }
  }

  void readFriends(){

    Graph::VertexDescriptor vs, vd;
    string eid  = "";
    string from = "";
    string to   = "";
    vector<string> attributes;

    while(getline(GDfile,line) != NULL){
      Graph::EdgePropertyList friendsProp;
      if(line == "</FRIENDS>"){
        return;
      }
      split(attributes, line, is_any_of("-"));
      
      if(attributes[0] != "?" ){
        eid = (attributes[0]);
      }
      if(attributes[1] != "?" ){
        from = (attributes[1]);
      }
       if(attributes[2] != "?" ){
        to = (attributes[2]);
      }
       //insert vertex in Graph
      friendsProp.set("eid", eid);
      friendsProp.set("endpts", from + "-" + to);

      vs = vertexMap.at(from);
      vd = vertexMap.at(to) ;
      
      graph.addEdge(vs, vd, "FRIENDS", friendsProp);
      friends++;
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
    
    while(getline(GDfile,line) != NULL){
      Graph::EdgePropertyList likesProp;
      if(line == "</LIKES>"){
        return;
      }
      split(attributes, line, is_any_of("-"));
      
      if(attributes[0] != "?" ){
        eid = (attributes[0]);
      }
      if(attributes[1] != "?" ){
        from = (attributes[1]);
      }
       if(attributes[2] != "?" ){
        to = (attributes[2]);
      }

      likesProp.set("eid", eid);
      likesProp.set("endpts", from + "-" + to);

      vs = vertexMap.at(from);
      vd = vertexMap.at(to);

      graph.addEdge(vs, vd, "LIKES", likesProp);
      friends++;
      }
  }
};




#endif  /*_FILEREADER_H_*/
