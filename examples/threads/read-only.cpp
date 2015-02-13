#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

using namespace std;

class BaseQuery {

public:

  virtual void h( int id ) {
    for (auto i = 0; i < 10; i++) 
      cout << "Base: " << id << "\n";
  }
};

class Query1 : public BaseQuery {

public:
  virtual void h(int id) {
    for (auto i = 0; i < 20; i++) 
      cout << "Query1: " << id << "\n";
  }
};

class Query2 : public BaseQuery {

public:
  virtual void h(int id) {
    for (auto i = 0; i < 30; i++) 
      cout << "Query2: " << id << "\n";
  }
};

// class Query {
// public:
//   explicit Query( std::thread& t_): t(t_) {}

//   ~Query() {
//     if ( t.joinable() ) {
//       t.join();
//     }
//   }

//   Query(Query const&)=delete;
//   Query& operator=(Query const& )=delete;


// private:
//   std::thread& t;

// };

int main() {
  
  
  // std::thread t(&Q::h,q,2);
  // t.join();

  Query1 q1;
  Query2 q2;

  vector<BaseQuery*> queries;
  queries.push_back(&q1);
  queries.push_back(&q2);

  //  BaseQuery* bq1 = queries[0];

  vector<thread> threads;
  for (auto i = 0; i < 20; ++i) {
    if (i <10) {
      threads.push_back( thread( &Query1::h, q1, i ));
    } else {
      threads.push_back( thread( &Query2::h, q2, i ));
    }
    
  }
  
  // for ( auto it = queries.begin(); it != queries.end();
  // 	++it) {
  //   BaseQuery* RunQuery = *it;
  //   RunQuery->h(1);
  // }
  
  
  
  for_each(threads.begin(), threads.end(),
	   std::mem_fn(&thread::join));

  return 0;

};
