cmake_minimum_required(VERSION 3.21)
project(QtVideoInfo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt build helpers
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Core Multimedia)

# Collect sources/headers without hard-coding file names
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

add_executable(qt_video_info
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

target_include_directories(qt_video_info
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(qt_video_info
    PRIVATE
        Qt6::Core
        Qt6::Multimedia
)

set_target_properties(qt_video_info PROPERTIES
    BUILD_WITH_INSTALL_RPATH ON
    INSTALL_RPATH "$ORIGIN"
)

# Formatting
include(cmake/clang_format.cmake)

# ---- Utilities ----
# Clean all build trees found inside the repo
add_custom_target(clean_all
  COMMAND ${CMAKE_COMMAND} -E echo "Removing: ${CMAKE_BINARY_DIR}"
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}"
  COMMENT "Remove the entire build directory"
)

# Make sure the command executes with CWD = source dir (not inside build/)
set_property(TARGET clean_all PROPERTY WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
