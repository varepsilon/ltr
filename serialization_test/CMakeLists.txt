##########################################################################################
# CMake build script for LTR scorer's serialization test
##########################################################################################

PROJECT(LTRSerializationTest CXX C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

IF(NOT DEFINED CMAKE_CURRENT_LIST_DIR)
    SET(CMAKE_CURRENT_LIST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF(NOT DEFINED CMAKE_CURRENT_LIST_DIR)

MESSAGE("CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")
MESSAGE("PROJECT_BINARY_DIR    : ${PROJECT_BINARY_DIR}")

# let use pthread on unix
IF (UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pthread")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -pthread")
ENDIF (UNIX)

##########################################################################################
# lets SETup ltr
SET(Serialization_Test_Path ${CMAKE_CURRENT_LIST_DIR})
SET(LTR_Source_Path ${Serialization_Test_Path}/..)
SET(LTR_Install_Path ${LTR_Source_Path}/lib)

INCLUDE_DIRECTORIES(${LTR_Source_Path})
INCLUDE_DIRECTORIES(${LTR_Source_Path}/contrib)
LINK_DIRECTORIES(${LTR_Install_Path})

FIND_LIBRARY(LTR_Library ltr HINTS ${LTR_Install_Path})

# lets SETup boost
FIND_LIBRARY(boost_filesystem_library boost_filesystem HINTS ${LTR_Install_Path})
FIND_LIBRARY(boost_system_library boost_system HINTS ${LTR_Install_Path})
FIND_LIBRARY(boost_date_time_library boost_date_time HINTS ${LTR_Install_Path})

# lets SETup Utilities
FIND_LIBRARY(util_timer_library util_timer HINTS ${LTR_Install_Path})
FIND_LIBRARY(util_logger_library util_logger HINTS ${LTR_Install_Path})
SET(UTILITY util_timer util_logger)

MESSAGE("util_timer_library          : ${util_timer_library}")

MESSAGE("LTR_Library           : ${LTR_Library}")
MESSAGE("LTR_Source_Path       : ${LTR_Source_Path}")

##########################################################################################
# lets SETup generator project, which learns scorers and serialize them
# SET(Generator_SOURCES ${Serialization_Test_Path}/generator/generator.cc
                      # ${Serialization_Test_Path}/generator/config.h
					  # ${Serialization_Test_Path}/generator/generator_utility.cc
					  # ${Serialization_Test_Path}/generator/generator_utility.h)
ADD_EXECUTABLE(generator ${Serialization_Test_Path}/generator/generator.cc)
TARGET_LINK_LIBRARIES(generator ltr boost_filesystem boost_system boost_date_time ${UTILITY})

# # lets copy the test data to the build directory
# EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_directory
    # ${LTR_Source_Path}/data/serialization_tests
    # ${CMAKE_BINARY_DIR}/data)

# # making tester.cc with scorer saved in code
# ADD_CUSTOM_COMMAND(
  # OUTPUT ${Serialization_Test_Path}/tester/tester.cc
  # DEPENDS generator
  # COMMAND generator ${Serialization_Test_Path}/tester/tester.cc)

# ##########################################################################################
# # lets SETup tester project, which tests scorers, serializated by generator
# SET(Tester_SOURCES ${Serialization_Test_Path}/tester/tester.cc
				   # ${Serialization_Test_Path}/tester/tester_utility.cc
				   # ${Serialization_Test_Path}/tester/tester_utility.h)
# ADD_EXECUTABLE(tester ${Tester_SOURCES})
# TARGET_LINK_LIBRARIES(tester ltr boost_filesystem boost_system)
