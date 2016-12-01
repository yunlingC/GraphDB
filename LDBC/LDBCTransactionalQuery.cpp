//===-- LDBC/LDBCTransactionalQuery.cpp - Query class ----------*- C++ -*-===//
//
//                     CAESR Graph Database
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the query class for LDBC derived from LDBCConcurrnetQuery.
///
//===----------------------------------------------------------------------===//

#ifndef _LDBCTRANSACTIONALQUERY_CPP_
#define _LDBCTRANSACTIONALQUERY_CPP_

#include "LdbcConcurrentQuery.h"
#include "TransactionalBFS.h"

#include <vector>
#include <string>
#include <time.h>
#include <unordered_map>

#define _TRANX_STATS_ true

#ifdef _PRINTLOG_
#include <iostream>
#endif

class LdbcQuery1 : public LdbcQuery {
	using LdbcQuery::LdbcQuery;
public:
	void runQuery(GraphType & Graph
                , VertexDescriptor StartVertex
	              , TransactionPointerType  Tranx
                , LockManagerType & LockManager
                ) {

		getStartTime();

    while (Tranx->checkStatus() != T_COMMIT) {
      Tranx->expand();

	  	FilterType TmpFilter;
	  	FilterType NameFilter;
	  	filtProperty(ParamPair.first, ParamPair.second, NameFilter);
	  	traverseThroughTypeAndDirection("KNOWS", "", TmpFilter);
	  	LimitedDepthVisitor  DepthVisitor;
	  	for (auto i = 0; i < 3; i++) {
	  		DepthVisitor.setFilter(TmpFilter);
	  	}
	  	DepthVisitor.setNameFilter(NameFilter);
	  	DepthVisitor.setDepth(3);
      TransactionalBFS txBFS;
	  	txBFS.breadthFirstSearch(Graph, StartVertex, DepthVisitor, Tranx, LockManager);
      Tranx->commit();

#ifdef _PRINTLOG_
      if (Tranx->checkStatus() == T_COMMIT) {
		    auto target = DepthVisitor.getVertexList();
	      LdbcFile << StartVertex << "\tis connected with\t" << target.size() 
                << "\tpeople with\t" << ParamPair.first <<": " << ParamPair.second
                << "\n";
      }
#endif
    }

		LockManager.releaseAll(Tranx->getId());
		getExecTime();
		LdbcFile.close();
	}
};

class LdbcQuery2 : public LdbcQuery {
	using LdbcQuery::LdbcQuery;
public:

	void runQuery(GraphType & Graph, VertexDescriptor StartVertex,
	              TransactionPointerType Tranx, LockManagerType & LockManager) {
		getStartTime();

    while (Tranx->checkStatus() != T_COMMIT) {
      Tranx->expand();

  		FilterType TmpFilter[3];
  		traverseThroughMultiRelType("KNOWS", TmpFilter[0]);
  		traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR", TmpFilter[1]);
  		TmpFilter[2].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second);
  
  		MultiRelTypeVisitor TypeVisitor;
  		TypeVisitor.setFilter(TmpFilter[0]);
  		TypeVisitor.setFilter(TmpFilter[1]);
  		TypeVisitor.setRangeFilter(TmpFilter[2]);
  		TypeVisitor.setDepth(2);
  		TypeVisitor.setDepthToCheckRange(2);
  		//check date
  		TypeVisitor.setPropToCheck(2);
      TransactionalBFS txBFS;
  		txBFS.breadthFirstSearch(Graph, StartVertex, TypeVisitor, Tranx, LockManager);
      Tranx->commit();
  
  #ifdef _PRINTLOG_
      if(Tranx->checkStatus() == T_COMMIT) {
    		auto target = TypeVisitor.getVertexList();
        auto targets = TypeVisitor.getTargetsMap();
        LdbcFile << StartVertex << "\thas friends made\t" << target.size() 
                  << "\tcomments and posts \n";
        for(auto it = targets.begin(); it != targets.end(); ++it) {
          LdbcFile <<"person " << (*it).first->getId() 
                  << "\t" << (*it).second->getPropertyValue("id").first 
                  << "\t" <<"comments/posts" 
                  << "\t" << (*it).first->getPropertyValue("id").first 
                  << "\t" << (*it).first->getPropertyValue("creationDate").first ;
          LdbcFile << "\n";
        }
      }
  #endif
    }

		LockManager.releaseAll(Tranx->getId());
		getExecTime();
		LdbcFile.close();
	}
};

class LdbcQuery3 : public LdbcQuery {
	using LdbcQuery::LdbcQuery;
public:
	typedef std::map<VertexPointer, std::vector<VertexPointer>> PersonListMapType;
	typedef std::pair<VertexPointer, std::vector<VertexPointer>> PersonListPair;
public:

	void runQuery(GraphType & Graph, VertexDescriptor StartVertex,
	              TransactionPointerType Tranx, LockManagerType & LockManager) {
		getStartTime();

    while (Tranx->checkStatus() != T_COMMIT) {
      Tranx->expand();

  		FilterType TmpFilter;
  		traverseThroughTypeAndDirection("KNOWS", "", TmpFilter);
  
  		SingleRelTypeVisitor SingleVisitor;
  		SingleVisitor.setFilter(TmpFilter);
  		///Do NOT delete
  		SingleVisitor.setFilter(TmpFilter);
  		SingleVisitor.setDepth(2);
      TransactionalBFS txBFS;
  		txBFS.breadthFirstSearch(Graph, StartVertex, SingleVisitor, Tranx, LockManager);
      if (Tranx->checkStatus() == T_ABORT)
        continue;

  		auto target = SingleVisitor.getVertexList();

  #ifdef _PRINTLOG_
  		LdbcFile << StartVertex << " is connected with " << target.size() << " friends and friends of friends" << "\n";
  #endif
  
  		FilterType FilterSet[4];
  		traverseThroughMultiRelType("PERSON_IS_LOCATED_IN", FilterSet[0]);
  		traverseThroughMultiRelType("IS_PART_OF", FilterSet[1]);
  		auto country1 = PropRange.second.first;
  		auto country2 = PropRange.second.second;
  		FilterSet[2].setProperty("name", country1);
  		FilterSet[3].setProperty("name", country2);
  
  		auto it = target.begin();
      bool needRestart = false;
  		while (it != target.end()) {
  			VertexPropertyRangeVisitor RangeVisitor;
  			RangeVisitor.setFilter(FilterSet[0]);
  			RangeVisitor.setFilter(FilterSet[1]);
  			RangeVisitor.setVertexFilters(FilterSet[2]);
  			RangeVisitor.setVertexFilters(FilterSet[3]);
  			RangeVisitor.setDepth(2);
  			RangeVisitor.setDepthToCheckVertexProp(2);
  			auto StartVertex = (*it)->getId();
        TransactionalBFS txBFS;
  			txBFS.breadthFirstSearch(Graph, StartVertex, RangeVisitor, Tranx, LockManager);

        if (Tranx->checkStatus() == T_ABORT) {
          needRestart = true;
          break;
        }

  			if (RangeVisitor.getIncludeState() == false) {
  				target.erase(it);
  			}
  			else {
  				it++;
  			}
  		}///while

      if(needRestart)  {
        continue;
      }
  
  		FilterType Filters[5];
  		PersonListMapType PersonListMap;
  		traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR",Filters[0]);
  		Filters[1].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second);
  		traverseThroughMultiRelType("POST_IS_LOCATED_IN+COMMENT_IS_LOCATED_IN",Filters[2]);
  		Filters[3].setProperty("name", country1);
  		Filters[4].setProperty("name", country2);
  
  		for (auto it = target.begin(), it_end = target.end();
  		     it != it_end; it++) {
  			VertexPropertyRangeVisitor RangeVisitor;
  			RangeVisitor.setFilter(Filters[0]);
  			RangeVisitor.setFilter(Filters[2]);
  			RangeVisitor.setRangeFilter(Filters[1]);
  			RangeVisitor.setVertexFilters(Filters[4]);
  			RangeVisitor.setVertexFilters(Filters[3]);
  			RangeVisitor.setDepth(2);
  			RangeVisitor.setDepthToCheckRange(2);
  			RangeVisitor.setPropToCheck(2);
  			RangeVisitor.setDepthToCheckVertexProp(2);
  			auto StartVertex = (*it)->getId();
        TransactionalBFS txBFS;
  			txBFS.breadthFirstSearch(Graph, StartVertex, RangeVisitor, Tranx, LockManager);

        if (Tranx->checkStatus() == T_ABORT) {
          needRestart = true;
          break;
        }

  			auto targetList = RangeVisitor.getVertexList();
  			PersonListMap.insert(PersonListPair((*it), targetList));
  		}///for

      if(needRestart)  {
        continue;
      }

      Tranx->commit();
  
  #ifdef _PRINTLOG_
      if (Tranx->checkStatus() == T_COMMIT) {
    		for(auto it = PersonListMap.begin(), it_end = PersonListMap.end();
            it != it_end; ++it) {
          LdbcFile << "friend\t" << (*it).first->getPropertyValue("firstName").first  
                   << "\thas\t" <<(*it).second.size() 
                   << "\tcommmets/posts made in\t" << country1 
                   << "\tor\t" << country2 << "\n";
        }
      }
  #endif
    }

