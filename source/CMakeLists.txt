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
add_executable(FractalColoring_sec secuential.cpp)
target_link_libraries(FractalColoring_sec PUBLIC ${EXTRA_LIBS})

find_package(OpenMP REQUIRED)
# add the executable - OpenMP v1
add_executable(FractalColoring_omp_v1 openmp_v1.cpp)
target_link_libraries(FractalColoring_omp_v1
    PUBLIC ${EXTRA_LIBS}
    PUBLIC OpenMP::OpenMP_CXX)

# add the executable - OpenMP v2
add_executable(FractalColoring_omp_v2 openmp_v2.cpp)
target_link_libraries(FractalColoring_omp_v2
    PUBLIC ${EXTRA_LIBS}
    PUBLIC OpenMP::OpenMP_CXX)

find_package(MPI REQUIRED)
set(CMAKE_CXX_COMPILER mpicxx)
# add the executable - MPI
add_executable(FractalColoring_mpi mpi.cpp)
target_link_libraries(FractalColoring_mpi
    PUBLIC ${EXTRA_LIBS}
    PUBLIC mpi)

# add the executable - MPI & OpenMP
add_executable(FractalColoring_mpi_openmp mpi_openmp.cpp)
target_link_libraries(FractalColoring_mpi_openmp
    PUBLIC ${EXTRA_LIBS}
    PUBLIC OpenMP::OpenMP_CXX
    PUBLIC mpi)
