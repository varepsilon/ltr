// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_LEAF_GENERATOR_LEAF_GENERATOR_H_
#define LTR_LEARNERS_DECISION_TREE_LEAF_GENERATOR_LEAF_GENERATOR_H_

#include "ltr/data/data_set.h"

#include "ltr/learners/decision_tree/vertex/leaf_vertex.h"

#include "ltr/interfaces/parameterized.h"

#include "ltr/utility/boost/shared_ptr.h"

namespace ltr {
namespace decision_tree {
/**
 * LeafGenerator is a base class for leaf generators.
 */
class LeafGenerator : public Parameterized {
 public:
  typedef ltr::utility::shared_ptr<LeafGenerator> Ptr;

  virtual ~LeafGenerator() {}

  virtual LeafVertex::Ptr operator()(const DataSet<Object>& data) const = 0;
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_LEAF_GENERATOR_LEAF_GENERATOR_H_
