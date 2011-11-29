# set paths to ConfigManager' sources and binaries

# !!!!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!!!!!!!!
# CONFIG MANAGER IN LTR DUPLICATES CONFIG MANAGER FROM YABRACADABRA
# DON'T DEVELOP CONFIG MANAGER IN LTR
# REBASE CONFIG MANAGER FROM YABRACADABRA TO LTR, IF NESSESARY

# set source files
				
SET(TIMER_SOURCES ${Source_Path}/utility/timer.h ${Source_Path}/utility/timer.cc)
SET(LOGGER_SOURCES ${Source_Path}/utility/logger.h ${Source_Path}/utility/logger.cc)

INCLUDE_DIRECTORIES(${Source_Path}/utility)