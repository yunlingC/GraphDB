//===-- readers/FileTypes.h - class to read LDBC file types  =---*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the class to read output files types from ldbc_snb_datagen.
///
//===----------------------------------------------------------------------===//

#ifndef _FILETYPES_H_
#define _FILETYPES_H_

class RelLabel {
public:
  RelLabel ( ) : First(""), Second(""), Edge("") {}
public:
  std::string First;
  std::string Second;
  std::string Edge;
};

class FileTypeReader {
public:
  typedef std::vector<std::string> LabelListType;
  typedef std::vector<std::string> & LabelListTypeReference;
  typedef RelLabel &   RelLabelReference;
public:
  auto readFileType(std::string FileName )
    -> void { 
    if (FileName == COMMENT) {
      _Label.First = "COMMENT";
      _FileTypeNum = 1; 
    } else 
    if (FileName == POST) {
      _Label.First = "POST";
      _FileTypeNum = 1; 
    } else 
    if (FileName == PERSON) {
      _Label.First = "PERSON";
      _FileTypeNum = 1; 
    } else 
    if (FileName == FORUM) {
      _Label.First = "FORUM";
      _FileTypeNum = 1; 
    } else 
    if (FileName == TAG) {
      _Label.First = "TAG";
      _FileTypeNum = 1; 
    } else 
    if (FileName == TAGCLASS) {
      _Label.First = "TAGCLASS";
      _FileTypeNum = 1; 
    } else 
    if (FileName == ORGANISATION) {
      _Label.First = "ORGANISATION";
      _FileTypeNum = 1; 
    } else 
    if (FileName == PLACE) {
      _Label.First = "PLACE";
      _FileTypeNum = 1; 
    } else 
    if (FileName == COMMENT_REPLY_OF_COMMENT) {
      _FileTypeNum = 2;
      _Label.Edge = "REPLY_OF_COMMENT";
      _Label.First = "COMMENT";
      _Label.Second = "COMMENT";
    } else  
    if (FileName == COMMENT_REPLY_OF_POST) {
      _FileTypeNum = 2;
      _Label.First = "COMMENT";
      _Label.Second = "POST";
      _Label.Edge = "REPLY_OF_POST";
    } else  
    if (FileName == COMMENT_IS_LOCATED_IN_PLACE) {
      _FileTypeNum = 2;
       _Label.Edge = "COMMENT_IS_LOCATED_IN";
      _Label.First = "COMMENT";
      _Label.Second = "PLACE";
    } else  
    if (FileName == PLACE_IS_PART_OF_PLACE) {
      _FileTypeNum = 2;
      _Label.Edge = "IS_PART_OF";
      _Label.First = "PLACE";
      _Label.Second = "PLACE";
    } else  
    if (FileName == PERSON_KNOWS_PERSON) {
      _FileTypeNum = 2;
      _Label.Edge = "KNOWS";
      _Label.First = "PERSON";
      _Label.Second = "PERSON";
    } else  
    if (FileName == PERSON_STUDIES_AT_ORGANISATION) {
      _FileTypeNum = 2;
      _Label.Edge = "STUDY_AT";
      _Label.First = "PERSON";
      _Label.Second = "ORGANISATION";
    } else  
    if (FileName == COMMENT_HAS_CREATOR_PERSON) {
      _FileTypeNum = 2;
      _Label.Edge = "COMMENT_HAS_CREATOR";
      _Label.First = "COMMENT";
      _Label.Second = "PERSON";
    } else  
    if (FileName == POST_HAS_CREATOR_PERSON) {
      _FileTypeNum = 2;
      _Label.Edge = "POST_HAS_CREATOR";
      _Label.First = "POST";
      _Label.Second = "PERSON";
    } else  
    if (FileName == FORUM_HAS_MODERATOR_PERSON) {
      _FileTypeNum = 2;
      _Label.Edge = "HAS_MODERATOR";
      _Label.First = "FORUM";
      _Label.Second = "PERSON";
    } else  
    if (FileName == PERSON_WORKS_AT_ORGANISATION) {
      _FileTypeNum = 2;
      _Label.Edge = "WORKS_AT";
      _Label.First = "PERSON";
      _Label.Second = "ORGANISATION";
    } else  
    if (FileName == PERSON_IS_LOCATED_IN_PLACE) {
      _FileTypeNum = 2;
      _Label.Edge = "PERSON_IS_LOCATED_IN";
      _Label.First = "PERSON";
      _Label.Second = "PLACE";
    } else  
    if (FileName == PERSON_HAS_INTEREST_TAG) {
      _FileTypeNum = 2;
      _Label.Edge = "HAS_INTEREST";
      _Label.First = "PERSON";
      _Label.Second = "TAG";
    } else  
    if (FileName == POST_HAS_TAG_TAG) {
      _FileTypeNum = 2;
      _Label.Edge = "POST_HAS_TAG";
      _Label.First = "POST";
      _Label.Second = "TAG";
    } else  
    if (FileName == PERSON_LIKES_POST) {
      _FileTypeNum = 2;
      _Label.Edge = "LIKES_POST";
      _Label.First = "PERSON";
      _Label.Second = "POST";
    } else  
    if (FileName == POST_IS_LOCATED_IN_PLACE) {
      _FileTypeNum = 2;
      _Label.Edge = "POST_IS_LOCATED_IN";
       _Label.First = "POST";
      _Label.Second = "PLACE";
    } else  
    if (FileName == FORUM_HAS_MEMBER_PERSON) {
      _FileTypeNum = 2;
      _Label.Edge = "HAS_MEMBER";
      _Label.First = "FORUM";
      _Label.Second = "PERSON";
    } else  
    if (FileName == FORUM_HAS_TAG_TAG) {
      _FileTypeNum = 2;
      _Label.Edge = "FORUM_HAS_TAG";
      _Label.First = "FORUM";
      _Label.Second = "TAG";
    } else  
    if (FileName == TAG_HAS_TYPE_TAGCLASS) {
      _FileTypeNum = 2;
      _Label.Edge = "HAS_TYPE";
      _Label.First = "TAG";
      _Label.Second = "TAGCLASS";
    } else  
    if (FileName == TAGCLASS_IS_SUBCLASS_OF_TAGCLASS) {
      _FileTypeNum = 2;
      _Label.Edge = "IS_SUBCLASS_OF";
      _Label.First = "TAGCLASS";
      _Label.Second = "TAGCLASS";
    } else  
    if (FileName == ORGANISATION_IS_LOCATED_IN_PLACE) {
      _FileTypeNum = 2;
      _Label.Edge = "ORGANISATION_IS_LOCATED_IN";
      _Label.First = "ORGANISATION";
      _Label.Second = "PLACE";
    } else  
    if (FileName == PERSON_LIKES_COMMENT) {
      _FileTypeNum = 2;
      _Label.Edge = "LIKES_COMMENT";
      _Label.First = "PERSON";
      _Label.Second = "COMMENT";
    } else  
    if (FileName == COMMENT_HAS_TAG_TAG) {
      _FileTypeNum = 2;
      _Label.Edge = "COMMENT_HAS_TAG";
      _Label.First = "COMMENT";
      _Label.Second = "TAG";
    } else
    if (FileName == FORUMS_CONTAINER_OF_POST) {
      _FileTypeNum = 2;
      _Label.Edge = "CONTAINER_OF";
      _Label.First = "FORUM";
      _Label.Second = "POST";
    } else
    if (FileName == PERSON_SPEAKS_LANGUAGE) {
      _FileTypeNum = 3;
      _Label.First = "PERSON";
    } else 
    if (FileName == PERSON_EMAIL_ADDRESS) {
      _FileTypeNum = 3;
      _Label.First = "";
      _Label.Second = "PERSON";
    }
  }

