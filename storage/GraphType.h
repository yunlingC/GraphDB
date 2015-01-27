#ifndef _BASE_GRAPH_H_
#define _BASE_GRAPH_H_

#include <vector>
#include <new>
#include <stdlib.h>

#include "Vertex.h"
#include "Edge.h"
#include "PropertyList.h"

using namespace std;

class GraphType {
public:
  typedef unsigned int VertexDescriptor;
  typedef unsigned int EdgeDescriptor;
  typedef Vertex* VertexPtr;
  typedef Edge* EdgePtr;
  typedef PropertyList<string, string> PropertyListType;
  typedef PropertyListType VertexPropertyList;
  typedef PropertyListType EdgePropertyList;

public:
  VertexPtr getVertexPointer(VertexDescriptor v) {
    if ( (v > _vertices.size() - 1) || (v < 0) ) {
      return NULL;
    }
    return _vertices[v];
  }

  VertexDescriptor addVertex() {
    /* if (_nodeMemory == NULL) { */
    /*   cerr << "ERROR: Edge space not allocated\n"; */
    /*   exit(1); */
    /* } */

    // Create new node
    //    char* placePtr = _nodeMemory + _numVertices*sizeof(Vertex);
    //    cout << "Place node at: " << reinterpret_cast<int*>(placePtr) << endl;

    VertexPtr n = new Vertex();

    n->setId(_numVertices); 
    _numVertices++;
    // Insert into list.
    _vertices.push_back(n);
    return n->getId();
  }

  VertexDescriptor addVertex(PropertyListType & p) {
    VertexPtr n = new Vertex();
    n->setPropertyList(p);
    n->setId(_numVertices); 
    _numVertices++;
    // Insert into list.
    _vertices.push_back(n);
    return n->getId();
  }

  void chainEdge(VertexPtr fvp, EdgePtr fnx, EdgePtr e) {
    EdgePtr fne = fnx;
    EdgePtr fpe = fne;
    
    // Setup  for the first vertex
    while (fne != NULL) {
      cout << "+ walk " << fne->getId() << endl;
      fpe = fne;
      if (fne->getFirstVertexPtr() == fvp) {
	cout << "+ down firstNext\n";
	fne = fne->getFirstNextEdge();
      } else if (fne->getSecondVertexPtr() == fvp) {
	cout << "+ down secondNext\n";
	fne = fne->getSecondNextEdge();
      }

    }

    cout << "end of chain edge: " << fpe->getId() << endl;
    // Got to the end of the chain.
    if (fpe == e)  {
      cout << "FIRST EDGE: " << endl;
      return;
    }

    if (fpe->getFirstVertexPtr() == fvp) {
      cout << "hooking up " << fpe->getId() << " to " << e->getId() << endl;
      fpe->setFirstNextEdge(e);
      if (e->getFirstVertexPtr() == fvp) {
	e->setFirstPreviousEdge(fpe);
	e->setFirstNextEdge(NULL); 
      } else {
	e->setSecondPreviousEdge(fpe);
	e->setSecondNextEdge(NULL); 
      }

    } else if (fpe->getSecondVertexPtr() == fvp) {
      cout << "hooking up " << fpe->getId() << " to " << e->getId() << endl;
      fpe->setSecondNextEdge(e);
      if (e->getFirstVertexPtr() == fvp) {
	e->setFirstPreviousEdge(fpe);
	e->setFirstNextEdge(NULL);

      } else {
	e->setSecondPreviousEdge(fpe);
	e->setSecondNextEdge(NULL);
      }
    }
  }

  void assignPointers(VertexDescriptor vs, VertexDescriptor vd, EdgePtr e) {
    
    VertexPtr fvp = e->getFirstVertexPtr();
    VertexPtr svp = e->getSecondVertexPtr();
    // 1. See if first's and second's nextEdge is set or not.
    // Doesn't matter who the next really is.
    if (fvp->getNextEdge() == NULL) {
      fvp->setNextEdge(e);
    }
    if (svp->getNextEdge() == NULL) {
      svp->setNextEdge(e);
    }

    // 2. Find the end of the chain for each first/second node.
    // The chain is going to iterate over first and second pointers based on who is source.
    cout << "assign fvp pointers\n";
    EdgePtr fne = fvp->getNextEdge();
    chainEdge(fvp, fne, e);
    e->dump();

    cout << "assign svp pointers\n";
    fne = svp->getNextEdge();
    chainEdge(svp, fne, e);

  }