		LockManager.releaseAll(Tranx->getId());
		getExecTime();
		LdbcFile.close();
	}
};

class LdbcQuery4 : public LdbcQuery {
	using LdbcQuery::LdbcQuery;
public:

	void runQuery(GraphType & Graph, VertexDescriptor StartVertex,
	              TransactionPointerType Tranx, LockManagerType & LockManager) {
		getStartTime();

    while (Tranx->checkStatus() != T_COMMIT) {
      Tranx->expand();

  		FilterType TmpFilter[4];
  		traverseThroughMultiRelType("KNOWS", TmpFilter[0]);
  		traverseThroughMultiRelType("POST_HAS_CREATOR", TmpFilter[1]);
  		traverseThroughMultiRelType("POST_HAS_TAG", TmpFilter[2]);
  		TmpFilter[3].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second);
  
  		MultiResultVisitor ResultsVisitor;
  		ResultsVisitor.setFilter(TmpFilter[0]);
  		ResultsVisitor.setFilter(TmpFilter[1]);
  		ResultsVisitor.setFilter(TmpFilter[2]);
  		ResultsVisitor.setRangeFilter(TmpFilter[3]);
  		ResultsVisitor.setDepth(3);
  		ResultsVisitor.setDepthToCheckRange(2);
  		ResultsVisitor.setPropToCheck(1); //check time
      TransactionalBFS txBFS;
  		txBFS.breadthFirstSearch(Graph, StartVertex, ResultsVisitor, Tranx, LockManager);
      Tranx->commit();

#ifdef _PRINTLOG_
      if (Tranx->checkStatus() == T_COMMIT) {
    		auto targets = ResultsVisitor.getReturnResultMap();
        LdbcFile << StartVertex << "\thas friends made posts of" 
                 << "\t" << targets.size() << "\ttags\n";
        for(auto it = targets.begin(), it_end = targets.end();
            it != it_end; ++it) {
          LdbcFile <<"tags\t" << (*it).first->getPropertyValue("id").first 
                   << "\t" <<"num of posts " << (*it).second <<  "\n";
        }
      }
#endif
    }

		LockManager.releaseAll(Tranx->getId());
		getExecTime();
		LdbcFile.close();
	}
};

class LdbcQuery5 : public LdbcQuery {
	using LdbcQuery::LdbcQuery;
public:
	typedef std::map<VertexPointer, std::vector<VertexPointer>> PersonListMapType;
	typedef std::pair<VertexPointer, std::vector<VertexPointer>> PersonListPair;
public:

	void runQuery(GraphType & Graph, VertexDescriptor StartVertex,
	              TransactionPointerType Tranx, LockManagerType & LockManager) {
		getStartTime();

    while (Tranx->checkStatus() != T_COMMIT) {
      Tranx->expand();

  		FilterType TmpFilter[2];
  		traverseThroughTypeAndDirection("KNOWS", "", TmpFilter[0]);
  		traverseThroughTypeAndDirection("KNOWS", "", TmpFilter[1]);
  
  		SingleRelTypeVisitor SingleVisitor;
  		SingleVisitor.setFilter(TmpFilter[0]);
  		SingleVisitor.setFilter(TmpFilter[1]);
  		SingleVisitor.setDepth(2);
      TransactionalBFS txBFS;
  		txBFS.breadthFirstSearch(Graph, StartVertex, SingleVisitor, Tranx, LockManager);

      if (Tranx->checkStatus() == T_ABORT)
        continue;
  
  		auto target = SingleVisitor.getVertexList();
#ifdef _PRINTLOG_
		  LdbcFile << StartVertex << " is connected with " << target.size() << " friends and friends of friends" << "\n";
#endif

  		FilterType Filters[4];
  		traverseThroughMultiRelType("HAS_MEMBER", Filters[0]);
  		Filters[1].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second);
  		traverseThroughMultiRelType("CONTAINER_OF",Filters[2]);
  		traverseThroughMultiRelType("POST_HAS_CREATOR",Filters[3]);
  		std::vector<FilterType> VertexFilter;
  		for (auto iter = target.begin(), iter_end = target.end();
  		     iter != iter_end; iter++) {
  			FilterType newFilter;
  			newFilter.setProperty("id", (*iter)->getPropertyValue("id").first.std_str());
  			VertexFilter.push_back(newFilter);
  		}
  
      bool needRestart = false;
  		std::map<VertexPointer, unsigned int> targetMap;
  		for (auto it = target.begin(), it_end = target.end();
  		     it != it_end; it++) {
  			MultiPropertyVisitor PropertiesVisitor;
  			PropertiesVisitor.setFilter(Filters[0]);
  			PropertiesVisitor.setFilter(Filters[2]);
  			PropertiesVisitor.setFilter(Filters[3]);
  			PropertiesVisitor.setRangeFilter(Filters[1]);
  			PropertiesVisitor.setVertexFilterList(VertexFilter);
  			PropertiesVisitor.setDepth(3);
  			PropertiesVisitor.setDepthToCheckVertexProp(3);
  			PropertiesVisitor.setDepthToCheckRange(1);
  			PropertiesVisitor.setPropToCheck(2); //check date
  			auto StartVertex = (*it)->getId();
        TransactionalBFS txBFS;
  			txBFS.breadthFirstSearch(Graph, StartVertex, PropertiesVisitor, Tranx, LockManager);
        if (Tranx->checkStatus() == T_ABORT) {
          needRestart = true;
          break;
        }
  			targetMap.insert(PropertiesVisitor.getResultMap().begin(),
  			                 PropertiesVisitor.getResultMap().end());
  		}///for

      if (needRestart)  {
        continue;
      }

      Tranx->commit();
#ifdef _PRINTLOG_
  		for (auto it = targetMap.begin(), it_end = targetMap.end();
           it != it_end; ++it) {
        if (!(*it).second)
          LdbcFile <<"forum " << (*it).first->getPropertyValue("id").first  << " has " <<(*it).second << " posts made by friends"<< "\n";
      }
#endif
    }

