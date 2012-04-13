PROJECT(LTRTestProject CXX C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(Test_Source_Path ${CMAKE_CURRENT_SOURCE_DIR})
SET(LTR_Source_Path ${Test_Source_Path}/..)

INCLUDE(${LTR_Source_Path}/cmake/setup_ltr.cmake)
SetUpLTR(${LTR_Source_Path})

INCLUDE_DIRECTORIES(${Test_Source_Path})
SET(Test_SOURCES ${Test_Source_Path}/file1.cpp
                 ${Test_Source_Path}/file2.cpp)

ADD_EXECUTABLE(test_executable ${Test_SOURCES})
TARGET_LINK_LIBRARIES(test_executable ${LTR_libs})
