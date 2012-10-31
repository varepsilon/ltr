// Copyright 2012 Yandex

#include "ltr/scorers/decision_tree_scorer.h"

namespace ltr {
namespace decision_tree {
void DecisionTreeScorer::setTree(DecisionTree::Ptr tree) {
  tree_ = tree;
}

string DecisionTreeScorer::toStringImpl() const {
  return "Decision of the tree";
}

double DecisionTreeScorer::scoreImpl(const Object& obj) const {
  return tree_->value(obj);
}

string DecisionTreeScorer::generateCppCodeImpl(
  const string& function_name) const {
    return tree_->generateCppCode(function_name);
}

string DecisionTreeScorer::getDefaultAlias() const {
  return "DecisionTreeScorer";
}
};
};
