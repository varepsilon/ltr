# lets list all LTR's binaries
# lets list all LTR's source binaries

SET(LTR_DENSITY_ESTIMATORS_SCORERS ${Source_Path}/ltr/density_estimators/base_probability_density_estimator.h
                                   ${Source_Path}/ltr/density_estimators/fisher_discriminant.h
                                   ${Source_Path}/ltr/density_estimators/fisher_discriminant.cc
                                   ${Source_Path}/ltr/density_estimators/non_linear_discriminant.h
                                   ${Source_Path}/ltr/density_estimators/non_linear_discriminant.cc)

SET(LTR_DENSITY_ESTIMATORS_LEARNERS ${Source_Path}/ltr/density_estimators/base_probability_density_learner.h
                                    ${Source_Path}/ltr/density_estimators/fisher_discriminant_density_learner.h
                                    ${Source_Path}/ltr/density_estimators/non_linear_discriminant_density_learner.h
                                    ${Source_Path}/ltr/density_estimators/normal_naive_bayes_density_learner.h
                                    ${Source_Path}/ltr/density_estimators/quadratic_discriminant_density_learner.h)

SET(LTR_DATA ${Source_Path}/ltr/data/data_set.h ${Source_Path}/ltr/data/data_set.cc
             ${Source_Path}/ltr/data/object_pair.h ${Source_Path}/ltr/data/object_pair.cc
             ${Source_Path}/ltr/data/object_list.h ${Source_Path}/ltr/data/object_list.cc
             ${Source_Path}/ltr/data/object.h ${Source_Path}/ltr/data/object.cc
             ${Source_Path}/ltr/data/feature_info.h ${Source_Path}/ltr/data/feature_info.cc
             ${Source_Path}/ltr/data/per_object_accessor.h)

SET(LTR_DECISION_TREE ${Source_Path}/ltr/learners/decision_tree/decision_tree.h
                      ${Source_Path}/ltr/learners/decision_tree/decision_tree.cc

                      ${Source_Path}/ltr/learners/decision_tree/decision_tree_learner.h
                      ${Source_Path}/ltr/learners/decision_tree/decision_tree_learner.cc
                      
                      ${Source_Path}/ltr/learners/decision_tree/base_splitter.h
                      ${Source_Path}/ltr/learners/decision_tree/base_splitter.cc

                      ${Source_Path}/ltr/learners/decision_tree/oblivious_tree_splitter.h
                      ${Source_Path}/ltr/learners/decision_tree/oblivious_tree_splitter.cc

                      ${Source_Path}/ltr/learners/decision_tree/id3_splitter.h
                      ${Source_Path}/ltr/learners/decision_tree/id3_splitter.cc)

SET(LTR_DECISION_TREE_CONDITION ${Source_Path}/ltr/learners/decision_tree/condition/condition.h
                                ${Source_Path}/ltr/learners/decision_tree/condition/threshold_condition.h
                                ${Source_Path}/ltr/learners/decision_tree/condition/threshold_condition.cc)                      

SET(LTR_DECISION_TREE_LEAF_GENERATOR ${Source_Path}/ltr/learners/decision_tree/leaf_generator/leaf_generator.h
                                     ${Source_Path}/ltr/learners/decision_tree/leaf_generator/most_common_label_leaf_generator.h
                                     ${Source_Path}/ltr/learners/decision_tree/leaf_generator/most_common_label_leaf_generator.cc)

SET(LTR_DECISION_TREE_SPLIT_CRITERIA ${Source_Path}/ltr/learners/decision_tree/split_criteria/stop_splitting_criteria.h
                                     ${Source_Path}/ltr/learners/decision_tree/split_criteria/same_label_stop_criteria.h
                                     ${Source_Path}/ltr/learners/decision_tree/split_criteria/same_label_stop_criteria.cc
                                     ${Source_Path}/ltr/learners/decision_tree/split_criteria/data_size_stop_criteria.h
                                     ${Source_Path}/ltr/learners/decision_tree/split_criteria/data_size_stop_criteria.cc)
                                
SET(LTR_DECISION_TREE_VERTEX ${Source_Path}/ltr/learners/decision_tree/vertex/vertex.h
                             ${Source_Path}/ltr/learners/decision_tree/vertex/vertex.cc
                             ${Source_Path}/ltr/learners/decision_tree/vertex/vertex_with_data.h
                             ${Source_Path}/ltr/learners/decision_tree/vertex/decision_vertex.h
                             ${Source_Path}/ltr/learners/decision_tree/vertex/decision_vertex.cc
                             ${Source_Path}/ltr/learners/decision_tree/vertex/leaf_vertex.h
                             ${Source_Path}/ltr/learners/decision_tree/vertex/leaf_vertex.cc)

