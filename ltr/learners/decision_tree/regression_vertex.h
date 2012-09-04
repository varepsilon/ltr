// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_REGRESSION_VERTEX_H_
#define LTR_LEARNERS_DECISION_TREE_REGRESSION_VERTEX_H_

#include <string>

#include "ltr/learners/decision_tree/decision_tree.h"

namespace ltr {
namespace decision_tree {
/**
 * DecisionVertex is a node, which returns lineal regression of values
 * in its children
 */
template <class TValue>
class RegressionVertex : public Vertex<TValue> {
 public:
  typedef ltr::utility::shared_ptr<RegressionVertex<TValue> > Ptr;

  RegressionVertex() : Vertex<TValue>() {}

  explicit RegressionVertex(Condition::Ptr condition)
  : Vertex<TValue>(condition) {}

  TValue value(const Object& object) const;

  string generateCppCode(const string& function_name) const;

 private:
  virtual string getDefaultAlias() const;
};

template<class TValue>
typename RegressionVertex<TValue>::Ptr RegressionVertexPtr() {
  return typename RegressionVertex<TValue>::Ptr(new RegressionVertex<TValue>());
}

template<class TValue>
typename RegressionVertex<TValue>::Ptr
  RegressionVertexPtr(Condition::Ptr condition) {
    return typename RegressionVertex<TValue>::Ptr(
      new RegressionVertex<TValue>(condition));
}

template <class TValue>
TValue RegressionVertex<TValue>::value(const Object& object) const {
  if (!this->hasChild()) {
    throw std::logic_error("non list vertex has no children");
  }

  typename Vertex<TValue>::Ptr child = this->firstChild();

  TValue result1, result2;

  double sum = 0;
  int number_of_childs = 0;
  while (child != NULL) {
    double cond_abs = std::fabs(child->condition()->value(object));
    sum += cond_abs;

    TValue val = child->value(object);

    if (number_of_childs == 0) {
      result1 = (result2 = val) * cond_abs;
    } else {
      result1 = result1 + val * cond_abs;
      result2 = result2 + val;
    }
    child = child->nextSibling();
    number_of_childs++;
  }

  if (sum == 0) {
    return result2 / number_of_childs;
  }
  return result1 * (1.0 / sum);
}

template <class TValue>
string RegressionVertex<TValue>::generateCppCode(
  const string& function_name) const {
    return "Not implemented yet";
}

template <class TValue>
string RegressionVertex<TValue>::getDefaultAlias() const {
  return "Regression vertex";
}
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_REGRESSION_VERTEX_H_
