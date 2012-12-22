# lets list tests' binaries
# it reflect the LTR_lib structure

SET(DENSITY_ESTIMATORS_TEST ${Source_Path}/ltr/density_estimators/density_estimators_learners_unittests.cc
                            ${Source_Path}/ltr/density_estimators/density_estimators_scorers_unittests.cc)

SET(NEIGHBOR_WEIGHTER_TEST ${Source_Path}/ltr/data/neighbor_weighter_unittest.cc)

SET(TOSTRING_TEST ${Source_Path}/ltr/data/tostring_unittest.cc)

SET(INDICES_TEST ${Source_Path}/ltr/data/indices_unittest.cc)

SET(NUMERICAL_TEST ${Source_Path}/ltr/data/numerical_unittest.cc)

SET(SAVE_PREDICTION_TEST ${Source_Path}/ltr/data/save_predictions_unittest.cc)

SET(LTR_TWO_CLASS_CONVERTERS_TEST ${Source_Path}/ltr/utility/two_class_actual_label_converter_unittest.cc
                                  ${Source_Path}/ltr/utility/two_class_predicted_label_converter_unittest.cc)

SET(LTR_PREDICTIONS_AGGREGATORS_TEST ${Source_Path}/ltr/predictions_aggregators/predictions_aggregators_unittest.cc)

SET(LIST_TO_ALL_PAIRS_CONVERTER_TEST ${Source_Path}/ltr/data/list_to_all_pairs_converter_unittest.cc)

SET(LTR_OBJECT_TEST ${Source_Path}/ltr/data/object_unittest.cc)

SET(LTR_FEATURE_INFO_TEST ${Source_Path}/ltr/data/feature_info_unittest.cc)

SET(LTR_OBJECT_LIST_TEST ${Source_Path}/ltr/data/object_list_unittest.cc)

SET(LTR_DATA_TEST ${Source_Path}/ltr/data/data_set_unittest.cc
                  ${Source_Path}/ltr/data/serialization_unittest.cc)

SET(LTR_IO_UILITY_TEST ${Source_Path}/ltr/data/utility/io_utility_unittest.cc)

SET(LTR_DECISION_TREE_TEST ${Source_Path}/ltr/learners/decision_tree/decision_tree_unittest.cc)

SET(LTR_FEATURE_CONVERTERS_TEST ${Source_Path}/ltr/feature_converters/feature_converters_unittest.cc
                                ${Source_Path}/ltr/feature_converters/feature_converters_learner_unittest.cc)

SET(LTR_DATA_PREPROCESSORS_TEST ${Source_Path}/ltr/data_preprocessors/data_preprocessors_unittest.cc)

SET(LTR_LEARNERS_TEST ${Source_Path}/ltr/learners/learners_unittest.cc
                      ${Source_Path}/ltr/learners/linear_learner/linear_learner_unittest.cc
                      ${Source_Path}/ltr/learners/gp_learner/gp_learner_unittest.cc
                      ${Source_Path}/ltr/learners/nearest_neighbor_learner/nearest_neighbor_learner_unittest.cc
                      ${Source_Path}/ltr/learners/bayesian_learner/bayesian_learner_unittest.cc)

SET(LTR_MEASURES_TEST ${Source_Path}/ltr/measures/measure_unittest.cc
                      ${Source_Path}/ltr/measures/listwise_measures_unittest.cc
                      ${Source_Path}/ltr/measures/pointwise_measures_unittest.cc
                      ${Source_Path}/ltr/measures/pairwise_measures_unittest.cc)

SET(LTR_SCORERS_TEST ${Source_Path}/ltr/scorers/fake_scorer_unittest.cc
                     ${Source_Path}/ltr/scorers/one_feature_scorer_unittest.cc
                     ${Source_Path}/ltr/scorers/linear_scorer_unittest.cc
                     ${Source_Path}/ltr/scorers/linear_composition_scorer_unittest.cc
                     ${Source_Path}/ltr/scorers/composition_scorers/composition_scorer_unittest.cc
                     ${Source_Path}/ltr/scorers/gp_scorer_unittest.cc
                     ${Source_Path}/ltr/scorers/nearest_neighbor_scorer_unittest.cc
                     ${Source_Path}/ltr/scorers/bayesian_scorer_unittest.cc)

SET(LTR_INTERFACES_TEST ${Source_Path}/ltr/interfaces/reporter_unittest.cc
                        ${Source_Path}/ltr/interfaces/aliaser_unittest.cc
                        ${Source_Path}/ltr/interfaces/parameterized_unittest.cc
                        ${Source_Path}/ltr/interfaces/vectorized_unittest.cc)

SET(LTR_PARAMETERS_CONTAINER_TEST ${Source_Path}/ltr/parameters_container/parameters_container_unittest.cc)

