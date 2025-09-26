# ------------------------------------------------------------------
# remove all CMake build directories under the source tree
# ------------------------------------------------------------------
message(STATUS "Remove ALL files inside CMAKE_BINARY_DIR (but keep the dir)")
get_filename_component(BIN "${CMAKE_BINARY_DIR}" ABSOLUTE)

# Remove all children of the build dir
file(GLOB children RELATIVE "${BIN}" "${BIN}/*" "${BIN}/.*")
foreach(child IN LISTS children)
  if(child STREQUAL "." OR child STREQUAL "..")
    continue()
  endif()
  file(REMOVE_RECURSE "${BIN}/${child}")
endforeach()

message(STATUS "clean_all: Done.")
