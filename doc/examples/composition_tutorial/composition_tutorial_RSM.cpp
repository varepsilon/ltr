CompositionLearner<Object, LinearCompositionScorer> linear_composition_learner;

AbsError::Ptr abs_error(new AbsError); // ::Ptr means a shared pointer wherever in LTR

FeatureRandomSamplerLearner<Object>::Ptr rsm(new FeatureRandomSamplerLearner<Object>);
BestFeatureLearner<Object>::Ptr best_feature_learner(new BestFeatureLearner<Object>(abs_error));
best_feature_learner->addFeatureConverterLearner(rsm);

linear_composition_learner.set_weak_learner(best_feature_learner);

linear_composition_learner.learn(data);
LinearCompositionScorer::Ptr linear_scorer = linear_composition_learner.makeSpecific();