// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_CONDITION_CONDITION_H_
#define LTR_LEARNERS_DECISION_TREE_CONDITION_CONDITION_H_

#include <string>

#include "ltr/data/object.h"

#include "ltr/interfaces/serializable.h"

#include "ltr/utility/boost/shared_ptr.h"

using ltr::utility::lexical_cast;

namespace ltr {
namespace decision_tree {
/**
 * Condition is a base class for decision tree conditions.
 * It returns a value for tested Object.
 */
class Condition : public Serializable {
 public:
  typedef ltr::utility::shared_ptr<Condition> Ptr;

  virtual ~Condition() {}
  /*
   * Returns the confidence that object satisfied this condition.
   */
  virtual double value(const Object& object) const = 0;
};

class FakeCondition : public Condition {
 public:
  typedef ltr::utility::shared_ptr<FakeCondition> Ptr;

  FakeCondition() {}

  double value(const Object& object) const {return 1.0;}

  virtual string generateCppCode(const string& function_name) const {
    string hpp_code;

    hpp_code.
      append("inline double ").
      append(function_name + "_id_" + lexical_cast<string>(this->getId())).
      append("(const std::vector<double>& features) {return 1;}\n\n");

    return hpp_code;
  }
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_CONDITION_CONDITION_H_
