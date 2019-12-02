get_filename_component(CURRENT_PROJECT ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_executable(
  ${CURRENT_PROJECT}_puzzle1
  puzzle1.cpp
)

add_executable(
  ${CURRENT_PROJECT}_puzzle2
  puzzle2.cpp
)

target_compile_definitions(
  ${CURRENT_PROJECT}_puzzle1
  PRIVATE
  -DCURRENT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

target_compile_definitions(
  ${CURRENT_PROJECT}_puzzle2
  PRIVATE
  -DCURRENT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(
  ${CURRENT_PROJECT}_puzzle1
  CONAN_PKG::boost
)

target_link_libraries(
  ${CURRENT_PROJECT}_puzzle2
  CONAN_PKG::boost
)

add_library(
  ${CURRENT_PROJECT}_shared
  STATIC
  shared.cpp
)

target_link_libraries(
  ${CURRENT_PROJECT}_puzzle1
  ${CURRENT_PROJECT}_shared
)

target_link_libraries(
  ${CURRENT_PROJECT}_puzzle2
  ${CURRENT_PROJECT}_shared
)