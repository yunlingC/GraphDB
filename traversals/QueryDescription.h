//===-- traversals/QueryDescription.h - Query class ----------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the class for GDBench queries.
///
//===----------------------------------------------------------------------===//

#ifndef _QUERYDESCRIPTION_H_
#define _QUERYDESCRIPTION_H_

#include <vector>
#include <string>
#include <iostream>

#include "FixedString.h"
#include "breadth_first_search.h"
#include "CustomVisitor.h"

class Query {
public:
  typedef std::string KeyType;
  typedef std::string ValueType;
  typedef GraphType Graph;
  typedef GraphType::VertexDescriptor Vertex;
public:
  Query() { }

private:
  KeyType   _key;
  ValueType _value; 
    
};

class Query1 : public Query {
public:
 void runQuery(Graph & graph ) {
    cout << "===============================\n";
    cout << "Query 1\n";
    Q1Visitor v1;
    filtProperty(_key, _value, v1.getFilter());
    traverseThroughType("FRIENDS", v1.getFilter());
    breadth_first_search(graph, 0, v1 );
    cout <<"People with " << _key << " = " << _value <<" is(are) as below\n";
    v1.dumpTarget();
    cout << endl;
 }
  void setPersonProperty(KeyType key, ValueType value)
  {
    _key = key;
    _value = value;
  }

private:
 KeyType     _key;
 ValueType   _value;
};


class Query2 : public Query {
public:
  typedef GraphType::VertexDescriptor Vertex;
public:
 void runQuery(Graph & graph, Vertex startVertex ) {
    cout << "===============================\n";
    cout << "Query 2 \n";
    Q2Visitor v2; 
    traverseThroughType("LIKES", v2.getFilter());
    breadth_first_search(graph, startVertex, v2);
    cout << "People who likes webpage id = " << startVertex   << " are as below\n";

    auto targetName = v2.getVertexTargetList();
    for(auto it = targetName.begin(); it != targetName.end(); ++it) {
      FixedString key("name");
      cout << "Vertex " << (*it)->getId() << (*it)->getPropertyValue(key).first;
      cout << endl;
    }
    cout << endl;
 }
  void setPersonProperty(KeyType key, ValueType value)
  {
    _key = key;
    _value = value;
  }

private:
 KeyType     _key;
 ValueType   _value;

};


class Query3 : public Query {
public:
  typedef GraphType::VertexDescriptor Vertex;
public:
 void runQuery(Graph & graph, Vertex startVertex ) {
    cout << "===============================\n";
    cout << "Query 3\n";
    Q2Visitor v3; 
    traverseThroughType("LIKES", v3.getFilter());
    breadth_first_search(graph, startVertex, v3);
    cout << "Person with vid = " << startVertex << " likes webpages:\n";

    auto targetName = v3.getVertexTargetList();
    for(auto it = targetName.begin(); it != targetName.end(); ++it) {
      FixedString key("wpurl");
      cout <<"Vertex " << (*it)->getId() << "\t"  << (*it)->getPropertyValue(key).first;
      cout << endl;
    }
    cout << endl;
 }
  void setPersonProperty(KeyType key, ValueType value)
  {
    _key = key;
    _value = value;
  }

private:
 KeyType     _key;
 ValueType   _value;

};

class Query4 : public Query {
public:
  void runQuery(Graph & graph ) {
    cout << "===============================\n";
    cout << "Query 4 \n";
    Q1Visitor v4;
    filtProperty(_key, _value, v4.getFilter());
    traverseThroughType("FRIENDS", v4.getFilter());
    breadth_first_search(graph, 0, v4 );
    cout <<"People with " << _key << " = " << _value <<" is(are) as below\n";

    auto targetName = v4.getVertexTargetList();
    for(auto it = targetName.begin(); it != targetName.end(); ++it) {
      FixedString key("name");
      cout <<"Vertex " << (*it)->getId() << "\t"  << (*it)->getPropertyValue(key).first;
      cout << endl;
    }
    

    cout << endl;
 }
  void setPersonProperty(KeyType key, ValueType value)
  {
    _key = key;
    _value = value;
  }

private:
 KeyType     _key;
 ValueType   _value;
};