SET(LTR_CROSSVALIDATION_TEST ${Source_Path}/ltr/crossvalidation/splitter_unittest.h
                        ${Source_Path}/ltr/crossvalidation/k_fold_simple_splitter_unittest.cc
                        ${Source_Path}/ltr/crossvalidation/leave_one_out_splitter_unittest.cc
                        ${Source_Path}/ltr/crossvalidation/tk_fold_simple_splitter_unittest.cc
                        ${Source_Path}/ltr/crossvalidation/crossvalidator_unittest.cc)

SET(LTR_UTILITY_TEST ${Source_Path}/ltr/utility/multitable_unittest.cc
                     ${Source_Path}/ltr/utility/macros_unittest.cc
                     ${Source_Path}/ltr/utility/boost/path_unittest.cc
                     ${Source_Path}/ltr/utility/boost/lexical_cast_unittest.cc
                     ${Source_Path}/ltr/utility/boost/multi_array_unittest.cc
                     ${Source_Path}/ltr/utility/boost/shared_ptr_unittest.cc
                     ${Source_Path}/ltr/utility/boost/any_unittest.cc
                     ${Source_Path}/ltr/utility/boost/string_utils_unittest.cc
                     ${Source_Path}/ltr/utility/boost/triple_unittest.cc)

SET(LTR_COMPOSITION_TEST ${Source_Path}/ltr/learners/composition_learner/linear_composition_unittest.cc
                     ${Source_Path}/ltr/learners/composition_learner/composition_learner_unittest.cc)
SET(LTR_METRICS_TEST ${Source_Path}/ltr/metrics/metrics_unittest.cc)

SET(LTR_OPTIMIZATION_TEST ${Source_Path}/ltr/optimization/sets/sets_unittest.cc
                          ${Source_Path}/ltr/optimization/stop_criteria/and_criterion_unittest.cc
                          ${Source_Path}/ltr/optimization/stop_criteria/or_criterion_unittest.cc
                          ${Source_Path}/ltr/optimization/functions/linear_function_unittest.cc
                          ${Source_Path}/ltr/optimization/functions/quadratic_function_unittest.cc)

SOURCE_GROUP(density_estimators FILES ${DENSITY_ESTIMATORS_TEST})
SOURCE_GROUP(aggregators FILES ${LTR_AGGREGATORS_TEST})
SOURCE_GROUP(interfaces FILES ${LTR_INTERFACES_TEST})
SOURCE_GROUP(parameters_container FILES ${LTR_PARAMETERS_CONTAINER_TEST})
SOURCE_GROUP(scorers FILES ${LTR_SCORERS_TEST})
SOURCE_GROUP(measures FILES ${LTR_MEASURES_TEST})
SOURCE_GROUP(crossvalidation FILES ${LTR_CROSSVALIDATION_TEST})
SOURCE_GROUP(metrics FILES ${LTR_METRICS_TEST})
SOURCE_GROUP(data_preprocessors FILES ${LTR_DATA_PREPROCESSORS_TEST})
SOURCE_GROUP(linear_composition FILES ${LTR_LINEAR_COMPOSITION_TEST})
SOURCE_GROUP(feature_converters FILES ${LTR_FEATURE_CONVERTERS_TEST})
SOURCE_GROUP(learners FILES ${LTR_LEARNERS_TEST})
SOURCE_GROUP(learners\\composition_learner FILES ${LTR_COMPOSITION_TEST})
SOURCE_GROUP(utility FILES ${LTR_UTILITY_TEST})
SOURCE_GROUP(optimization FILES ${LTR_OPTIMIZATION_TEST})

# lets list all LTR_TEST's source binaries
SET(LTR_TEST ${LTR_PREDICTIONS_AGGREGATORS_TEST} ${LTR_TWO_CLASS_CONVERTERS_TEST} ${NEIGHBOR_WEIGHTER_TEST} ${TOSTRING_TEST} ${INDICES_TEST} ${NUMERICAL_TEST} ${SAVE_PREDICTION_TEST} ${LIST_TO_ALL_PAIRS_CONVERTER_TEST} ${LTR_OBJECT_TEST}
             ${LTR_OBJECT_LIST_TEST} ${LTR_FEATURE_INFO_TEST} ${LTR_DATA_TEST} ${LTR_LEARNERS_TEST} ${LTR_SCORERS_TEST} ${LTR_FEATURE_CONVERTERS_TEST}
             ${LTR_IO_UILITY_TEST} ${LTR_INTERFACES_TEST} ${LTR_PARAMETERS_CONTAINER_TEST} ${LTR_CROSSVALIDATION_TEST} ${LTR_DECISION_TREE_TEST}
             ${LTR_DATA_PREPROCESSORS_TEST} ${LTR_MEASURES_TEST} ${LTR_COMPOSITION_TEST} ${LTR_METRICS_TEST} ${DENSITY_ESTIMATORS_TEST} ${LTR_UTILITY_TEST} ${LTR_OPTIMIZATION_TEST})

SET(LTR_TEST_H)
SET(LTR_TEST_ALL ${LTR_TEST} ${LTR_TEST_H} ${gtest_SOURCE_DIR}/src/gtest_main.cc)
