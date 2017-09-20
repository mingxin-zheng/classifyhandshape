cmake_minimum_required( VERSION 2.8 )
project( handshape )

set( CMAKE_CXX_FLAGS "-std=c++11" )
# Find OpenCV library
find_package( OpenCV REQUIRED )
# Include OpenCV Directories
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( train train.cpp)
add_executable( demo demo.cpp)
add_executable( testResult testResult.cpp)

# link libraries
target_link_libraries( train ${OpenCV_LIBS} )
target_link_libraries( demo ${OpenCV_LIBS} )
target_link_libraries( testResult ${OpenCV_LIBS} )