cmake_minimum_required(VERSION 3.16)

# set the project name
project(FractalColoring)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the headers
add_subdirectory(headers)
list(APPEND EXTRA_LIBS headers)

# add the executable - secuential
add_executable(FractalColoring_sec main.cpp)
target_link_libraries(FractalColoring_sec PUBLIC ${EXTRA_LIBS})

# add the executable - OpenMP
find_package(OpenMP)
add_executable(FractalColoring_omp main.cpp)
target_link_libraries(FractalColoring_omp
    PUBLIC ${EXTRA_LIBS}
    PUBLIC OpenMP::OpenMP_CXX)