		LockManager.releaseAll(Tranx->getId());
		getExecTime();
		LdbcFile.close();
	}
};

class LdbcQuery6 : public LdbcQuery {
	using LdbcQuery::LdbcQuery;
public:
	typedef std::map<VertexPointer, std::vector<VertexPointer>> PersonListMapType;
	typedef std::pair<VertexPointer, std::vector<VertexPointer>> PersonListPair;
public:

	void runQuery(GraphType & Graph, VertexDescriptor StartVertex,
	              TransactionPointerType Tranx, LockManagerType & LockManager) {
		getStartTime();

    while (Tranx->checkStatus() != T_COMMIT) {
      Tranx->expand();

  		FilterType TmpFilter[2];
  		traverseThroughTypeAndDirection("KNOWS", "", TmpFilter[0]);
  		traverseThroughTypeAndDirection("KNOWS", "", TmpFilter[1]);
  
  		SingleRelTypeVisitor SingleVisitor;
  		SingleVisitor.setFilter(TmpFilter[0]);
  		SingleVisitor.setFilter(TmpFilter[1]);
  		SingleVisitor.setDepth(2);
      TransactionalBFS txBFS;
  		txBFS.breadthFirstSearch(Graph, StartVertex, SingleVisitor, Tranx, LockManager);
      if (Tranx->checkStatus() == T_ABORT)
        continue;

		auto target = SingleVisitor.getVertexList();

#ifdef _PRINTLOG_
		  LdbcFile << StartVertex << "\tis connected with\t" 
               << target.size() << "\tfriends and friends of friends" 
               << "\n";
#endif

  		FilterType Filters[3];
  		traverseThroughMultiRelType("POST_HAS_CREATOR", Filters[0]);
  		traverseThroughMultiRelType("POST_HAS_TAG", Filters[1]);
  		Filters[2].setProperty(ParamPair.first, ParamPair.second);
  
      bool needRestart = false;
  		std::map<std::string, unsigned int> TagMap;
  		for(auto it = target.begin(); it != target.end(); it++) {
  			SinglePropertyVisitor  SPVisitor;
  			SPVisitor.setFilter(Filters[0]);
  			SPVisitor.setFilter(Filters[1]);
  			SPVisitor.setVertexFilter(Filters[2]);
  			SPVisitor.setDepthToCheckVertexProp(2);
  			SPVisitor.setDepth(2);
  			unsigned int StartVertex = (*it)->getId();
        TransactionalBFS txBFS;
  			txBFS.breadthFirstSearch(Graph, StartVertex, SPVisitor, Tranx, LockManager);
  
        if (Tranx->checkStatus() == T_ABORT) {
          needRestart = true;
          break;
        }

  			auto personMap = SPVisitor.getPersonMap();
  			for (auto it = personMap.begin(), it_end = personMap.end();
  			     it != it_end; it++) {
  				if ((*it).second ) {
  					for (auto iter = SPVisitor.getResultTargetsMap()[(*it).first].begin(); iter != SPVisitor.getResultTargetsMap()[(*it).first].end(); iter++) {
  						// (*iter) --> Tag id (string)
  						if (TagMap.find(*iter) == TagMap.end()) {
  							TagMap.insert(std::pair<std::string, unsigned int>((*iter), 1));
  						} else {
  							TagMap[*iter]++;
  						}
  					}///for
  				}///if
  			}///for
  		}///for

      if(needRestart)  {
        continue;
      }

      Tranx->commit();
#ifdef _PRINTLOG_
  		for (auto it = TagMap.begin(), it_end = TagMap.end();
          it != it_end; it++) {
        LdbcFile << "Tag " << (*it).first << " has " << (*it).second << " posts\n";
      }
#endif
    }

		LockManager.releaseAll(Tranx->getId());
		getExecTime();
		LdbcFile.close();
	}
};

class LdbcQuery7 : public LdbcQuery {
	using LdbcQuery::LdbcQuery;
public:
	typedef std::map<VertexPointer, std::vector<VertexPointer>> PersonListMapType;
	typedef std::pair<VertexPointer, std::vector<VertexPointer>> PersonListPair;
public:

	void runQuery(GraphType & Graph, VertexDescriptor StartVertex,
	              TransactionPointerType Tranx, LockManagerType & LockManager) {
		getStartTime();

    while (Tranx->checkStatus() != T_COMMIT) {
      Tranx->expand();

  		FilterType TmpFilter[2];
  		traverseThroughTypeAndDirection("KNOWS", "", TmpFilter[0]);
  
  		SingleRelTypeVisitor RelVisitor;
  		RelVisitor.setFilter(TmpFilter[0]);
  		RelVisitor.setDepth(1);
      TransactionalBFS txBFS;
  		txBFS.breadthFirstSearch(Graph, StartVertex, RelVisitor, Tranx, LockManager);
      if (Tranx->checkStatus() == T_ABORT)
        continue;

  		auto target = RelVisitor.getVertexList();
  		std::vector<FilterType> VertexFilter;
  		for (auto iter = target.begin(); iter != target.end(); iter++) {
  			FilterType newFilter;
  			newFilter.setProperty("id", (*iter)->getPropertyValue("id").first.std_str());
  			VertexFilter.push_back(newFilter);
  		}

#ifdef _PRINTLOG_
		  LdbcFile << StartVertex << "\tis connected with\t" 
                << target.size() << "\tfriends" << "\n";
#endif

  		FilterType Filters[2];
  		traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR", Filters[0]);
  		traverseThroughMultiRelType("LIKES_COMMENT+LIKES_POST", Filters[1]);
  		VertexMatchVisitor MatchVisitor;
  		MatchVisitor.setFilter(Filters[0]);
  		MatchVisitor.setFilter(Filters[1]);
  		MatchVisitor.setDepth(2);
  		MatchVisitor.setVertexFilterList(VertexFilter);
  		MatchVisitor.setDepthToCheckVertexProp(2);
  		MatchVisitor.setDepthToCompareTime(2);
      TransactionalBFS txBFS2;
  		txBFS2.breadthFirstSearch(Graph, StartVertex, MatchVisitor, Tranx, LockManager);

      if (Tranx->checkStatus() == T_ABORT)
        continue;

      Tranx->commit();
#ifdef _PRINTLOG_
  		auto targetsMap = MatchVisitor.getTimeMap();
      for (auto it = targetsMap.begin(), it_end = targetsMap.end();
          it != it_end; it++) {
        LdbcFile << (*it).first->getPropertyValue("firstName").first 
                 << "\t" << (*it).first->getPropertyValue("id").first
                 << "\tlikes comment/posts at\t" << (*it).second << "\n";
      }
      auto personMap = MatchVisitor.getPersonMap();
      for (auto it = personMap.begin(), it_end = personMap.end();
          it != it_end; it++)  {
        if ((*it).second)
          LdbcFile << "comment/post " << (*it).first->getPropertyValue("id").first << " are made by friend\n" ;
      }
#endif
    }

		LockManager.releaseAll(Tranx->getId());
		getExecTime();
		LdbcFile.close();
	}
};

class LdbcQuery8 : public LdbcQuery {
	using LdbcQuery::LdbcQuery;
public:

