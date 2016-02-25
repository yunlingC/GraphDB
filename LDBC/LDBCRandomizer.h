//===-- LDBC/LDBCRandomizer.h - class to provide randomize input---*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the class to provide randomized input for LDBC queries.
///
//===----------------------------------------------------------------------===//

#ifndef _LDBCRANDOMIZER_H_
#define _LDBCRANDOMIZER_H_

#include <fstream>

class LDBCRandomizer {
public:
//  auto getFileAttribute(std::string FileName, std::string Attr) {
//    _FileName = FileName;
//    _Attr = Attr;
//  }

  auto getFileName(const std::string & FileName) 
    -> void {
      _FileName = FileName;
      readPersonFile();
  }

  auto getRandPersonInput(unsigned int num);
    

  auto getIntervalPersonInput(unsigned int num, std::vector<unsigned int> & params) 
  -> void {
    _Num = _InputParams.size();
    unsigned int interval = 0;
    if ( (_Num != 0 ) && (num != 0) && (_Num >= num) ) {
      interval = _Num/num;
//      _Start = std::stoi(_inputParams[0]);
    } else {
      std::cout <<"Error :\tFailed to get any input\n";
    }
    unsigned int j = 0;
    params.clear();
    for ( unsigned int i = 0; i < num; i++ ) {
      params.push_back(std::stoi(_InputParams[j]));
      j += interval;
    }
  }

private:
  auto readPersonFile() 
    -> void {
    try{
      _InputFile.open(_FileName.c_str(), std::ifstream::in);
      if ( _InputFile.fail() ) {
        throw 1;
      }
      std::string line;
      std::vector<std::string> Attrs;

      //skip the first line, which is the attribute name
      if (getline(_InputFile, line) != NULL );
       
      while(getline(_InputFile, line) != NULL) {
        boost::split(Attrs, line, boost::is_any_of("|"));
        _InputParams.push_back(Attrs[0]);
      }

      _InputFile.close();
    } catch (unsigned int i) {
      std::cout << "Error :" << i << "\tFailed to open file\n";
      exit(0);
    }
  }

private:
  std::string _FileName;
  std::string _Attr;
  std::ifstream _InputFile;
  std::vector<std::string> _InputParams;
  unsigned int _Start = 0;
  unsigned int _Num = 0;
};



#endif /*_LDBCRANDOMIZER_H_*/
