# lets list all LTR's binaries


# lets list all LTR's source binaries
SET(LTR_DATA ${Source_Path}/ltr/data/data_set.h
             ${Source_Path}/ltr/data/object_pair.h ${Source_Path}/ltr/data/object_pair.cc
             ${Source_Path}/ltr/data/object_list.h ${Source_Path}/ltr/data/object_list.cc
             ${Source_Path}/ltr/data/object.h ${Source_Path}/ltr/data/object.cc
             ${Source_Path}/ltr/data/features_info.h ${Source_Path}/ltr/data/features_info.cc
             )

SET(LTR_DATA_UTILS ${Source_Path}/ltr/data/utility/object_utility.h)

SET(LTR_UTILS ${Source_Path}/ltr/utility/numerical.h ${Source_Path}/ltr/utility/numerical.cc)

SET(LTR_CORE ${Source_Path}/ltr/interfaces.h)

SET(LTR_SCORERS ${Source_Path}/ltr/scorers/scorer.h)

SET(LTR_PARAMETERSCONTAINER ${Source_Path}/ltr/parameters_container/parameters_container.h
                            ${Source_Path}/ltr/parameters_container/parameters_container.cc)
							
SET(LTR_MEASURES ${Source_Path}/ltr/measures/measure.h)

SET(LTR_LEARNERS ${Source_Path}/ltr/learners/learner.h)


# lets divide binaries in groups, for comfort navigation
SOURCE_GROUP(data FILES ${LTR_DATA})
SOURCE_GROUP(utils FILES ${LTR_UTILS})
SOURCE_GROUP(data\\utils FILES ${LTR_DATA_UTILS})
SOURCE_GROUP(core FILES ${LTR_CORE})
SOURCE_GROUP(scorers FILES ${LTR_SCORERS})
SOURCE_GROUP(parameters_container FILES ${LTR_PARAMETERSCONTAINER})
SOURCE_GROUP(measures FILES ${LTR_MEASURES})
SOURCE_GROUP(learners FILES ${LTR_LEARNERS})



SET(LTR_ALL ${LTR_DATA} ${LTR_DATA_UTILS} ${LTR_UTILS} ${LTR_CORE} ${LTR_SCORERS} ${LTR_PARAMETERSCONTAINER} ${LTR_MEASURES} ${LTR_LEARNERS})