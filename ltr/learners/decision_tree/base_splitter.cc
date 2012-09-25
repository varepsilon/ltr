// Copyright 2012 Yandex

#include <algorithm>
#include <vector>

#include "ltr/learners/decision_tree/condition/threshold_condition.h"
#include "ltr/learners/decision_tree/vertex/decision_vertex.h"
#include "ltr/learners/decision_tree/base_splitter.h"

#include "ltr/utility/numerical.h"

using std::vector;
using std::sort;
using std::pair;
using std::max_element;

using ltr::utility::Inf;
using ltr::utility::DoubleEqual;
using ltr::utility::DoubleLessOrEqual;
using ltr::utility::getDataSetEntropy;

namespace ltr {
namespace decision_tree {
struct FeatureWithLabelAndWeight {
  FeatureWithLabelAndWeight() {}

  FeatureWithLabelAndWeight(double feature,
                            double label,
                            double weight)
  : feature(feature),
    label(label),
    weight(weight) {}

  bool operator<(const FeatureWithLabelAndWeight& other) const {
    return feature < other.feature;
  }

  double feature;
  double label;
  double weight;
};

void BaseSplitter::findBestThresholdWithQuality(const DataSet<Object>& data,
                                                int feature_index,
                                                Threshold* best_threshold,
                                                double* quality) const {
  // get sorted list of interested feature value
  // with corresponding labels and weights
  vector<FeatureWithLabelAndWeight> feature_with_label_and_weight(data.size());
  for (int object_index = 0; object_index < data.size(); ++object_index) {
    feature_with_label_and_weight[object_index] =
      FeatureWithLabelAndWeight(
        data[object_index][feature_index],
        data[object_index].actual_label(),
        data.getWeight(object_index));
  }

  sort(feature_with_label_and_weight.begin(),
       feature_with_label_and_weight.end());

  // initial parameters for split
  LabelStatisticComputer all_label_stat_computer(data);
  LabelStatisticComputer first_part_label_stat_computer
    = all_label_stat_computer;
  LabelStatisticComputer second_part_label_stat_computer;

  *best_threshold =
    Threshold(feature_index, feature_with_label_and_weight[0].feature);
  *quality = -Inf;

  // try all split positions
  for (int split_position = 1;
       split_position < (int)feature_with_label_and_weight.size();
       ++split_position) {
    FeatureWithLabelAndWeight
      left_threshold_neighbor =
        feature_with_label_and_weight[split_position - 1],
      right_threshold_neighbor =
        feature_with_label_and_weight[split_position];

    first_part_label_stat_computer.remove(left_threshold_neighbor.label,
                                          left_threshold_neighbor.weight);
    second_part_label_stat_computer.add(left_threshold_neighbor.label,
                                        left_threshold_neighbor.weight);

    if (DoubleEqual(left_threshold_neighbor.feature,
                    right_threshold_neighbor.feature)) {
      continue;
    }

    double threshold =
      (left_threshold_neighbor.feature + right_threshold_neighbor.feature) / 2;

    // update best threshold and quality if needed
    double current_quality = computeQuality(all_label_stat_computer,
                                            first_part_label_stat_computer,
                                            second_part_label_stat_computer);
    if (current_quality > *quality) {
      *best_threshold = Threshold(feature_index, threshold);
      *quality = current_quality;
    }
  }
}

void BaseSplitter::splitLayer(const vector<VertexWithData>& layer,
                              const vector<Threshold>& thresholds,
                              vector<VertexWithData>* next_layer) const {
  next_layer->clear();

  for (int vertex_index = 0; vertex_index < (int)layer.size();
       ++vertex_index) {
    vector<VertexWithData> children;
    splitVertex(layer[vertex_index],
                thresholds[vertex_index],
                &children);
    for (int children_index = 0; children_index < (int)children.size();
         ++children_index) {
      next_layer->push_back(children[children_index]);
    }
  }
}

void BaseSplitter::splitVertex(const VertexWithData& vertex,
                               const Threshold& threshold,
                               vector<VertexWithData>* children) const {
  children->clear();

  DataSet<Object> first_part_of_data, second_part_of_data;
  for (int object_index = 0; object_index < vertex.data.size();
       ++object_index) {
    if (DoubleLessOrEqual(
      vertex.data[object_index][threshold.feature_index], threshold.value)) {
        first_part_of_data.add(vertex.data[object_index]);
    } else {
      second_part_of_data.add(vertex.data[object_index]);
    }
  }

  if (first_part_of_data.size() != 0) {
    DecisionVertex::Ptr first_child = new DecisionVertex;
    vertex.vertex->addChild(first_child,
      new ThresholdCondition(threshold.feature_index, -Inf, threshold.value));
    children->push_back(VertexWithData(first_child, first_part_of_data));
  }

  if (second_part_of_data.size() != 0) {
    DecisionVertex::Ptr second_child = new DecisionVertex;
    vertex.vertex->addChild(second_child,
      new ThresholdCondition(threshold.feature_index, threshold.value, Inf));
    children->push_back(VertexWithData(second_child, second_part_of_data));
  }
}

double BaseSplitter::computeQuality(
  const LabelStatisticComputer& all_label_stat_computer,
  const LabelStatisticComputer& first_part_computer,
  const LabelStatisticComputer& second_part_computer) const {
    double all_entropy = getDataSetEntropy(all_label_stat_computer);
    double first_part_entropy = getDataSetEntropy(first_part_computer);
    double second_part_entropy = getDataSetEntropy(second_part_computer);

    double data_size = all_label_stat_computer.getHoldingDataSize();
    double first_part_size = first_part_computer.getHoldingDataSize();
    double second_part_size = second_part_computer.getHoldingDataSize();

    return all_entropy -
      (first_part_entropy * first_part_size +
       second_part_entropy * second_part_size) / data_size;
}
};
};
