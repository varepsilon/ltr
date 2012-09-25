// Copyright 2012 Yandex

#include "ltr/learners/decision_tree/leaf_generator/most_common_label_leaf_generator.h"

using ltr::utility::LabelStatisticComputer;

namespace ltr {
namespace decision_tree {
LeafVertex::Ptr MostCommonLabelLeafGenerator::
  operator()(const DataSet<Object>& data) const {
    LabelStatisticComputer label_stat_computer(data);
    return new LeafVertex(label_stat_computer.getMostCommonLabel());
}
};
};
