# lets list all LTR's binaries


# lets list all LTR's source binaries
SET(LTR_DATA ${Source_Path}/ltr/data/data_set.h
             ${Source_Path}/ltr/data/object_pair.h ${Source_Path}/ltr/data/object_pair.cc
             ${Source_Path}/ltr/data/object_list.h ${Source_Path}/ltr/data/object_list.cc
             ${Source_Path}/ltr/data/object.h ${Source_Path}/ltr/data/object.cc
             ${Source_Path}/ltr/data/features_info.h ${Source_Path}/ltr/data/features_info.cc
             )

SET(LTR_DATA_UTILS ${Source_Path}/ltr/data/utility/object_utility.h)

SET(LTR_IO_UTILS ${Source_Path}/ltr/data/utility/io_utility.h
				 ${Source_Path}/ltr/data/utility/parsers/object_parser.h
				 ${Source_Path}/ltr/data/utility/parsers/object_parser.cc
				 ${Source_Path}/ltr/data/utility/parsers/parse_yandex.h
				 ${Source_Path}/ltr/data/utility/parsers/parse_yandex.cc
				 ${Source_Path}/ltr/data/utility/parsers/parse_svm.h
				 ${Source_Path}/ltr/data/utility/parsers/parse_svm.cc
				 )

SET(LTR_UTILS ${Source_Path}/ltr/utility/numerical.h ${Source_Path}/ltr/utility/numerical.cc)

SET(LTR_CORE ${Source_Path}/ltr/interfaces.h
			 ${Source_Path}/ltr/interfaces.cc)

SET(LTR_SCORERS ${Source_Path}/ltr/scorers/scorer.h
			${Source_Path}/ltr/scorers/fake_scorer.h ${Source_Path}/ltr/scorers/fake_scorer.cc
			${Source_Path}/ltr/scorers/one_feature_scorer.h ${Source_Path}/ltr/scorers/one_feature_scorer.cc
			${Source_Path}/ltr/scorers/linear_composition_scorer.h ${Source_Path}/ltr/scorers/linear_composition_scorer.cc)
			
SET (LTR_SCORERS_UTILS ${Source_Path}/ltr/scorers/utility/scorer_utility.h ${Source_Path}/ltr/scorers/utility/scorer_utility.cc)

SET(LTR_PARAMETERSCONTAINER ${Source_Path}/ltr/parameters_container/parameters_container.h
                            ${Source_Path}/ltr/parameters_container/parameters_container.cc)
							
SET(LTR_MEASURES ${Source_Path}/ltr/measures/measure.h)

SET(LTR_LEARNERS ${Source_Path}/ltr/learners/learner.h)

SET(LTR_FEATURE_CONVERTERS ${Source_Path}/ltr/feature_converters/feature_converter.h
                           ${Source_Path}/ltr/feature_converters/feature_subset_chooser.h
                           ${Source_Path}/ltr/feature_converters/feature_subset_chooser.cc)

# lets divide binaries in groups, for comfort navigation
SOURCE_GROUP(data FILES ${LTR_DATA})
SOURCE_GROUP(utils FILES ${LTR_UTILS})
SOURCE_GROUP(data\\utils FILES ${LTR_DATA_UTILS})
SOURCE_GROUP(data\\ioutils FILES ${LTR_IO_UTILS})
SOURCE_GROUP(core FILES ${LTR_CORE})
SOURCE_GROUP(feature_converters FILES ${LTR_FEATURE_CONVERTERS})
SOURCE_GROUP(scorers FILES ${LTR_SCORERS})
SOURCE_GROUP(scorers\\utils FILES ${LTR_SCORERS_UTILS})
SOURCE_GROUP(parameters_container FILES ${LTR_PARAMETERSCONTAINER})
SOURCE_GROUP(measures FILES ${LTR_MEASURES})
SOURCE_GROUP(learners FILES ${LTR_LEARNERS})

SET(LTR_ALL ${LTR_DATA} ${LTR_DATA_UTILS} ${LTR_UTILS} ${LTR_CORE} ${LTR_IO_UTILS} ${LTR_SCORERS}
			${LTR_LEARNERS} ${LTR_PARAMETERSCONTAINER} ${LTR_SCORERS_UTILS} ${LTR_FEATURE_CONVERTERS})
