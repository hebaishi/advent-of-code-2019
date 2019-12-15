add_library(
  puzzles_common
  SHARED
  src/emulator/intcode.cpp
  src/io/io.cpp
  include/io/io.hpp
  include/emulator/intcode.hpp
)

target_include_directories(
  puzzles_common
  PUBLIC
  include
)