  /// FileTypeNum is, basically, the create order of different objects in graph
  /// Vertices first, then edges, then property 
  auto getFileTypeNum() 
    -> unsigned int {
    return _FileTypeNum;
  }

  auto getLabel() 
    -> RelLabelReference { 
    return _Label;
  }

  auto getVertexLabelList() 
    ->LabelListTypeReference {
      _VertexLabelList.push_back("COMMENT");
      _VertexLabelList.push_back("POST");
      _VertexLabelList.push_back("PERSON");
      _VertexLabelList.push_back("PLACE");
      _VertexLabelList.push_back("ORGANISATION");
      _VertexLabelList.push_back("TAG");
      _VertexLabelList.push_back("TAGCLASS");
    return _VertexLabelList;
  }
  auto getRelLabelList() 
    -> LabelListTypeReference {
      return _RelLabelList;
  }
private:
  unsigned int _FileTypeNum = 0; /// 0. default 1. vertex 2. relationship 3. property
  RelLabel _Label;
  LabelListType _VertexLabelList;
  LabelListType _RelLabelList;
private:
  /// Nodes: 8
  const std::string COMMENT = "comment_0.csv";
  const std::string POST = "post_0.csv";
  const std::string PERSON = "person_0.csv";
  const std::string FORUM = "forum_0.csv";
  const std::string TAG = "tag_0.csv";
  const std::string TAGCLASS = "tagclass_0.csv";
  const std::string ORGANISATION = "organisation_0.csv";
  const std::string PLACE = "place_0.csv";

