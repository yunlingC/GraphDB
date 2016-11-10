#include "GDBReader.h"

#include <iostream>
#include <unordered_map>

using namespace std;

int main(int argc, char *argv[]) {
  
  typedef GraphType Graph;
  typedef Graph::VertexPointer VertexPointer;

  Graph g;
  cout << "Begin testing\n";

  unsigned int InputSize;
  if ( argc < 2) {
    std::cout <<"Need to choose a file\n";
    exit(1);
  } else {
    InputSize = std::stoi(argv[1]);
//    if ( (InputSize != 1000) && (InputSize != 5000) ) {
//      std::cout <<"Arg error : should be: 1000 or 5000\n";
//      exit(1);
//    }
  }
  
#ifdef _FIXALLOC_
  g.allocVertexMemory(InputSize*1.1);
  g.allocEdgeMemory(InputSize*20);
#endif /* _FIXALLOC */
 
  GDBReader reader(g);
  std::string FileName = "../tests/gd/sndata" +std::to_string(InputSize)+".gd";
  std::cout <<"Read file name " << FileName <<"\n";
  reader.readFile(FileName);

  cout << "Finish reading\n";
  
  cout << "Finish analyzing data input\n";

  std::ofstream InputFile;

  InputFile.open("Input_Data_"+std::to_string(InputSize)+".csv", std::ios_base::out | std::ios_base::app );
  
  InputFile << FileName << "\tVertexId\tOutdegree\tIndegree\n";
  
  unordered_map<int, int, std::greater<int>> OutDegreeMap;
  unordered_map<int, int, std::greater<int>> InDegreeMap; 

  int InDegree = 0;
  int OutDegree = 0;

  for (VertexPointer VertexPtr : g.getAllVertices())  {
    OutDegree = g.getOutEdges(VertexPtr).size();
    InDegree = g.getInEdges(VertexPtr).size();
    InputFile << VertexPtr->getId() // << "\t" << VertexPtr->getType().std_str()
              << "\t" << OutDegree
              << "\t" << InDegree  << "\n";
    fflush(stdout);
    if (OutDegreeMap.find())
  }

  InputFile.close();

}
