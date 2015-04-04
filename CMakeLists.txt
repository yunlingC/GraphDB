cmake_minimum_required(VERSION 2.6)
 
Project(graphdb)
 
set(Boost_USE_MULTITHREADED OFF)
find_package(Boost 1.57 COMPONENTS required)
 
set(READERS_INCLUDE_DIR "readers")
set(SRC_DIR "src")
set(STORAGE_DIR "storage")
set(TRAVERSALS_DIR "traversals")
set(GDBENCH_DIR "GDBench")
set(LDBC_DIR "LDBC")

include_directories( 
        ${INCLUDE_DIRECTORIES} 
        ${Boost_INCLUDE_DIRS} 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${READERS_INCLUDE_DIR}
        ${SRC_DIR} 
        ${STORAGE_DIR}
        ${GDBENCH_DIR}
	      ${TRAVERSALS_DIR}
        ${LDBC_DIR}
        )

link_directories(
        ${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS}
        )


# Set the release build for all optimizations
#set(CMAKE_BUILD_TYPE Release)

# Set the target folder
set(TARGET t)

# Set additional flags and parameters
set(STORAGE_LIB storage)
set(GDB_LD_FLAGS storage) 

# Compiler flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11 -D_FIXALLOC_ -Wall")

# Debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pthread -std=c++11 -D_FIXALLOC_ -Wall")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pthread -std=c++11 -D_FIXALLOC_ -Wall")


# Storage sub directory
add_subdirectory(storage)
set(TESTS 
  # main-storage.cpp 
  #        main-linkedlist.cpp
        #      main-dfs.cpp
        main-gdbench.cpp
        #        main-ldbc.cpp
        )

# This will generate all the executables listed in the TESTS variable
foreach( testsourcefile ${TESTS} ) 
        string( REPLACE ".cpp" "" testname ${testsourcefile} ) 
        add_executable( ${testname} ${testsourcefile} ) 
        # Make sure YourLib is linked to each app 
        target_link_libraries( ${testname} ${STORAGE_LIB}) 
endforeach( testsourcefile ${TESTS} )
