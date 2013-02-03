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

void ObliviousTreeSplitter::findBestThresholdWithQualityByFeature(
  const vector<VertexWithData>& layer,
  vector<Threshold>& thresholds,
  int feature_index,
  double* layer_split_quality) const {
    *layer_split_quality = 0;
    for (int vertex_index = 0; vertex_index < (int)layer.size();
           ++vertex_index) {
        double quality;
        findBestThresholdWithQuality(layer[vertex_index].data,
                                     feature_index,
                                     &thresholds[vertex_index],
                                     &quality);
        *layer_split_quality += quality;
    }
}

void ObliviousTreeSplitter::generateNextLayer(
  const vector<VertexWithData>& layer,
  vector<VertexWithData>* next_layer) const {
    double best_layer_split_quality;
    vector<Threshold> best_thresholds(layer.size());
    findBestThresholdWithQualityByFeature(layer,
                                          best_thresholds,
                                          0,
                                          &best_layer_split_quality);
    int number_of_features = layer[0].data.feature_count();
    for (int feature_index = 1; feature_index < number_of_features;
         ++feature_index) {
      double layer_split_quality;
      vector<Threshold> thresholds(layer.size());
      findBestThresholdWithQualityByFeature(layer,
                                            thresholds,
                                            feature_index,
                                            &layer_split_quality);
      if (layer_split_quality > best_layer_split_quality) {
        best_layer_split_quality = layer_split_quality;
        best_thresholds.swap(thresholds);
      }
    }
    splitLayer(layer, best_thresholds, next_layer);
}
};
};
