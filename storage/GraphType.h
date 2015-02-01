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
  typedef Vertex* VertexPointer;
  typedef Vertex* VertexPtr;
  typedef Edge* EdgePointer;
  typedef Edge* EdgePtr;
  typedef PropertyList<string, string> PropertyListType;
  typedef PropertyListType VertexPropertyList;
  typedef PropertyListType EdgePropertyList;

public:
  VertexPtr getVertexPointer(VertexDescriptor v) {
    if ( (v > _vertices.size() - 1) || (v < 0) ) {
      return nullptr;
    }
    return _vertices[v];
  }

  VertexDescriptor addVertex() {
    /* if (_nodeMemory == nullptr) { */
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

  void chainEdge(VertexPtr v, EdgePtr fnx, EdgePtr newEdge) {
    EdgePtr next = fnx;
    EdgePtr prev = nullptr;

    while (next != nullptr) {
      prev = next;
      if (next->getFirstVertexPtr()->getId() == v->getId()) {
	cout << "+ down firstNext\n";
	next = next->getFirstNextEdge();
      } else if (next->getSecondVertexPtr()->getId() == v->getId()) {
	cout << "+ down secondNext: \n";
	next = next->getSecondNextEdge();
      } else {
	cout << "+ \nERROR: no forward movement" << endl;
	//	cout << "+ Vertex focus: " << v->getId() << endl;
	//	cout << "+ next: End of chain edge" << endl;
	//	next->dump();
	//	cout << "+ newEdge: New edge to insert" << endl;
	//	newEdge->dump();
      }
    }

    cout << "end of chain edge: " << prev->getId() << endl;
    if (prev == newEdge)  {
      cout << "FIRST EDGE: Not hooking up " << prev->getId() << " to " << newEdge->getId() << endl;
      return;
    }
    
    // Got to the end of the chain.
    if (prev->getFirstVertexPtr() == v) {
      cout << "hooking up " << prev->getId() << " to " << newEdge->getId() << endl;
      prev->setFirstNextEdge(newEdge);
      if (newEdge->getFirstVertexPtr() == v) {
	newEdge->setFirstPreviousEdge(prev);
	newEdge->setFirstNextEdge(nullptr); 
      } else if (newEdge->getSecondVertexPtr() == v) {
	newEdge->setSecondPreviousEdge(prev);
	newEdge->setSecondNextEdge(nullptr); 
      }

    } else if (prev->getSecondVertexPtr() == v) {
      cout << "hooking up " << prev->getId() << " to " << newEdge->getId() << endl;
      prev->setSecondNextEdge(newEdge);
      if (newEdge->getFirstVertexPtr() == v) {
	newEdge->setFirstPreviousEdge(prev);
	newEdge->setFirstNextEdge(nullptr);

      } else if (newEdge->getSecondVertexPtr() == v) {
	cout << "hooking up " << prev->getId() << " to " << newEdge->getId() << endl;
	newEdge->setSecondPreviousEdge(prev);
	newEdge->setSecondNextEdge(nullptr);
      }
    }
  }

  void assignPointers(VertexDescriptor vs, VertexDescriptor vd, EdgePtr e) {
    
    VertexPtr fvp = e->getFirstVertexPtr();
    VertexPtr svp = e->getSecondVertexPtr();
    // 1. See if first's and second's nextEdge is set or not.
    // Doesn't matter who the next really is.
    if (fvp->getNextEdge() == nullptr) {
      fvp->setNextEdge(e);
    }
    if (svp->getNextEdge() == nullptr) {
      svp->setNextEdge(e);
    }

    // 2. Find the end of the chain for each first/second node.
    // The chain is going to iterate over first and second pointers based on who is source.
    cout << "\nassignPointers: fvp pointers\n";
    EdgePtr fne = fvp->getNextEdge();
    chainEdge(fvp, fne, e);
    e->dump();

    cout << "\nassignPointers:: svp pointers\n";
    fne = svp->getNextEdge();
    chainEdge(svp, fne, e);

  }

  EdgeDescriptor addEdge(VertexDescriptor vs, VertexDescriptor vd) {
    /* if (_edgeMemory == nullptr) { */
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

    //    cin.get();
    return e->getId();
  }

  EdgeDescriptor addEdge(VertexDescriptor vs, VertexDescriptor vd, const string & l) {
    /* if (_edgeMemory == nullptr) { */
    /*   cerr << "ERROR: Edge space not allocated\n"; */
    /*   exit(1); */
    /* } */
    // Create new edge by retrieving VertexPtr from vertices.
    //    char * placePtr = _edgeMemory + _numEdges*sizeof(Edge);
    //    cout << "Place edge at: " << reinterpret_cast<int*>(placePtr) << endl;
    EdgePtr e = new Edge(_vertices[vs], _vertices[vd]);
    
    e->setId(_numEdges);
e->setType(l);
 cout << "\n\nassign pointers for edge: " << e->getId() << "(" << vs << ", " << vd <<")\n";
    assignPointers(vs, vd, e);
    e->dump();
    cout << "\ndone assign pointers\n";
    _numEdges++;
    _edges.push_back(e);

    //    cin.get();
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
    cout << "\n\naddEdge:: assign pointers for edge: " << e->getId() << "\n";
    assignPointers(vs, vd, e);
    cout << "\naddEdge:: done assign pointers\n";
    e->dump();
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

 GraphType(): _numVertices(0), _numEdges(0) {//, _nodeMemory(nullptr), _edgeMemory(nullptr) {
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

