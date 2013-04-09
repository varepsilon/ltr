// Copyright 2011 Yandex

#include "ltr/scorers/nearest_neighbor_scorer.h"

#include <rlog/rlog.h>

#include <algorithm>
#include <string>
#include <sstream>

#include "ltr/data/object.h"
#include "ltr/utility/boost/lexical_cast.h"

using std::string;
using std::pair;
using std::make_pair;
using std::min;
using std::nth_element;
using std::sort;

using ltr::utility::lexical_cast;

using ltr::NNScorer;
using ltr::Object;

namespace ltr {
string NNScorer::toStringImpl() const {
  string result = "Nearest neigbor scorer, that processes ";
  result += lexical_cast<string>(number_of_neighbors_to_process_);
  result += " nearest neighbors";
  return result;
}

struct DistanceToNeighbor {
  double distance;
  int neighbor_id;

  bool operator<(const DistanceToNeighbor& arg) const {
    return this->distance < arg.distance;
  }
};

double NNScorer::scoreImpl(const Object& object) const {
  rDebug("Starting to score an object with the NNScorer");

  rDebug("Starting to find distances to neighbors");
  vector<DistanceToNeighbor> distances_to_neighbors(data_.size());
  for (int neighbor_index = 0;
       neighbor_index < data_.size();
       ++neighbor_index) {
    distances_to_neighbors[neighbor_index].distance =
      metric_->distance(object, data_[neighbor_index]);
    distances_to_neighbors[neighbor_index].neighbor_id = neighbor_index;
  }

  int k_neighbors = min(number_of_neighbors_to_process_,
                        (int)(data_.size()));

  nth_element(distances_to_neighbors.begin(),
              distances_to_neighbors.begin() + k_neighbors,
              distances_to_neighbors.end());

  sort(distances_to_neighbors.begin(),
       distances_to_neighbors.begin() + k_neighbors);

  vector<double> labels(k_neighbors);
  for (int label_index = 0; label_index < k_neighbors; ++label_index) {
    labels[label_index] =
      data_[distances_to_neighbors[label_index].neighbor_id].actual_label();
  }

  vector<double> weights(k_neighbors);
  for (int weight_index = 0; weight_index < k_neighbors; ++weight_index) {
    weights[weight_index] =
      neighbor_weighter_->
        getWeight(distances_to_neighbors[weight_index].distance,
                  weight_index);

    weights[weight_index] *=
      data_.getWeight(distances_to_neighbors[weight_index].neighbor_id);
  }
  return predictions_aggregator_->aggregate(labels, weights);
}

string NNScorer::generateCppCodeImpl(const string& function_name) const {
  stringstream code;
  code
    << metric_->generateCppCode("distance")
    << predictions_aggregator_->generateCppCode("aggregator")
    << neighbor_weighter_->generateCppCode("weighter")

    << "double " << function_name << "(const std::vector<double>& features) {\n"
    << "  Object object;\n"
    << "  for (int index = 0; index < features.size(); ++index) {\n"
    << "    object << features[index];\n"
    << "  }\n"
    << "  DataSet<Object> data;\n";
  for (int neighbor_index = 0;
       neighbor_index < data_.size();
       ++neighbor_index) {
    string object_name = "object" + lexical_cast<string>(neighbor_index);
    code
      << "  Object " << object_name << ";\n";
    for (int feature_index = 0;
         feature_index < data_[neighbor_index].feature_count();
         ++feature_index) {
      code
        << "  " << object_name << " << "
          << lexical_cast<string>(data_[neighbor_index][feature_index])
          << ";\n";
    }
    code
      << "  " << object_name << ".set_actual_label("
        << lexical_cast<string>(data_[neighbor_index].actual_label()) << ");\n"
      << "  data.add(" + object_name + ");\n";
  }

  code
    << "  int number_of_neighbors_to_process = "
      << lexical_cast<string>(number_of_neighbors_to_process_) << ";\n"

    << "  vector<std::pair<double, int> > "
      << "distances_to_neighbors(data.size());\n"
    << "  for (int neighbor_index = 0;\n"
    << "       neighbor_index < data.size();\n"
    << "       ++neighbor_index) {\n"
    << "    distances_to_neighbors[neighbor_index].first =\n"
    << "      distance(object, data[neighbor_index]);\n"
    << "    distances_to_neighbors[neighbor_index].second = neighbor_index;\n"
    << "  }\n"

    << " int k_neighbors = std::min(number_of_neighbors_to_process,\n"
    << "                            (int)(data.size()));\n"

    << "  nth_element(distances_to_neighbors.begin(),\n"
    << "              distances_to_neighbors.begin() + k_neighbors,\n"
    << "              distances_to_neighbors.end());\n"

    << "  sort(distances_to_neighbors.begin(),\n"
    << "       distances_to_neighbors.begin() + k_neighbors);\n"

    << "  vector<double> labels(k_neighbors);\n"
    << "  for (int label_index = 0;\n"
    << "       label_index < k_neighbors;\n"
    << "       ++label_index) {\n"
    << "    labels[label_index] =\n"
    << "      data[distances_to_neighbors[label_index].second]"
      << ".actual_label();\n"
    << "  }\n"

    << "  vector<double> weights(k_neighbors);\n"
    << "  for (int weight_index = 0;\n"
    << "       weight_index < k_neighbors;\n"
    << "       ++weight_index) {\n"
    << "    weights[weight_index] =\n"
    << "      weighter(distances_to_neighbors[weight_index].first,\n"
    << "      weight_index);\n"
    << "  }\n"
    << "  return aggregator(labels, weights);\n"
    << "}\n";
  return code.str();
}
};
