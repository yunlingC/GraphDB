#include "GDBReader.h"
#include "LDBCReader.h"

#include <iostream>
#include <map>

using namespace std;

int main(int argc, char *argv[]) {
  
  typedef GraphType Graph;
  typedef Graph::VertexPointer VertexPointer;
  typedef std::map<int, int, std::less<int> > ordered_map;
  typedef std::multimap<int, int, std::less<int> > ordered_multimap;

  Graph g;
  cout << "Begin testing\n";

  unsigned int InputSize = 10000000;
  unsigned int Path = 0;

//  string GDBPath = "../tests/gd/sndata";
//  string LDBCPath = "../tests/ldbc/social_network_";
//  string Path = "";

  string FileName = "";

  if ( argc < 3) {
    std::cout <<"Need to choose a file\n";
    exit(1);
  } else {
    Path = std::stoi(argv[1]);
    /// GDBench 
    if (Path == 1) {
      InputSize = std::stoi(argv[2]);
      GDBReader reader(g);
      FileName =  "sndata" + std::to_string(InputSize)+".gd";
      string File =  "../tests/gd/" + FileName;
      std::cout <<"Read file name " << FileName <<"\n";
      reader.readFile(File);
    } 
    /// LDBC
    else if (Path == 2) {
      FileName = "social_network_100";
      std::cout <<"Read file name " << FileName <<"\n";
      LDBCReader reader(g);
      reader.readDirectory("../tests/ldbc/social_network_100");
    }
  }
  
  cout << "\nFinish reading\n";
  
  cout << "Finish analyzing data input\n";

  std::ofstream InputFile;

  InputFile.open("Input_" + FileName +".csv", std::ios_base::out | std::ios_base::app );
  
  
  ordered_map OutDegreeMap;
  ordered_map InDegreeMap; 

  ordered_map WebDegMap;

  ordered_multimap VertexDegMap;

  int InDegree = 0;
  int OutDegree = 0;

  int totalInDeg = 0;
  int totalOutDeg = 0;

  InputFile << FileName << "\tVertexId\tOutdegree\tIndegree\n";

  for (VertexPointer VertexPtr : g.getAllVertices())  {
    OutDegree = g.getOutEdges(VertexPtr).size();
    InDegree = g.getInEdges(VertexPtr).size();
//    InputFile << VertexPtr->getId() // << "\t" << VertexPtr->getType().std_str()
//              << "\t" << OutDegree
//              << "\t" << InDegree  << "\n";
//    fflush(stdout);

    if (OutDegreeMap.find(OutDegree) == OutDegreeMap.end()) {
      OutDegreeMap.insert(std::pair<int, int>(OutDegree, 0));
    }

    OutDegreeMap[OutDegree]++;
    totalOutDeg += OutDegree;

    if (InDegreeMap.find(InDegree) == InDegreeMap.end()) {
      InDegreeMap.insert(std::pair<int, int>(InDegree, 0));
    }

    InDegreeMap[InDegree]++;
    totalInDeg += InDegree;

    VertexDegMap.insert(std::pair<int, int>(OutDegree+InDegree, VertexPtr->getId()));

    if (VertexPtr->getId() > InputSize/2 -1 ) {
      if (WebDegMap.find(InDegree) == WebDegMap.end()) {
        WebDegMap.insert(std::pair<int, int>(InDegree, 0));
      }
      WebDegMap[InDegree]++;
    }
  }

  int lastNum = 0;

  InputFile << "Outdgree\n";
  for (auto OutEntry : OutDegreeMap)  {
    /// This while loop is for plotting purpose
    while ((OutEntry.first) > lastNum)  {
      InputFile << lastNum++ << "\t" << "?"<< "\n";
    }
    lastNum = (OutEntry.first) + 1;
    InputFile << OutEntry.first << "\t" << OutEntry.second << "\n";
  }

  InputFile << "Indgree\n";

  lastNum = 0;

  for (auto InEntry : InDegreeMap)  {
    while ((InEntry.first) > lastNum)  {
      InputFile << lastNum++ << "\t" << "?"<< "\n";
    }
    lastNum = (InEntry.first) + 1;
    InputFile << InEntry.first << "\t" << InEntry.second << "\n";
  }

  InputFile << "total outdegree \t" << totalOutDeg << "\ttotal Indgree\t" << totalInDeg << "\ttotal\t" << totalInDeg+totalOutDeg << "\n";

  InputFile << "\nVertex degree \n";

  for (auto VEntry : VertexDegMap)  {
//    if (VEntry.second > 49999)
      InputFile << VEntry.second << "\t" << VEntry.first<< "\n";
  }

  InputFile << "\nWebpage degree \n";

  int totalNum = 0;
  for (auto WebEntry : WebDegMap ) {
    totalNum += WebEntry.second;
    InputFile << WebEntry.first << "\t" << WebEntry.second << "\t" << totalNum << "\n";
  }

  InputFile.close();

}
