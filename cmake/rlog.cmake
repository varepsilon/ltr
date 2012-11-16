SET(HAVE_PRINTF_FP_PROTOTYPE "0")
SET(C99_VARIADAC_MACROS "0")
SET(PREC99_VARIADAC_MACROS "0")
SET(RLOG_COMPONENT "rlog")
CONFIGURE_FILE(${Source_Path}/contrib/rlog/common.h.in ${Source_Path}/contrib/rlog/common.h)
CONFIGURE_FILE(${Source_Path}/contrib/rlog/config.h.in ${Source_Path}/contrib/rlog/config.h)

SET(RLOG_HEADERS ${Source_Path}/contrib/rlog/common.h
                 ${Source_Path}/contrib/rlog/config.h
                 ${Source_Path}/contrib/rlog/Error.h
                 ${Source_Path}/contrib/rlog/Lock.h
				 ${Source_Path}/contrib/rlog/Mutex.h
				 ${Source_Path}/contrib/rlog/rlog.h
				 ${Source_Path}/contrib/rlog/RLogChannel.h
				 ${Source_Path}/contrib/rlog/rloginit.h
				 ${Source_Path}/contrib/rlog/rloglocation.h
				 ${Source_Path}/contrib/rlog/RLogNode.h
				 ${Source_Path}/contrib/rlog/RLogPublisher.h
				 ${Source_Path}/contrib/rlog/StdioNode.h
				 ${Source_Path}/contrib/rlog/rlog-c99.h
				 ${Source_Path}/contrib/rlog/rlog-novariadic.h
				 ${Source_Path}/contrib/rlog/rlog-prec99.h
				 ${Source_Path}/contrib/rlog/RLogTime.h
				 ${Source_Path}/contrib/rlog/rlog_default.h)

SET(RLOG_SOURCES ${Source_Path}/contrib/rlog/Error.cpp
                 ${Source_Path}/contrib/rlog/rlog.cpp
				 ${Source_Path}/contrib/rlog/RLogChannel.cpp
				 ${Source_Path}/contrib/rlog/rloginit.cpp
				 ${Source_Path}/contrib/rlog/rloglocation.cpp
				 ${Source_Path}/contrib/rlog/RLogNode.cpp
				 ${Source_Path}/contrib/rlog/RLogPublisher.cpp
				 ${Source_Path}/contrib/rlog/StdioNode.cpp
				 ${Source_Path}/contrib/rlog/RLogTime.cpp
				 ${Source_Path}/contrib/rlog/rlog_default.cpp)
