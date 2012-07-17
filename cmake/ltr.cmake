# lets list all LTR's binaries
# lets list all LTR's source binaries

SET(LTR_DATA ${Source_Path}/ltr/data/data_set.h ${Source_Path}/ltr/data/data_set.cc
             ${Source_Path}/ltr/data/object_pair.h ${Source_Path}/ltr/data/object_pair.cc
             ${Source_Path}/ltr/data/object_list.h ${Source_Path}/ltr/data/object_list.cc
             ${Source_Path}/ltr/data/object.h ${Source_Path}/ltr/data/object.cc
             ${Source_Path}/ltr/data/feature_info.h ${Source_Path}/ltr/data/feature_info.cc
             ${Source_Path}/ltr/data/per_object_accessor.h)

SET(LTR_DECISION_TREE ${Source_Path}/ltr/learners/decision_tree/decision_tree_learner.h
                      ${Source_Path}/ltr/learners/decision_tree/decision_tree_learner.cc
                      ${Source_Path}/ltr/learners/decision_tree/id3_learner.h

                      ${Source_Path}/ltr/learners/decision_tree/conditions_learner.h
                      ${Source_Path}/ltr/learners/decision_tree/splitting_quality.h

                      ${Source_Path}/ltr/learners/decision_tree/id3_splitter.h
                      ${Source_Path}/ltr/learners/decision_tree/id3_splitter.cc
                      ${Source_Path}/ltr/learners/decision_tree/sqr_error_quality.h

                      ${Source_Path}/ltr/learners/decision_tree/decision_tree.h
                      ${Source_Path}/ltr/learners/decision_tree/decision_tree.cc
                      ${Source_Path}/ltr/learners/decision_tree/classification_result.h
                      ${Source_Path}/ltr/learners/decision_tree/classification_result.cc

                      ${Source_Path}/ltr/learners/decision_tree/leaf_vertex.h
                      ${Source_Path}/ltr/learners/decision_tree/decision_vertex.h
                      ${Source_Path}/ltr/learners/decision_tree/regression_vertex.h

                      ${Source_Path}/ltr/learners/decision_tree/condition.h
                      ${Source_Path}/ltr/learners/decision_tree/condition.cc
                      ${Source_Path}/ltr/learners/decision_tree/compare_condition.h
                      ${Source_Path}/ltr/learners/decision_tree/compare_condition.cc)

SET(LTR_DECISION_TREE_UTILITY ${Source_Path}/ltr/learners/decision_tree/utility/utility.h
                              ${Source_Path}/ltr/learners/decision_tree/utility/utility.cc)

SET(LTR_DATA_UTILS ${Source_Path}/ltr/data/utility/object_utility.h
                   ${Source_Path}/ltr/data/utility/data_set_converters.h
                   ${Source_Path}/ltr/data/utility/data_set_converters.cc
                   ${Source_Path}/ltr/data/utility/data_set_statistics.h
                   ${Source_Path}/ltr/data/utility/data_set_statistics.cc)

SET(LTR_IO_UTILS ${Source_Path}/ltr/data/utility/io_utility.h
                 ${Source_Path}/ltr/data/utility/io_utility.cc
                 ${Source_Path}/ltr/data/utility/parsers/parser.h
                 ${Source_Path}/ltr/data/utility/parsers/parser.cc
                 ${Source_Path}/ltr/data/utility/parsers/parse_yandex.h
                 ${Source_Path}/ltr/data/utility/parsers/parse_yandex.cc
                 ${Source_Path}/ltr/data/utility/parsers/parse_svm.h
                 ${Source_Path}/ltr/data/utility/parsers/parse_svm.cc
                 ${Source_Path}/ltr/data/utility/parsers/parse_arff.h
                 ${Source_Path}/ltr/data/utility/parsers/parse_arff.cc)

