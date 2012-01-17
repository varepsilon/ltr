// Copyright 2011 Yandex

#ifndef LTR_DECISION_TREE_DECISION_TREE_H_
#define LTR_DECISION_TREE_DECISION_TREE_H_

#include <boost/shared_ptr.hpp>

#include <map>
#include <vector>
#include <stdexcept>

#include "ltr/data/object.h"

using std::map;
using std::vector;

namespace ltr {
namespace DecisionTree {

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

    Vertex();
    explicit Vertex(Condition::Ptr condition) : condition_(condition) {}
    void addChild(Ptr child);
    bool hasNextSibling();
    bool hasChild();
    Ptr nextSibling();
    Ptr firstChild();

    void removeChild(Ptr child);

    virtual TValue value(const Object& obj) const = 0;

    void setCondition(Condition::Ptr condition);

    virtual ~Vertex() {}

  private:
    Ptr first_child_;
    Ptr last_child_;
    Ptr sibling_;
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
  Vertex<TValue>::Ptr now = first_child_;
  while (now != NULL) {
    if (now->nextSibling() == child) {
      now->sibling_ = now->nextSibling()->nextSibling();
      return;
    }
    now = now->nextSibling();
  }
}

template<class TValue>
bool Vertex<TValue>::hasNextSibling() {
  return (sibling != NULL);
}

template<class TValue>
bool Vertex<TValue>::hasChild() {
  return (first_child_ != NULL);
}

template<class TValue>
typename Vertex<TValue>::Ptr Vertex<TValue>::nextSibling() {
  return sibling;
}

template<class TValue>
typename Vertex<TValue>::Ptr Vertex<TValue>::firstChild() {
  return first_child_;
}

template<class TValue>
void Vertex<TValue>::setCondition(Condition::Ptr condition) {
  return condition_ = condition;
}

template<class TValue>
void Vertex<TValue>::addChild(typename Vertex<TValue>::Ptr child) {
  if (last_child == NULL)
    first_child = last_child_ = child;
  else
    last_child = last_child->sibling = child;
}

template<class TValue>
class DecisionTree {
  public:
    typedef typename Vertex<TValue>::Ptr VertexPtr;
    TValue value(const Object& obj) {
      return root_->value(obj);
    }
    void setRoot(VertexPtr root);
  private:
    VertexPtr root_;
};

template <class TValue>
class LeafVertex : public Vertex<TValue> {
  public:
    TValue value(const Object& obj) const {
      return value_;
    }
    LeafVertex() {}
    LeafVertex(Condition::Ptr condition, const TValue& value) :
        value_(value), Vertex<TValue>(condition) {}
  private:
    TValue value_;
};

template <class TValue>
class DecisionVertex : public Vertex<TValue> {
  public:
    TValue value(const Object& obj) const;
};

template <class TValue>
class RegressionVertex : public Vertex<TValue> {
  public:
    TValue value(const Object& obj) const;
};

template <class TValue>
TValue DecisionVertex<TValue>::value(const Object& obj) const {
  Vertex<TValue>::Ptr best_child = NULL;
  double max_value = 0;
  if (!hasChild())
    throw std::logic_error("non list vertex has no children");
  Vertex<TValue>::Ptr child = firstChild();
  while (child != NULL) {
    if (best_child == NULL || max_value < child->condition_->value(obj)) {
      best_child = child;
      max_value = child->condition->value(obj);
    }
    child = child->nextSibling();
  }
  return best_child->value(obj);
}

template <class TValue>
TValue RegressionVertex<TValue>::value(const Object& obj) const {
  if (!hasChild())
    throw std::logic_error("non list vertex has no children");
  Vertex<TValue>::Ptr child = firstChild();

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
void DecisionTree<TValue>::setRoot(typename Vertex<TValue>::Ptr root) {
  root_ = root;
}

}
}

#endif  // LTR_DECISION_TREE_DECISION_TREE_H_
