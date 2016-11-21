//===-- storage/Index.h - Index class type ----------------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the main class for Index structure on graphs.
///
//===----------------------------------------------------------------------===//

#ifndef _INDEX_H_
#define _INDEX_H_

#include "FileTypes.h"
#include "PropertyList.h"
#include "GraphType.h"

#include <unordered_map>
#include <vector>
#include <assert.h>
#ifdef _DEBUG_
#include <iostream>
#endif

class GraphType;

//enum VertexLabel {COMMENT, POST, PERSON, FORUM, TAG, TAGCLASS, ORGANISATION, PLACE}
//
//enum EdgeLabel {REPLY_OF_COMMENT, REPLY_OF_POST, COMMENT_IS_LOCATED_IN, IS_PART_OF,
//                KNOWS, STUDY_AT, COMMENT_HAS_CREATOR, POST_HAS_CREATOR, 
//                HAS_MODERATOR, WORKS_AT, PERSON_IS_LOCATED_IN, HAS_INTEREST, 
//                POST_HAS_TAG, LIKES_POST, POST_IS_LOCATED_IN, HAS_MEMBER,
//                FORUM_HAS_TAG, HAS_TYPE, IS_SUBCLASS_OF, ORGANASACTION_IS_LOCATED_IN,
//                LIKES_COMMENT, COMMENT_HAS_TAG, CONTAINER_OF}

///template<typename KeyType, ValueType>
template<typename ObjectType>
class IndexMap {
public:
  typedef std::string KeyType;
  typedef std::string ValueType;
  typedef std::string LabelType;
  typedef std::pair<ValueType, ObjectType> IndexEntryType;
  typedef std::unordered_map<ValueType, ObjectType> LabeledIndexMapType;
  typedef std::pair<LabelType, LabeledIndexMapType> MapEntryType;
  typedef std::unordered_map<LabelType, LabeledIndexMapType> IndexMapType;
  typedef std::pair<ObjectType, bool> ReturnIndexType;
public:
  IndexMap() :  _NumIndices (0){}
  bool  buildIndex(KeyType key, ObjectType Object) {
    assert(Object != nullptr);
    auto retValue = Object->getPropertyValue(key);
    auto Label = Object->getType().std_str();
///    std::cout << "++ Vertex " << Label << " Value " << retValue.first << "\n";
      if (retValue.second) {
        if (_IndexMap.find(Label) != _IndexMap.end()) {
          _IndexMap[Label].insert(IndexEntryType(retValue.first.std_str(), Object));
        } else {
         LabeledIndexMapType  NewMap;
         NewMap.insert(IndexEntryType(retValue.first.std_str(), Object));
         _IndexMap.insert(MapEntryType(Label, NewMap));
        }
        _NumIndices++;
        return true;
      }
      return false;
  }

  IndexMapType & getIndexMap()  {
    return _IndexMap;
  }

  unsigned int getNumIndices()  {
    return _NumIndices;
  }

  /// Property Value is the key for indexed graph object (vertex, edge)
  ReturnIndexType getIndex(LabelType Label, ValueType Key) {
    std::pair<ObjectType, bool> retValue(nullptr, false);
    auto MapEntry = _IndexMap.find(Label);
    if  (MapEntry != _IndexMap.end()) {
      if ((*MapEntry).second.find(Key) != (*MapEntry).second.end()) {
        retValue.first = (*MapEntry).second[Key];
        retValue.second = true;
      }
    }
    return retValue;
  }

private:
  IndexMapType _IndexMap;
  unsigned int _NumIndices;
};

class Index {
public:
  typedef std::string KeyType;
  typedef std::string ValueType;
  typedef std::string LabelType;
  typedef GraphType::VertexPointer VertexPointer;
  typedef GraphType::EdgePointer EdgePointer;
  typedef std::pair<ValueType, VertexPointer> VertexIndexEntryType;
  typedef std::pair<ValueType, EdgePointer> EdgeIndexEntryType;
  typedef std::unordered_map<ValueType, VertexPointer> LabeledVertexIndexMapType;
  typedef std::unordered_map<ValueType, EdgePointer> LabeledEdgeIndexMapType;
  typedef std::pair<LabelType, LabeledVertexIndexMapType> VertexMapEntryType;
  typedef std::pair<LabelType, LabeledEdgeIndexMapType> EdgeMapEntryType;
  typedef std::unordered_map<LabelType, LabeledVertexIndexMapType> VertexIndexMapType; 
  typedef std::unordered_map<LabelType, LabeledEdgeIndexMapType> EdgeIndexMapType;

public:
  Index(GraphType & graph) : _Graph(graph){}

  /// Build index on vertex and edge on key
  unsigned int  buildVertexIndex(const KeyType & key) {
    /// Return vertex list
    auto Vertices = _Graph.getAllVertices();
    
    for  (unsigned int i = 0; i < Vertices.size(); i++) {
      auto VertexPtr = Vertices[i];
//      auto retValue = VertexPtr->getPropertyValue(key);
//      auto Label = VertexPtr->getType();
//        if (retValue.second) {
//           auto LabeledMap = VertexIndexMap.find(Label);
//           if (LabeledMap != VertexIndexMap.end()) {
//             LabeledMap.insert(VertexIndexEntryType(retValue.first, VertexPtr));
//           }
//           LabeledVertexIndexMapType LabeledMap;
//           LabeledMap.insert(VertexIndexEntryType(retValue.first, VertexPtr));
//           VertexIndexMap.insert(VertexMapEntryType(Label));
//        }
//    }
//    
//    /// This vertex does NOT have entry with <key>
//    return false;
    
      VertexIndexMap.buildIndex(key, VertexPtr);
    }
    /// If all vertices are indexed, return true; 
    return VertexIndexMap.getNumIndices();
  }


  unsigned int  buildEdgeIndex(KeyType key) {
    auto Edges = _Graph.getAllEdges();
    for (unsigned int i = 0; i < Edges.size(); i++) {
      auto EdgePtr = Edges[i];
      EdgeIndexMap.buildIndex(key, EdgePtr);
    }
    return EdgeIndexMap.getNumIndices(); 
  }

//  template<typename ObjectType>
//  bool  buildIndex(KeyType key, ObjectType Object) {
//    assert(Object != nullptr);
//    auto retValue = Object->getPropertyValue(key);
//      if (retValue.second) {
//        auto Label = Object->getType();
//        auto LabeledMap = IndexMap.find(Label);
//        if (LabeledMap != IndexMap.end()) {
//          (*LabeledMap).insert(IndexEntryType(retValue.first, Object));
//        } else {
//         LabeledIndexMapType NewMap;
//         NewMap.insert(IndexEntryType(retValue.first, Object));
//         VertexIndexMap.IndexMap.insert(MapEntryType(Label, NewMap));
//        }
//        return true;
//      }
//      return false;
//  }
//
  bool buildGraphIndex(const KeyType & key) {
    return buildVertexIndex(key) + buildEdgeIndex(key);
  }

  /// The following three functions are to build index on Vertex/Edge with Label=label
///  template<typename LabelType>
  IndexMap<VertexPointer>::ReturnIndexType 
  getVertexIndex(LabelType label, ValueType key)  {
    return VertexIndexMap.getIndex(label, key); 
  }

  IndexMap<EdgePointer>::ReturnIndexType getEdgeIndex(LabelType label, ValueType key)  {
    return EdgeIndexMap.getIndex(label, key); 
  }

private:
  GraphType & _Graph;
  IndexMap<VertexPointer> VertexIndexMap;
  IndexMap<EdgePointer>  EdgeIndexMap;
};

#endif /*_INDEX_H_*/
