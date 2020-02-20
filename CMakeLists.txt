cmake_minimum_required(VERSION 3.15)
project(clustering)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV REQUIRED)

add_executable(clustering main.cpp clustering.hpp debug.h)
add_executable(clustering_test tests/clustering_test.cpp clustering.hpp)

target_link_libraries(clustering ${OpenCV_LIBS})
target_link_libraries(clustering_test ${OpenCV_LIBS})