SET(LTR_UTILS ${Source_Path}/ltr/utility/random_number_generator.h
              ${Source_Path}/ltr/utility/random_number_generator.cc
              ${Source_Path}/ltr/utility/numerical.h
              ${Source_Path}/ltr/utility/numerical.cc
              ${Source_Path}/ltr/utility/murmur_hash.h
              ${Source_Path}/ltr/utility/murmur_hash.cc
              ${Source_Path}/ltr/utility/indices.h
              ${Source_Path}/ltr/utility/indices.cc)

SET(LTR_INTERFACES ${Source_Path}/ltr/interfaces/reporter.h
                   ${Source_Path}/ltr/interfaces/aliaser.h
                   ${Source_Path}/ltr/interfaces/parameterized.h ${Source_Path}/ltr/interfaces/parameterized.cc
                   ${Source_Path}/ltr/interfaces/functor.h
                   ${Source_Path}/ltr/interfaces/serializable.h ${Source_Path}/ltr/interfaces/serializable.cc
                   ${Source_Path}/ltr/interfaces/serializable_functor.h
                   ${Source_Path}/ltr/interfaces/printable.h ${Source_Path}/ltr/interfaces/printable.cc)

SET(LTR_SCORERS ${Source_Path}/ltr/scorers/scorer.h ${Source_Path}/ltr/scorers/scorer.cc
                ${Source_Path}/ltr/scorers/fake_scorer.h ${Source_Path}/ltr/scorers/fake_scorer.cc
                ${Source_Path}/ltr/scorers/one_feature_scorer.h ${Source_Path}/ltr/scorers/one_feature_scorer.cc
                ${Source_Path}/ltr/scorers/gp_scorer.h
                ${Source_Path}/ltr/scorers/decision_tree_scorer.h ${Source_Path}/ltr/scorers/decision_tree_scorer.cc
                ${Source_Path}/ltr/scorers/linear_scorer.h ${Source_Path}/ltr/scorers/linear_scorer.cc)

SET(LTR_COMPOSITION_SCORERS
                ${Source_Path}/ltr/scorers/composition_scorers/composition_scorer.h
                ${Source_Path}/ltr/scorers/composition_scorers/composition_scorer.cc
                ${Source_Path}/ltr/scorers/composition_scorers/soft_composition_scorer.h
                ${Source_Path}/ltr/scorers/composition_scorers/linear_composition_scorer.h
                ${Source_Path}/ltr/scorers/composition_scorers/linear_composition_scorer.cc
                ${Source_Path}/ltr/scorers/composition_scorers/order_statistic_composition_scorer.h
                ${Source_Path}/ltr/scorers/composition_scorers/order_statistic_composition_scorer.cc
                ${Source_Path}/ltr/scorers/composition_scorers/median_composition_scorer.h
                ${Source_Path}/ltr/scorers/composition_scorers/median_composition_scorer.cc
                ${Source_Path}/ltr/scorers/composition_scorers/max_weight_composition_scorer.h
                ${Source_Path}/ltr/scorers/composition_scorers/max_weight_composition_scorer.cc)

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
                 ${Source_Path}/ltr/measures/gmrr.h ${Source_Path}/ltr/measures/gmrr.cc
                 ${Source_Path}/ltr/measures/normalized_measure.h)

SET(LTR_MEASURES_UTILS ${Source_Path}/ltr/measures/utils/measure_utility.h
                       ${Source_Path}/ltr/measures/utils/measure_utility.cc)

SET(LTR_LEARNERS ${Source_Path}/ltr/learners/learner.h
                 ${Source_Path}/ltr/learners/best_feature_learner/best_feature_learner.h
                 ${Source_Path}/ltr/learners/linear_learner/linear_learner.h
                 ${Source_Path}/ltr/learners/linear_learner/linear_learner.cc)

