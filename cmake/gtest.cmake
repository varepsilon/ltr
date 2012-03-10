# SET paths to gtest's sources and binaries
SET (gtest_SOURCE_DIR ${Source_Path}/contrib/gtest)
SET (gtest_BINARY_DIR ${PROJECT_BINARY_DIR}/gtest)

# Define helper functions and macros used by Google Test.
INCLUDE(${gtest_SOURCE_DIR}/cmake/internal_utils.cmake)

config_compiler_and_linker()  # Defined in internal_utils.cmake.

# Where Google Test's .h files can be found.
INCLUDE_DIRECTORIES(
  ${gtest_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR})

# Where Google Test's libraries can be found.
LINK_DIRECTORIES(${gtest_BINARY_DIR})