SET(LTR_DATA_UTILS ${Source_Path}/ltr/data/utility/object_utility.h
                   ${Source_Path}/ltr/data/utility/data_set_converters.h
                   ${Source_Path}/ltr/data/utility/data_set_converters.cc
                   ${Source_Path}/ltr/data/utility/data_set_statistics.h
                   ${Source_Path}/ltr/data/utility/data_set_statistics.cc
                   ${Source_Path}/ltr/data/utility/data_set_label_statistics.h
                   ${Source_Path}/ltr/data/utility/data_set_label_statistics.cc)

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

SET(LTR_UTILS ${Source_Path}/ltr/utility/two_class_predicted_label_converter.h
              ${Source_Path}/ltr/utility/two_class_actual_label_converter.h
              ${Source_Path}/ltr/utility/multitable.h
              ${Source_Path}/ltr/utility/multi_vector_print.h
              ${Source_Path}/ltr/utility/random_number_generator.h
              ${Source_Path}/ltr/utility/random_number_generator.cc
              ${Source_Path}/ltr/utility/numerical.h
              ${Source_Path}/ltr/utility/numerical.cc
              ${Source_Path}/ltr/utility/murmur_hash.h
              ${Source_Path}/ltr/utility/murmur_hash.cc
              ${Source_Path}/ltr/utility/multitable.h
              ${Source_Path}/ltr/utility/indices.h
              ${Source_Path}/ltr/utility/indices.cc
              ${Source_Path}/ltr/utility/neighbor_weighter.h
              ${Source_Path}/ltr/utility/neighbor_weighter.cc
              ${Source_Path}/ltr/utility/container_utility.h
              ${Source_Path}/ltr/utility/eigen_converters.h
              ${Source_Path}/ltr/utility/eigen_converters.cc
              ${Source_Path}/ltr/utility/statistics_calculation.h)

SET(LTR_BOOST ${Source_Path}/ltr/utility/boost/any.h
              ${Source_Path}/ltr/utility/boost/path.h
              ${Source_Path}/ltr/utility/boost/path.cc
              ${Source_Path}/ltr/utility/boost/lexical_cast.h
              ${Source_Path}/ltr/utility/boost/multi_array.h
              ${Source_Path}/ltr/utility/boost/shared_ptr.h)

SET(LTR_INTERFACES ${Source_Path}/ltr/interfaces/reporter.h
                   ${Source_Path}/ltr/interfaces/aliaser.h
                   ${Source_Path}/ltr/interfaces/parameterized.h
                   ${Source_Path}/ltr/interfaces/parameterized.cc
                   ${Source_Path}/ltr/interfaces/functor.h
                   ${Source_Path}/ltr/interfaces/serializable.h
                   ${Source_Path}/ltr/interfaces/serializable.cc
                   ${Source_Path}/ltr/interfaces/serializable_functor.h
                   ${Source_Path}/ltr/interfaces/printable.h
                   ${Source_Path}/ltr/interfaces/printable.cc)

SET(LTR_SCORERS ${Source_Path}/ltr/scorers/scorer.h
                ${Source_Path}/ltr/scorers/scorer.cc
                ${Source_Path}/ltr/scorers/fake_scorer.h
                ${Source_Path}/ltr/scorers/fake_scorer.cc
                ${Source_Path}/ltr/scorers/one_feature_scorer.h
                ${Source_Path}/ltr/scorers/one_feature_scorer.cc
                ${Source_Path}/ltr/scorers/gp_scorer.h
                ${Source_Path}/ltr/scorers/gp_scorer.cc
                ${Source_Path}/ltr/scorers/decision_tree_scorer.h
                ${Source_Path}/ltr/scorers/decision_tree_scorer.cc
                ${Source_Path}/ltr/scorers/linear_scorer.h
                ${Source_Path}/ltr/scorers/linear_scorer.cc
                ${Source_Path}/ltr/scorers/nearest_neighbor_scorer.h
                ${Source_Path}/ltr/scorers/nearest_neighbor_scorer.cc
                ${Source_Path}/ltr/scorers/bayesian_scorer.h
                ${Source_Path}/ltr/scorers/bayesian_scorer.cc)

SET(LTR_COMPOSITION_SCORERS
                ${Source_Path}/ltr/scorers/composition_scorers/composition_scorer.h
                ${Source_Path}/ltr/scorers/composition_scorers/composition_scorer.cc)

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
                 ${Source_Path}/ltr/measures/normalized_measure.h
                 ${Source_Path}/ltr/measures/binary_classification_accuracy.h
				 ${Source_Path}/ltr/measures/binary_classification_margin.h
                 ${Source_Path}/ltr/measures/auc.h ${Source_Path}/ltr/measures/auc.cc)

