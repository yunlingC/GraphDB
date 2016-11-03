//===-- readers/GDBReader.h - class to read from GDbench output ---*- C++ -*-===//
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

#include "GraphType.h"

#include <fstream>
/// This lib is used for split functions 
#include <boost/algorithm/string.hpp>
#include <map>

enum DataLabel {  DL_People
                , DL_Webpages
                , DL_Friends
                , DL_Likes
                , DL_Default
                };

class GDBReader {
public:
  typedef GraphType Graph;
  typedef Graph::VertexDescriptor VertexDescriptor;
  typedef Graph::VertexPropertyList VertexPropertyList;
  typedef Graph::EdgePropertyList EdgePropertyList;
private:
    Graph &_Graph;
    unsigned int  _People;
    unsigned int  _Webpages;
    unsigned int  _Friends;
    unsigned int  _Likes;
    
    std::vector<std::string> _NameList;
    std::vector<std::string> _PersonIdList;
 
    std::map<std::string, VertexDescriptor> _VertexMap;
    std::ifstream _GDfile;

public:
    std::string Str_Line;
    GDBReader(Graph &graph):_Graph(graph){};

    void readFile(std::string FileName) {
      try{
        _GDfile.open(FileName.c_str());
        if (_GDfile.fail())
          throw 1;
       
        while (getline(_GDfile, Str_Line)){
          switch(String2Int(Str_Line)) {
          
            case DL_People:
              readPeople();
              break;
          
            case DL_Webpages:
              readWebpages();
              break;
         
            case DL_Friends:
              readFriends();
              break;
          
            case DL_Likes:
              readLikes();
              break;

              ///TODO bring it back later
//            default:
//              cout << "Error: " << "2" <<"\t Failed to recognize type" << std::endl;
          }//END_SWITCH
        } //END_WHILE
      }//END_TRY

    catch (int i){
//      cout << "Error:"<< i <<"\tFailed to open file" << std::endl;
    }
      _GDfile.close();

  } //END_READFILE

  unsigned int  getPeopleCounter(){
    return _People;
  } 
 
  unsigned int  getWebpagesCounter(){
    return _Webpages;
  }

  unsigned int  getFriendsCounter(){
    return _Friends;
  }

  unsigned int  getLikesCounter(){
    return _Likes;
  }

  std::vector<std::string> & getNameList() {
    return _NameList;
  }

  std::vector<std::string> & getPidList() {
    return _PersonIdList;
  }

private:
  unsigned int  String2Int(std::string Type){
    DataLabel Label = DL_Default;
  
    if (Type == "<PEOPLE>")
      Label = DL_People;
  
    else if (Type == "<WEBPAGES>")
      Label = DL_Webpages;
  
    else if (Type == "<FRIENDS>")
      Label = DL_Friends;
  
    else if (Type == "<LIKES>")
      Label = DL_Likes;
   
    return Label;
  }

  void readPeople(){

    _People = 0;
    std::vector<std::string> attributes;

    while (getline(_GDfile, Str_Line)){

      VertexPropertyList PeopleProp;

      if (Str_Line == "</PEOPLE>") {
        return;
      }

      boost::split(attributes, Str_Line, boost::is_any_of("-"));


      PeopleProp.set("pid", attributes[0]);
      PeopleProp.set("name", attributes[1]);

      if (attributes[2] != "?" ){
        PeopleProp.set("age", attributes[2]);
      }

      if (attributes[3] != "?" ){
        PeopleProp.set("location", attributes[3]);
      }

       //insert vertex in Graph
      _NameList.push_back(attributes[1]);
      _PersonIdList.push_back(attributes[0]);
      VertexDescriptor Vertex = _Graph.addVertex(PeopleProp);
      _VertexMap.insert(std::pair<std::string, VertexDescriptor>
                                  (attributes[0], Vertex));
      _People++;

    }
  }
  
  void readWebpages(){
    _Webpages = 0;
    std::string URL = "";
    VertexPropertyList WebpagesProp;
    std::vector<std::string> attributes;

    while (getline(_GDfile, Str_Line)){
      VertexPropertyList WebpagesProp;
      if (Str_Line == "</WEBPAGES>"){
        return;
      }
      boost::split(attributes, Str_Line, boost::is_any_of("-"));
      
      WebpagesProp.set("wpid", attributes[0]);
      
      if (attributes[1] != "?" ){
        WebpagesProp.set("wpdate", attributes[1]);
      }

     //insert vertex in Graph
     URL = "www.uwaterloo.ca/web" + attributes[0] + ".html";
     WebpagesProp.set("URL", URL);

     VertexDescriptor Vertex = _Graph.addVertex(WebpagesProp);
     _VertexMap.insert(std::pair<std::string, VertexDescriptor>
                                  (attributes[0], Vertex));
     _Webpages++;

    }
  }

  void readFriends(){

    VertexDescriptor Source, Dest;
    std::string from = "";
    std::string to   = "";
    std::vector<std::string> attributes;

    while (getline(_GDfile, Str_Line)){
      EdgePropertyList FriendProp;
      if (Str_Line == "</FRIENDS>"){
        return;
      }
      boost::split(attributes, Str_Line, boost::is_any_of("-"));
      
      from = (attributes[1]);
      to = (attributes[2]);
       //insert vertex in Graph
      FriendProp.set("eid", attributes[0]);

      if ((_VertexMap.find(from) == _VertexMap.end()) || 
          (_VertexMap.find(to) == _VertexMap.end()) ) {
//        cout << "Error: Failed to recognize " << from << "\t" << to << endl;
        exit(0);
      }

      Source = _VertexMap.at(from);
      Dest = _VertexMap.at(to) ;
      
      _Graph.addEdge(Source, Dest, "FRIENDS", FriendProp);
      _Friends++;

    }
  }

  void readLikes() {
    VertexDescriptor Source, Dest;
    std::string from = "";
    std::string to   = "";
    std::vector<std::string> attributes;
    
    while (getline(_GDfile, Str_Line)){
      EdgePropertyList LikesProp;
      if (Str_Line == "</LIKES>"){
        return;
      }
      boost::split(attributes, Str_Line, boost::is_any_of("-"));
      
      from = (attributes[1]);
      to = (attributes[2]);

      LikesProp.set("eid", attributes[0]);

      if ((_VertexMap.find(from) == _VertexMap.end()) || 
          (_VertexMap.find(to) == _VertexMap.end()) ) {
//        cout << "Error: Failed to recognize " << from << "\t" << to << endl;
        exit(0);
      }

      Source = _VertexMap.at(from);
      Dest = _VertexMap.at(to);

      _Graph.addEdge(Source, Dest, "LIKES", LikesProp);
      _Likes++;
    }
  }
};

#endif  /*_GDBREADER_H_*/
