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
     ${Source_Path}/ltr_client/utility/common_utility.h
     ${Source_Path}/ltr_client/utility/crossvalidation_launch_info.h
     ${Source_Path}/ltr_client/utility/crossvalidation_launch_info.cc
     ${Source_Path}/ltr_client/utility/data_info.h
     ${Source_Path}/ltr_client/utility/data_info.cc
     ${Source_Path}/ltr_client/utility/parameterized_info.h
     ${Source_Path}/ltr_client/utility/parameterized_info.cc
     ${Source_Path}/ltr_client/utility/tag_handlers.h
     ${Source_Path}/ltr_client/utility/tag_handlers.cc
     ${Source_Path}/ltr_client/utility/train_launch_info.h
     ${Source_Path}/ltr_client/utility/train_launch_info.cc
     ${Source_Path}/ltr_client/registration.h
)

source_group(initers FILES ${INITERS_SRC})
