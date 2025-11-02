###############################################################################
# cmake variables
cmake_minimum_required(VERSION 4.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

###############################################################################
# convert version info to header
configure_file(${CMAKE_SOURCE_DIR}/cmake/version.hpp.in ${CMAKE_BINARY_DIR}/version.hpp)

###############################################################################
# project definition
project(${CONAN_PROJECT_NAME}
        VERSION ${CONAN_PROJECT_VERSION}
        DESCRIPTION ${CONAN_PROJECT_DESCRIPTION}
        LANGUAGES CXX
)

###############################################################################
# find dependencies

find_program(CMAKE_CXX_CPPCHECK cppcheck REQUIRED)
find_package(mp-units REQUIRED)

###############################################################################
# cppcheck configuration
list(
    APPEND CMAKE_CXX_CPPCHECK
      "--enable=warning,style"
      "-cppcheck-build-dir=${CMAKE_BINARY_DIR}"
)

###############################################################################
# embedded executable definition
add_subdirectory(src)

install(TARGETS ${CONAN_PROJECT_NAME})
