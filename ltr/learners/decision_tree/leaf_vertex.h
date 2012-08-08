// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_LEAF_VERTEX_H_
#define LTR_LEARNERS_DECISION_TREE_LEAF_VERTEX_H_

#include <string>

#include "ltr/learners/decision_tree/utility/utility.h"
#include "ltr/learners/decision_tree/decision_tree.h"

namespace ltr {
namespace decision_tree {
/**
 * Leaf Vertex is a node, which returns constant value for any object.
 */
template <class TValue>
class LeafVertex : public Vertex<TValue> {
 public:
  typedef typename ltr::utility::shared_ptr<LeafVertex<TValue> > Ptr;

  LeafVertex() : Vertex<TValue>() {}

  LeafVertex(Condition::Ptr condition, const TValue& value)
  : value_(value),
    Vertex<TValue>(condition) {}

  TValue value(const Object& object) const;

  void setValue(TValue value);

  string generateCppCode(const string& function_name) const;

 private:
  virtual string getDefaultAlias() const;

  TValue value_;
};

template<class TValue>
typename LeafVertex<TValue>::Ptr LeafVertexPtr() {
  return typename LeafVertex<TValue>::Ptr(new LeafVertex<TValue>());
}

template<class TValue>
typename LeafVertex<TValue>::Ptr
  LeafVertexPtr(Condition::Ptr condition, const TValue& value) {
    return typename LeafVertex<TValue>::Ptr(
      new LeafVertex<TValue>(condition, value));
}

template <class TValue>
TValue LeafVertex<TValue>::value(const Object& object) const {
  return value_;
}

template <class TValue>
void LeafVertex<TValue>::setValue(TValue value) {
  value_ = value;
}

template <class TValue>
string LeafVertex<TValue>::generateCppCode(const string& function_name) const {
  string hpp_code;

  hpp_code.
    append("inline ").
    append(ClassName<TValue>()).
    append(" ").
    append(function_name).
    append("(const std::vector<double>& features) { return ").
    append(boost::lexical_cast<string>(value_)).
    append("; }\n");

  return hpp_code;
}

template <class TValue>
string LeafVertex<TValue>::getDefaultAlias() const {
  return "Leaf Vertex";
}
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_LEAF_VERTEX_H_
