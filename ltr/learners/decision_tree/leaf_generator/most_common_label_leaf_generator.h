// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_LEAF_GENERATOR_MOST_COMMON_LABEL_LEAF_GENERATOR_H_
#define LTR_LEARNERS_DECISION_TREE_LEAF_GENERATOR_MOST_COMMON_LABEL_LEAF_GENERATOR_H_

#include "ltr/data/data_set.h"

#include "ltr/data/utility/data_set_statistics.h"

#include "ltr/learners/decision_tree/vertex/leaf_vertex.h"

#include "ltr/learners/decision_tree/leaf_generator/leaf_generator.h"

#include "ltr/utility/boost/shared_ptr.h"

namespace ltr {
namespace decision_tree {
/**
 * MostCommonLabelLeafGenerator generate the leaf with value
 * equals to the most common actual label in given DataSet.
 */
class MostCommonLabelLeafGenerator : public LeafGenerator {
 public:
  typedef ltr::utility::shared_ptr<MostCommonLabelLeafGenerator> Ptr;

  LeafVertex::Ptr operator()(const DataSet<Object>& data) const;
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_LEAF_GENERATOR_MOST_COMMON_LABEL_LEAF_GENERATOR_H_
