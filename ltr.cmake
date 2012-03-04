# lets list all LTR's binaries


# lets list all LTR's source binaries
SET(LTR_DATA ${Source_Path}/ltr/data/data_set.h ${Source_Path}/ltr/data/data_set.cc
             ${Source_Path}/ltr/data/object_pair.h ${Source_Path}/ltr/data/object_pair.cc
             ${Source_Path}/ltr/data/object_list.h ${Source_Path}/ltr/data/object_list.cc
             ${Source_Path}/ltr/data/object.h ${Source_Path}/ltr/data/object.cc
             ${Source_Path}/ltr/data/feature_info.h ${Source_Path}/ltr/data/feature_info.cc)

SET(LTR_DECISION_TREE ${Source_Path}/ltr/decision_tree/decision_tree.h
                      ${Source_Path}/ltr/decision_tree/decision_tree.cc
                      ${Source_Path}/ltr/decision_tree/classification_result.h
                      ${Source_Path}/ltr/decision_tree/classification_result.cc
                      
                      ${Source_Path}/ltr/decision_tree/leaf_vertex.h
                      ${Source_Path}/ltr/decision_tree/decision_vertex.h
                      ${Source_Path}/ltr/decision_tree/regression_vertex.h
                      
                      ${Source_Path}/ltr/decision_tree/condition.h
                      ${Source_Path}/ltr/decision_tree/condition.cc
                      ${Source_Path}/ltr/decision_tree/compare_condition.h
                      ${Source_Path}/ltr/decision_tree/compare_condition.cc)

SET(LTR_DECISION_TREE_UTILITY ${Source_Path}/ltr/decision_tree/utility/utility.h
                              ${Source_Path}/ltr/decision_tree/utility/utility.cc)

SET(LTR_DATA_UTILS ${Source_Path}/ltr/data/utility/object_utility.h
                   ${Source_Path}/ltr/data/utility/data_set_utility.h
                   ${Source_Path}/ltr/data/utility/data_set_utility.cc
                   ${Source_Path}/ltr/data/utility/data_set_converters.h
                   ${Source_Path}/ltr/data/utility/data_set_converters.cc
                   ${Source_Path}/ltr/data/utility/data_set_statistics.h
                   ${Source_Path}/ltr/data/utility/data_set_statistics.cc)

SET(LTR_IO_UTILS ${Source_Path}/ltr/data/utility/io_utility.h                
                 ${Source_Path}/ltr/data/utility/io_utility.cc
                 ${Source_Path}/ltr/data/utility/nominal_feature_handler.h
                 ${Source_Path}/ltr/data/utility/nominal_feature_handler.cc
                 ${Source_Path}/ltr/data/utility/parsers/object_parser.h
                 ${Source_Path}/ltr/data/utility/parsers/object_parser.cc
                 ${Source_Path}/ltr/data/utility/parsers/parse_yandex.h
                 ${Source_Path}/ltr/data/utility/parsers/parse_yandex.cc
                 ${Source_Path}/ltr/data/utility/parsers/parse_svm.h
                 ${Source_Path}/ltr/data/utility/parsers/parse_svm.cc
                 ${Source_Path}/ltr/data/utility/parsers/parse_arff.h
                 ${Source_Path}/ltr/data/utility/parsers/parse_arff.cc)

SET(LTR_UTILS ${Source_Path}/ltr/utility/numerical.h
              ${Source_Path}/ltr/utility/numerical.cc)

SET(LTR_INTERFACES ${Source_Path}/ltr/interfaces/reporter.h
                   ${Source_Path}/ltr/interfaces/aliaser.h
                   ${Source_Path}/ltr/interfaces/parameterized.h ${Source_Path}/ltr/interfaces/parameterized.cc
                   ${Source_Path}/ltr/interfaces/functor.h
                   ${Source_Path}/ltr/interfaces/serializable.h ${Source_Path}/ltr/interfaces/serializable.cc
                   ${Source_Path}/ltr/interfaces/serializable_functor.h)

SET(LTR_SCORERS ${Source_Path}/ltr/scorers/scorer.h ${Source_Path}/ltr/scorers/scorer.cc
                ${Source_Path}/ltr/scorers/fake_scorer.h ${Source_Path}/ltr/scorers/fake_scorer.cc
                ${Source_Path}/ltr/scorers/one_feature_scorer.h ${Source_Path}/ltr/scorers/one_feature_scorer.cc
                ${Source_Path}/ltr/scorers/linear_composition_scorer.h ${Source_Path}/ltr/scorers/linear_composition_scorer.cc
                ${Source_Path}/ltr/scorers/soft_scorer.h ${Source_Path}/ltr/scorers/soft_scorer.cc
                ${Source_Path}/ltr/scorers/gp_scorer.h
                ${Source_Path}/ltr/scorers/decision_tree_scorer.h ${Source_Path}/ltr/scorers/decision_tree_scorer.cc)
            
