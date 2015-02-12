#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

using namespace std;

class Q {
 public:
  void h( int id ) {
  for (auto i = 0; i < 500; i++) 
    cout << "hello " << id << "\n";
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

  Q q;
  // std::thread t(&Q::h,q,2);
  // t.join();
  
  vector<thread> threads;
  for (auto i = 0; i < 20; ++i) {
    threads.push_back( thread( &Q::h, q, i ));
  }
  
  for_each(threads.begin(), threads.end(),
           std::mem_fn(&thread::join));

  return 0;

};