SET(LTR_MEASURES_UTILS ${Source_Path}/ltr/measures/utils/measure_utility.h
                       ${Source_Path}/ltr/measures/utils/measure_utility.cc)

SET(LTR_METRICS ${Source_Path}/ltr/metrics/metric.h
                ${Source_Path}/ltr/metrics/euclidean_metric.h
                ${Source_Path}/ltr/metrics/euclidean_metric.cc
                ${Source_Path}/ltr/metrics/manhattan_metric.h
                ${Source_Path}/ltr/metrics/manhattan_metric.cc)

SET(LTR_LEARNERS ${Source_Path}/ltr/learners/learner.h
                 ${Source_Path}/ltr/learners/best_feature_learner/best_feature_learner.h
                 ${Source_Path}/ltr/learners/linear_learner/linear_learner.h
                 ${Source_Path}/ltr/learners/linear_learner/linear_learner.cc
                 ${Source_Path}/ltr/learners/nearest_neighbor_learner/nearest_neighbor_learner.h
                 ${Source_Path}/ltr/learners/nearest_neighbor_learner/nearest_neighbor_learner.cc
                 ${Source_Path}/ltr/learners/bayesian_learner/bayesian_learner.h)

SET(LTR_GP_LEARNER ${Source_Path}/ltr/learners/gp_learner/gp_learner.h
                   ${Source_Path}/ltr/learners/gp_learner/gp_learner.cc
                   ${Source_Path}/ltr/learners/gp_learner/gp_primitives.h)

SET(LTR_GP_LEARNER_STRATEGIES ${Source_Path}/ltr/learners/gp_learner/strategies/default_selection_strategy.h
                              ${Source_Path}/ltr/learners/gp_learner/strategies/default_selection_strategy.cc
                              ${Source_Path}/ltr/learners/gp_learner/strategies/select_top_trees_strategy.h
                              ${Source_Path}/ltr/learners/gp_learner/strategies/select_top_trees_strategy.cc
                              ${Source_Path}/ltr/learners/gp_learner/strategies/default_crossover_strategy.h
                              ${Source_Path}/ltr/learners/gp_learner/strategies/default_crossover_strategy.cc
                              ${Source_Path}/ltr/learners/gp_learner/strategies/crossover_adjacent_trees_strategy.h
                              ${Source_Path}/ltr/learners/gp_learner/strategies/crossover_adjacent_trees_strategy.cc
                              ${Source_Path}/ltr/learners/gp_learner/strategies/default_mutation_standart_strategy.h
                              ${Source_Path}/ltr/learners/gp_learner/strategies/default_mutation_standart_strategy.cc
                              ${Source_Path}/ltr/learners/gp_learner/strategies/default_mutation_swap_strategy.h
                              ${Source_Path}/ltr/learners/gp_learner/strategies/default_mutation_swap_strategy.cc
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
                           ${Source_Path}/ltr/feature_converters/nan_to_zero_learner.h
                           ${Source_Path}/ltr/feature_converters/nan_to_average_learner.h
                           ${Source_Path}/ltr/feature_converters/nan_to_neutral_converter.h
                           ${Source_Path}/ltr/feature_converters/nan_to_neutral_converter.cc
                           ${Source_Path}/ltr/feature_converters/nominal_to_bool_learner.h
                           ${Source_Path}/ltr/feature_converters/nominal_to_bool_converter.h
                           ${Source_Path}/ltr/feature_converters/nominal_to_bool_converter.cc
                           ${Source_Path}/ltr/feature_converters/remove_nominal_learner.h
                           ${Source_Path}/ltr/feature_converters/remove_nominal_converter.h
                           ${Source_Path}/ltr/feature_converters/remove_nominal_converter.cc)

SET(LTR_DATA_PREPROCESSORS ${Source_Path}/ltr/data_preprocessors/data_preprocessor.h
                           ${Source_Path}/ltr/data_preprocessors/data_sampler.h
                           ${Source_Path}/ltr/data_preprocessors/data_random_sampler.h
                           ${Source_Path}/ltr/data_preprocessors/fake_data_preprocessor.h)

SET(LTR_CROSSVALIDATION ${Source_Path}/ltr/crossvalidation/splitter.h
                        ${Source_Path}/ltr/crossvalidation/leave_one_out_splitter.h
                        ${Source_Path}/ltr/crossvalidation/k_fold_simple_splitter.h
                        ${Source_Path}/ltr/crossvalidation/tk_fold_simple_splitter.h
                        ${Source_Path}/ltr/crossvalidation/crossvalidator.h)