  EdgeDescriptor addEdge(VertexDescriptor vs, VertexDescriptor vd) {
    /* if (_edgeMemory == NULL) { */
    /*   cerr << "ERROR: Edge space not allocated\n"; */
    /*   exit(1); */
    /* } */
    // Create new edge by retrieving VertexPtr from vertices.
    //    char * placePtr = _edgeMemory + _numEdges*sizeof(Edge);
    //    cout << "Place edge at: " << reinterpret_cast<int*>(placePtr) << endl;
    EdgePtr e = new Edge(_vertices[vs], _vertices[vd]);
    
    e->setId(_numEdges);
    cout << "\nassign pointers for edge: " << e->getId() << "\n";
    assignPointers(vs, vd, e);
    e->dump();
    cout << "\ndone assign pointers\n";
    _numEdges++;
    _edges.push_back(e);

    cin.get();
    return e->getId();
  }

  EdgeDescriptor addEdge(VertexDescriptor vs, VertexDescriptor vd, PropertyListType & p) {
    EdgePtr e = new Edge(_vertices[vs], _vertices[vd]);
    e->setPropertyList(p);
    e->setId(_numEdges);    
    assignPointers(vs, vd, e);
    _numEdges++;
    _edges.push_back(e);
    return e->getId();
  }

  EdgeDescriptor addEdge(VertexDescriptor vs, VertexDescriptor vd, const string & l, PropertyListType & p) {
    EdgePtr e = new Edge(_vertices[vs], _vertices[vd]);
    e->setType(l);
    e->setPropertyList(p);
    e->setId(_numEdges);    
    assignPointers(vs, vd, e);
    _numEdges++;
    _edges.push_back(e);
    return e->getId();
  }

  void dump() {
    for (int i = 0; i < _vertices.size(); i++) {
      _vertices[i]->dump();
    }

    for (int i =0; i <_edges.size() ; i++) {
      cout << "--------------------\n";
      _edges[i]->dump();
      cout << endl;
    }
  }

 GraphType(): _numVertices(0), _numEdges(0) {//, _nodeMemory(NULL), _edgeMemory(NULL) {
  }

  ~GraphType() {
    // Must manually delete the objects.  
    // However, only one place is necessary since everywhere else, I am storing pointers.
    // Thus, _vertices and _edges contain all newly created objects.
    // for (int i =0; i < _vertices.size(); i++) {
    //   //      delete _vertices[i];
    // }
    // for (int i =0; i < _edges.size(); i++) {
    //   //      delete _edges[i];
    // }

    // Delete the memory spaces.
    //    delete _nodeMemory;
    //cout << "Delete edge  memory\n";
    //    delete _edgeMemory;
  }

  void allocVertexMemory(unsigned int sz) {

    // Allocation sz number of Vertex objects.
    /* _nodeMemory = new char[sizeof(Vertex)*sz]; */
    /* cout << "Vertex Memory\n + Starting address: " << reinterpret_cast<int*>(_nodeMemory)  */
    /* 	 << ", ending address: " << reinterpret_cast<int*>(_nodeMemory + sizeof(Vertex)*sz) << "\n"; */
  }

  void allocEdgeMemory(unsigned int sz) {
    // Allocation sz number of Vertex objects.
    //cout << "Edge space: " << sizeof(Edge)*sz << "\n";
    //    _edgeMemory = new char[sizeof(Edge)*sz];
    //    cout << "Edge Memory\n + Starting address: " << reinterpret_cast<int*>(_edgeMemory) 
    //	 << ", ending address: " << reinterpret_cast<int*>(_edgeMemory + sizeof(Edge)*sz) << "\n" << endl;
  }

protected:
  vector<VertexPtr> _vertices;
  vector<EdgePtr> _edges;
  unsigned int _numVertices;
  unsigned int _numEdges;
  //  char* _nodeMemory;
  //  char* _edgeMemory;

};

#endif /* _BASE_GRAPH_H */

