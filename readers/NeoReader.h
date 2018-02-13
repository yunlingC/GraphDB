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
#ifndef _NEOREADER_H_
#define _NEOREADER_H_

#include <fstream>
/// This lib is used for split functions 
#include <boost/algorithm/string.hpp>
#include <map>
#include <iostream>

class NeoReader{
private:
    unsigned int  _People;
    unsigned int  _Webpages;
    unsigned int  _Friends;
    unsigned int  _Likes;
    
    std::vector<std::string> Q1;
    std::vector<uint64_t> Q2; 
    std::vector<uint64_t> Q3; 
 
    std::ifstream _NeoInput;


public:
    std::string Str_Line;
    NeoReader() {};

    std::vector<std::string> getInput(){
      return Q1;
    }

    void readFile(std::string FileName) {
//      try{
        _NeoInput.open(FileName.c_str());
        if (_NeoInput.fail())
          exit(0);
//          throw 1;

        while (getline(_NeoInput, Str_Line) != NULL){
//          switch(String2Int(Str_Line)) {
//          
//            case T_NAME:
              readInput();
//              break;
          
//            default:
//              cout << "Error: " << "2" <<"\t Failed to recognize type" << std::endl;
//          }//END_SWITCH
        } //END_WHILE
//      }//END_TRY

//    catch (int i){
//     std::cout << "Cannot read file\n";
//    }
      _NeoInput.close();

  } //END_READFILE

private:
//  unsigned int  String2Int(std::string Type){
//    DataLabel Label = DL_Default;
//  
//    if (Type == "<PEOPLE>")
//      Label = T_NAME;
//  
//    else if (Type == "<WEBPAGES>")
//      Label = DL_Webpages;
//  
//    else if (Type == "<FRIENDS>")
//      Label = DL_Friends;
//  
//    else if (Type == "<LIKES>")
//      Label = DL_Likes;
//   
//    return Label;
//  }

  void readInput(){

//    int _Counter = 0;
    std::vector<std::string> attributes;

    while (getline(_NeoInput, Str_Line) != NULL ){

      boost::split(attributes, Str_Line, boost::is_any_of(";"));
      
      Q1.push_back(attributes[2]);
    }
  }

};

#endif  /*_NEOREADER_H_*/
