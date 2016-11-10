cmake_minimum_required(VERSION 2.6)
 
Project(graphdb)
 
set(Boost_USE_MULTITHREADED OFF)
#find_package(Boost 1.57 COMPONENTS required)

# Set the variables for GROMIT.
set(READERS_INCLUDE_DIR "readers")
set(SRC_DIR "src")
set(STORAGE_DIR "storage")
set(TRAVERSALS_DIR "traversals")
set(GDBENCH_DIR "GDBench")
set(LDBC_DIR "LDBC")
set(ALGO_DIR "algorithm")
set(TRANSACTIONS_DIR "transactions")

include_directories(
  # Add all the boost and python includes.
  ${Boost_INCLUDE_DIRS}
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
##set(TRANSACTION_LIB transactions)
#set(GDB_LD_FLAGS storage) 

# Compiler flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11 -D_FIXALLOC_ -Wall")

# Debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pthread -std=c++14  -Wall -fPIC")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pthread -std=c++14 -D_FIXALLOC_ -D_SKIP_ -Wall")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pthread -std=c++11 -Wall")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pthread -std=c++14   -Wall -fPIC")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pthread -std=c++14 -D_PRINTLOG_ -D_LOCKING_ -Wall -fPIC")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pthread -std=c++14 -D_FIXALLOC_  -D_SKIP_ -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pthread -std=c++11 -D_FIXALLOC_ -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pthread -std=c++11 -Wall")

#add_library (transaction
#  ../transactions/Transaction.cpp
#  ../transactions/LocksManager.cpp
#  ../transactions/TransactionManager.cpp
#  )

# Storage sub directory
add_subdirectory(storage)
##add_subdirectory(transactions)

set(TESTS 
  #  main-gdbench.cpp
  #  main-algo.cpp
  #  main-ldbc.cpp
  #  main-dfs.cpp
  #  main-rdfs-ldbc.cpp
  #  main-txbfs.cpp
  main-multithread.cpp
  main-input.cpp
  )


# This will generate all the executables listed in the TESTS variable
foreach( testsourcefile ${TESTS} ) 
  string( REPLACE ".cpp" "" testname ${testsourcefile} ) 
  add_executable( ${testname} ${testsourcefile} )
  # Make sure YourLib is linked to each app 
  target_link_libraries( ${testname} ${STORAGE_LIB} ) #${TRANSACTION_LIB}) 
endforeach( testsourcefile ${TESTS} )

# macro(PYEXPORT _name pybind/_srccpp)
#   PYTHON_ADD_MODULE(${_name} ${_srccpp})
#   target_link_libraries(${_name} ${STORAGE_LIB})
#   FILE(COPY pybind/${_name}.py DESTINATION .)
# endmacro()
          
#PYEXPORT(graph_python pybind/graph_python.cpp)

# This works
#python_add_module(graph_python pybind/graph_python.cpp)
#target_link_libraries(graph_python ${STORAGE_LIB})
#file(COPY pybind/graph_python.py DESTINATION .)
