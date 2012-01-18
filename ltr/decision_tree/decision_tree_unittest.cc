// Copyright 2011 Yandex

#include "gtest/gtest.h"

#include "ltr/decision_tree/decision_tree.h"
#include "ltr/decision_tree/fake_condition.h"
#include "ltr/decision_tree/one_feature_condition.h"
#include "ltr/decision_tree/compare_condition.h"

class DecisionTreeTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }
};

using ltr::DecisionTree::DecisionTree;
using ltr::DecisionTree::DecisionVertexPtr;
using ltr::DecisionTree::LeafVertexPtr;
using ltr::DecisionTree::RegressionVertexPtr;
using ltr::DecisionTree::FakeConditionPtr;
using ltr::DecisionTree::OneFeatureConditionPtr;
using ltr::DecisionTree::CompareConditionPtr;
using ltr::DecisionTree::CompareType;

using ltr::DecisionTree::Vertex;

using ltr::Object;

#define LESS ltr::DecisionTree::LESS
#define GREATER ltr::DecisionTree::GREATER
#define EQUAL ltr::DecisionTree::EQUAL
#define GREATER_OR_EQUAL ltr::DecisionTree::GREATER_OR_EQUAL


TEST_F(DecisionTreeTest, TestingBasicOperations) {
  DecisionTree<double> tree;
  typedef Vertex<double>::Ptr VertexPtr;
  typedef ltr::DecisionTree::Condition::Ptr ConditionPtr;
  ConditionPtr first = OneFeatureConditionPtr(0);
  ConditionPtr second = OneFeatureConditionPtr(1);

  VertexPtr leaf1 = LeafVertexPtr<double>(
      CompareConditionPtr(first, LESS, 10.0), 1.0);
  VertexPtr leaf2 = LeafVertexPtr<double>(
      CompareConditionPtr(first, EQUAL, 10.0), 2.0);
  VertexPtr leaf3 = LeafVertexPtr<double>(
      CompareConditionPtr(second, LESS, 50.0), 3.0);
  VertexPtr leaf4 = LeafVertexPtr<double>(
      CompareConditionPtr(second, GREATER_OR_EQUAL, 50.0), 4.0);

  VertexPtr v1 = DecisionVertexPtr<double>();

  VertexPtr v2 = DecisionVertexPtr<double>(
      CompareConditionPtr(first, GREATER, 10.0));

  v1->addChild(leaf1);
  v1->addChild(leaf2);
  v1->addChild(v2);

  v2->addChild(leaf3);
  v2->addChild(leaf4);

  tree.setRoot(v1);

  Object obj;

  ltr::DecisionTree::_deep_cache_conditions = true;

  obj << 5.0 << 1.0;
  EXPECT_EQ(1.0, tree.value(obj));

  obj.features().clear();
  obj << 10.0 << 1.0;
  EXPECT_EQ(2.0, tree.value(obj));

  obj.features().clear();
  obj << 15.0 << 20.0;
  EXPECT_EQ(3.0, tree.value(obj));

  obj.features().clear();
  obj << 15.0 << 60.0;
  EXPECT_EQ(4.0, tree.value(obj));
}
