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

#include <unordered_map>
#include <vector>

class GraphType;

enum VertexLabel {COMMENT, POST, PERSON, FORUM, TAG, TAGCLASS, ORGANISATION, PLACE}

enum EdgeLabel {REPLY_OF_COMMENT, REPLY_OF_POST, COMMENT_IS_LOCATED_IN, IS_PART_OF,
                KNOWS, STUDY_AT, COMMENT_HAS_CREATOR, POST_HAS_CREATOR, 
                HAS_MODERATOR, WORKS_AT, PERSON_IS_LOCATED_IN, HAS_INTEREST, 
                POST_HAS_TAG, LIKES_POST, POST_IS_LOCATED_IN, HAS_MEMBER,
                FORUM_HAS_TAG, HAS_TYPE, IS_SUBCLASS_OF, ORGANASACTION_IS_LOCATED_IN,
                LIKES_COMMENT, COMMENT_HAS_TAG, CONTAINER_OF}

template<typename KeyType>
class Index {
public:
///  typedef std::string KeyType;
  typedef GraphType::VertexPointer VertexPointer;
  typedef GraphType::EdgePointer EdgePointer;
  typedef std::pair<KeyType, VertexPointer> VertexIndexEntryType;
  typedef std::pair<KeyType, EdgePointer> EdgeIndexEntryType;
  typedef std::unordered_map<KeyType, VertexPointer> LabeledVertexIndexMapType;
  typedef std::unordered_map<KeyType, EdgePointer> LabeledEdgeIndexMapType;
  typedef std::unordered_map<VertexLabel, LabeledVertexIndexMapType> VertexIndexMapType;
  typedef std::unordered_map<EdgeLabel, LabeledEdgeIndexMapType> VertexIndexMapType;

public:
  /// Build index on vertex and edge on key
  bool  buildVertexIndex(KeyType key) {

  }

  bool  buildEdgeIndex(KeyType key);
  bool  buildIndex(KeyType key);

  /// The following three functions are to build index on Vertex/Edge with Label=label
  template<typename LabelType>
  bool  buildVertexIndex(KeyType key, LabelType label);
  bool  buildEdgeIndex(KeyType key, LabelType label);

private:
  
};

#endif /*_INDEX_H_*/
