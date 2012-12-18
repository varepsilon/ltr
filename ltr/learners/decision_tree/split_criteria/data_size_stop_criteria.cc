// Copyright 2012 Yandex

#include "ltr/learners/decision_tree/split_criteria/data_size_stop_criteria.h"

#include "ltr/utility/numerical.h"

using ltr::utility::Inf;

namespace ltr {
namespace decision_tree {
DataSizeStopSplittingCriteria::
  DataSizeStopSplittingCriteria(const ParametersContainer& parameters) {
    this->setParameters(parameters);
}

bool DataSizeStopSplittingCriteria::
  needToStopSplitting(const DataSet<Object>& data) const {
    if (data.size() <= min_vertex_size_) {
      return true;
    }
    return false;
}
};
};
