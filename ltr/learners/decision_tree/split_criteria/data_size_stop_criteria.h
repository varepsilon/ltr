// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_SPLIT_CRITERIA_DATA_SIZE_STOP_CRITERIA_H_
#define LTR_LEARNERS_DECISION_TREE_SPLIT_CRITERIA_DATA_SIZE_STOP_CRITERIA_H_

#include "ltr/data/data_set.h"

#include "ltr/learners/decision_tree/split_criteria/stop_splitting_criteria.h"

#include "ltr/utility/boost/shared_ptr.h"

namespace ltr {
namespace decision_tree {
/**
 * DataSizeStopSplittingCriteria decides to generate leaf when
 * data size less then min_vertex_size.
 */
class DataSizeStopSplittingCriteria : public StopSplittingCriteria {
 public:
  typedef ltr::utility::shared_ptr<DataSizeStopSplittingCriteria> Ptr;

  explicit DataSizeStopSplittingCriteria(int min_vertex_size = 5)
  : min_vertex_size_(min_vertex_size) {}

  bool needToStopSplitting(const DataSet<Object>& data) const;

 private:
  int min_vertex_size_;
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_SPLIT_CRITERIA_DATA_SIZE_STOP_CRITERIA_H_
