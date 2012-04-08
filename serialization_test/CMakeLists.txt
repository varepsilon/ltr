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

INCLUDE (${LTR_Source_Path}/cmake/setup_ltr.cmake)
SetUpLTR(${LTR_Source_Path})

##########################################################################################
# lets SETup generator project, which learns scorers and serialize them
SET(Generator_SOURCES ${Serialization_Test_Path}/generator/generator.cc
                      ${Serialization_Test_Path}/generator/config.h
					  ${Serialization_Test_Path}/generator/generator_utility.cc
					  ${Serialization_Test_Path}/generator/generator_utility.h)
ADD_EXECUTABLE(generator ${Generator_SOURCES})
TARGET_LINK_LIBRARIES(generator ${LTR_libs})

# lets copy the test data to the build directory
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${LTR_Source_Path}/data/serialization_tests
    ${CMAKE_BINARY_DIR}/data)

# making tester.cc with scorer saved in code
ADD_CUSTOM_COMMAND(
  OUTPUT ${Serialization_Test_Path}/tester/tester.cc
  DEPENDS generator
  COMMAND generator ${Serialization_Test_Path}/tester/tester.cc)

##########################################################################################
# lets SETup tester project, which tests scorers, serializated by generator
SET(Tester_SOURCES ${Serialization_Test_Path}/tester/tester.cc
				   ${Serialization_Test_Path}/tester/tester_utility.cc
				   ${Serialization_Test_Path}/tester/tester_utility.h)
ADD_EXECUTABLE(tester ${Tester_SOURCES})
TARGET_LINK_LIBRARIES(tester ${LTR_libs})
