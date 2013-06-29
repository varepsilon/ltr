// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_SPLIT_CRITERIA_SAME_LABEL_STOP_CRITERIA_H_
#define LTR_LEARNERS_DECISION_TREE_SPLIT_CRITERIA_SAME_LABEL_STOP_CRITERIA_H_

#include "ltr/data/data_set.h"

#include "ltr/learners/decision_tree/split_criteria/stop_splitting_criteria.h"

#include "ltr/utility/boost/shared_ptr.h"

namespace ltr {
namespace decision_tree {
/**
 * SameLabelStopSplittingCriteria decides to generate leaf when all
 * actual labels in data are equal or almost equal.
 */
class SameLabelStopSplittingCriteria : public StopSplittingCriteria {
  ALLOW_SHARED_PTR_ONLY_CREATION(SameLabelStopSplittingCriteria)
 public:
  explicit SameLabelStopSplittingCriteria(double label_eps = 0.001)
  : label_eps_(label_eps) {}

  explicit SameLabelStopSplittingCriteria(
        const ParametersContainer& parameters);

  bool needToStopSplitting(const DataSet<Object>& data) const;

 private:
  double label_eps_;
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_SPLIT_CRITERIA_SAME_LABEL_STOP_CRITERIA_H_
