###############################################################################
# target definition
add_library(${CONAN_PROJECT_NAME}
  motor.cpp
  motor.hpp
)

target_include_directories(${CONAN_PROJECT_NAME}
  PRIVATE
    ${CMAKE_BINARY_DIR} # version.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
  PUBLIC
    mp-units::mp-units
)

target_compile_features(${CONAN_PROJECT_NAME}
  PRIVATE
    cxx_std_23
)

target_compile_options(${CONAN_PROJECT_NAME}
  PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)

