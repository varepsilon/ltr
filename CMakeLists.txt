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
# lets SETup boost. All possible libs are linked and all paths are INCLUDEd. Is it bad?
# DON'T FORGET TO CHANGE Boost'S PATHS!

FIND_PACKAGE(Boost)

IF (NOT Boost_FOUND)
    MESSAGE("Boost wasn't found, install it.")
    IF (WIN32)
        MESSAGE(
"Use BoostPro intaller (http://www.boostpro.com/download/)
and set the BOOST_ROOT environment variable to installation path 
(My Computer->Advanced Settings)."
                )
    ENDIF (WIN32)
ENDIF (NOT Boost_FOUND)

FIND_PACKAGE(Boost REQUIRED)

IF (NOT DEFINED Boost_LIBRARYDIR)
    IF (WIN32)
        SET(Boost_LIBRARYDIR "$ENV{BOOST_ROOT}/lib")
    ENDIF (WIN32)
ENDIF (NOT DEFINED Boost_LIBRARYDIR)

LINK_DIRECTORIES(${Boost_LIBRARYDIR})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

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
# lets SETup Puppy library
INCLUDE (${Source_Path}/puppy.cmake)
ADD_LIBRARY(puppy ${Puppy_HEADERS} ${Puppy_SOURCES})
TARGET_LINK_LIBRARIES (puppy ${Boost_LIBRARIES})

##########################################################################################
# lets SETup utility and tinyxml library
INCLUDE (${Source_Path}/utility.cmake)
ADD_LIBRARY(util_logger ${LOGGER_SOURCES})
ADD_LIBRARY(util_timer ${TIMER_SOURCES})
SET(UTILITY util_logger util_timer)

##########################################################################################
# lets SETup LTR as a static library project, called LTR_lib
INCLUDE (${Source_Path}/ltr.cmake NO_POLICY_SCOPE)
ADD_LIBRARY (ltr_lib STATIC ${LTR_ALL})
TARGET_LINK_LIBRARIES (ltr_lib ${Boost_LIBRARIES} puppy ${UTILITY})

INCLUDE_DIRECTORIES(${Source_Path}/ltr ${Source_Path})