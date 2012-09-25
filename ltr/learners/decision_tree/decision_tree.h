// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_DECISION_TREE_H_
#define LTR_LEARNERS_DECISION_TREE_DECISION_TREE_H_

#include <string>
#include <vector>
#include <stdexcept>

#include "ltr/learners/decision_tree/vertex/vertex.h"

#include "ltr/utility/shared_ptr.h"

using std::string;
using std::vector;

namespace ltr {
namespace decision_tree {
/**
 * Decision tree. It contains a pointer to the root Vertex.
 */
class DecisionTree : public SerializableFunctor<double> {
 public:
  typedef ltr::utility::shared_ptr<DecisionTree> Ptr;

  explicit DecisionTree(Vertex::Ptr root)
  : root_(root) {}

  ~DecisionTree();
  /**
   * Returns the decision of the tree for the given object.
   * @param object - object to decide.
   */
  double value(const Object& object) const;
  /**
   * Sets the root vertex.
   * @param root - new root of the tree.
   */
  void setRoot(Vertex::Ptr root);

  string generateCppCode(const string& function_name) const;

 private:
  virtual string getDefaultAlias() const;

  Vertex::Ptr root_;
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_DECISION_TREE_H_
