#ifndef _RECURSIVE_DEPTH_FIRST_SEARCH_H_
#define _RECURSIVE_DEPTH_FIRST_SEARCH_H_

#include "GraphType.h"

void RDFSImp(GraphType & g, GraphType::VertexDescriptor vs, std::map<GraphType::VertexPointer, bool> & colorMap) {

  typedef std::pair<GraphType::VertexPointer, bool> ColorPair;
  typedef std::map<GraphType::VertexPointer, bool> ColorMap;

  GraphType::VertexPointer vp = g.getVertexPointer(vs);
  colorMap.insert(ColorPair(vp, true));
    cout << "==> vid: " << vp->getId() << endl;
  // Get the outgoing edges for vertex
  std::vector<Edge::EdgePtr> & Edges = vp->getOutEdges();

  // Iterate over all the edges.
  for (auto eit = Edges.begin(); eit != Edges.end(); eit++) {
    GraphType::VertexPointer target = (*eit)->getTarget(vp);

    // Get color and check if false.
    auto visitedVertex = colorMap.find(target);
    if ( visitedVertex == colorMap.end() ) {
      RDFSImp(g, target->getId(), colorMap);
    } else {
      colorMap.insert(ColorPair(target, true));
    }
  }

};

void RecursiveDepthFirstSearch(GraphType & g, GraphType::VertexDescriptor vs) {

  typedef std::pair<GraphType::VertexPointer, bool> ColorPair;
  typedef std::map<GraphType::VertexPointer, bool> ColorMap;

  //  GraphType::VertexPointer vp = g.getVertexPointer(vs);
  ColorMap Color;
  //Color.insert(ColorPair(vp, true));
  RDFSImp(g, vs, Color);

  // // Get the outgoing edges for vertex
  // std::vector<Edge::EdgePtr> & Edges = vp->getEdges();

  // // Iterate over all the edges.
  // for (Edge::EdgePtr eit = Edges.begin(); eit != Edges.end(); eit++) {
  //   VertexPointer target = eit->getTarget(vp);

  //   // Get color and check if false.
  //   auto visitedVertex = Color.find(target);
  //   if ( visitedVertex == Color.end() ) {
  //     RecursiveDepthFirstSearchImplementation(target->getId(), g, Color);
  //   } else {
  //     Color.insert(ColorPair(target, true));
  //   }
  // }
  
  
};
#endif /* _RECURSIVE_DEPTH_FIRST_SEARCH_H_ */
