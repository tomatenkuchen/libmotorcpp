###############################################################################
# cmake variables
cmake_minimum_required(VERSION 4.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

###############################################################################
# project definition
project(test-package
  LANGUAGES CXX
)

###############################################################################
# find dependencies

find_package(libmotor CONFIG REQUIRED)
find_package(Catch2 REQUIRED)

# cppcheck 
find_program(CMAKE_CXX_CPPCHECK cppcheck REQUIRED)
list(
  APPEND CMAKE_CXX_CPPCHECK
    "--enable=all"
    "--cppcheck-build-dir=${CMAKE_BINARY_DIR}"
)

###############################################################################
# test executable definition
add_subdirectory(src)

add_executable(tests 
  src/tests.cpp
)
target_link_libraries(tests 
  PRIVATE 
    libmotor::libmotor
    Catch2::Catch2WithMain
)