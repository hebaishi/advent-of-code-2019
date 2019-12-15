get_filename_component(CURRENT_PROJECT ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_executable(
  ${CURRENT_PROJECT}_puzzle1
  puzzle1.cpp
)

target_compile_definitions(
  ${CURRENT_PROJECT}_puzzle1
  PRIVATE
  -DCURRENT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)