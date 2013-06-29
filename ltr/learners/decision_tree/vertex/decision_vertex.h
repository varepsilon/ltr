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
  ALLOW_SHARED_PTR_ONLY_CREATION(DecisionVertex)
 public:
  DecisionVertex() {}

  virtual double value(const Object& object) const;

  virtual string generateCppCode(const string& function_name) const;

  virtual string generateCppCodeWithId(const string& function_name) const;

 private:
  Vertex::Ptr findBestChild(const Object& object) const;

  string generateCppCodeImpl(const string& function_name, bool add_id) const;

  virtual string getDefaultAlias() const;
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_VERTEX_DECISION_VERTEX_H_
