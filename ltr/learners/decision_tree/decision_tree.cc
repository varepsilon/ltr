// Copyright 2012 Yandex

#include "ltr/learners/decision_tree/decision_tree.h"

namespace ltr {
namespace decision_tree {
DecisionTree::~DecisionTree() {
  root_ = Vertex::Ptr();
}

double DecisionTree::value(const Object& object) const {
  return root_->value(object);
}

void DecisionTree::setRoot(Vertex::Ptr root) {
  root_ = root;
}

string DecisionTree::generateCppCode(const string& function_name) const {
    return root_->generateCppCode(function_name);
}

string DecisionTree::getDefaultAlias() const {
  return "Decision Tree";
}
};
};
