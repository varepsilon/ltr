// Copyright 2011 Yandex

#ifndef LTR_DECISION_TREE_DECISION_TREE_H_
#define LTR_DECISION_TREE_DECISION_TREE_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <map>
#include <cmath>
#include <vector>
#include <stdexcept>

#include "ltr/data/object.h"

using std::map;
using std::vector;

namespace ltr {
namespace DecisionTree {

extern bool _cache_conditions;
extern bool _deep_cache_conditions;

class Condition {
  public:
    typedef boost::shared_ptr<Condition> Ptr;
    double operator()(const Object& obj) const;
    double value(const Object& obj) const;

    virtual ~Condition() {}
  private:
    virtual double valueImpl(const Object& obj) const = 0;
    mutable Object last_object_;
    mutable double last_value_;
};

template <class TValue>
class Vertex {
  public:
    typedef boost::shared_ptr<Vertex> Ptr;
    typedef boost::weak_ptr<Vertex> WeakPtr;


    Vertex() : parent_(0) {}
    explicit Vertex(Condition::Ptr condition) :
        condition_(condition), parent_(0) {}
    void addChild(Ptr child);
    bool hasNextSibling() const;
    bool hasChild() const;
    Ptr nextSibling() const;
    Ptr firstChild() const;

    void removeChild(Ptr child);

    virtual TValue value(const Object& obj) const = 0;

    void setCondition(Condition::Ptr condition);

    virtual ~Vertex() {}

  protected:
    Ptr first_child_;
    Ptr last_child_;
    Ptr sibling_;
    Vertex<TValue>* parent_;
    Condition::Ptr condition_;

    template<class TValue1>
    friend class DecisionVertex;

    template<class TValue1>
    friend class RegressionVertex;

    template<class TValue1>
    friend class LeafVertex;
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
  return condition_ = condition;
}

template<class TValue>
void Vertex<TValue>::addChild(typename Vertex<TValue>::Ptr child) {
  if (last_child_ == NULL)
    first_child_ = last_child_ = child;
  else
    last_child_ = last_child_->sibling_ = child;
  child->parent_ = this;
}

template <class TValue>
class LeafVertex : public Vertex<TValue> {
  public:
    typedef typename boost::shared_ptr<LeafVertex<TValue> > Ptr;
    TValue value(const Object& obj) const {
      return value_;
    }
    LeafVertex() : Vertex<TValue>() {}
    LeafVertex(Condition::Ptr condition, const TValue& value) :
        value_(value), Vertex<TValue>(condition) {}
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

template <class TValue>
class DecisionVertex : public Vertex<TValue> {
  public:
    typedef boost::shared_ptr<DecisionVertex<TValue> > Ptr;

    DecisionVertex() : Vertex<TValue>() {}
    explicit DecisionVertex(Condition::Ptr condition) :
        Vertex<TValue>(condition) {}
    TValue value(const Object& obj) const;
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
class RegressionVertex : public Vertex<TValue> {
  public:
    typedef boost::shared_ptr<RegressionVertex<TValue> > Ptr;

    RegressionVertex() : Vertex<TValue>() {}
    explicit RegressionVertex(Condition::Ptr condition) :
        Vertex<TValue>(condition) {}
    TValue value(const Object& obj) const;
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
TValue DecisionVertex<TValue>::value(const Object& obj) const {
  typename Vertex<TValue>::Ptr best_child;
  double max_value = 0;
  if (!this->hasChild())
    throw std::logic_error("non list vertex has no children");
  typename Vertex<TValue>::Ptr child = this->firstChild();
  while (child != NULL) {
    if (best_child == NULL || max_value < child->condition_->value(obj)) {
      best_child = child;
      max_value = child->condition_->value(obj);
    }
    child = child->nextSibling();
  }
  return best_child->value(obj);
}

template <class TValue>
TValue RegressionVertex<TValue>::value(const Object& obj) const {
  if (!this->hasChild())
    throw std::logic_error("non list vertex has no children");
  typename Vertex<TValue>::Ptr child = this->firstChild();

  vector<double> conditions;
  vector<TValue> values;
  double sum = 0;
  while (child != NULL) {
    conditions.push_back(std::fabs(child->condition_->dicide(obj)));
    sum += std::fabs(child->condition_->dicide(obj));
    values.push_back(child->value(obj));
    child = child->nextSibling();
  }
  TValue result;
  if (sum == 0) {
    result = values[0];
    for (int i = 1; i < values.size(); i++)
      result = result + values[i];
    result = result / values.size();
    return result;
  }
  result = values[0] * conditions[0];
  for (int i = 1; i < values.size(); i++)
    result = result + values[i] * conditions[i];
  return result;
}

template<class TValue>
class DecisionTree {
  public:
    typedef typename Vertex<TValue>::Ptr VertexPtr;
    TValue value(const Object& obj) {
      return root_->value(obj);
    }
    void setRoot(VertexPtr root);
    void removeVertex(VertexPtr vertex);
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
