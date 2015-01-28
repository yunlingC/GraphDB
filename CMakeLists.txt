cmake_minimum_required(VERSION 2.6)
 
Project(graphdb)
 
set(Boost_USE_MULTITHREADED OFF)
find_package(Boost 1.57 COMPONENTS required)
 
set(READERS_INCLUDE_DIR "readers")
set(SRC_DIR "src")

set(STORAGE_DIR "storage")

include_directories( 
        ${INCLUDE_DIRECTORIES} 
        ${Boost_INCLUDE_DIRS} 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${READERS_INCLUDE_DIR}
        ${SRC_DIR} 
        ${STORAGE_DIR}
        )

link_directories(
        ${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS}
        )


# Set the release build for all optimizations
set(CMAKE_BUILD_TYPE Debug)

set(TARGET t)
set( STORAGE_LIB storage)
set(GDB_LD_FLAGS storage) 
set(CMAKE_CXX_FLAGS "-std=c++11 -D_FIXALLOC_")

# Storage sub directory
add_subdirectory(storage)


#foreach (ld_flag ${GDB_LD_FLAGS})
#	target_link_libraries( ${TARGET} ${ld_flag} )
#endforeach()

#ADD_EXECUTABLE(graphdb main.cpp)
#ADD_EXECUTABLE(graphdb1 main1.cpp)
add_executable(
        ${TARGET} main-storage.cpp  )

target_link_libraries(${TARGET} ${STORAGE_LIB})