	void runQuery(GraphType & Graph, VertexDescriptor StartVertex,
	              TransactionPointerType Tranx, LockManagerType & LockManager) {
		getStartTime();

    while (Tranx->checkStatus() != T_COMMIT) {
      Tranx->expand();

  		FilterType Filters[3];
  		traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR", Filters[0]);
  		traverseThroughMultiRelType("REPLY_OF_COMMENT+REPLY_OF_POST", Filters[1]);
  		traverseThroughMultiRelType("COMMENT_HAS_CREATOR", Filters[2]);
  
  		TimeCompareVisitor TimeVisitor;
  		TimeVisitor.setFilter(Filters[0]);
  		TimeVisitor.setFilter(Filters[1]);
  		TimeVisitor.setFilter(Filters[2]);
  		TimeVisitor.setDepth(3);
  		TimeVisitor.setDepthToCompareTime(2);
      TransactionalBFS txBFS;
  		txBFS.breadthFirstSearch(Graph, StartVertex, TimeVisitor, Tranx, LockManager);

      if (Tranx->checkStatus() == T_ABORT)
        continue;

      Tranx->commit();
#ifdef _PRINTLOG_
  		auto vertexMap = TimeVisitor.getVertexMap();
      for (auto it = vertexMap.begin(), it_end = vertexMap.end();
          it != it_end; it++) {
        LdbcFile << "Person\t" << (*it).second->getPropertyValue("firstName").first 
                << "\tmade replies " << (*it).first->getPropertyValue("id").first 
                << "\tat " << (*it).first->getPropertyValue("creationDate").first 
                << " to startperson's posts/comments\n";
      }
#endif
    }

		LockManager.releaseAll(Tranx->getId());
		getExecTime();
		LdbcFile.close();
	}
};

class LdbcQuery9 : public LdbcQuery {
	using LdbcQuery::LdbcQuery;
public:
	typedef std::map<VertexPointer, VertexPointer> ReturnMapType;
	typedef std::pair<VertexPointer, VertexPointer> ReturnMapPair;
public:

	void runQuery(GraphType & Graph, VertexDescriptor StartVertex,
	              TransactionPointerType Tranx, LockManagerType & LockManager) {
		getStartTime();

    while (Tranx->checkStatus() != T_COMMIT) {
      Tranx->expand();

  		FilterType TmpFilter[2];
  		traverseThroughTypeAndDirection("KNOWS", "", TmpFilter[0]);
  		traverseThroughTypeAndDirection("KNOWS", "", TmpFilter[1]);
  		SingleRelTypeVisitor RelVisitor;
  		RelVisitor.setFilter(TmpFilter[0]);
  		RelVisitor.setFilter(TmpFilter[1]);
  		RelVisitor.setDepth(2);
      TransactionalBFS txBFS;
  		txBFS.breadthFirstSearch(Graph, StartVertex, RelVisitor, Tranx, LockManager);
      if (Tranx->checkStatus() == T_ABORT)
        continue;

      auto target = RelVisitor.getVertexList();

#ifdef _PRINTLOG_
  		LdbcFile << StartVertex << "\tis connected with " 
              << target.size() << " friends and friends of friends" 
              << "\n";
#endif

  		FilterType Filters[2];
  		traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR",Filters[0]);
  		Filters[1].setValueRange("creationDate", "", "2011-07-16T23:59:00.255");
  
      bool needRestart = false;
  		ReturnMapType TargetsMap;
  		for (auto it = target.begin(), it_end = target.end();
  		     it != it_end; ++it) {

#ifdef _PRINTLOG_
			LdbcFile <<"friend " << (*it)->getId() << "\t" << (*it)->getPropertyValue("id").first << "\t" << (*it)->getPropertyValue("firstName").first  << "\n";
#endif

  			MultiRelTypeVisitor RelVisitor;
  			RelVisitor.setFilter(Filters[0]);
  			RelVisitor.setRangeFilter(Filters[1]);
  			RelVisitor.setDepth(1);
  			RelVisitor.setDepthToCheckRange(1);
  			RelVisitor.setPropToCheck(2); //check date
  			unsigned int StartVertex = (*it)->getId();
        TransactionalBFS txBFS;
  			txBFS.breadthFirstSearch(Graph, StartVertex, RelVisitor, Tranx, LockManager);
  
        if (Tranx->checkStatus() == T_ABORT) {
          needRestart = true;
          break;
        }

  			auto targets = RelVisitor.getTargetsMap();
  			TargetsMap.insert(targets.begin(), targets.end());
      }///for

      if(needRestart)  {
        continue;
      }

      Tranx->commit();
#ifdef _PRINTLOG_
  		for (auto iter = TargetsMap.begin(), iter_end = TargetsMap.end();
          iter != iter_end; ++iter) {
        LdbcFile << "posts/comments\t" << (*iter).first->getPropertyValue("id").first 
                << "\t" << (*iter).first->getPropertyValue("creationDate").first 
                << "\tmade by person " << (*iter).second->getPropertyValue("id").first 
                << "\t" <<  (*iter).second->getPropertyValue("firstName").first 
                << "\n";
      }
#endif
    }

		LockManager.releaseAll(Tranx->getId());
		getExecTime();
		LdbcFile.close();
	}
};

class LdbcQuery10 : public LdbcQuery {
	using LdbcQuery::LdbcQuery;
public:
	typedef std::unordered_map<VertexPointer, unsigned int> SimMapType;
public:

  virtual void clearStorage( ) {
	  SimMap.clear();
  }

	void runQuery(GraphType & Graph, VertexDescriptor StartVertex,
	              TransactionPointerType Tranx, LockManagerType & LockManager) {
		getStartTime();

    while (Tranx->checkStatus() != T_COMMIT) {
      Tranx->expand();
      clearStorage();

  		///first find start person's friends
  		AdjacencyVisitor AdjVisitor;
  		traverseThroughTypeAndDirection("KNOWS", "out", AdjVisitor.getFilter());
      TransactionalBFS txBFS;
  		txBFS.breadthFirstSearch(Graph, StartVertex, AdjVisitor, Tranx, LockManager);
      if (Tranx->checkStatus() == T_ABORT)
        continue;

  		auto startId = Graph.getVertexPointer(StartVertex)->getPropertyValue("id").first.std_str();
  
  		///find friends of friends of start person
  		std::vector<VertexPointer> targets;
  		FilterType BDFilter;
  		BDFilter.setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second);

      bool needRestart = false;
  		for (VertexPointer StartVertex : AdjVisitor.getVertexList()) {
  			auto EqualFlag = false;
  			if (checkRange<VertexPointer>(4, StartVertex, BDFilter, EqualFlag)){
  				SimMap[StartVertex] = 0;
  				AdjacencyVisitor AdjVisitor;
  				traverseThroughTypeAndDirection("KNOWS", "out", AdjVisitor.getFilter());
          TransactionalBFS txBFS;
  				txBFS.breadthFirstSearch(Graph, StartVertex->getId(), AdjVisitor, Tranx, LockManager);
          if (Tranx->checkStatus() == T_ABORT) {
            needRestart = true;
            break;
          }
  				///concatenate two lists
  				targets.insert(targets.end(),AdjVisitor.getVertexList().begin(),
  				               AdjVisitor.getVertexList().end());
  			}///if
  		}///for
  
      if(needRestart)  {
        continue;
      }

  		for (VertexPointer Vertex : targets) {
  			auto EqualFlag = false;
  			if (checkRange<VertexPointer>(4, Vertex, BDFilter, EqualFlag)) {
  				SimMap[Vertex] = 0;
  			}
  		}
  
  		///iterate over the friends who satisfy the condition
  		auto itend  = SimMap.end();
  		for ( auto it = SimMap.begin(); it != itend; it++ ) {
  			FilterType TmpFilter[4];
  			traverseThroughMultiRelType("POST_HAS_CREATOR", TmpFilter[0]);
  			traverseThroughMultiRelType("POST_HAS_TAG", TmpFilter[1]);
  			traverseThroughMultiRelType("HAS_INTEREST", TmpFilter[2]);
  			TmpFilter[3].setProperty("id", startId);
  
  			SimilarityVisitor SimVisitor;
  			SimVisitor.setDepth(3);
  			SimVisitor.setDepthToCheckVertexProp(3);
  			SimVisitor.setVertexFilter(TmpFilter[3]);
  			for ( unsigned int i = 0; i < 3; i++ ) {
  				SimVisitor.setFilter(TmpFilter[i]);
  			}

        TransactionalBFS txBFS;
  			txBFS.breadthFirstSearch(Graph, (*it).first->getId()\
                                , SimVisitor, Tranx, LockManager);

        if (Tranx->checkStatus() == T_ABORT) {
          needRestart = true;
          break;
        }

  			auto iterend = SimVisitor.getPostMap().end();
  			unsigned int PostItrd = 0;
  			for ( auto iter = SimVisitor.getPostMap().begin();
  			      iter != iterend; iter++ ) {
  				if ((*iter).second ) { PostItrd++; }
  			}

  			SimMap[(*it).first] = 2*PostItrd - SimVisitor.getPostMap().size();

#ifdef _PRINTLOG_
			LdbcFile << "person: " << (*it).first->getId() << " similarity score " << (*it).second << "\n";
#endif
		  }///For 

      if(needRestart)  {
        continue;
      }

      Tranx->commit();
    }

