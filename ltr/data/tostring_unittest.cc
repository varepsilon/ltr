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
#include "ltr/scorers/linear_scorer.h"
#include "ltr/scorers/one_feature_scorer.h"
#include "ltr/scorers/composition_scorers/composition_scorer.h"

using std::cout;
using std::endl;


TEST(ToStringTest, TestToStringOperation) {
  ltr::Object object1;
  object1 << 1 << 2 << 3.67;
  rInfo(object1.toString().c_str());

  ltr::Object object2;
  object2 << 1.234 << 3.0 << -3;

  ltr::ObjectPair object_pair(object1, object2);
  rInfo(object_pair.toString().c_str());

  ltr::ObjectList object_list;
  object_list.add(object1);
  object_list.add(object2);
  rInfo(object_list.toString().c_str());

  ltr::DataSet<ltr::Object> data;
  data.add(object1);
  data.add(object2);
  rInfo(data.toString().c_str());

  ltr::DataRandomSampler<ltr::Object> random_sampler;
  rInfo(random_sampler.toString().c_str());

  Indices indices;
  for (int index = 1; index <= 10; ++index) {
    indices.push_back(index);
  }

  ltr::DataSampler<ltr::Object> sampler(indices);
  rInfo(sampler.toString().c_str());

  ltr::ParametersContainer container;
  container.AddNew("first", 1);
  container.AddNew("second", 2);
  container.AddNew("third", 3);
  rInfo(container.toString().c_str());

  ltr::decision_tree::DecisionTreeScorer decision_tree_scorer;
  rInfo(decision_tree_scorer.toString().c_str());

  ltr::FakeScorer fake_scorer;
  rInfo(fake_scorer.toString().c_str());

  ltr::gp::GPScorer gp_scorer;
  rInfo(gp_scorer.toString().c_str());

  ltr::composition::CompositionScorer composition_scorer;
  composition_scorer.add(ltr::Scorer::Ptr(new ltr::FakeScorer()), 0.2);
  composition_scorer.add(ltr::Scorer::Ptr(new ltr::gp::GPScorer), 0.7);
  composition_scorer.add(ltr::Scorer::Ptr(
    new ltr::decision_tree::DecisionTreeScorer), 0.1);

  rInfo(composition_scorer.toString().c_str());
  composition_scorer.clear();

  ltr::LinearScorer linear_scorer(vector<double>(5, 0.3));
  rInfo(linear_scorer.toString().c_str());

  ltr::OneFeatureScorer one_feature_scorer;
  rInfo(one_feature_scorer.toString().c_str());
}
