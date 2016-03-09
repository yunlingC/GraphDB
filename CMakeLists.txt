cmake_minimum_required(VERSION 2.6)
 
Project(graphdb)
 
set(Boost_USE_MULTITHREADED OFF)
#find_package(Boost 1.57 COMPONENTS required)


# You need to find these packages for exporting C++ to Python.
find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(Boost COMPONENTS python)

# Set the variables for GROMIT.
set(READERS_INCLUDE_DIR "readers")
set(SRC_DIR "src")
set(STORAGE_DIR "storage")
set(TRAVERSALS_DIR "traversals")
set(GDBENCH_DIR "GDBench")
set(LDBC_DIR "LDBC")
set(ALGO_DIR "algorithm")
set(TRANSACTIONS_DIR "transactions")
set(PYBIND_DIR "pybind")

include_directories(
  # Add all the boost and python includes.
  ${Boost_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIRS}
  ${INCLUDE_DIRECTORIES}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${READERS_INCLUDE_DIR}
  
  # Start adding the GROMIT sources.
  ${SRC_DIR} 
  ${STORAGE_DIR}
  ${GDBENCH_DIR}
  ${TRAVERSALS_DIR}
  ${TRANSACTIONS_DIR}
  ${LDBC_DIR}
  ${ALGO_DIR}
  ${PYBIND_DIR}
  )

link_directories(
  ${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS}
  )


# You must link the libraries to the boost and python libs.
link_libraries(${Boost_LIBRARIES} ${PYTHON_LIBRARIES})


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
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pthread -std=c++11  -Wall")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pthread -std=c++14 -D_FIXALLOC_ -D_SKIP_ -Wall")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pthread -std=c++11 -D_PRINTLOG_  -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pthread -std=c++14 -D_FIXALLOC_  -D_SKIP_ -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pthread -std=c++14 -Wall")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pthread -std=c++11 -Wall")

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pthread -std=c++11 -D_FIXALLOC_ -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pthread -std=c++11 -D_FIXALLOC_ -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pthread -std=c++11 -Wall")


# Storage sub directory
add_subdirectory(storage)
add_subdirectory(pybind)
set(TESTS 
  main-gdbench.cpp
  main-algo.cpp
  main-ldbc.cpp
  main-dfs.cpp
  )

# This will generate all the executables listed in the TESTS variable
foreach( testsourcefile ${TESTS} ) 
  string( REPLACE ".cpp" "" testname ${testsourcefile} ) 
  add_executable( ${testname} ${testsourcefile} ) 
  # Make sure YourLib is linked to each app 
  target_link_libraries( ${testname} ${STORAGE_LIB}) 
endforeach( testsourcefile ${TESTS} )
