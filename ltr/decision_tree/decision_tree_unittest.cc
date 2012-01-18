// Copyright 2011 Yandex

#include "gtest/gtest.h"

#include "ltr/decision_tree/decision_tree.h"
#include "ltr/decision_tree/fake_condition.h"
#include "ltr/decision_tree/one_feature_condition.h"

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

using ltr::DecisionTree::Vertex;

using ltr::Object;

TEST_F(DecisionTreeTest, TestingBasicOperations) {
  DecisionTree<double> tree;
  typedef Vertex<double>::Ptr VertexPtr;
  VertexPtr leaf1 = LeafVertexPtr<double>(OneFeatureConditionPtr(0), 1);
  VertexPtr leaf2 = LeafVertexPtr<double>(OneFeatureConditionPtr(1), 2);
  VertexPtr leaf3 = LeafVertexPtr<double>(OneFeatureConditionPtr(2), 3);

  VertexPtr v1 = DecisionVertexPtr<double>();

  v1->addChild(leaf1);
  v1->addChild(leaf2);
  v1->addChild(leaf3);

  tree.setRoot(v1);

  Object obj;
  obj << 1.0 << 5.0 << 2.0;

  EXPECT_EQ(2, tree.value(obj));
}
