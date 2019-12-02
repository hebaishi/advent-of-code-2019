cmake_minimum_required(VERSION 3.15)

project(advent_of_code_2019)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src)