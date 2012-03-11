// Copyright 2012 Yandex

#ifndef LTR_UTILITY_DECISION_TREE_REGRESSION_VERTEX_H_
#define LTR_UTILITY_DECISION_TREE_REGRESSION_VERTEX_H_

#include <string>

#include "ltr/utility/decision_tree/decision_tree.h"

namespace ltr {
namespace decision_tree {

/**
DecisionVertex is a node, which returns lineal regression of values
in its children
*/
template <class TValue>
class RegressionVertex : public Vertex<TValue> {
  public:
    typedef boost::shared_ptr<RegressionVertex<TValue> > Ptr;

    RegressionVertex() : Vertex<TValue>() {}
    explicit RegressionVertex(Condition::Ptr condition) :
        Vertex<TValue>(condition) {}
    TValue value(const Object& obj) const;

    string generateCppCode(const string& function_name) const {
      return "Not implemented yet";
    }
};

template<class TValue>
typename RegressionVertex<TValue>::Ptr RegressionVertexPtr() {
  return typename RegressionVertex<TValue>::Ptr(new RegressionVertex<TValue>());
}

template<class TValue>
typename RegressionVertex<TValue>::Ptr RegressionVertexPtr(
    Condition::Ptr condition) {
  return typename RegressionVertex<TValue>::Ptr(
      new RegressionVertex<TValue>(condition));
}

template <class TValue>
TValue RegressionVertex<TValue>::value(const Object& obj) const {
  if (!this->hasChild())
    throw std::logic_error("non list vertex has no children");
  typename Vertex<TValue>::Ptr child = this->firstChild();

  TValue result1, result2;
  double sum = 0;
  int n_child = 0;
  while (child != NULL) {
    double cond_abs = std::fabs(child->condition()->value(obj));
    sum += cond_abs;

    TValue val = child->value(obj);
    if (n_child == 0) {
      result1 = (result2 = val) * cond_abs;
    } else {
      result1 = result1 + val * cond_abs;
      result2 = result2 + val;
    }
    child = child->nextSibling();
    n_child++;
  }
  if (sum == 0) {
    return result2 / n_child;
  }
  return result1 * (1.0 / sum);
}
}
}
#endif  // LTR_UTILITY_DECISION_TREE_REGRESSION_VERTEX_H_
