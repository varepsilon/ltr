FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
ENDIF(OPENMP_FOUND)

CONFIGURE_FILE(${Source_Path}/ltr/utility/include_openmp.h.in ${Source_Path}/ltr/utility/include_openmp.h)