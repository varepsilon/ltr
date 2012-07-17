// Copyright 2012 Yandex School Practice

#include <stdlib.h>
#include <string>

#include "gtest/gtest.h"

#include "ltr/data/object.h"
#include "ltr/data/object_list.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/data_set.h"
#include "ltr/data_preprocessors/data_random_sampler.h"
#include "ltr/data_preprocessors/data_sampler.h"
#include "ltr/utility/indices.h"
#include "ltr/parameters_container/parameters_container.h"
#include "ltr/scorers/decision_tree_scorer.h"
#include "ltr/scorers/fake_scorer.h"
#include "ltr/scorers/gp_scorer.h"
#include "ltr/scorers/composition_scorers/linear_composition_scorer.h"
#include "ltr/scorers/linear_scorer.h"
#include "ltr/scorers/one_feature_scorer.h"

using std::cout;
using std::endl;


TEST(ToStringTest, TestToStringOperation) {
  ltr::Object object1;
  object1 << 1 << 2 << 3.67;
  cout << object1.toString() << endl;

  ltr::Object object2;
  object2 << 1.234 << 3.0 << -3;

  ltr::ObjectPair object_pair(object1, object2);
  cout << object_pair.toString() << endl;

  ltr::ObjectList object_list;
  object_list.add(object1);
  object_list.add(object2);
  cout << object_list.toString() << endl;

  ltr::DataSet<ltr::Object> data;
  data.add(object1);
  data.add(object2);
  cout << data.toString() << endl;

  ltr::DataRandomSampler<ltr::Object> random_sampler;
  cout << random_sampler.toString() << endl;

  IndicesPtr indices_ptr(new Indices);
  for (int index = 1; index <= 10; ++index) {
    indices_ptr->push_back(index);
  }

  ltr::DataSampler<ltr::Object> sampler(indices_ptr);
  cout << sampler.toString() << endl;

  ltr::ParametersContainer container;
  container.AddNew("first", 1);
  container.AddNew("second", 2);
  container.AddNew("third", 3);
  cout << container.toString() << endl;

  ltr::decision_tree::DecisionTreeScorer decision_tree_scorer;
  cout << decision_tree_scorer.toString() << endl;

  ltr::FakeScorer fake_scorer;
  cout << fake_scorer.toString() << endl;

  ltr::gp::GPScorer gp_scorer;
  cout << gp_scorer.toString() << endl;

  ltr::LinearCompositionScorer linearCompositionScorer;
  linearCompositionScorer.add(ltr::Scorer::Ptr(new ltr::FakeScorer()), 0.2);
  linearCompositionScorer.add(ltr::Scorer::Ptr(new ltr::gp::GPScorer), 0.7);
  linearCompositionScorer.add(ltr::Scorer::Ptr(new ltr::decision_tree::DecisionTreeScorer), 0.1);

  cout << linearCompositionScorer.toString() << endl;
  linearCompositionScorer.clear();

  ltr::LinearScorer linear_scorer(vector<double>(5, 0.3));
  cout << linear_scorer.toString() << endl;

  ltr::OneFeatureScorer one_feature_scorer;
  cout << one_feature_scorer.toString() << endl;
}