cmake_minimum_required(VERSION 2.6)
 
Project(graphdb)
 
set(Boost_USE_MULTITHREADED OFF)
find_package(Boost 1.57 COMPONENTS required)
 
set(READERS_INCLUDE_DIR "readers")
set(SRC_DIR "src")
set(STORAGE_DIR "storage")
set(TRAVERSALS_DIR "traversals")

include_directories( 
        ${INCLUDE_DIRECTORIES} 
        ${Boost_INCLUDE_DIRS} 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${READERS_INCLUDE_DIR}
        ${SRC_DIR} 
        ${STORAGE_DIR}
	${TRAVERSALS_DIR}
        )

link_directories(
        ${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS}
        )


# Set the release build for all optimizations
set(CMAKE_BUILD_TYPE Debug)

# Set the target folder
set(TARGET t)

# Set additional flags and parameters
set(STORAGE_LIB storage)
set(GDB_LD_FLAGS storage) 

# Compiler flags
set(CMAKE_CXX_FLAGS "-std=c++11 -D_FIXALLOC_")

# Storage sub directory
add_subdirectory(storage)
set(TESTS 
        main-storage.cpp 
        main-fixstring.cpp
        main-linkedlist.cpp
        main-dfs.cpp
        )

# This will generate all the executables listed in the TESTS variable
foreach( testsourcefile ${TESTS} ) 
        string( REPLACE ".cpp" "" testname ${testsourcefile} ) 
        add_executable( ${testname} ${testsourcefile} ) 
        # Make sure YourLib is linked to each app 
        target_link_libraries( ${testname} ${STORAGE_LIB}) 
endforeach( testsourcefile ${TESTS} )
