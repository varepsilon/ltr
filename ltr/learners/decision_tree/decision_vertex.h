// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_DECISION_VERTEX_H_
#define LTR_LEARNERS_DECISION_TREE_DECISION_VERTEX_H_

#include <string>

#include "ltr/learners/decision_tree/utility/utility.h"
#include "ltr/learners/decision_tree/decision_tree.h"

namespace ltr {
namespace decision_tree {

/**
DecisionVertex is a node, which returns value of the child
with the greatest condition
*/
template <class TValue>
class DecisionVertex : public Vertex<TValue> {
  public:
    typedef ltr::utility::shared_ptr<DecisionVertex<TValue> > Ptr;

    DecisionVertex() : Vertex<TValue>() {}
    explicit DecisionVertex(Condition::Ptr condition) :
        Vertex<TValue>(condition) {}
    TValue value(const Object& obj) const;

    string generateCppCode(const string& function_name) const {
      string hpp_code;
      int n_children = 0;
      typename Vertex<TValue>::Ptr child = this->firstChild();
      while (child != NULL) {
        hpp_code.append(child->condition()->generateCppCode());
        hpp_code.append(child->generateCppCode());
        child = child->nextSibling();
        n_children++;
      }

      hpp_code.
        append("inline ").
        append(ClassName<TValue>()).
        append(" ").
        append(function_name).
        append("(const std::vector<double>& features) {\n").
        append("  typedef ").
          append(ClassName<TValue>()).
          append(" (*vertex_function_ptr)").
          append("(const std::vector<double>&);\n").
        append("  typedef ").
          append("double (*condition_function_ptr)").
          append("(const std::vector<double>&);\n").
        append("  condition_function_ptr conditions[] = {");

      // Create array of condition functions
      child = this->firstChild();
      if (child != NULL) {
        hpp_code.
          append(child->condition()->getDefaultSerializableObjectName());
        child = child->nextSibling();
      }
      while (child != NULL) {
        hpp_code.
          append(", ").
          append(child->condition()->getDefaultSerializableObjectName());
        child = child->nextSibling();
      }
      hpp_code.
        append("};\n").
        append("  vertex_function_ptr children[] = {");

      // Create array of children functions
      child = this->firstChild();
      if (child != NULL) {
        hpp_code.append(child->getDefaultSerializableObjectName());
        child = child->nextSibling();
      }
      while (child != NULL) {
        hpp_code.
          append(", ").
          append(child->getDefaultSerializableObjectName());
        child = child->nextSibling();
      }
      hpp_code.
        append("};\n").
        append("  int best_id = -1;\n").
        append("  double best_condition = 0;\n").
        append("  for (int i = 0; i < ").
        append(boost::lexical_cast<string>(n_children)).
        append("; ++i) {\n").
        append("    double val = conditions[i](features);\n").
        append("    if (best_id == -1 || val > best_condition) {\n").
          append("      best_id = i;\n").
          append("      best_condition = val;\n").
        append("    }\n").
        append("  }\n").
        append("  return children[best_id](features);\n").
        append("}\n");

      return hpp_code;
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
#endif  // LTR_LEARNERS_DECISION_TREE_DECISION_VERTEX_H_
