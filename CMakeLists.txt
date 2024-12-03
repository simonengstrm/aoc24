cmake_minimum_required(VERSION 3.9)
project(aoc24 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_BUILD_TYPE Release)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    add_compile_options(-Wno-c23-extensions)
endif()

set(executables d1 d2 d3)

foreach(executable ${executables})
    file(GLOB_RECURSE sources ${executable}/*.cxx ${executable}/*.hxx)
    add_executable(${executable} ${sources}) 
endforeach()