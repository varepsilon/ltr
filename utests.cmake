# lets list tests' binaries
# it reflect the LTR_lib structure

SET(LTR_DATA_TEST ${Source_Path}/ltr/data/data_set_unittest.cc)

SET(LTR_IO_UILITY_TEST ${Source_Path}/ltr/data/utility/io_utility_unittest.cc)

SET(LTR_FEATURE_CONVERTERS_TEST ${Source_Path}/ltr/feature_converters/feature_converters_unittest.cc)

SET(LTR_LEARNERS_TEST ${Source_Path}/ltr/learners/learners_unittest.cc)

SET(LTR_SCORERS_TEST ${Source_Path}/ltr/scorers/scorers_unittest.cc)

# lets list all LTR_TEST's source binaries
SET(LTR_TEST ${LTR_DATA_TEST} ${LTR_LEARNERS_TEST} ${LTR_SCORERS_TEST} ${LTR_FEATURE_CONVERTERS_TEST} ${LTR_IO_UILITY_TEST})
SET(LTR_TEST_H)
SET(LTR_TEST_ALL ${LTR_TEST} ${LTR_TEST_H} ${gtest_SOURCE_DIR}/src/gtest_main.cc)