		LockManager.releaseAll(Tranx->getId());
		getExecTime();
		LdbcFile.close();
	}

protected:
	SimMapType SimMap;
};

class LdbcQuery11 : public LdbcQuery {
	using LdbcQuery::LdbcQuery;
public:
	typedef std::pair<EdgePointer, VertexPointer> MapPair;
	typedef std::multimap<VertexPointer, MapPair> MatchMapType;
	typedef std::pair<VertexPointer, VertexPointer> ReturnMapPair;
	typedef std::vector<MatchMapType> ReturnTargetsType;
public:

	void runQuery(GraphType & Graph, VertexDescriptor StartVertex,
	              TransactionPointerType Tranx, LockManagerType & LockManager) {
		getStartTime();

    while (Tranx->checkStatus() != T_COMMIT) {
      Tranx->expand();

  		FilterType TmpFilter;
  		traverseThroughTypeAndDirection("KNOWS", "", TmpFilter);
  		SingleRelTypeVisitor RelVisitor;
  		RelVisitor.setFilter(TmpFilter);
  		RelVisitor.setFilter(TmpFilter);
  		RelVisitor.setDepth(2);
      TransactionalBFS txBFS;
  		txBFS.breadthFirstSearch(Graph, StartVertex, RelVisitor, Tranx, LockManager);

      if (Tranx->checkStatus() == T_ABORT)
        continue;

		  auto target = RelVisitor.getVertexList();
#ifdef _PRINTLOG_
		  LdbcFile << StartVertex << "\tis connected with\t" 
               << target.size() << "\tfriends and friends of friends" 
               << "\n";
#endif

  		FilterType Filters[4];
  		traverseThroughMultiRelType("WORKS_AT",Filters[0]);
  		traverseThroughMultiRelType("ORGANISATION_IS_LOCATED_IN",Filters[1]);
  		Filters[2].setValueRange(ValueRange.first, ValueRange.second.first, ValueRange.second.second);
  		Filters[3].setProperty(ParamPair.first, ParamPair.second);
  
  		MatchMapType TargetsMap;
      bool needRestart = false;
  		for(auto it = target.begin(), it_end = target.end();
  		    it != it_end; ++it) {

#ifdef _PRINTLOG_
			  LdbcFile <<"friend " << (*it)->getId() 
                 << "\t" << (*it)->getPropertyValue("id").first 
                 << "\t" << (*it)->getPropertyValue("firstName").first  
                 << "\n";
#endif

  			VertexPropertyVisitor VPVisitor;
  			VPVisitor.setFilter(Filters[0]);
  			VPVisitor.setFilter(Filters[1]);
  			VPVisitor.setRangeFilter(Filters[2]);
  			VPVisitor.setVertexFilter(Filters[3]);
  			VPVisitor.setDepth(2);
  			VPVisitor.setDepthToCheckRange(1);
  			VPVisitor.setDepthToCheckVertexProp(2);
  			unsigned int StartVertex = (*it)->getId();
        TransactionalBFS txBFS;
  			txBFS.breadthFirstSearch(Graph, StartVertex, VPVisitor, Tranx, LockManager);

        if (Tranx->checkStatus() == T_ABORT) {
          needRestart = true;
          break;
        }

  			auto targets = VPVisitor.getMatchMap();
  			TargetsMap.insert(targets.begin(), targets.end());
      }

      if (needRestart)  {
        continue;
      }

      Tranx->commit();
#ifdef _PRINTLOG_
  		FixedString key("workFrom");
      for (auto iter = TargetsMap.begin(), iter_end = TargetsMap.end();
        iter != iter_end; ++iter) {
        LdbcFile << (*iter).first->getPropertyValue("firstName").first 
                 << "\tworks at "  << (*iter).second.second->getPropertyValue("id").first
                 << "\tfrom " << (*iter).second.first->getPropertyValue(key).first 
                 << "\n";
      }
#endif
    }

		LockManager.releaseAll(Tranx->getId());
		getExecTime();
		LdbcFile.close();
	}
};

class LdbcQuery12 : public LdbcQuery {
	using LdbcQuery::LdbcQuery;
public:
	typedef std::unordered_map<VertexPointer, unsigned int> SimMapType;
public:

  virtual void clearStorage( ) {
	  SimMap.clear();
  }

	void runQuery(GraphType & Graph, VertexDescriptor StartVertex,
	              TransactionPointerType Tranx, LockManagerType & LockManager) {
		getStartTime();

    while (Tranx->checkStatus() != T_COMMIT) {
      Tranx->expand();
      clearStorage();

  		///first find start person's friends
  		AdjacencyVisitor AdjVisitor;
  		traverseThroughTypeAndDirection("KNOWS", "out", AdjVisitor.getFilter());
      TransactionalBFS txBFS;
  		txBFS.breadthFirstSearch(Graph, StartVertex, AdjVisitor, Tranx, LockManager);
      if (Tranx->checkStatus() == T_ABORT)
        continue;
  
  		///find friends of friends of start person
  		std::vector<VertexPointer> targets;
  		FilterType TmpFilter[6];
  		traverseThroughMultiRelType("COMMENT_HAS_CREATOR", TmpFilter[0]);
  		traverseThroughMultiRelType("REPLY_OF_POST", TmpFilter[1]);
  		traverseThroughMultiRelType("POST_HAS_TAG", TmpFilter[2]);
  		traverseThroughMultiRelType("HAS_TYPE", TmpFilter[3]);
  		traverseThroughMultiRelType("IS_SUBCLASS_OF", TmpFilter[4]);
  		TmpFilter[5].setProperty(ParamPair.first, ParamPair.second);
  
      bool needRestart = false;
  		for (VertexPointer StartVertex : AdjVisitor.getVertexList()) {
  			SimMap[StartVertex] = 0;
  
  			ExpertVisitor ExpertVisitor;
  			ExpertVisitor.setDepth(5);
  			ExpertVisitor.setDepthToCheckVertexProp(4);
  			ExpertVisitor.setVertexFilter(TmpFilter[5]);
  
  			for ( unsigned int i = 0; i < 5; i++ ) {
  				ExpertVisitor.setFilter(TmpFilter[i]);
  			}
  
        TransactionalBFS txBFS;
  			txBFS.breadthFirstSearch(Graph, StartVertex->getId(),
  			                         ExpertVisitor, Tranx, LockManager);

        if (Tranx->checkStatus() == T_ABORT) {
          needRestart = true;
          break;
        }

  			auto iterend = ExpertVisitor.getPostMap().end();
  			auto PostNum = 0;
  			for ( auto iter = ExpertVisitor.getPostMap().begin(); iter != iterend; iter++ ) {
  				if ((*iter).second ) { PostNum++; }
  			}

#ifdef _PRINTLOG_
			SimMap[StartVertex] = PostNum;
      LdbcFile << "person: " << StartVertex->getId() 
               << "\texpert post " << PostNum 
               << "\n";
#endif
      }///for

      if(needRestart)  {
        continue;
      }

      Tranx->commit();
		}///while

		LockManager.releaseAll(Tranx->getId());
		getExecTime();
		LdbcFile.close();
	}
protected:
	SimMapType SimMap;
};

