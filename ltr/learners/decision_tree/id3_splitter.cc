// Copyright 2012 Yandex

#include "ltr/learners/decision_tree/id3_splitter.h"

#include "ltr/utility/numerical.h"

using ltr::utility::Inf;

namespace ltr {
namespace decision_tree {
ID3Splitter::ID3Splitter(const ParametersContainer& parameters) {
  this->setParameters(parameters);
}

string ID3Splitter::toString() const {
  return "ID3 tree splitter";
}

void ID3Splitter::generateNextLayer(const vector<VertexWithData>& layer,
  vector<VertexWithData>* next_layer) const {
    vector<Threshold> best_thresholds(layer.size());
    for (int vertex_index = 0; vertex_index < (int)layer.size();
         ++vertex_index) {
      double best_quality = -Inf;
      Threshold threshold;
      for (int feature_index = 0;
           feature_index < layer[vertex_index].data.feature_count();
           ++feature_index) {
        double quality;
        findBestThresholdWithQuality(layer[vertex_index].data,
                                     feature_index,
                                     &threshold,
                                     &quality);
        if (quality > best_quality) {
          best_quality = quality;
          best_thresholds[vertex_index] = threshold;
        }
      }
    }
    splitLayer(layer, best_thresholds, next_layer);
}
};
};
