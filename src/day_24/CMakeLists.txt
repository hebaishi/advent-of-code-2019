get_filename_component(CURRENT_PROJECT ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file(
  GLOB
  INPUT_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/*.txt
)

file(
  COPY
  ${INPUT_FILES}
  DESTINATION
  ${CMAKE_BINARY_DIR}/bin
)

add_executable(
  ${CURRENT_PROJECT}_puzzle1
  puzzle1.cpp
)

add_executable(
  ${CURRENT_PROJECT}_puzzle2
  puzzle2.cpp
)