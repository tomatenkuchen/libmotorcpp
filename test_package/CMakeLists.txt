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
find_package(mp-units CONFIG REQUIRED)

# cppcheck 
find_program(CMAKE_CXX_CPPCHECK cppcheck REQUIRED)
list(
  APPEND CMAKE_CXX_CPPCHECK
    "--enable=all"
    "--cppcheck-build-dir=${CMAKE_BINARY_DIR}"
    "--suppress=missingIncludeSystem"
    "--suppress=missingInclude"
)

###############################################################################
# test executable definition
add_executable(tests 
  src/tests.cpp
)
target_link_libraries(tests 
  PRIVATE 
    libmotor::libmotor
    Catch2::Catch2WithMain
    mp-units::mp-units
)

###############################################################################
# cmake test definition
include(CTest)
add_test(
  NAME tests 
  COMMAND tests
)