class LdbcQuery13 : public LdbcQuery {
	using LdbcQuery::LdbcQuery;
public:

	void runQuery(GraphType & Graph
              , VertexDescriptor StartVertex
              , VertexDescriptor endVertex
              , TransactionPointerType Tranx
              , LockManagerType & LockManager) {

		getStartTime();

		if ( StartVertex == endVertex ) {
			std::cout << "StartVertex == endVertex \n";
      Tranx->commit();
      return;
//			exit(0);
		}

    while (Tranx->checkStatus() != T_COMMIT) {
      Tranx->expand();

  		PathVisitor PVisitor;
  		PVisitor.setEndVertex(endVertex);
      TransactionalBFS txBFS;
  		txBFS.breadthFirstSearch(Graph, StartVertex, PVisitor, Tranx, LockManager);
      if (Tranx->checkStatus() == T_ABORT)
        continue;

#ifdef _PRINTLOG_
  		auto target = PVisitor.getVertexList();
      if (target.empty()) {
        LdbcFile << StartVertex << "\tand\t" 
                <<  endVertex <<"\tare not connected" 
                << "\n";
      }
      else {
        LdbcFile << "There are  shortest paths of length \t" << target.size() 
                << "\tfrom " << StartVertex 
                << "\tto " << endVertex 
                << "\n";
        for (auto it = target.begin(); it != target.end(); ++it) {
          LdbcFile <<"Vertex\t" << (*it)->getId() 
                  << "\t" << (*it)->getPropertyValue("id").first 
                  << "\t" << (*it)->getPropertyValue("firstName").first
                  << "\n";
        }
      }
#endif
      Tranx->commit();
    }

		LockManager.releaseAll(Tranx->getId());
		getExecTime();
		LdbcFile.close();
	}

};

class LdbcQuery14 : public LdbcQuery {
	using LdbcQuery::LdbcQuery;
public:

	void runQuery(GraphType & Graph
              , VertexDescriptor StartVertex
	            , VertexDescriptor endVertex
              , TransactionPointerType Tranx 
              , LockManagerType & LockManager
              ) {
		getStartTime();

    while (Tranx->checkStatus() != T_COMMIT) {
      Tranx->expand();

  		SubGraphVisitor SubgVisitor;
  		SubgVisitor.setEndVertex(endVertex);
  		FilterType EdgeFilter;
  		traverseThroughTypeAndDirection("KNOWS", "out",  EdgeFilter);
  		SubgVisitor.setEdgeFilter(EdgeFilter);
      TransactionalBFS txBFS;
  		txBFS.breadthFirstSearch(Graph, StartVertex, SubgVisitor, Tranx, LockManager);

      if (Tranx->checkStatus() == T_ABORT)
        continue;

  		auto target = SubgVisitor.getVertexList();

#ifdef _PRINTLOG_
  		if (target.empty()) {
        LdbcFile << StartVertex << " and " <<  endVertex <<" are not connected" << "\n";
      }
      else {
        LdbcFile << "There are  shortest paths of length\t" 
                 << target.size() << "\tfrom\t" << StartVertex 
                 << "\tto\t" << endVertex 
                 << "\n";
        for(auto it = target.begin(); it != target.end(); ++it) {
          LdbcFile <<"Vertex\t" << (*it)->getId() 
                   << "\t" << (*it)->getPropertyValue("id").first 
                   << "\t" << (*it)->getPropertyValue("firstName").first
                   << "\n";
        }//for
      }//else
#endif

		///already found all the paths, calculate weights now
		auto itend = target.end();
    bool needRestart = false;
		float Weight = 0.0;
		for ( auto it = target.begin(); it!= itend && it != itend-1; it++ ) {

			FilterType TmpFilter[4];
			traverseThroughMultiRelType("COMMENT_HAS_CREATOR+POST_HAS_CREATOR", TmpFilter[0]);
			traverseThroughMultiRelType("REPLY_OF_POST+REPLY_OF_COMMENT", TmpFilter[1]);
			traverseThroughMultiRelType("POST_HAS_CREATOR+COMMENT_HAS_CREATOR", TmpFilter[2]);

			WeightedPathVisitor WPathVisitor;
			for ( unsigned int i = 0; i < 3; i++ ) {
				WPathVisitor.setFilter(TmpFilter[i]);
			}

			auto it2 = it+1;
			TmpFilter[3].setProperty("id",(*it2)->getPropertyValue("id").first.std_str());
			WPathVisitor.setVertexFilter(TmpFilter[3]);
			WPathVisitor.setDepth(3);
      TransactionalBFS txBFS;
			txBFS.breadthFirstSearch(Graph, (*it)->getId(), WPathVisitor, Tranx, LockManager);

      if (Tranx->checkStatus() == T_ABORT) {
        needRestart = true;
        break;
      }

			Weight += WPathVisitor.getScore();
		}///for

    if(needRestart)  {
      continue;
    }

    Tranx->commit();
#ifdef _PRINTLOG_
		LdbcFile << "weight " << Weight <<"\n";
#endif
    }///while

		LockManager.releaseAll(Tranx->getId());
		getExecTime();
		LdbcFile.close();
	}
};

class LdbcQueryAddVertex : public LdbcAddVertexQuery {
  using LdbcAddVertexQuery::LdbcAddVertexQuery;
public:
  typedef std::pair<EdgePointer, std::pair<bool, bool> > EdgePointerEntryType;
  typedef std::vector<EdgePointerEntryType> EdgePointerMapType;
  
  void clearStorage() {
    EdgePtrMap.clear();
    NewVertex->setNextEdge(nullptr);
  }
  /// This function is a simplified function of chainEdges in Edge class
  /// We simplify it because we know more information about how/what edges are created
  /// We don't need to query the whole graph to chain edges as in Edge class
  /// Basically the newly created edges will be chained first
  /// Edges involved with existing vertices won't be chained in this function
  void chainEdges() {
      for (auto EdgeEntry : EdgePtrMap) {
        /// Search for Vertex
        EdgePointer NewEdge = EdgeEntry.first;
        bool isFirstExisted = EdgeEntry.second.first;
        bool isExistedFirst = EdgeEntry.second.second;
        VertexPointer ExistVertex = NewEdge->getSecondVertexPtr();
        if (isFirstExisted) {
          ExistVertex = NewEdge->getFirstVertexPtr();
        }

        std::cout << "Existing vertex\n";
        std::cout << ExistVertex->getId() << "\t"
                  << ExistVertex->getType().std_str() 
                  << "\n";
  
        auto ExistNextEdge = ExistVertex->getNextEdge();
        /// Existed vertex does NOT have NextEdge or is the FirstVertex of NextEdge
        if (isExistedFirst && ExistNextEdge) {
          ExistNextEdge->setFirstPreviousEdge(NewEdge);
        } 
        else if (ExistNextEdge) {
          ExistNextEdge->setSecondPreviousEdge(NewEdge);
        }

        if (ExistNextEdge)  {
          std::cout <<"Existing Next Edge\n";
          std::cout << ExistNextEdge->getId() << "\t"
                    << ExistNextEdge->getType().std_str() << "\n";
        }

        ExistVertex->setNextEdge(NewEdge);

      }/// FOR 
  }
  
