cmake_minimum_required(VERSION 2.6)
 
Project(graphdb)
 
set(Boost_USE_MULTITHREADED OFF)
FIND_PACKAGE(Boost 1.57 COMPONENTS required)
 
set(READERS_INCLUDE_DIR "readers")
set(SRC_DIR "src")

set(STORAGE_DIR "storage")

INCLUDE_DIRECTORIES( 
        ${INCLUDE_DIRECTORIES} 
        ${Boost_INCLUDE_DIRS} 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${READERS_INCLUDE_DIR}
        ${SRC_DIR}
	${STORAGE_DIR}
        )

LINK_DIRECTORIES(${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS})


ADD_SUBDIRECTORY(storage)
SET(STORAGE_LIB storage)
SET(GDB_LD_FLAGS storage) 

SET(CMAKE_CXX_FLAGS "$ENV{CMAKE_CXX_FLAGS} -std=c++11")
SET(TARGET t)

# Set the release build for all optimizations
SET(CMAKE_BUILD_TYPE Debug)

#foreach (ld_flag ${GDB_LD_FLAGS})
#	target_link_libraries( ${TARGET} ${ld_flag} )
#endforeach()

#ADD_EXECUTABLE(graphdb main.cpp)
#ADD_EXECUTABLE(graphdb1 main1.cpp)
ADD_EXECUTABLE(${TARGET} main-storage.cpp  )

target_link_libraries(${TARGET} ${STORAGE_LIB})
