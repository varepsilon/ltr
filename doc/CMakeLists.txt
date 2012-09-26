##########################################################################################
# CMake build script for LTR doxygen outputs
##########################################################################################

PROJECT(LTRDoxygen CXX C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

IF(NOT CMAKE_BUILD_TYPE)
    MESSAGE(STATUS "No build type selected, default is Debug")
    SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()

IF(NOT DEFINED CMAKE_CURRENT_LIST_DIR)
    SET(CMAKE_CURRENT_LIST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF(NOT DEFINED CMAKE_CURRENT_LIST_DIR)

MESSAGE("CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")
MESSAGE("PROJECT_BINARY_DIR    : ${PROJECT_BINARY_DIR}")

##########################################################################################
# lets SETup ltr
SET(Doc_Path ${CMAKE_CURRENT_LIST_DIR})
SET(LTR_Source_Path ${Doc_Path}/..)

INCLUDE (${LTR_Source_Path}/cmake/setup_ltr.cmake)
SetUpLTR(${LTR_Source_Path})

##########################################################################################
# lets add datafiles

EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_directory
	${LTR_Source_Path}/data
	${PROJECT_BINARY_DIR}/data)

##########################################################################################
# lets make executables
INCLUDE (${Doc_Path}/sources.cmake)

SET(index 0)
FOREACH(Cfile ${Sources})
	MATH(EXPR index "${index} + 1")
	SET(Cname "Doc${index}")

	ADD_EXECUTABLE(${Cname} ${Cfile}.cpp)
	TARGET_LINK_LIBRARIES(${Cname} ${LTR_libs})

	ADD_CUSTOM_COMMAND(
        TARGET ${Cname} POST_BUILD
        COMMAND ${Cname} > ${Cfile}.out)
ENDFOREACH()
