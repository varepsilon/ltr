##########################################################################################
# CMake build script for LTR.
#
##########################################################################################
#Options. Mostly for Google Test jet.

#let all libraries be static, not shared
OPTION(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

# When other libraries are using a shared version of runtime libraries,
# Google Test also has to use one.
OPTION(
  gtest_force_shared_crt
  "Use shared (DLL) run-time lib even when Google Test is built as static lib."
  OFF)

OPTION(gtest_disable_pthreads "Disable uses of pthreads in gtest." OFF)

##########################################################################################
# lets start describing our project.
project (LTRdevelop CXX C)
cmake_minimum_required(VERSION 2.6)

IF (NOT DEFINED CMAKE_CURRENT_LIST_DIR)
    SET(CMAKE_CURRENT_LIST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF (NOT DEFINED CMAKE_CURRENT_LIST_DIR)

MESSAGE("CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")
SET (Source_Path ${CMAKE_CURRENT_LIST_DIR})
MESSAGE("PROJECT_BINARY_DIR    : ${PROJECT_BINARY_DIR}")

##########################################################################################
# lets SETup OpenMP flags
FIND_PACKAGE(OpenMP)
IF (OPENMP_FOUND)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
ENDIF (OPENMP_FOUND)

IF (UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
ENDIF (UNIX)

##########################################################################################
# lets SETup boost manually (1.48.0 used)
INCLUDE (${Source_Path}/cmake/boost.cmake)
ADD_LIBRARY (boost_date_time ${Boost_date_time_SOURCES})
ADD_LIBRARY (boost_system ${Boost_system_SOURCES})
ADD_LIBRARY (boost_filesystem ${Boost_filesystem_SOURCES})
TARGET_LINK_LIBRARIES (boost_filesystem boost_system)

##########################################################################################
# lets SETup Puppy library
INCLUDE (${Source_Path}/cmake/puppy.cmake)
ADD_LIBRARY (puppy ${Puppy_SOURCES} ${Puppy_HEADERS})

##########################################################################################
# lets SETup utility
INCLUDE (${Source_Path}/cmake/utility.cmake)
ADD_LIBRARY (util_logger ${LOGGER_SOURCES})
ADD_LIBRARY (util_timer ${TIMER_SOURCES})
SET (UTILITY util_timer util_logger)

##########################################################################################
# lets SETup tinyxml library
INCLUDE (${Source_Path}/cmake/tinyxml.cmake)
ADD_LIBRARY (tinyxml ${TINYXML_SOURCES})

##########################################################################################
# lets SETup LTR as a static library project, called LTR_lib
INCLUDE (${Source_Path}/cmake/ltr.cmake NO_POLICY_SCOPE)
ADD_LIBRARY (ltr_lib STATIC ${LTR_ALL})
TARGET_LINK_LIBRARIES (ltr_lib boost_date_time puppy ${UTILITY})

##########################################################################################
# lets SETup LTR_client
INCLUDE (${Source_Path}/cmake/ltr_client.cmake NO_POLICY_SCOPE)
ADD_EXECUTABLE (ltr_client ${LTR_CLIENT_SRC})
TARGET_LINK_LIBRARIES (ltr_client ltr_lib tinyxml ${UTILITY})

##########################################################################################
# lets SETup Google Test
INCLUDE (${Source_Path}/cmake/gtest.cmake NO_POLICY_SCOPE)

# Google Test librariry. Leads to high warning level in entire solution. i don't know yet how to avoid it
cxx_library(gtest "${cxx_strict}" ${gtest_SOURCE_DIR}/src/gtest-all.cc)

##########################################################################################
# lets SETup our unit-tests for LTR (executed by Google Test)
INCLUDE (${Source_Path}/cmake/utests.cmake NO_POLICY_SCOPE)
ADD_EXECUTABLE (ltr_lib_test ${LTR_TEST_ALL})
TARGET_LINK_LIBRARIES (ltr_lib_test ltr_lib gtest boost_filesystem)

# lets copy the test data to the build directory
INCLUDE (${Source_Path}/cmake/copy_test_data.cmake NO_POLICY_SCOPE)
