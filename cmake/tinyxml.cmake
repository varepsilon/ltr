set (TINYXML_SOURCES
	${Source_Path}/contrib/tinyxml/tinyxml.cpp
	${Source_Path}/contrib/tinyxml/tinystr.cpp
	${Source_Path}/contrib/tinyxml/tinyxmlerror.cpp
	${Source_Path}/contrib/tinyxml/tinyxmlparser.cpp
)

SET (TINYXML_INCLUDE_DIR ${Source_Path}/contrib)
INCLUDE_DIRECTORIES(${TINYXML_INCLUDE_DIR})