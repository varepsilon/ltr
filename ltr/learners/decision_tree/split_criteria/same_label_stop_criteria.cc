// Copyright 2012 Yandex

#include "ltr/learners/decision_tree/split_criteria/same_label_stop_criteria.h"

#include "ltr/utility/numerical.h"

using ltr::utility::Inf;

namespace ltr {
namespace decision_tree {
bool SameLabelStopSplittingCriteria::
  needToStopSplitting(const DataSet<Object>& data) const {
    double min_label = Inf;
    double max_label = -Inf;

    for (int object_index = 0;
      object_index < (int)data.size(); object_index++) {
        min_label = std::min(min_label, data[object_index].actual_label());
        max_label = std::max(max_label, data[object_index].actual_label());
    }

    if (max_label - min_label <= label_eps_) {
      return true;
    }
    return false;
}
};
};
