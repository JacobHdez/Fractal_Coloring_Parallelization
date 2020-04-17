cmake_minimum_required(VERSION 3.16)

# set the project name and version
project(Test VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(USE_HEADERS "Use headers" ON)

configure_file(TestConfig.h.in TestConfig.h)

if(USE_HEADERS)
    # add the headers libraries
    add_subdirectory(headers)
    list(APPEND EXTRA_LIBS headers)
    list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/headers")
endif()

# add the executable
add_executable(Test test.cpp)

target_link_libraries(Test PUBLIC ${EXTRA_LIBS})

target_include_directories(Test PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/headers"
                           )
