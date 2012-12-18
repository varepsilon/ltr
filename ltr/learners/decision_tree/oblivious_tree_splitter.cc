// Copyright 2012 Yandex

#include "ltr/learners/decision_tree/oblivious_tree_splitter.h"

#include "ltr/utility/numerical.h"

using ltr::utility::Inf;

namespace ltr {
namespace decision_tree {
ObliviousTreeSplitter::
  ObliviousTreeSplitter(const ParametersContainer& parameters) {
    this->setParameters(parameters);
}

string ObliviousTreeSplitter::toString() const {
  return "Oblivious tree splitter";
}

void ObliviousTreeSplitter::generateNextLayer(
  const vector<VertexWithData>& layer,
  vector<VertexWithData>* next_layer) const {
    double best_layer_split_quality = -Inf;
    vector<Threshold> best_thresholds;

    int number_of_features = layer[0].data.feature_count();
    for (int feature_index = 0; feature_index < number_of_features;
         ++feature_index) {
      double layer_split_quality = 0;
      vector<Threshold> thresholds(layer.size());
      for (int vertex_index = 0; vertex_index < (int)layer.size();
           ++vertex_index) {
        double quality;
        findBestThresholdWithQuality(layer[vertex_index].data,
                                     feature_index,
                                     &thresholds[vertex_index],
                                     &quality);
        layer_split_quality += quality;
      }
      if (layer_split_quality > best_layer_split_quality) {
        best_layer_split_quality = layer_split_quality;
        best_thresholds.swap(thresholds);
      }
    }
    splitLayer(layer, best_thresholds, next_layer);
}
};
};
