SET(LOGOG_SOURCES
	${Source_Path}/contrib/logog/src/api.cpp 
	${Source_Path}/contrib/logog/src/checkpoint.cpp
	${Source_Path}/contrib/logog/src/formatter.cpp
	${Source_Path}/contrib/logog/src/lobject.cpp
	${Source_Path}/contrib/logog/src/lstring.cpp
	${Source_Path}/contrib/logog/src/message.cpp
	${Source_Path}/contrib/logog/src/mutex.cpp
	${Source_Path}/contrib/logog/src/node.cpp
	${Source_Path}/contrib/logog/src/platform.cpp
	${Source_Path}/contrib/logog/src/socket.cpp
	${Source_Path}/contrib/logog/src/statics.cpp
	${Source_Path}/contrib/logog/src/target.cpp
	${Source_Path}/contrib/logog/src/timer.cpp
	${Source_Path}/contrib/logog/src/topic.cpp
	${Source_Path}/contrib/logog/src/unittest.cpp
)

SET(LOGOG_INCLUDE_DIR ${Source_Path}/contrib)
INCLUDE_DIRECTORIES(${LOGOG_INCLUDE_DIR})