  /// Nodes properties: 2
  const std::string PERSON_SPEAKS_LANGUAGE = "person_speaks_language_0.csv";
  const std::string PERSON_EMAIL_ADDRESS = "person_email_emailaddress_0.csv";
 
  /// Relationships 23
  const std::string COMMENT_REPLY_OF_COMMENT = "comment_replyOf_comment_0.csv";
  const std::string COMMENT_REPLY_OF_POST = "comment_replyOf_post_0.csv";
  const std::string COMMENT_IS_LOCATED_IN_PLACE = "comment_isLocatedIn_place_0.csv";
  const std::string PLACE_IS_PART_OF_PLACE = "place_isPartOf_place_0.csv";
  const std::string PERSON_KNOWS_PERSON = "person_knows_person_0.csv";
  const std::string PERSON_STUDIES_AT_ORGANISATION = "person_studyAt_organisation_0.csv";
  const std::string COMMENT_HAS_CREATOR_PERSON = "comment_hasCreator_person_0.csv";
  const std::string POST_HAS_CREATOR_PERSON = "post_hasCreator_person_0.csv";
  const std::string FORUM_HAS_MODERATOR_PERSON = "forum_hasModerator_person_0.csv";
  const std::string PERSON_IS_LOCATED_IN_PLACE = "person_isLocatedIn_place_0.csv";
  const std::string PERSON_WORKS_AT_ORGANISATION = "person_workAt_organisation_0.csv";
  const std::string PERSON_HAS_INTEREST_TAG = "person_hasInterest_tag_0.csv";
  const std::string POST_HAS_TAG_TAG = "post_hasTag_tag_0.csv";
  const std::string PERSON_LIKES_POST = "person_likes_post_0.csv";
  const std::string POST_IS_LOCATED_IN_PLACE = "post_isLocatedIn_place_0.csv";
  const std::string FORUM_HAS_MEMBER_PERSON = "forum_hasMember_person_0.csv";
  const std::string FORUMS_CONTAINER_OF_POST = "forum_containerOf_post_0.csv";
  const std::string FORUM_HAS_TAG_TAG = "forum_hasTag_tag_0.csv";
  const std::string TAG_HAS_TYPE_TAGCLASS = "tag_hasType_tagclass_0.csv";
  const std::string TAGCLASS_IS_SUBCLASS_OF_TAGCLASS = "tagclass_isSubclassOf_tagclass_0.csv";
  const std::string ORGANISATION_IS_LOCATED_IN_PLACE = "organisation_isLocatedIn_place_0.csv";
  const std::string PERSON_LIKES_COMMENT = "person_likes_comment_0.csv";
  const std::string COMMENT_HAS_TAG_TAG = "comment_hasTag_tag_0.csv";

} ;

#endif /*_FILETYPES_H_*/

