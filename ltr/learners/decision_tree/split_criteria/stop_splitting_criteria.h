// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_SPLIT_CRITERIA_STOP_SPLITTING_CRITERIA_H_
#define LTR_LEARNERS_DECISION_TREE_SPLIT_CRITERIA_STOP_SPLITTING_CRITERIA_H_

#include "ltr/data/data_set.h"

#include "ltr/utility/shared_ptr.h"

namespace ltr {
namespace decision_tree {
/**
 * StopSplittingCriteria is a base class for stop splitting criterias.
 * It checks the given data and desides should we generate leaf now or not.
 */
class StopSplittingCriteria {
 public:
  typedef ltr::utility::shared_ptr<StopSplittingCriteria> Ptr;

  virtual ~StopSplittingCriteria() {}

  virtual bool needToStopSplitting(const DataSet<Object>& data) const = 0;
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_SPLIT_CRITERIA_STOP_SPLITTING_CRITERIA_H_
