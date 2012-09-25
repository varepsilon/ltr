// Copyright 2012 Yandex

#include "gtest/gtest.h"

#include "ltr/data/object.h"
#include "ltr/data/data_set.h"

#include "ltr/learners/decision_tree/decision_tree_learner.h"
#include "ltr/learners/decision_tree/oblivious_tree_splitter.h"
#include "ltr/learners/decision_tree/id3_splitter.h"

#include "ltr/learners/decision_tree/leaf_generator/most_common_label_leaf_generator.h"

#include "ltr/learners/decision_tree/split_criteria/same_label_stop_criteria.h"

using ltr::Object;
using ltr::DataSet;
using ltr::FeatureInfo;
using ltr::Scorer;

using ltr::decision_tree::LeafGenerator;
using ltr::decision_tree::MostCommonLabelLeafGenerator;
using ltr::decision_tree::StopSplittingCriteria;
using ltr::decision_tree::SameLabelStopSplittingCriteria;
using ltr::decision_tree::DecisionTreeLearner;
using ltr::decision_tree::ObliviousTreeSplitter;
using ltr::decision_tree::ID3Splitter;

class DecisionTreeTest : public ::testing::Test {
 public:
  DecisionTreeTest()
  : data(FeatureInfo(2)) {}

 protected:
  virtual void SetUp() {
    {
      Object point1, point2, point3, point4, point5;
      point1 << 0.0 << 0.0;
      point2 << 0.0 << 1.0;
      point3 << 1.0 << 0.0;
      point4 << 1.0 << 1.0;
      point5 << 1.0 << 2.0;

      point1.set_actual_label(1);
      point2.set_actual_label(1);
      point3.set_actual_label(1);
      point4.set_actual_label(1);
      point5.set_actual_label(1);

      data.add(point1);
      data.add(point2);
      data.add(point3);
      data.add(point4);
      data.add(point5);
    }
    {
      Object point1, point2, point3, point4, point5;
      point1 << 3.0 << 3.0;
      point2 << 4.0 << 3.0;
      point3 << 3.0 << 4.0;
      point4 << 4.0 << 4.0;
      point5 << 5.0 << 4.0;

      point1.set_actual_label(2);
      point2.set_actual_label(2);
      point3.set_actual_label(2);
      point4.set_actual_label(2);
      point5.set_actual_label(2);

      data.add(point1);
      data.add(point2);
      data.add(point3);
      data.add(point4);
      data.add(point5);
    }
    {
      Object point1, point2, point3, point4, point5;
      point1 << 3.0 << 1.0;
      point2 << 4.0 << 1.0;
      point3 << 5.0 << 1.0;
      point4 << 6.0 << 1.0;
      point5 << 5.0 << 0.0;

      point1.set_actual_label(3);
      point2.set_actual_label(3);
      point3.set_actual_label(3);
      point4.set_actual_label(3);
      point5.set_actual_label(3);

      data.add(point1);
      data.add(point2);
      data.add(point3);
      data.add(point4);
      data.add(point5);
    }
  }

  DataSet<Object> data;
};

TEST_F(DecisionTreeTest, TestingObliviousTree) {
  DecisionTreeLearner decision_tree_learner(
    new ObliviousTreeSplitter, new MostCommonLabelLeafGenerator);
  decision_tree_learner.addStopSplittingCriteria(
    new SameLabelStopSplittingCriteria);

  decision_tree_learner.learn(data);

  Scorer::Ptr decision_tree_scorer = decision_tree_learner.make();

  std::cout << decision_tree_scorer->generateCppCode("fun");

  Object test_point1;
  test_point1 << 0.5 << 0.7;
  EXPECT_EQ(1, decision_tree_scorer->score(test_point1));

  Object test_point2;
  test_point2 << 5.6 << 3.7;
  EXPECT_EQ(2, decision_tree_scorer->score(test_point2));

  Object test_point3;
  test_point3 << 4.3 << 1.1;
  EXPECT_EQ(3, decision_tree_scorer->score(test_point3));
}

TEST_F(DecisionTreeTest, TestingID3) {
  DecisionTreeLearner decision_tree_learner(
    new ID3Splitter, new MostCommonLabelLeafGenerator);
  decision_tree_learner.addStopSplittingCriteria(
    new SameLabelStopSplittingCriteria);

  decision_tree_learner.learn(data);

  Scorer::Ptr decision_tree_scorer = decision_tree_learner.make();

  Object test_point1;
  test_point1 << 0.5 << 0.7;
  EXPECT_EQ(1, decision_tree_scorer->score(test_point1));

  Object test_point2;
  test_point2 << 5.6 << 3.7;
  EXPECT_EQ(2, decision_tree_scorer->score(test_point2));

  Object test_point3;
  test_point3 << 4.3 << 1.1;
  EXPECT_EQ(3, decision_tree_scorer->score(test_point3));
}
