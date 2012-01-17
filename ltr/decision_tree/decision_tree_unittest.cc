// Copyright 2011 Yandex

#include "gtest/gtest.h"

#include "ltr/decision_tree/decision_tree.h"

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
TEST_F(DecisionTreeTest, TestingBasicOperations) {
  DecisionTree<double> tree;
}
