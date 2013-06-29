// Copyright 2012 Yandex

#include <iostream> // NOLINT

#include "ltr/data/object.h"
#include "ltr/data/data_set.h"

#include "ltr/learners/decision_tree/decision_tree_learner.h"
#include "ltr/learners/decision_tree/id3_splitter.h"
#include "ltr/learners/decision_tree/leaf_generator/most_common_label_leaf_generator.h"
#include "ltr/learners/decision_tree/split_criteria/same_label_stop_criteria.h"

using std::cout;
using std::endl;

using ltr::Object;
using ltr::DataSet;
using ltr::Scorer;

using ltr::decision_tree::DecisionTreeLearner;
using ltr::decision_tree::ID3Splitter;
using ltr::decision_tree::MostCommonLabelLeafGenerator;
using ltr::decision_tree::SameLabelStopSplittingCriteria;

int main() {
  DataSet<Object> data;

  Object point1, point2;
  point1 << 1 << 1;
  point2 << 2 << 2;
  point1.set_actual_label(1);
  point1.set_actual_label(1);

  data.add(point1);
  data.add(point2);

  Object point3, point4;
  point3 << -1 << -1;
  point4 << -2 << -2;
  point3.set_actual_label(2);
  point4.set_actual_label(2);

  data.add(point3);
  data.add(point4);

  DecisionTreeLearner<Object>::Ptr decision_tree_learner(
    new DecisionTreeLearner<Object>(new ID3Splitter,
    new MostCommonLabelLeafGenerator));
  decision_tree_learner->add_stop_splitting_criteria(
    new SameLabelStopSplittingCriteria);

  decision_tree_learner->learn(data);

  Scorer::Ptr decision_tree_scorer = decision_tree_learner->make();

  Object test_point1;
  test_point1 << 3 << 3;
  cout << "First test point label = " <<
    decision_tree_scorer->score(test_point1) << endl;

  Object test_point2;
  test_point2 << -3 << -3;
  cout << "Second test point label = " <<
    decision_tree_scorer->score(test_point2) << endl;

  return 0;
}
