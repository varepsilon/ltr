AdaBoostLearner ada_learner(5);

BinaryClassificationAccuracy::Ptr binary_classification_accuracy(new BinaryClassificationAccuracy);
BestFeatureLearner<Object>::Ptr bf_learner(new BestFeatureLearner<Object>(binary_classification_accuracy));
ada_learner.set_weak_learner(bf_learner);

ada_learner.learn(data);
LinearCompositionScorer::Ptr ada_scorer = ada_learner.makeSpecific();

std::cout << *ada_scorer;