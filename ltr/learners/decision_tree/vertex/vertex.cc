// Copyright 2012 Yandex

#include "ltr/learners/decision_tree/vertex/vertex.h"

namespace ltr {
namespace decision_tree {
void Vertex::addChild(Vertex::Ptr child, Condition::Ptr condition) {
  INFO("Add a child to vertex.");
  childrens_.push_back(child);
  conditions_.push_back(condition);
}

bool Vertex::hasChild() const {
  return !childrens_.empty();
}

string Vertex::generateCppCode(const string& function_name) const {
  return generateVertexCppCode(function_name, 0, 0);
}
};
};
