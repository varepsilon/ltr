SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_STATIC_RUNTIME OFF)
SET(Boost_ADDITIONAL_VERSIONS "1.46.0" "1.46" "1.47" "1.47.0" "1.48" "1.48.0")

FIND_PACKAGE(Boost)

IF (NOT Boost_FOUND)
    MESSAGE("Boost wasn't found, install it.")
    IF (WIN32)
		MESSAGE("")
        MESSAGE("Use BoostPro intaller (http://www.boostpro.com/download/) and set the BOOST_ROOT environment variable to installation path (My Computer->Advanced Settings).")
		MESSAGE("")
    ENDIF (WIN32)
ENDIF (NOT Boost_FOUND)

FIND_PACKAGE(Boost COMPONENTS filesystem system date_time random REQUIRED)

IF (NOT DEFINED Boost_LIBRARYDIR)
    IF (WIN32)
        SET(Boost_LIBRARYDIR "$ENV{BOOST_ROOT}/lib")
    ENDIF (WIN32)
ENDIF (NOT DEFINED Boost_LIBRARYDIR)

LINK_DIRECTORIES(${Boost_LIBRARYDIR})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
