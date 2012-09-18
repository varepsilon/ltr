CompositionLearner<Object, LinearCompositionScorer> linear_composition_learner(5); // 5 is the nunber of iterations

BinaryClassificationMargin::Ptr binary_classification_margin(new BinaryClassificationMargin);
BinaryClassificationAccuracy::Ptr binary_classification_accuracy(new BinaryClassificationAccuracy);

AdaRankScorerWeightsUpdater<Object, LinearCompositionScorer>::Ptr
  ada_rank_scorer_weights_updater(new AdaRankScorerWeightsUpdater<Object, LinearCompositionScorer>);
ada_rank_scorer_weights_updater->set_measure(binary_classification_accuracy); // note that measure for AdaRankScorerWeightsUpdater must be bounded
linear_composition_learner.set_composition_scorer_weights_updater(ada_rank_scorer_weights_updater);

AdaBoostDataSetWeightsUpdater<Object>::Ptr
  ada_boost_data_set_weights_updater(new AdaBoostDataSetWeightsUpdater<Object>);
ada_boost_data_set_weights_updater->set_measure(binary_classification_margin);
linear_composition_learner.set_data_set_weights_updater(ada_boost_data_set_weights_updater);

BestFeatureLearner<Object>::Ptr bf_learner(new BestFeatureLearner<Object>(binary_classification_accuracy));
linear_composition_learner.set_weak_learner(bf_learner);

linear_composition_learner.learn(data);
LinearCompositionScorer::Ptr linear_composition_scorer = linear_composition_learner.makeSpecific();