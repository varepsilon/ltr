# lets list sample's binaries

set(LAUNCH_PATH ${PROJECT_BINARY_DIR} CACHE STRING "")
set(SOURCE_PATH ${Source_Path} CACHE STRING "")

set (LTR_CLIENT_SRC
     ${Source_Path}/ltr_client/factory.h
     ${Source_Path}/ltr_client/factory.cc
     ${Source_Path}/ltr_client/ltr_client.cc
     ${Source_Path}/ltr_client/ltr_client.h
     ${Source_Path}/ltr_client/configurator.cc
     ${Source_Path}/ltr_client/configurator.h
)

source_group(initers FILES ${INITERS_SRC})
