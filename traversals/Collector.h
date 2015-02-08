#ifndef _COLLECTOR_H_
#define _COLLECTOR_H_

#include <vector>

template<typename ElementType>
struct Collector {
public: 
  Collector():_counter(0) { }


  void incrementCounter() {
    _counter ++;
  }

  unsigned int getCounter() {
    return _counter;
  }

  unsigned int getSize() {
    return _container.size();
  }

  void collect(ElementType elem) {
    _container.push_back(elem);
  }


private:
  unsigned int _counter;
  std::vector<ElementType> _container;

};


#endif /**COLLECTOR_H_*/
