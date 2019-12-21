cmake_minimum_required(VERSION 3.15)

set (CMAKE_CXX_STANDARD 17)

project(advent_of_code_2019)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR})

set(PEDANTIC_FLAGS "-Werror -Wall -Wextra -Wpedantic -fdiagnostics-color")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PEDANTIC_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PEDANTIC_FLAGS}")

add_subdirectory(src)
