cmake_minimum_required(VERSION 2.8)

project( PatternMatch )
find_package( OpenCV )
include_directories( /usr/local/Cellar/opencv/2.4.11/include )
add_executable( PatternMatch PatternMatch.cpp )
link_directories(/usr/local/Cellar/opencv/2.4.11/lib)
target_link_libraries( PatternMatch ${OpenCV_LIBS} )