SET(LTR_GP_LEARNER ${Source_Path}/ltr/learners/gp_learner/gp_learner.h
                   ${Source_Path}/ltr/learners/gp_learner/gp_learner.cc

                   ${Source_Path}/ltr/learners/gp_learner/gp_primitives.h
                   ${Source_Path}/ltr/learners/gp_learner/gp_functions.h
                   ${Source_Path}/ltr/learners/gp_learner/gp_functions.cc)

SET(LTR_GP_LEARNER_STRATEGIES ${Source_Path}/ltr/learners/gp_learner/strategies/default_selection_strategy.h
                              ${Source_Path}/ltr/learners/gp_learner/strategies/multiply_top_trees_strategy.h
                              ${Source_Path}/ltr/learners/gp_learner/strategies/default_crossover_strategy.h
                              ${Source_Path}/ltr/learners/gp_learner/strategies/all_pairs_crossover_strategy.h
                              ${Source_Path}/ltr/learners/gp_learner/strategies/default_mutation_standart_strategy.h
                              ${Source_Path}/ltr/learners/gp_learner/strategies/all_trees_mutation_standart_strategy.h
                              ${Source_Path}/ltr/learners/gp_learner/strategies/default_mutation_swap_strategy.h
                              ${Source_Path}/ltr/learners/gp_learner/strategies/all_trees_mutation_swap_strategy.h
                              ${Source_Path}/ltr/learners/gp_learner/strategies/population_handler.h)

SET(LTR_FEATURE_CONVERTERS ${Source_Path}/ltr/feature_converters/feature_converter.h
                           ${Source_Path}/ltr/feature_converters/feature_sampler.h
                           ${Source_Path}/ltr/feature_converters/feature_sampler.cc
                           ${Source_Path}/ltr/feature_converters/per_feature_linear_converter.h
                           ${Source_Path}/ltr/feature_converters/per_feature_linear_converter.cc
                           ${Source_Path}/ltr/feature_converters/feature_converter_learner.h
                           ${Source_Path}/ltr/feature_converters/feature_normalizer_learner.h
                           ${Source_Path}/ltr/feature_converters/feature_sampler_learner.h
                           ${Source_Path}/ltr/feature_converters/feature_random_sampler_learner.h
                           ${Source_Path}/ltr/feature_converters/fake_feature_converter.h
                           ${Source_Path}/ltr/feature_converters/fake_feature_converter.cc
                           ${Source_Path}/ltr/feature_converters/fake_feature_converter_learner.h
                           ${Source_Path}/ltr/feature_converters/nan_to_zero_converter.h
                           ${Source_Path}/ltr/feature_converters/nan_to_zero_converter.cc
                           ${Source_Path}/ltr/feature_converters/nominal_to_bool_converter.h
                           ${Source_Path}/ltr/feature_converters/nominal_to_bool_converter.cc
                           ${Source_Path}/ltr/feature_converters/remove_nominal_converter.h
                           ${Source_Path}/ltr/feature_converters/remove_nominal_converter.cc)

SET(LTR_DATA_PREPROCESSORS ${Source_Path}/ltr/data_preprocessors/data_preprocessor.h
                           ${Source_Path}/ltr/data_preprocessors/data_sampler.h
                           ${Source_Path}/ltr/data_preprocessors/data_random_sampler.h
                           ${Source_Path}/ltr/data_preprocessors/fake_data_preprocessor.h)

SET(LTR_CROSSVALIDATION ${Source_Path}/ltr/crossvalidation/splitter.h
                        ${Source_Path}/ltr/crossvalidation/leave_one_out_splitter.h
                        ${Source_Path}/ltr/crossvalidation/validation_result.h
                        ${Source_Path}/ltr/crossvalidation/validation_result.cc
                        ${Source_Path}/ltr/crossvalidation/crossvalidation.h
                        ${Source_Path}/ltr/crossvalidation/k_fold_simple_splitter.h
                        ${Source_Path}/ltr/crossvalidation/tk_fold_simple_splitter.h
                        ${Source_Path}/ltr/crossvalidation/crossvalidation_result.h
                        ${Source_Path}/ltr/crossvalidation/crossvalidation_result.cc)

