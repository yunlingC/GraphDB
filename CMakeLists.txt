cmake_minimum_required(VERSION 2.6)
 
Project(graphdb)
 
set(Boost_USE_MULTITHREADED OFF)
FIND_PACKAGE(Boost 1.57 COMPONENTS required)
 
set(READERS_INCLUDE_DIR "readers")
set(SRC_DIR "src")
          
INCLUDE_DIRECTORIES( 
        ${INCLUDE_DIRECTORIES} 
        ${Boost_INCLUDE_DIRS} 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${READERS_INCLUDE_DIR}
        ${SRC_DIR}
        )

LINK_DIRECTORIES(${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS})

ADD_EXECUTABLE(graphdb main.cpp)