SET(LTR_SCORERS_UTILS ${Source_Path}/ltr/scorers/utility/scorer_utility.h
                      ${Source_Path}/ltr/scorers/utility/scorer_utility.cc)

SET(LTR_PARAMETERS_CONTAINER ${Source_Path}/ltr/parameters_container/parameters_container.h
                             ${Source_Path}/ltr/parameters_container/parameters_container.cc)
                            
SET(LTR_MEASURES ${Source_Path}/ltr/measures/measure.h
                 ${Source_Path}/ltr/measures/abs_error.h
                 ${Source_Path}/ltr/measures/squared_error.h
                 ${Source_Path}/ltr/measures/true_point.h
                 ${Source_Path}/ltr/measures/accuracy.h ${Source_Path}/ltr/measures/accuracy.cc
                 ${Source_Path}/ltr/measures/average_precision.h ${Source_Path}/ltr/measures/average_precision.cc
                 ${Source_Path}/ltr/measures/dcg.h ${Source_Path}/ltr/measures/dcg.cc
                 ${Source_Path}/ltr/measures/ndcg.h
                 ${Source_Path}/ltr/measures/reciprocal_rank.h ${Source_Path}/ltr/measures/reciprocal_rank.cc
                 ${Source_Path}/ltr/measures/pfound.h ${Source_Path}/ltr/measures/pfound.cc
                 ${Source_Path}/ltr/measures/gmrr.h ${Source_Path}/ltr/measures/gmrr.cc)

SET(LTR_MEASURES_UTILS ${Source_Path}/ltr/measures/utils/measure_utility.h ${Source_Path}/ltr/measures/utils/measure_utility.cc)

SET(LTR_LEARNERS ${Source_Path}/ltr/learners/learner.h
                 ${Source_Path}/ltr/learners/best_feature_learner.h
                 ${Source_Path}/ltr/learners/gp_learner.h
                 ${Source_Path}/ltr/learners/gp_learner.cc
                 ${Source_Path}/ltr/learners/gp_learner_determinant_strategy.h
                 ${Source_Path}/ltr/learners/gp_learner_determinant_strategy.cc
                 ${Source_Path}/ltr/learners/decision_tree_learner.h
                 ${Source_Path}/ltr/learners/decision_tree_learner.cc
                 ${Source_Path}/ltr/learners/id3_learner.h)

SET(LTR_LEARNERS_UTILS ${Source_Path}/ltr/learners/utility/gp_primitives.h
                       ${Source_Path}/ltr/learners/utility/gp_functions.h
                       ${Source_Path}/ltr/learners/utility/gp_functions.cc
                       
                       ${Source_Path}/ltr/learners/utility/id3_splitter.cc
                       ${Source_Path}/ltr/learners/utility/id3_splitter.h
                       ${Source_Path}/ltr/learners/utility/sqr_error_quality.h)


SET(LTR_FEATURE_CONVERTERS ${Source_Path}/ltr/feature_converters/feature_converter.h
                           ${Source_Path}/ltr/feature_converters/feature_subset_chooser.h
                           ${Source_Path}/ltr/feature_converters/feature_subset_chooser.cc
                           ${Source_Path}/ltr/feature_converters/per_feature_linear_converter.h
                           ${Source_Path}/ltr/feature_converters/per_feature_linear_converter.cc
                           ${Source_Path}/ltr/feature_converters/feature_converter_learner.h
                           ${Source_Path}/ltr/feature_converters/feature_normalizer_learner.h
                           ${Source_Path}/ltr/feature_converters/feature_subset_chooser_learner.h
                           ${Source_Path}/ltr/feature_converters/RSM_feature_converter_learner.h
                           ${Source_Path}/ltr/feature_converters/fake_feature_converter.h
                           ${Source_Path}/ltr/feature_converters/fake_feature_converter.cc
                           ${Source_Path}/ltr/feature_converters/fake_feature_converter_learner.h)

SET(LTR_FEATURE_CONVERTERS_UTILITY ${Source_Path}/ltr/feature_converters/utility/utility.h)
                           