SET(LTR_COMPOSITION ${Source_Path}/ltr/learners/composition_learner/composition_learner.h
                    ${Source_Path}/ltr/learners/composition_learner/data_set_weights_updater.h
                    ${Source_Path}/ltr/learners/composition_learner/composition_scorer_weights_updater.h
                    ${Source_Path}/ltr/learners/composition_learner/ada_rank_data_set_weights_updater.h
					${Source_Path}/ltr/learners/composition_learner/ada_boost_data_set_weights_updater.h
                    ${Source_Path}/ltr/learners/composition_learner/ada_rank_composition_scorer_weights_updater.h
                    ${Source_Path}/ltr/learners/composition_learner/average_composition_scorer_weights_updater.h
					${Source_Path}/ltr/learners/composition_learner/ada_boost_learner.h)

SET(LTR_PREDICTIONS_AGGREGATORS ${Source_Path}/ltr/predictions_aggregators/predictions_aggregator.h
                    ${Source_Path}/ltr/predictions_aggregators/average_predictions_aggregator.h
                    ${Source_Path}/ltr/predictions_aggregators/average_predictions_aggregator.cc
                    ${Source_Path}/ltr/predictions_aggregators/sum_predictions_aggregator.h
                    ${Source_Path}/ltr/predictions_aggregators/sum_predictions_aggregator.cc
                    ${Source_Path}/ltr/predictions_aggregators/vote_predictions_aggregator.h
                    ${Source_Path}/ltr/predictions_aggregators/vote_predictions_aggregator.cc
                    ${Source_Path}/ltr/predictions_aggregators/max_weight_predictions_aggregator.h
                    ${Source_Path}/ltr/predictions_aggregators/max_weight_predictions_aggregator.cc
                    ${Source_Path}/ltr/predictions_aggregators/order_statistic_predictions_aggregator.h
                    ${Source_Path}/ltr/predictions_aggregators/order_statistic_predictions_aggregator.cc)

SET(OPTIMIZATION_FUNCTIONS ${Source_Path}/ltr/optimization/functions/differentiable_function.h
                    ${Source_Path}/ltr/optimization/functions/twice_differentiable_function.h
                    ${Source_Path}/ltr/optimization/functions/function.h
                    ${Source_Path}/ltr/optimization/functions/function.cc
                    ${Source_Path}/ltr/optimization/functions/linear_function.h
                    ${Source_Path}/ltr/optimization/functions/linear_function.cc
                    ${Source_Path}/ltr/optimization/functions/quadratic_function.h
                    ${Source_Path}/ltr/optimization/functions/quadratic_function.cc)

SET(OPTIMIZATION_SETS ${Source_Path}/ltr/optimization/sets/set.h
                    ${Source_Path}/ltr/optimization/sets/set.cc
                    ${Source_Path}/ltr/optimization/sets/ball_set.h
                    ${Source_Path}/ltr/optimization/sets/ball_set.cc
                    ${Source_Path}/ltr/optimization/sets/sphere_set.h
                    ${Source_Path}/ltr/optimization/sets/sphere_set.cc
                    ${Source_Path}/ltr/optimization/sets/linear_inequality_set.h
                    ${Source_Path}/ltr/optimization/sets/linear_inequality_set.cc)

SET(OPTIMIZATION_SOLVERS ${Source_Path}/ltr/optimization/solvers/solver.h
                    ${Source_Path}/ltr/optimization/solvers/solution.h
                    ${Source_Path}/ltr/optimization/solvers/iteration_solver.h
                    ${Source_Path}/ltr/optimization/solvers/simulated_annealing_solver.h
                    ${Source_Path}/ltr/optimization/solvers/simulated_annealing_solver.cc
                    ${Source_Path}/ltr/optimization/solvers/gradient_descent_solver.h
                    ${Source_Path}/ltr/optimization/solvers/gradient_descent_solver.cc)

