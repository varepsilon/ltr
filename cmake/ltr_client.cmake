# lets list sample's binaries

set(LAUNCH_PATH ${PROJECT_BINARY_DIR} CACHE STRING "")
set(SOURCE_PATH ${Source_Path} CACHE STRING "")

set (LTR_CLIENT_SRC
     ${Source_Path}/ltr_client/factory.h
     ${Source_Path}/ltr_client/factory.cc
     ${Source_Path}/ltr_client/dependency_resolving_factory.h
     ${Source_Path}/ltr_client/dependency_resolving_factory.cc
     ${Source_Path}/ltr_client/ltr_client.cc
     ${Source_Path}/ltr_client/ltr_client.h
     ${Source_Path}/ltr_client/configuration.h
     ${Source_Path}/ltr_client/config_parser.cc
     ${Source_Path}/ltr_client/config_parser_constants.cc
     ${Source_Path}/ltr_client/config_parser.h
     ${Source_Path}/ltr_client/utility/crossvalidation_launch_info.h
     ${Source_Path}/ltr_client/utility/data_info.h
     ${Source_Path}/ltr_client/utility/object_info.h
     ${Source_Path}/ltr_client/utility/object_info.cc
     ${Source_Path}/ltr_client/utility/train_launch_info.h
     ${Source_Path}/ltr_client/registration.h
)

set (LTR_CLIENT_MAIN
     ${Source_Path}/ltr_client/main.cc
)

source_group(initers FILES ${INITERS_SRC})
