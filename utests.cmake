# lets list tests' binaries
# it reflect the LTR_lib structure

SET(LTR_DATA_TEST ${Source_Path}/ltr/data/data_set_unittest.cc)

SET(LTR_IO_UILITY_TEST ${Source_Path}/ltr/data/utility/io_utility_unittest.cc)

SET(LTR_FEATURE_CONVERTERS_TEST ${Source_Path}/ltr/feature_converters/feature_converters_unittest.cc)

SET(LTR_LEARNERS_TEST ${Source_Path}/ltr/learners/learners_unittest.cc)

SET(LTR_SCORERS_TEST ${Source_Path}/ltr/scorers/fake_scorer_unittest.cc
					 ${Source_Path}/ltr/scorers/one_feature_scorer_unittest.cc)

SET(LTR_INTERFACES_TEST ${Source_Path}/ltr/interfaces/reporter_unittest.cc ${Source_Path}/ltr/interfaces/aliaser_unittest.cc
						${Source_Path}/ltr/interfaces/parameterized_unittest.cc)

SET(LTR_PARAMETERS_CONTAINER_TEST ${Source_Path}/ltr/parameters_container/parameters_container_unittest.cc)


SOURCE_GROUP(interfaces FILES ${LTR_INTERFACES_TEST})
SOURCE_GROUP(parameters_container FILES ${LTR_PARAMETERS_CONTAINER_TEST})
SOURCE_GROUP(scorers FILES ${LTR_SCORERS_TEST})

# lets list all LTR_TEST's source binaries
SET(LTR_TEST ${LTR_DATA_TEST} ${LTR_LEARNERS_TEST} ${LTR_SCORERS_TEST} ${LTR_FEATURE_CONVERTERS_TEST} ${LTR_IO_UILITY_TEST}
			${LTR_INTERFACES_TEST} ${LTR_PARAMETERS_CONTAINER_TEST})
SET(LTR_TEST_H)
SET(LTR_TEST_ALL ${LTR_TEST} ${LTR_TEST_H} ${gtest_SOURCE_DIR}/src/gtest_main.cc)