SET(LTR_LINEAR_COMPOSITION ${Source_Path}/ltr/learners/linear_composition/linear_composition_learner.h
                           ${Source_Path}/ltr/learners/linear_composition/data_set_weights_updater.h
                           ${Source_Path}/ltr/learners/linear_composition/linear_composition_scorer_weights_updater.h
                           ${Source_Path}/ltr/learners/linear_composition/ada_rank_data_set_weights_updater.h
                           ${Source_Path}/ltr/learners/linear_composition/ada_rank_lc_scorer_weights_updater.h
                           ${Source_Path}/ltr/learners/linear_composition/average_lc_scorer_weights_updater.h)


# lets divide binaries in groups, for comfort navigation
SOURCE_GROUP(data FILES ${LTR_DATA})
SOURCE_GROUP(utils FILES ${LTR_UTILS})
SOURCE_GROUP(learners FILES ${LTR_LEARNERS})
SOURCE_GROUP(learners\\decision_tree FILES ${LTR_DECISION_TREE})
SOURCE_GROUP(learners\\decision_tree\\utils FILES ${LTR_DECISION_TREE_UTILITY})
SOURCE_GROUP(learners\\linear_composition FILES ${LTR_LINEAR_COMPOSITION})
SOURCE_GROUP(learners\\gp FILES ${LTR_GP_LEARNER})
SOURCE_GROUP(learners\\gp\\strategies FILES ${LTR_GP_LEARNER_STRATEGIES})
SOURCE_GROUP(data\\utils FILES ${LTR_DATA_UTILS})
SOURCE_GROUP(data\\ioutils FILES ${LTR_IO_UTILS})
SOURCE_GROUP(interfaces FILES ${LTR_INTERFACES})
SOURCE_GROUP(feature_converters FILES ${LTR_FEATURE_CONVERTERS})
SOURCE_GROUP(feature_converters\\utils FILES ${LTR_FEATURE_CONVERTERS_UTILITY})
SOURCE_GROUP(data_preprocessor FILES ${LTR_DATA_PREPROCESSORS})
SOURCE_GROUP(scorers FILES ${LTR_SCORERS})
SOURCE_GROUP(scorers\\composition_scorers FILES ${LTR_COMPOSITION_SCORERS})
SOURCE_GROUP(scorers\\utils FILES ${LTR_SCORERS_UTILS})
SOURCE_GROUP(parameters_container FILES ${LTR_PARAMETERS_CONTAINER})
SOURCE_GROUP(measures FILES ${LTR_MEASURES})
SOURCE_GROUP(measures\\utils FILES ${LTR_MEASURES_UTILS})
SOURCE_GROUP(crossvalidation FILES ${LTR_CROSSVALIDATION})


SET(LTR_ALL ${LTR_DATA} ${LTR_DATA_UTILS} ${LTR_UTILS} ${LTR_INTERFACES} ${LTR_IO_UTILS} ${LTR_SCORERS}
            ${LTR_LEARNERS} ${LTR_PARAMETERS_CONTAINER} ${LTR_SCORERS_UTILS} ${LTR_GP_LEARNER} ${LTR_GP_LEARNER_STRATEGIES}
            ${LTR_FEATURE_CONVERTERS} ${LTR_FEATURE_CONVERTERS_UTILITY} ${LTR_MEASURES} ${LTR_MEASURES_UTILS}
            ${LTR_CROSSVALIDATION} ${LTR_DECISION_TREE} ${LTR_COMPOSITION_SCORERS}
            ${LTR_DATA_PREPROCESSORS} ${LTR_LINEAR_COMPOSITION} ${LTR_DECISION_TREE_UTILITY})

INCLUDE_DIRECTORIES(${Source_Path})
