######################################################################################
# gtest_force_shared_crt == ON should be used, otherwise ltr may be not-linkable
# (when one's solution links to ltr, link errors may be received)
OPTION(gtest_force_shared_crt
  "Use shared (DLL) run-time lib even when Google Test is built as static lib."
  ON)
# Now, as we don't want gtest to affect global variables such as CMAKE_CXX_FLAGS,
# some fixes were done in internal_utils.cmake(56).
# Hence gtest_force_shared_crt == ON is hardcoded and high warning level in
# entire solution is avoided

OPTION(gtest_disable_pthreads "Disable uses of pthreads in gtest." OFF)
######################################################################################

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