class Query5 : public Query {
public:
 typedef GraphType::VertexDescriptor vertex;
 
public:
  void runQuery(Graph & graph, Vertex startVertex ) {
    cout << "===============================\n";
    cout << "Query 5\n";
    Q5Visitor v5;
    Filter tmpFilter;
    traverseThroughTypeAndDirection("FRIENDS", "out",  tmpFilter);
    v5.setFilter(tmpFilter);
    traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter);
    v5.setFilter(tmpFilter);
    v5.setDepth(2);
    breadth_first_search(graph, startVertex, v5);
    FixedString key("name");
    cout << "The friends of Person with vid = " << startVertex << " has " << v5.getVertexTargetList().size() << " friends\n";
    auto targetName = v5.getVertexTargetList();
    for(auto it = targetName.begin(); it != targetName.end(); ++it) {
      FixedString key("name");
      cout <<"Vertex " << (*it)->getId() << "\t"  << (*it)->getPropertyValue(key).first;
      cout << endl;
    }
  }

};

class Query6: public Query {
public:
 typedef GraphType::VertexDescriptor vertex;
 
public:
  void runQuery(Graph & graph, Vertex startVertex ) {
    cout << "===============================\n";
    cout << "Query 6\n";
    Q5Visitor v6;
    Filter tmpFilter;
    traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter);
    v6.setFilter(tmpFilter);
    traverseThroughTypeAndDirection("LIKES", "out", tmpFilter);
    v6.setFilter(tmpFilter);
    v6.setDepth(2);
    breadth_first_search(graph, startVertex, v6);
    FixedString key("name");
    cout << "The friends of Person with vid = " << startVertex << " like " << v6.getVertexTargetList().size() << " webpages\n";
    auto targetName = v6.getVertexTargetList();
    for(auto it = targetName.begin(); it != targetName.end(); ++it) {
      FixedString key("wpurl");
      cout <<"Vertex " << (*it)->getId() << "\t"  << (*it)->getPropertyValue(key).first;
      cout << endl;
    }
  }
};

class Query7: public Query {
public:
 typedef GraphType::VertexDescriptor vertex;
 
public:
  void runQuery(Graph & graph, Vertex startVertex ) {
    cout << "===============================\n";
    cout << "Query 7\n";
    Q5Visitor v7;
    Filter tmpFilter;
    traverseThroughType("LIKES", tmpFilter);
    v7.setFilter(tmpFilter);
    traverseThroughType("LIKES", tmpFilter);
    v7.setFilter(tmpFilter);
    v7.setDepth(2);
    breadth_first_search(graph, startVertex, v7);
    FixedString key("name");
    cout << "The webpages liked by person vid = " << startVertex << " are liked by people: \n";
    auto targetName = v7.getVertexTargetList();
    for(auto it = targetName.begin(); it != targetName.end(); ++it) {
      FixedString key("name");
      if( (*it)->getId() != startVertex ) {
      cout <<"Vertex " << (*it)->getId() << "\t"  << (*it)->getPropertyValue(key).first;
      cout << endl;
      }
    }
  }
};

class Query8 : public Query {
public:
 typedef GraphType::VertexDescriptor vertex;
 
public:
  void runQuery(Graph & graph, Vertex startVertex, Vertex endVertex ) {
    cout << "===============================\n";
    cout << "Query 8\n";
    Q8Visitor v8;
    v8.setEndVertex(endVertex);
    breadth_first_search(graph, startVertex, v8);
    cout << "There are " << v8.getVertexTargetList().size() << " vertex on the path from " << startVertex << " to " <<  endVertex << endl;
    auto targetName = v8.getVertexTargetList();
    for(auto it = targetName.begin(); it != targetName.end(); ++it) {
      cout <<"Vertex " << (*it)->getId() << endl; 
    }
  }

};

class Query9: public Query {
public:
 typedef GraphType::VertexDescriptor vertex;
 
public:
  void runQuery(Graph & graph, Vertex startVertex, Vertex endVertex ) {
    cout << "===============================\n";
    cout << "Query 9\n";
    Q8Visitor v9;
    v9.setEndVertex(endVertex);
    breadth_first_search(graph, startVertex, v9);
    cout << "The shortest path from " << startVertex << " to " <<  endVertex << " has " << v9.getVertexTargetList().size() << " vertex" << endl;
    auto targetName = v9.getVertexTargetList();
    for(auto it = targetName.begin(); it != targetName.end(); ++it) {
      cout <<"Vertex " << (*it)->getId() << endl; 
    }
  }

};