  /// Get locks only on exisiting edges and vertices
	virtual void runQuery(GraphType & Graph
                , VertexDescriptor StartVertex
                , TransactionPointerType Tranx 
                , LockManagerType & LockManager
                , IndexType & Index
                ) {

		getStartTime();

    while (Tranx->checkStatus() != T_COMMIT) {
      Tranx->expand();
      clearStorage();

      bool needRestart = false;
      for (auto EdgeEntry : EdgeMap) {
        /// Search for Vertex
        EdgePointer NewEdge = EdgeEntry.first;
        VertexInfoPairType VertexInfoPair = EdgeEntry.second;
        LabelType existVertexLabel = VertexInfoPair.first;
        ValueType existVertexId = VertexInfoPair.second;
        auto ExistIndex = Index.getVertexIndex(existVertexLabel, existVertexId);
  
        /// FirstVertex of NewEdge is existed
        bool isFirstExisted = false;
        /// The NextEdge of existed vertex has this vertex as FirstVertex
        bool isExistedFirst = true;
  
        if (!ExistIndex.second) {
#if _DEBUG_PRINT_
          std::cout << "Error: Fail in getting index on\n" ;
#endif
          continue;
        }

#if _DEBUG_PRINT_
        std::cout << "Existing vertex\n";
        std::cout << ExistIndex.first->getId() << "\t"
                  << ExistIndex.first->getType().std_str() 
                  << "\n";
#endif
        if (NewEdge->getFirstVertexPtr() == NewVertex) {
  
          NewEdge->setSecondVertexPtr(ExistIndex.first);
        } else  {
          isFirstExisted = true;
          NewEdge->setFirstVertexPtr(ExistIndex.first);
        } ///IF_FIRST_EXISTED
  
        /// Get locks on existing vertex and edge

#ifdef _TRANX_STATS_
        auto NEMutexPtr = LockManager.getVertexLockPointer(ExistIndex.first->getId(), T_NextEdge); 
        Tranx->visitMutex(NEMutexPtr);
#endif
        if (!LockManager.getVertexLock(ExistIndex.first->getId(), T_NextEdge, T_EX, Tranx->getId())) {

#ifdef _TRANX_STATS_
          Tranx->abortMutex(NEMutexPtr);
#endif
          Tranx->abort();
          needRestart = true;
          break;
        }
  
        auto ExistNextEdge = ExistIndex.first->getNextEdge();
        if (ExistNextEdge && (ExistNextEdge->getFirstVertexPtr() == ExistIndex.first)) {

#if _DEBUG_PRINT_
          std::cout <<"Existing Next Edge\n";
          std::cout << ExistNextEdge->getId() << "\t"
                    << ExistNextEdge->getType().std_str() << "\n";
#endif

#ifdef _TRANX_STATS_
          auto FPEMutexPtr = LockManager.getEdgeLockPointer(ExistNextEdge->getId(), T_FirstPrevEdge);
          Tranx->visitMutex(FPEMutexPtr);
#endif
          if (!LockManager.getEdgeLock(ExistNextEdge->getId(), T_FirstPrevEdge, T_EX, Tranx->getId()))  {

#ifdef _TRANX_STATS_
            Tranx->abortMutex(FPEMutexPtr);
#endif
            Tranx->abort();
            needRestart = true;
            break;
          }
        }
        else if(ExistNextEdge && (ExistNextEdge->getSecondVertexPtr() == ExistIndex.first)) {

#if _DEBUG_PRINT_
          std::cout <<"Existing Next Edge\n";
          std::cout << ExistNextEdge->getId() << "\t"
                    << ExistNextEdge->getType().std_str() << "\n";
#endif

          isExistedFirst = false;

#ifdef _TRANX_STATS_
          auto SPEMutexPtr = LockManager.getEdgeLockPointer(ExistNextEdge->getId(), T_SecondPrevEdge);
          Tranx->visitMutex(SPEMutexPtr);
#endif
          if (!LockManager.getEdgeLock(ExistNextEdge->getId(), T_SecondPrevEdge, T_EX, Tranx->getId()))  {

#ifdef _TRANX_STATS_
            Tranx->abortMutex(SPEMutexPtr);
#endif
            Tranx->abort();
            needRestart = true;
            break;
          }
        } ///IF_EXISTED_FIRST
  
        /// /***************
        ///  Chain edges
        /// ****************/
  
        /// NewEdge -> FirstNextEdge,SecondNextEdge
        if (isFirstExisted) {
          NewEdge->setFirstNextEdge(ExistNextEdge);
          /// NewVertex->getNextEdge() == NULL
          NewEdge->setSecondNextEdge(NewVertex->getNextEdge());
        } else {
          NewEdge->setSecondNextEdge(ExistNextEdge);
          NewEdge->setFirstNextEdge(NewVertex->getNextEdge());
        }
  
        ///NewVertex's NextEdge -> FirstPreviousEdge/SecondPreviousEdge
        auto NewVertexNextEdge = NewVertex->getNextEdge();
        if (NewVertexNextEdge)  {
          if (NewVertexNextEdge->getFirstVertexPtr() == NewVertex)  {
            NewVertexNextEdge->setFirstPreviousEdge(NewEdge);  
          } else {
            NewVertexNextEdge->setSecondPreviousEdge(NewEdge);  
          }
        }
        
        /// ExistedVertex & NewVertex -> NextEdge
        NewVertex->setNextEdge(NewEdge);
//        ExistIndex.first->setNextEdge(NewEdge);
        EdgePtrMap.push_back(EdgePointerEntryType(NewEdge, 
                         std::pair<bool, bool>(isFirstExisted, isExistedFirst)));
      }/// FOR 
  
      if(needRestart)  {
        continue;
      }

      Tranx->commit();
#ifdef _PRINTLOG_
      int i = 0;
      EdgePointer NextEdge = NewVertex->getNextEdge();
  		LdbcFile << "NewVertex\t" << NewVertex->getId() << "\n";
      while (NextEdge && i++ < 5) {
        LdbcFile << "FirstVertex\t" << NextEdge->getFirstVertexPtr()->getType() 
                  << "\nEdge\t" << NextEdge->getType()
                  << "\nSecondVertex\t" << NextEdge->getSecondVertexPtr()->getType()
                  << "\nSecondPrevEdge\t" << NextEdge->getSecondNextEdge()->getType()
                  << "\n\n";
  
        NextEdge = NextEdge->getNextEdge(NewVertex);
      }
#endif
    }///while
/// TODO add locks to map LockManager

    chainEdges();
    Graph.addVertex(NewVertex);
    Index.buildVertexIndex("id", NewVertex);
    LockManager.addToVertexLockMap(NewVertex->getId());

    for(auto EdgePtr : EdgePtrMap)  {
      Graph.addEdge(EdgePtr.first, false);
      LockManager.addToEdgeLockMap(EdgePtr.first->getId());
    }

		LockManager.releaseAll(Tranx->getId());
		getExecTime();
		LdbcFile.close();
  }

protected:
  ValueType NewVertexId;
  EdgePointerMapType EdgePtrMap;
};

