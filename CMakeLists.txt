cmake_minimum_required( VERSION 3.10)

project(STLLearn)

add_definitions(-std=c++11 -g)

add_subdirectory(src/ContainerDemo)
add_subdirectory(src/STL)
add_subdirectory(src/Test)
