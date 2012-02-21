// Copyright 2012 Yandex

#ifndef LTR_DECISION_TREE_LEAF_VERTEX_H_
#define LTR_DECISION_TREE_LEAF_VERTEX_H_

#include <string>

#include "ltr/decision_tree/decision_tree.h"

namespace ltr {
namespace decision_tree {

/**
Leaf Vertex is a node, which returns constant value for any object.
*/
template <class TValue>
class LeafVertex : public Vertex<TValue> {
  public:
    typedef typename boost::shared_ptr<LeafVertex<TValue> > Ptr;
    TValue value(const Object& obj) const {
      return value_;
    }
    void setValue(TValue value) {
      value_ = value;
    }
    LeafVertex() : Vertex<TValue>() {}
    LeafVertex(Condition::Ptr condition, const TValue& value) :
        value_(value), Vertex<TValue>(condition) {}

    string generateCppCode(const string& function_name) const {
      return "Not implemented yet";
    }
  private:
    TValue value_;
};

template<class TValue>
typename LeafVertex<TValue>::Ptr LeafVertexPtr() {
  return typename LeafVertex<TValue>::Ptr(new LeafVertex<TValue>());
}

template<class TValue>
typename LeafVertex<TValue>::Ptr LeafVertexPtr(Condition::Ptr condition,
                                               const TValue& value) {
  return typename LeafVertex<TValue>::Ptr(
      new LeafVertex<TValue>(condition, value));
}
}
}

#endif  // LTR_DECISION_TREE_LEAF_VERTEX_H_
