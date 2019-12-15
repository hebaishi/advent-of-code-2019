add_library(
  catch2
  src/catch_main.cpp
)

target_include_directories(
  catch2
  PUBLIC
  include
)