// Copyright 2011 Yandex

#include "gtest/gtest.h"

#include "ltr/utility/numerical.h"

#include "ltr/utility/decision_tree/decision_tree.h"
#include "ltr/utility/decision_tree/leaf_vertex.h"
#include "ltr/utility/decision_tree/decision_vertex.h"
#include "ltr/utility/decision_tree/regression_vertex.h"
#include "ltr/utility/decision_tree/compare_condition.h"

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

using ltr::decision_tree::DecisionTree;
using ltr::decision_tree::DecisionVertexPtr;
using ltr::decision_tree::LeafVertexPtr;
using ltr::decision_tree::RegressionVertexPtr;
using ltr::decision_tree::FakeConditionPtr;
using ltr::decision_tree::OneFeatureConditionPtr;
using ltr::decision_tree::CompareConditionPtr;
using ltr::decision_tree::CompareType;

using ltr::decision_tree::Vertex;

using ltr::Object;
using ltr::utility::DoubleEqual;

#define LESS ltr::decision_tree::LESS
#define GREATER ltr::decision_tree::GREATER
#define EQUAL ltr::decision_tree::EQUAL
#define GREATER_OR_EQUAL ltr::decision_tree::GREATER_OR_EQUAL

TEST_F(DecisionTreeTest, TestingBasicOperations) {
  DecisionTree<double> tree;
  typedef Vertex<double>::Ptr VertexPtr;
  typedef ltr::decision_tree::Condition::Ptr ConditionPtr;

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

TEST_F(DecisionTreeTest, TestingRegressionVertrex) {
  DecisionTree<double> tree;
  typedef Vertex<double>::Ptr VertexPtr;
  typedef ltr::decision_tree::Condition::Ptr ConditionPtr;

  ConditionPtr first = OneFeatureConditionPtr(0);
  ConditionPtr second = OneFeatureConditionPtr(1);
  ConditionPtr third = OneFeatureConditionPtr(2);
  ConditionPtr fourth = OneFeatureConditionPtr(3);
  ConditionPtr fifth = OneFeatureConditionPtr(4);

  VertexPtr leaf1 = LeafVertexPtr<double>(
      first, 1.0);
  VertexPtr leaf2 = LeafVertexPtr<double>(
      second, 2.0);
  VertexPtr leaf3 = LeafVertexPtr<double>(
      third, 3.0);
  VertexPtr leaf4 = LeafVertexPtr<double>(
      fourth, 4.0);
  VertexPtr leaf5 = LeafVertexPtr<double>(
      fifth, 5.0);

  VertexPtr v1 = RegressionVertexPtr<double>();

  VertexPtr v2 = RegressionVertexPtr<double>(
      CompareConditionPtr(first, GREATER, 10.0));

  v1->addChild(leaf1);
  v1->addChild(leaf2);
  v1->addChild(leaf3);
  v1->addChild(v2);

  v2->addChild(leaf4);
  v2->addChild(leaf5);

  tree.setRoot(v1);

  Object obj;
  obj << 1 << 2 << 3 << 100 << 32;
  EXPECT_TRUE(DoubleEqual(tree.value(obj), (1*1.0 + 2*2 + 3*3)/(1.0 + 2 + 3)));

  obj.features().clear();
  obj << 4 << 1 << 5 << 34 << 12;
  EXPECT_EQ(tree.value(obj), (4*1 + 1*2 + 5*3)/(4.0 + 1 + 5));

  obj.features().clear();
  obj << 9 << -5 << 3.4 << 6 << 2;
  EXPECT_EQ(tree.value(obj), (9*1 + 5*2 + 3.4*3)/(9.0 + 5 + 3.4));

  obj.features().clear();
  obj << 11 << 0 << 0 << 2 << 1;
  EXPECT_EQ(tree.value(obj), (11*1 + (4*2.0 + 5)/3.0)/(11.0 + 1));
}
