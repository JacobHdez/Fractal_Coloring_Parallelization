cmake_minimum_required(VERSION 3.16)

# set the project name
project(FractalColoring)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the headers
add_subdirectory(headers)
list(APPEND EXTRA_LIBS headers)

# add the executable
add_executable(FractalColoring main.cpp)

target_link_libraries(FractalColoring PUBLIC ${EXTRA_LIBS})