class LdbcQueryAddEdge : public LdbcAddEdgeQuery {
  using LdbcAddEdgeQuery::LdbcAddEdgeQuery;
public:
	virtual void runQuery(GraphType & Graph
                , VertexDescriptor StartVertex
//                , Visitor  & GraphVisitor
                , TransactionPointerType Tranx
                , LockManagerType & LockManager
                , IndexType & Index
                ) {

		getStartTime();

    bool addSuccess = false;
    while (Tranx->checkStatus() != T_COMMIT) {
      Tranx->expand();

      auto FirstIndex = Index.getVertexIndex(FirstLabel, FirstId);
      auto SecondIndex = Index.getVertexIndex(SecondLabel, SecondId);
      /// If both end vertices are retrievable, get locks on both vertex pointers

      if (FirstIndex.second && SecondIndex.second) {
        std::cout << "First Vertex\t" << FirstIndex.first->getId()
                  << "\tLabel\t" << FirstIndex.first->getType().std_str()
                  << "\n"
                  << "Second Vertex\t" << SecondIndex.first->getId()
                  << "\tLabel\t" << SecondIndex.first->getType().std_str()
                  << "\n";
        /// Switch here

#ifdef _TRANX_STATS_
        auto NEMutexPtr = LockManager.getVertexLockPointer(FirstIndex.first->getId(), T_NextEdge);
        Tranx->visitMutex(NEMutexPtr);
#endif

        if (!LockManager.getVertexLock(FirstIndex.first->getId(), T_NextEdge, T_EX, Tranx->getId())) {

#ifdef _TRANX_STATS_
          Tranx->abortMutex(NEMutexPtr);
#endif
          Tranx->abort();
          continue;
        }

#ifdef _TRANX_STATS_
        auto SNEMutexPtr = LockManager.getVertexLockPointer(SecondIndex.first->getId(), T_NextEdge);
        Tranx->visitMutex(SNEMutexPtr);
#endif
        if (!LockManager.getVertexLock(SecondIndex.first->getId(), T_NextEdge, T_EX, Tranx->getId()))  {

#ifdef _TRANX_STATS_
          Tranx->abortMutex(SNEMutexPtr);
#endif
          Tranx->abort();
          continue;
        }
        auto FNEdge = FirstIndex.first->getNextEdge();
        if (FNEdge) {
#ifdef _TRANX_STATS_
          auto FPEMutexPtr = LockManager.getEdgeLockPointer(FNEdge->getId(), T_FirstPrevEdge);
          Tranx->visitMutex(FPEMutexPtr);
#endif

          if (!LockManager.getEdgeLock(FNEdge->getId(), T_FirstPrevEdge, T_EX, Tranx->getId()))  {
#ifdef _TRANX_STATS_
            Tranx->abortMutex(FPEMutexPtr);
#endif
            Tranx->abort();
            continue;
          }

#ifdef _TRANX_STATS_
          auto SPEMutexPtr = LockManager.getEdgeLockPointer(FNEdge->getId(), T_SecondPrevEdge);
          Tranx->visitMutex(SPEMutexPtr);
#endif
          if (!LockManager.getEdgeLock(FNEdge->getId(), T_SecondPrevEdge, T_EX, Tranx->getId()))  {
#ifdef _TRANX_STATS_
            Tranx->abortMutex(SPEMutexPtr);
#endif
            Tranx->abort();
            continue;
          }
        }

        auto SNEdge = SecondIndex.first->getNextEdge();
        if (SNEdge && (SNEdge != FNEdge) ) {

#ifdef _TRANX_STATS_
          auto SFPEMutexPtr = LockManager.getEdgeLockPointer(SNEdge->getId(), T_FirstPrevEdge);
          Tranx->visitMutex(SFPEMutexPtr);
#endif
          if (!LockManager.getEdgeLock(SNEdge->getId(), T_FirstPrevEdge, T_EX, Tranx->getId()))  {
#ifdef _TRANX_STATS_
            Tranx->abortMutex(SFPEMutexPtr);
#endif
            Tranx->abort();
            continue;
          }

#ifdef _TRANX_STATS_
          auto SSPEMutexPtr = LockManager.getEdgeLockPointer(SNEdge->getId(), T_SecondPrevEdge);
          Tranx->visitMutex(SSPEMutexPtr);
#endif
          if (!LockManager.getEdgeLock(SNEdge->getId(), T_SecondPrevEdge, T_EX, Tranx->getId()))  {

#ifdef _TRANX_STATS_
            Tranx->abortMutex(SSPEMutexPtr);
#endif
            Tranx->abort();
            continue;
          }
        }

        std::cout <<"Addedge\tget all locks\n";
        FirstIndex.first->setNextEdge(NewEdge);
        SecondIndex.first->setNextEdge(NewEdge);
        NewEdge->setFirstVertexPtr(FirstIndex.first);
        NewEdge->setSecondVertexPtr(SecondIndex.first);
        NewEdge->setFirstNextEdge(FNEdge);
        NewEdge->setSecondNextEdge(SNEdge);
 
        if (FNEdge) {
          FNEdge->setFirstPreviousEdge(NewEdge);
          FNEdge->setSecondPreviousEdge(NewEdge);
        }
        if (SNEdge) {
          SNEdge->setFirstPreviousEdge(NewEdge);
          SNEdge->setSecondPreviousEdge(NewEdge);
        }
        addSuccess = true;
      }///if
      else {

        std::cout << "Error: At least one vertex do NOT exist\n";
      }

      Tranx->commit();
    }///while

    if ( addSuccess ) {
      auto NewEdgeId = Graph.addEdge(NewEdge, false);
      LockManager.addToEdgeLockMap(NewEdgeId);
    }

#ifdef _PRINTLOG_
    VertexPointer FirstVertex = NewEdge->getFirstVertexPtr();
    VertexPointer SecondVertex = NewEdge->getSecondVertexPtr();
    if (addSuccess)  {
  		LdbcFile << "NewEdge\t" << NewEdge->getType() << "\t" << NewEdge->getId() 
                << "\nFirstVertex\t" << FirstVertex->getType() 
                << "\t" << FirstVertex->getId()
                << "\nSecondVertex\t" << SecondVertex->getType()
                << "\t" << SecondVertex->getId()
                << "\nFirstNextEdge\t" <<  NewEdge->getNextEdge(FirstVertex)->getType() 
                << "\nSecondNextEdge\t" << NewEdge->getNextEdge(SecondVertex)->getType() 
                << "\n\n";
    }
#endif

		LockManager.releaseAll(Tranx->getId());
		getExecTime();
		LdbcFile.close();
  }

};

/*
class Query17 : public LDBCQuery {
	using LDBCQuery::LDBCQuery;
public:
	typedef std::string KeyType;
	typedef std::string ValueType;
	typedef std::vector<ValueType> ValueListType;
	typedef std::pair<std::string, GraphType::PropertyListType> EdgePairType;
	typedef std::map<std::pair<std::string, std::string>, EdgePairType> BranchMapType;
public:

	void setValueList(ValueListType & vl) {
		ValueList = vl;
	}

	void setBranchMap(BranchMapType & bm) {
		BranchMap = bm;
	}

	virtual void runQuery(GraphType & Graph, TransactionPointerType Tranx) {
		getStartTime();
		//a new vertex vs
		//need new propertylist for vertex and branchmap (criteria) for searching neighbor
		AddVisitor AdVisitor(Tranx, Graph);
		AdVisitor.setVertexProperty(VertexPropertyList);
		AdVisitor.getFilter().setBranchMap(BranchMap);
    TransactionalBFS txBFS;
		txBFS.breadthFirstSearch(Graph, 0, AdVisitor);

		getExecTime();
		LdbcFile << "Add one more node into network \n";
		LdbcFile.close();
    Tranx->commit();
	}
protected:
	BranchMapType BranchMap;
	ValueListType ValueList;
};
*/
#endif /*_LDBCTRANSACTIONALQUERY_CPP_*/
