# lets list sample's binaries

set (INITERS_SRC ${Source_Path}/ltr_client/learners_initer.h ${Source_Path}/ltr_client/learners_initer.cc
        		${Source_Path}/ltr_client/measures_initer.h ${Source_Path}/ltr_client/measures_initer.cc
        		${Source_Path}/ltr_client/learners_initer.h ${Source_Path}/ltr_client/register_learners.cc
        		${Source_Path}/ltr_client/learners_initer.h ${Source_Path}/ltr_client/register_measures.cc
                )

set (INFOS_SRC ${Source_Path}/ltr_client/learners_info.h
               ${Source_Path}/ltr_client/datas_info.h
               ${Source_Path}/ltr_client/measures_info.h
               )

set(LAUNCH_PATH ${PROJECT_BINARY_DIR} CACHE STRING "")
set(SOURCE_PATH ${Source_Path} CACHE STRING "")

set (LTR_CLIENT_SRC ${INITERS_SRC} ${LOADER_SRC} ${INFOS_SRC} ${Source_Path}/ltr_client/ltr_client.cc ${Source_Path}/ltr_client/ltr_client.h ${Source_Path}/ltr_client/constants.h ${Source_Path}/ltr_client/visitors.h)

source_group(infos FILES ${INFOS_SRC})
source_group(initers FILES ${INITERS_SRC})