SET(LTR_DATA_PREPROCESSORS ${Source_Path}/ltr/data_preprocessors/data_preprocessor.h
                           ${Source_Path}/ltr/data_preprocessors/simple_subset_preprocessor.h
                           ${Source_Path}/ltr/data_preprocessors/data_preprocessor_learner.h
                           ${Source_Path}/ltr/data_preprocessors/simple_subset_preprocessor_learner.h
                           ${Source_Path}/ltr/data_preprocessors/begging_preprocessor_learner.h
                           ${Source_Path}/ltr/data_preprocessors/fake_data_preprocessor.h
                           ${Source_Path}/ltr/data_preprocessors/fake_preprocessor_learner.h)
                           
SET(LTR_CROSSVALIDATION ${Source_Path}/ltr/crossvalidation/splitter.h
                        ${Source_Path}/ltr/crossvalidation/leave_one_out_splitter.h
                        ${Source_Path}/ltr/crossvalidation/validation_result.h
                        ${Source_Path}/ltr/crossvalidation/validation_result.cc
                        ${Source_Path}/ltr/crossvalidation/crossvalidation.h
                        ${Source_Path}/ltr/crossvalidation/k_fold_simple_splitter.h
                        ${Source_Path}/ltr/crossvalidation/tk_fold_simple_splitter.h)
                        
SET(LTR_LINEAR_COMPOSITION ${Source_Path}/ltr/linear_composition/linear_composition_learner.h
                           ${Source_Path}/ltr/linear_composition/data_set_weights_updater.h
                           ${Source_Path}/ltr/linear_composition/linear_composition_scorer_weights_updater.h
                           ${Source_Path}/ltr/linear_composition/ada_rank_data_set_weights_updater.h
                           ${Source_Path}/ltr/linear_composition/ada_rank_lc_scorer_weights_updater.h
                           ${Source_Path}/ltr/linear_composition/average_lc_scorer_weights_updater.h)
                            

# lets divide binaries in groups, for comfort navigation
SOURCE_GROUP(data FILES ${LTR_DATA})
SOURCE_GROUP(utils FILES ${LTR_UTILS})
SOURCE_GROUP(data\\utils FILES ${LTR_DATA_UTILS})
SOURCE_GROUP(data\\ioutils FILES ${LTR_IO_UTILS})
SOURCE_GROUP(interfaces FILES ${LTR_INTERFACES})
SOURCE_GROUP(feature_converters FILES ${LTR_FEATURE_CONVERTERS})
SOURCE_GROUP(feature_converters\\utils FILES ${LTR_FEATURE_CONVERTERS_UTILITY})
SOURCE_GROUP(data_preprocessor FILES ${LTR_DATA_PREPROCESSORS})
SOURCE_GROUP(scorers FILES ${LTR_SCORERS})
SOURCE_GROUP(scorers\\utils FILES ${LTR_SCORERS_UTILS})
SOURCE_GROUP(parameters_container FILES ${LTR_PARAMETERS_CONTAINER})
SOURCE_GROUP(measures FILES ${LTR_MEASURES})
SOURCE_GROUP(measures\\utils FILES ${LTR_MEASURES_UTILS})
SOURCE_GROUP(learners FILES ${LTR_LEARNERS})
SOURCE_GROUP(learners\\utils FILES ${LTR_LEARNERS_UTILS})
SOURCE_GROUP(decision_tree FILES ${LTR_DECISION_TREE})
SOURCE_GROUP(decision_tree\\utils FILES ${LTR_DECISION_TREE_UTILITY})
SOURCE_GROUP(crossvalidation FILES ${LTR_CROSSVALIDATION})
SOURCE_GROUP(linear_composition FILES ${LTR_LINEAR_COMPOSITION})

SET(LTR_ALL ${LTR_DATA} ${LTR_DATA_UTILS} ${LTR_UTILS} ${LTR_INTERFACES} ${LTR_IO_UTILS} ${LTR_SCORERS}
            ${LTR_LEARNERS} ${LTR_LEARNERS_UTILS} ${LTR_PARAMETERS_CONTAINER} ${LTR_SCORERS_UTILS}
            ${LTR_FEATURE_CONVERTERS} ${LTR_FEATURE_CONVERTERS_UTILITY} ${LTR_MEASURES} ${LTR_MEASURES_UTILS} ${LTR_CROSSVALIDATION} ${LTR_DECISION_TREE}
            ${LTR_DATA_PREPROCESSORS} ${LTR_LINEAR_COMPOSITION} ${LTR_DECISION_TREE_UTILITY})