SET(OPTIMIZATION_STOPCRITERIA ${Source_Path}/ltr/optimization/stop_criteria/and_criterion.h
                    ${Source_Path}/ltr/optimization/stop_criteria/and_criterion.cc
                    ${Source_Path}/ltr/optimization/stop_criteria/delta_argument_stop_criterion.h
                    ${Source_Path}/ltr/optimization/stop_criteria/delta_argument_stop_criterion.cc
                    ${Source_Path}/ltr/optimization/stop_criteria/delta_function_stop_criterion.h
                    ${Source_Path}/ltr/optimization/stop_criteria/delta_function_stop_criterion.cc
                    ${Source_Path}/ltr/optimization/stop_criteria/gradient_stop_criterion.h
                    ${Source_Path}/ltr/optimization/stop_criteria/gradient_stop_criterion.cc
                    ${Source_Path}/ltr/optimization/stop_criteria/iteration_stop_criterion.h
                    ${Source_Path}/ltr/optimization/stop_criteria/iteration_stop_criterion.cc
                    ${Source_Path}/ltr/optimization/stop_criteria/or_criterion.h
                    ${Source_Path}/ltr/optimization/stop_criteria/or_criterion.cc
                    ${Source_Path}/ltr/optimization/stop_criteria/stop_criterion.h
                    ${Source_Path}/ltr/optimization/stop_criteria/state.h)

SET(GETOPTPP  ${Source_Path}/contrib/getopt_pp/getopt_pp.h
              ${Source_Path}/contrib/getopt_pp/getopt_pp.cc)

# lets divide binaries in groups, for comfort navigation
SOURCE_GROUP(density_estimators\\scorers FILES ${LTR_DENSITY_ESTIMATORS_SCORERS})
SOURCE_GROUP(density_estimators\\learners FILES ${LTR_DENSITY_ESTIMATORS_LEARNERS})
SOURCE_GROUP(predictions_aggregators FILES ${LTR_PREDICTIONS_AGGREGATORS})
SOURCE_GROUP(data FILES ${LTR_DATA})
SOURCE_GROUP(utils FILES ${LTR_UTILS})
SOURCE_GROUP(learners FILES ${LTR_LEARNERS})
SOURCE_GROUP(learners\\decision_tree FILES ${LTR_DECISION_TREE})
SOURCE_GROUP(learners\\decision_tree\\condition FILES ${LTR_DECISION_TREE_CONDITION})
SOURCE_GROUP(learners\\decision_tree\\vertex FILES ${LTR_DECISION_TREE_VERTEX})
SOURCE_GROUP(learners\\decision_tree\\leaf_generator FILES ${LTR_DECISION_TREE_LEAF_GENERATOR})
SOURCE_GROUP(learners\\decision_tree\\split_criteria FILES ${LTR_DECISION_TREE_SPLIT_CRITERIA})
SOURCE_GROUP(learners\\composition_learner FILES ${LTR_COMPOSITION})
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
SOURCE_GROUP(metrics FILES ${LTR_METRICS})
SOURCE_GROUP(crossvalidation FILES ${LTR_CROSSVALIDATION})
SOURCE_GROUP(optimization\\functions FILES ${OPTIMIZATION_FUNCTIONS})
SOURCE_GROUP(optimization\\sets FILES ${OPTIMIZATION_SETS})
SOURCE_GROUP(optimization\\solvers FILES ${OPTIMIZATION_SOLVERS})
SOURCE_GROUP(contrib\\getoptpp FILES ${GETOPTPP})
SOURCE_GROUP(optimization\\stop_criteria FILES ${OPTIMIZATION_STOPCRITERIA})

SET(LTR_ALL ${LTR_PREDICTIONS_AGGREGATORS} ${LTR_DATA} ${LTR_DATA_UTILS} ${LTR_UTILS} ${LTR_INTERFACES}
            ${LTR_IO_UTILS} ${LTR_SCORERS} ${LTR_LEARNERS} ${LTR_PARAMETERS_CONTAINER} ${LTR_SCORERS_UTILS} 
            ${LTR_GP_LEARNER} ${LTR_GP_LEARNER_STRATEGIES} ${LTR_FEATURE_CONVERTERS} 
            ${LTR_FEATURE_CONVERTERS_UTILITY} ${LTR_MEASURES} ${LTR_MEASURES_UTILS} ${LTR_CROSSVALIDATION}
            ${LTR_DECISION_TREE} ${LTR_DECISION_TREE_VERTEX} ${LTR_DECISION_TREE_CONDITION}
            ${LTR_DECISION_TREE_LEAF_GENERATOR} ${LTR_DECISION_TREE_SPLIT_CRITERIA}
            ${LTR_COMPOSITION_SCORERS} ${LTR_METRICS} ${LTR_DATA_PREPROCESSORS} ${LTR_COMPOSITION}
            ${OPTIMIZATION_SETS} ${OPTIMIZATION_FUNCTIONS} ${OPTIMIZATION_SOLVERS}
			      ${LTR_DENSITY_ESTIMATORS_SCORERS} ${LTR_DENSITY_ESTIMATORS_LEARNERS} ${LTR_BOOST} ${GETOPTPP} ${OPTIMIZATION_STOPCRITERIA})

INCLUDE_DIRECTORIES(${Source_Path})
