##########################################################################################
# CMake build script for LTR.
##########################################################################################

# let all libraries be static, not shared
OPTION(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

##########################################################################################
PROJECT(LTRdevelop CXX C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

IF (NOT DEFINED CMAKE_CURRENT_LIST_DIR)
    SET(CMAKE_CURRENT_LIST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF (NOT DEFINED CMAKE_CURRENT_LIST_DIR)

MESSAGE("CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")
MESSAGE("PROJECT_BINARY_DIR    : ${PROJECT_BINARY_DIR}")

SET (Source_Path ${CMAKE_CURRENT_LIST_DIR})
SET (Install_Path ${Source_Path}/lib)

MESSAGE("Source_Path           : ${Source_Path}")
MESSAGE("Install_Path          : ${Install_Path}")

##########################################################################################
# lets SETup OpenMP flags
FIND_PACKAGE(OpenMP)
IF (OPENMP_FOUND)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
ENDIF (OPENMP_FOUND)

# let use pthread on unix
IF (UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pthread")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -pthread")
ENDIF (UNIX)

########################################################################################
# lets SETup boost 
INCLUDE (${Source_Path}/cmake/boost.cmake)

##########################################################################################
# lets SETup Puppy library
INCLUDE (${Source_Path}/cmake/puppy.cmake)
ADD_LIBRARY (puppy STATIC ${Puppy_SOURCES} ${Puppy_HEADERS})

INSTALL(TARGETS puppy ARCHIVE DESTINATION ${Install_Path})

##########################################################################################
# lets SETup utility
INCLUDE (${Source_Path}/cmake/utility.cmake)
ADD_LIBRARY (util_timer STATIC ${TIMER_SOURCES})
ADD_LIBRARY (util_logger STATIC ${LOGGER_SOURCES})
TARGET_LINK_LIBRARIES (util_timer ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES (util_logger util_timer)
SET (UTILITY util_logger util_timer)

INSTALL(TARGETS ${UTILITY} ARCHIVE DESTINATION ${Install_Path})

##########################################################################################
# lets SETup tinyxml library
INCLUDE (${Source_Path}/cmake/tinyxml.cmake)
ADD_LIBRARY (tinyxml STATIC ${TINYXML_SOURCES})

##########################################################################################
# lets SETup LTR as a static library project, called ltr
INCLUDE (${Source_Path}/cmake/ltr.cmake)
ADD_LIBRARY (ltr STATIC ${LTR_ALL})
TARGET_LINK_LIBRARIES (ltr puppy ${UTILITY} ${Boost_LIBRARIES})

INSTALL(TARGETS ltr ARCHIVE DESTINATION ${Install_Path})

##########################################################################################
# lets SETup LTR_client
INCLUDE (${Source_Path}/cmake/ltr_client.cmake)
ADD_EXECUTABLE (ltr_client ${LTR_CLIENT_SRC})
TARGET_LINK_LIBRARIES (ltr_client ltr tinyxml ${UTILITY})

##########################################################################################
# lets SETup Google Test
INCLUDE (${Source_Path}/cmake/gtest.cmake)
cxx_library(gtest "${cxx_strict}" ${gtest_SOURCE_DIR}/src/gtest-all.cc)

##########################################################################################
# lets SETup our unit-tests for LTR (executed by Google Test)
INCLUDE (${Source_Path}/cmake/utests.cmake)
ADD_EXECUTABLE (ltr_test ${LTR_TEST_ALL})
TARGET_LINK_LIBRARIES (ltr_test ltr gtest ${Boost_LIBRARIES})

# lets copy the test data to the build directory
INCLUDE (${Source_Path}/cmake/copy_test_data.cmake)
