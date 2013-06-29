// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_VERTEX_VERTEX_H_
#define LTR_LEARNERS_DECISION_TREE_VERTEX_VERTEX_H_

#include <rlog/rlog.h>

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "ltr/interfaces/serializable_functor.h"

#include "ltr/learners/decision_tree/condition/condition.h"

#include "ltr/utility/boost/shared_ptr.h"

using std::string;
using std::vector;

namespace ltr {
namespace decision_tree {
/**
 * Vertex is a node in the decision tree. It has pointers to all children
 * and conditions of transition to each children.
 */
class Vertex : public SerializableFunctor<double> {
  ALLOW_SHARED_PTR_ONLY_CREATION(Vertex)
 public:
  /**
   * Adds child to the vertex.
   * @param child - child to add,
   * @param condition - condition of transition to this child.
   */
  void addChild(Vertex::Ptr child, Condition::Ptr condition);
  /**
   * Returns true, if vertex has a child.
   */
  bool hasChild() const;

  virtual double value(const Object& object) const = 0;

  virtual string generateCppCodeWithId(const string& function_name) const = 0;

 protected:
  vector<Vertex::Ptr> childrens_;
  vector<Condition::Ptr> conditions_;
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_VERTEX_VERTEX_H_
