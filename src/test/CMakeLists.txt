cmake_minimum_required(VERSION 3.16)

project(test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_FILES test.cpp rgb.cpp image.cpp)

add_executable(test ${SOURCE_FILES})
