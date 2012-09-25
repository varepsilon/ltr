// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_VERTEX_DECISION_VERTEX_H_
#define LTR_LEARNERS_DECISION_TREE_VERTEX_DECISION_VERTEX_H_

#include <string>

#include "ltr/learners/decision_tree/vertex/vertex.h"

namespace ltr {
namespace decision_tree {
/**
 * DecisionVertex is a node, which returns value of the child
 * with the greatest condition value.
 */
class DecisionVertex : public Vertex {
 public:
  typedef ltr::utility::shared_ptr<DecisionVertex> Ptr;

  DecisionVertex() {}

  double value(const Object& object) const;

  string generateVertexCppCode(
    const string& function_name, int level_id, int vertex_id) const;

 private:
  virtual string getDefaultAlias() const;
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_VERTEX_DECISION_VERTEX_H_
