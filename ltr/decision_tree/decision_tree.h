// Copyright 2011 Yandex

#ifndef LTR_DECISION_TREE_DECISION_TREE_H_
#define LTR_DECISION_TREE_DECISION_TREE_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <map>
#include <cmath>
#include <string>
#include <vector>
#include <stdexcept>

#include "ltr/decision_tree/condition.h"

using std::map;
using std::string;
using std::vector;

namespace ltr {
namespace decision_tree {

template <class TValue>
/**
Vertex is a node in the decision tree. It has a condition, leading in it.
Condition is a functor, which returns probability for object
to be decided by this vertex.
It can have children.
*/
class Vertex : public SerializableFunctor<TValue> {
  public:
    typedef boost::shared_ptr<Vertex> Ptr;
    typedef boost::weak_ptr<Vertex> WeakPtr;

    Vertex() : parent_(0) {}
    /**
    This constructor creates a vertex with a condition leading in it.
    @param condition - condition leading in the vertex
    */
    explicit Vertex(Condition::Ptr condition) :
        condition_(condition), parent_(0) {}
    /**
    Adds child to the vertex.
    @param child - child to add.
    */
    void addChild(Ptr child);
    /** Returns true, if vertex has a next sibling vertex.
    */
    bool hasNextSibling() const;
    /** Returns true, if vertex has a child.
    */
    bool hasChild() const;
    /** Returns a pointer to its sibling vertex.
    */
    Ptr nextSibling() const;
    /** Returns a pointer to the first child of this vertex.
    */
    Ptr firstChild() const;
    /** Removes child from this vertex.
    @param child - child to remove.
    */
    void removeChild(Ptr child);
    /** Sets a condition leading to this vertex.
    @param condition - condition to set.
    */
    void setCondition(Condition::Ptr condition);
    virtual ~Vertex() {}
    /** Returns a condition, leading to the vertex.
    */
    Condition::Ptr condition() {return condition_;}

  protected:
    Ptr first_child_;
    Ptr last_child_;
    Ptr sibling_;
    Vertex<TValue>* parent_;
    Condition::Ptr condition_;
};

template<class TValue>
void Vertex<TValue>::removeChild(typename Vertex<TValue>::Ptr child) {
  if (first_child_ == child) {
    first_child_ = first_child_->nextSibling();
    if (first_child_ == NULL)
      last_child_ = NULL;
    return;
  }
  typename Vertex<TValue>::Ptr now = first_child_;
  while (now != NULL) {
    if (now->nextSibling() == child) {
      now->sibling_ = now->nextSibling()->nextSibling();
      child->parent_ = NULL;
      return;
    }
    now = now->nextSibling();
  }
}

template<class TValue>
bool Vertex<TValue>::hasNextSibling() const {
  return (sibling_ != NULL);
}

template<class TValue>
bool Vertex<TValue>::hasChild() const {
  return (first_child_ != NULL);
}

template<class TValue>
typename Vertex<TValue>::Ptr Vertex<TValue>::nextSibling() const {
  return sibling_;
}

template<class TValue>
typename Vertex<TValue>::Ptr Vertex<TValue>::firstChild() const {
  return first_child_;
}

template<class TValue>
void Vertex<TValue>::setCondition(Condition::Ptr condition) {
  condition_ = condition;
}

template<class TValue>
void Vertex<TValue>::addChild(typename Vertex<TValue>::Ptr child) {
  if (last_child_ == NULL)
    first_child_ = last_child_ = child;
  else
    last_child_ = last_child_->sibling_ = child;
  child->parent_ = this;
}

/**
Decision tree. It contains a pointer to the root Vertex.
*/
template<class TValue>
class DecisionTree : public SerializableFunctor<TValue> {
  public:
    typedef typename Vertex<TValue>::Ptr VertexPtr;
    /**
    Returns the decision of the tree for the given object.
    @param obj - object to decide.
    */
    TValue value(const Object& obj) const {
      return root_->value(obj);
    }
    /**
    Sets the root vertex.
    @param root - new root of the tree.
    */
    void setRoot(VertexPtr root);
    /**
    Removes vertex out of the tree.
    @param vertex - vertex to remove.
    */
    void removeVertex(VertexPtr vertex);

    string generateCppCode(const string& function_name) const {
      return root_->generateCppCode(function_name);
    }
  private:
    VertexPtr root_;
};

template<class TValue>
void DecisionTree<TValue>::setRoot(typename Vertex<TValue>::Ptr root) {
  root_ = root;
}

template<class TValue>
void DecisionTree<TValue>::removeVertex(typename Vertex<TValue>::Ptr vertex) {
  typename Vertex<TValue>::Ptr v = vertex;
  while (v != NULL)
    v = v->parent_;
  if (v != this->root)
    throw std::logic_error("can't remove vertex: vertex from another tree");

  if (vertex == this->root) {
    this->root = typename Vertex<TValue>::Ptr(NULL);
    return;
  }

  vertex->parent_->removeChild(vertex);
}
}
}

#endif  // LTR_DECISION_TREE_DECISION_TREE_H_
