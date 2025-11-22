###############################################################################
# cmake variables
cmake_minimum_required(VERSION 4.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

###############################################################################
# project definition
project(${CONAN_PROJECT_NAME}
  VERSION ${CONAN_PROJECT_VERSION}
  DESCRIPTION ${CONAN_PROJECT_DESCRIPTION}
  LANGUAGES CXX
)

###############################################################################
# find dependencies

find_package(mp-units REQUIRED)

# cppcheck 
find_program(CMAKE_CXX_CPPCHECK cppcheck REQUIRED)
list(
  APPEND CMAKE_CXX_CPPCHECK
    "--enable=all"
    "--cppcheck-build-dir=${CMAKE_BINARY_DIR}"
)

###############################################################################
# embedded executable definition
add_subdirectory(src)

