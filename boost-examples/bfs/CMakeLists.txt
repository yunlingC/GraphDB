cmake_minimum_required(VERSION 2.6)
   
  Project(BreadthFirstSearch)
   
  set(Boost_USE_MULTITHREADED ON)
  FIND_PACKAGE(Boost 1.46 COMPONENTS required)
   
  INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${Boost_INCLUDE_DIRS})
  LINK_DIRECTORIES(${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS})
   
  ADD_EXECUTABLE(BreadthFirstSearch BreadthFirstSearch.cpp)
