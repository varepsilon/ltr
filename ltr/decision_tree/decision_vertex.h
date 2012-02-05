// Copyright 2012 Yandex

#ifndef LTR_DECISION_TREE_DECISION_VERTEX_H_
#define LTR_DECISION_TREE_DECISION_VERTEX_H_

#include <string>

#include "ltr/decision_tree/decision_tree.h"

namespace ltr {
namespace decision_tree {

template <class TValue>
class DecisionVertex : public Vertex<TValue> {
  public:
    typedef boost::shared_ptr<DecisionVertex<TValue> > Ptr;

    DecisionVertex() : Vertex<TValue>() {}
    explicit DecisionVertex(Condition::Ptr condition) :
        Vertex<TValue>(condition) {}
    TValue value(const Object& obj) const;

    string generateCppCode(const string& function_name) const {
      return "Not implemented yet";
    }
};

template<class TValue>
typename DecisionVertex<TValue>::Ptr DecisionVertexPtr() {
  return typename DecisionVertex<TValue>::Ptr(new DecisionVertex<TValue>());
}

template<class TValue>
typename DecisionVertex<TValue>::Ptr DecisionVertexPtr(
    Condition::Ptr condition) {
  return typename DecisionVertex<TValue>::Ptr(
      new DecisionVertex<TValue>(condition));
}

template <class TValue>
TValue DecisionVertex<TValue>::value(const Object& obj) const {
  typename Vertex<TValue>::Ptr best_child;
  double max_value = 0;
  if (!this->hasChild())
    throw std::logic_error("non list vertex has no children");
  typename Vertex<TValue>::Ptr child = this->firstChild();
  while (child != NULL) {
    double cond_value = child->condition()->value(obj);
    if (best_child == NULL || max_value < cond_value) {
      best_child = child;
      max_value = cond_value;
    }
    child = child->nextSibling();
  }
  return best_child->value(obj);
}
}
}
#endif  // LTR_DECISION_TREE_DECISION_VERTEX_H_