class Query10 : public Query {
public:
 typedef GraphType::VertexDescriptor vertex;
 
public:
  void runQuery(Graph & graph, Vertex startVertex, Vertex endVertex ) {
    cout << "===============================\n";
    cout << "Query 10\n";
    Q10Visitor v10;
    Filter tmpFilter;
    traverseThroughTypeAndDirection("FRIENDS", "out",  tmpFilter);
    v10.setFilter(tmpFilter);
    traverseThroughTypeAndDirection("FRIENDS", "in", tmpFilter);
    v10.setFilter(tmpFilter);
    v10.setDepth(2);
    v10.setEndVertex(endVertex);
    breadth_first_search(graph, startVertex, v10);
    FixedString key("name");
    cout << "There are " << v10.getVertexTargetList().size() << " common friends between  " << startVertex << " and " <<  endVertex << endl;
    auto targetName = v10.getVertexTargetList();
    for(auto it = targetName.begin(); it != targetName.end(); ++it) {
      FixedString key("name");
      cout <<"Vertex " << (*it)->getId() << "\t"  << (*it)->getPropertyValue(key).first;
      cout << endl;
    }
  }
};


class Query11: public Query {
public:
 typedef GraphType::VertexDescriptor vertex;
 
public:
  void runQuery(Graph & graph, Vertex startVertex, Vertex endVertex ) {
    cout << "===============================\n";
    cout << "Query 11\n";
    Q10Visitor v11;
    Filter tmpFilter;
    traverseThroughTypeAndDirection("LIKES", "out",  tmpFilter);
    v11.setFilter(tmpFilter);
    traverseThroughTypeAndDirection("LIKES", "in", tmpFilter);
    v11.setFilter(tmpFilter);
    v11.setDepth(2);
    v11.setEndVertex(endVertex);
    breadth_first_search(graph, startVertex, v11);
    FixedString key("name");
    cout << "There are " << v11.getVertexTargetList().size() << " common webpages liked by both " << startVertex << " and " <<  endVertex << endl;
    auto targetName = v11.getVertexTargetList();
    for(auto it = targetName.begin(); it != targetName.end(); ++it) {
      FixedString key("wpurl");
      cout <<"Vertex " << (*it)->getId() << "\t"  << (*it)->getPropertyValue(key).first;
      cout << endl;
    }
  }

};


    
class Query13: public Query {
public:
 typedef GraphType::VertexDescriptor vertex;
 
public:
  void runQuery(Graph & graph, Vertex startVertex ) {
    cout << "===============================\n";
    cout << "Query 13\n";
    Q5Visitor v13;
    Filter tmpFilter;
    traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter);
    v13.setFilter(tmpFilter);
    traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter);
    v13.setFilter(tmpFilter);
    traverseThroughTypeAndDirection("FRIENDS", "out", tmpFilter);
    v13.setFilter(tmpFilter);
//    traverseThroughDirection("out", v13.getFilter()); 
    v13.setDepth(3);
    breadth_first_search(graph, startVertex, v13);
    FixedString key("name");
    cout << "The friends of friends of person vid = " << startVertex << " has " << v13.getVertexTargetList().size() << " friends: \n";
    auto targetName = v13.getVertexTargetList();
    for(auto it = targetName.begin(); it != targetName.end(); ++it) {
      FixedString key("name");
      if( (*it)->getId() != startVertex ) {
      cout <<"Vertex " << (*it)->getId() << "\t"  << (*it)->getPropertyValue(key).first;
      cout << endl;
      }
    }
  }
};


 
class Query12 : public Query {
public:
  typedef GraphType::VertexDescriptor Vertex;
public:
 void runQuery(Graph & graph, Vertex startVertex ) {
    cout << "===============================\n";
    cout << "Query 12 \n";
    Q2Visitor v12; 
    traverseThroughType("FRIENDS", v12.getFilter());
    breadth_first_search(graph, startVertex, v12);
    FixedString key("name");
    cout << "Person with vid = " << startVertex << " has name: " << graph.getVertexPointer(startVertex)->getPropertyValue(key).first <<" and  " << v12.getVertexTargetList().size() << " friends\n";
 }

  void setPersonProperty(KeyType key, ValueType value)
  {
    _key = key;
    _value = value;
  }

private:
 KeyType     _key;
 ValueType   _value;

};




#endif 

