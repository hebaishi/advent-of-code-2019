get_filename_component(CURRENT_PROJECT ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_executable(
  puzzles
  src/day01.cpp
  src/day02.cpp
  src/day03.cpp
  src/day04.cpp
  src/day05.cpp
  src/day06.cpp
  src/day07.cpp
  src/day08.cpp
  src/day09.cpp
)

target_compile_definitions(
  puzzles
  PRIVATE
  -DCURRENT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data"
)

target_link_libraries(
  puzzles
  catch2
  puzzles_common
)