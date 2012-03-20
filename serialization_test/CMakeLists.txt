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

##########################################################################################
# lets SETup ltr
SET(Serialization_Test_Path ${CMAKE_CURRENT_LIST_DIR})
SET(LTR_Source_Path ${Serialization_Test_Path}/..)
SET(LTR_Install_Path ${LTR_Source_Path}/lib)
FIND_LIBRARY(LTR_Library ltr HINTS ${LTR_Install_Path})

# lets SETup boost
FIND_LIBRARY(boost_filesystem_library boost_filesystem HINTS ${LTR_Install_Path})
FIND_LIBRARY(boost_system_library boost_system HINTS ${LTR_Install_Path})

MESSAGE("LTR_Library           : ${LTR_Library}")
MESSAGE("LTR_Source_Path       : ${LTR_Source_Path}")

INCLUDE_DIRECTORIES(${LTR_Source_Path})
INCLUDE_DIRECTORIES(${LTR_Source_Path}/contrib)
LINK_DIRECTORIES(${LTR_Install_Path})

##########################################################################################
# lets SETup generator project, which learns scorers and serialize them
SET(Generator_SOURCES ${Serialization_Test_Path}/generator.cc
                      ${Serialization_Test_Path}/config.h)
ADD_EXECUTABLE(generator ${Generator_SOURCES})
TARGET_LINK_LIBRARIES(generator ltr boost_filesystem boost_system)

# lets copy the test data to the build directory
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${LTR_Source_Path}/data/serialization_tests
    ${CMAKE_BINARY_DIR}/data)
