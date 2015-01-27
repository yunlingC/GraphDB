#set(CMAKE_BUILD_TYPE Release)

SET(CMAKE_CXX_FLAGS "$ENV{CMAKE_CXX_FLAGS} -std=c++11")

add_library (storage
	    Vertex.cpp
	    Edge.cpp
	    )
