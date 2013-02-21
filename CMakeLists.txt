# Use cmake -DCMAKE_CXX_COMPILER=g++46 /mnt/freedata/home/Void/Yandex/ltr1
# to compile via gcc46

##########################################################################################
# CMake build script for LTR.
##########################################################################################

# let all libraries be static, not shared
OPTION(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

##########################################################################################
PROJECT(LTRdevelop CXX C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

IF(NOT CMAKE_BUILD_TYPE)
    MESSAGE(STATUS "No build type selected, default is Debug")
    SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()

IF(NOT DEFINED CMAKE_CURRENT_LIST_DIR)
    SET(CMAKE_CURRENT_LIST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF(NOT DEFINED CMAKE_CURRENT_LIST_DIR)

MESSAGE(STATUS "CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")
MESSAGE(STATUS "PROJECT_BINARY_DIR    : ${PROJECT_BINARY_DIR}")

SET(Source_Path ${CMAKE_CURRENT_LIST_DIR})
SET(Install_Path ${Source_Path}/lib)

MESSAGE(STATUS "Source_Path           : ${Source_Path}")
MESSAGE(STATUS "Install_Path          : ${Install_Path}")

SET(CMAKE_DEBUG_POSTFIX "_d")
MESSAGE(STATUS "CMAKE_DEBUG_POSTFIX   : ${CMAKE_DEBUG_POSTFIX}")

##########################################################################################
# lets SETup OpenMP flags
INCLUDE(${Source_Path}/cmake/include_openmp.cmake)

# let use pthread on unix
IF(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pthread")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -pthread")
ENDIF(UNIX)

########################################################################################
# lets SETup rlog
INCLUDE(${Source_Path}/cmake/rlog.cmake)
ADD_LIBRARY(rlog STATIC ${RLOG_HEADERS} ${RLOG_SOURCES})

##########################################################################################
# lets SETup Puppy library
INCLUDE(${Source_Path}/cmake/puppy.cmake)
ADD_LIBRARY(puppy STATIC ${Puppy_SOURCES} ${Puppy_HEADERS})

##########################################################################################
# lets SETup tinyxml library
INCLUDE(${Source_Path}/cmake/tinyxml.cmake)
ADD_LIBRARY(tinyxml STATIC ${TINYXML_SOURCES})

##########################################################################################
# lets SETup LTR as a static library project, called ltr
INCLUDE(${Source_Path}/cmake/ltr.cmake)
ADD_LIBRARY(ltr STATIC ${LTR_ALL})
TARGET_LINK_LIBRARIES(ltr puppy rlog)

# installing
INSTALL(TARGETS ltr puppy rlog ARCHIVE DESTINATION ${Install_Path})

##########################################################################################
# lets SETup LTR_client
INCLUDE(${Source_Path}/cmake/ltr_client.cmake)
ADD_EXECUTABLE(ltr_client ${LTR_CLIENT_SRC})
TARGET_LINK_LIBRARIES(ltr_client ltr tinyxml)

##########################################################################################
# lets SETup generator project, which learns scorers and serialize them
INCLUDE(${Source_Path}/cmake/serialization.cmake)
ADD_EXECUTABLE(serialization_generator ${Serialization_generator_SOURCES})
TARGET_LINK_LIBRARIES(serialization_generator ltr puppy rlog)
CreateEmptyTesterFile()

##########################################################################################
# lets SETup RUN_SERIALIZATION fake project, which is for
# making tester.cc file with scorers saved in code
ADD_CUSTOM_TARGET(RUN_SERIALIZATION
	COMMAND serialization_generator ${Source_Path}/ltr/serialization_test/tester/tester.cc)
ADD_DEPENDENCIES(RUN_SERIALIZATION serialization_generator)

##########################################################################################
# lets SETup Google Test
INCLUDE(${Source_Path}/cmake/gtest.cmake)
cxx_library(gtest "${cxx_strict}" ${gtest_SOURCE_DIR}/src/gtest-all.cc)

##########################################################################################
# lets SETup our unit-tests for LTR (executed by Google Test)
INCLUDE(${Source_Path}/cmake/utests.cmake)
ADD_EXECUTABLE(ltr_test ${LTR_TEST_ALL} ${Serialization_tester_SOURCES})
TARGET_LINK_LIBRARIES(ltr_test ltr gtest)

# lets copy the test data to the build directory
INCLUDE(${Source_Path}/cmake/copy_test_data.cmake)
