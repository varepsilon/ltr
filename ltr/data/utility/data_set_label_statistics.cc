// Copyright 2012 Yandex

#include <stdexcept>

#include "ltr/data/utility/data_set_label_statistics.h"

using std::logic_error;

namespace ltr {
namespace utility {
void LabelStatisticComputer::add(double label, double weight) {
  if (labels_id_.find(label) == labels_id_.end()) {
    labels_id_[label] = label_statistics_.size();
    label_statistics_.push_back(LabelStatistic(label, 1, weight));
  } else {
    ++label_statistics_[labels_id_[label]].count;
    label_statistics_[labels_id_[label]].weight += weight;
  }
}

void LabelStatisticComputer::remove(double label, double weight) {
  if (labels_id_.find(label) == labels_id_.end() ||
    label_statistics_[labels_id_[label]].count == 0) {
      throw logic_error(
        "there is no object with that label in LabelStatisticComputer");
  }
  --label_statistics_[labels_id_[label]].count;
  label_statistics_[labels_id_[label]].weight -= weight;
}

void LabelStatisticComputer::reset() {
  labels_id_.clear();
  label_statistics_.clear();
}

double LabelStatisticComputer::getMostCommonLabel() const {
  int biggest_count = 0;
  double most_common_label;
  for (int stat_index = 0;
       stat_index < (int)label_statistics_.size(); ++stat_index ) {
    if (label_statistics_[stat_index].count > biggest_count) {
      biggest_count = label_statistics_[stat_index].count;
      most_common_label = label_statistics_[stat_index].label;
    }
  }
  return most_common_label;
}

double LabelStatisticComputer::getHeaviestLabel() const {
  int biggest_weight = 0;
  double heaviest_label;
  for (int stat_index = 0;
       stat_index < (int)label_statistics_.size(); ++stat_index ) {
    if (label_statistics_[stat_index].weight > biggest_weight) {
      biggest_weight = label_statistics_[stat_index].weight;
      heaviest_label = label_statistics_[stat_index].label;
    }
  }
  return heaviest_label;
}

int LabelStatisticComputer::getHoldingDataSize() const {
  int data_size = 0;
  for (int stat_index = 0;
       stat_index < (int)label_statistics_.size(); ++stat_index ) {
         data_size += label_statistics_[stat_index].count;
  }
  return data_size;
}

int LabelStatisticComputer::getLabelsCount() const {
  return label_statistics_.size();
}

int LabelStatisticComputer::getLabelSizeById(int id) const {
  return label_statistics_[id].count;
}

double LabelStatisticComputer::getLabelWeightById(int id) const {
  return label_statistics_[id].weight;
}
};
};
