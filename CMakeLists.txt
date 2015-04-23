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
        1.cpp
        2.cpp
        3.cpp
        4.cpp
        5.cpp
        6.cpp
        7.cpp
        8.cpp
        9.cpp
        10.cpp
        11.cpp
        12.cpp
        13.cpp
        14.cpp
        15.cpp
        16.cpp
        17.cpp
        18.cpp
        19.cpp
        20.cpp
        21.cpp
        22.cpp
        23.cpp
        24.cpp
        25.cpp
        26.cpp
        27.cpp
        28.cpp
        
        #main-ldbc.cpp
        )

# This will generate all the executables listed in the TESTS variable
foreach( testsourcefile ${TESTS} ) 
        string( REPLACE ".cpp" "" testname ${testsourcefile} ) 
        add_executable( ${testname} ${testsourcefile} ) 
        # Make sure YourLib is linked to each app 
        target_link_libraries( ${testname} ${STORAGE_LIB}) 
endforeach( testsourcefile ${TESTS} )
