// Copyright 2011 Yandex

#include <rlog/rlog.h>
#include <sstream>


#include "ltr/scorers/composition_scorers/composition_scorer.h"

using std::string;
using std::ostringstream;

namespace ltr {
namespace composition {
  void CompositionScorer::clear() {
    weighted_scorers_.clear();
  }

  size_t CompositionScorer::size() const {
    return weighted_scorers_.size();
  }

  const CompositionScorer::ScorerAndWeight&
      CompositionScorer::at(int scorer_index) const {
    rDebug("Getting scorer at %d index", scorer_index);
    return weighted_scorers_.at(scorer_index);
  }
  CompositionScorer::ScorerAndWeight&
      CompositionScorer::at(int scorer_index) {
    rDebug("Getting scorer at %d index", scorer_index);
    return weighted_scorers_.at(scorer_index);
  }
  const CompositionScorer::ScorerAndWeight&
      CompositionScorer::operator[](int scorer_index) const {
    rDebug("Setting scorer at %d index", scorer_index);
    return at(scorer_index);
  }
  CompositionScorer::ScorerAndWeight&
      CompositionScorer::operator[](int scorer_index) {
    rDebug("Setting scorer at %d index", scorer_index);
    return at(scorer_index);
  }

  void CompositionScorer::add
      (const ScorerAndWeight& weighted_scorer) {
    rDebug("Adding a scorer and weight equal to %lf", weighted_scorer.weight);
    weighted_scorers_.push_back(weighted_scorer);
  }

  void CompositionScorer::add
      (Scorer::Ptr scorer, double weight) {
    add(ScorerAndWeight(scorer, weight));
  }

  double CompositionScorer::scoreImpl(const Object& object) const {
    vector<double> weights(size()), labels(size());
    for (int index = 0; index < size(); ++index) {
      weights[index] = at(index).weight;
      labels[index] = at(index).scorer->score(object);
    }
    return predictions_aggregator_->aggregate(labels, weights);
  }

  string CompositionScorer::generateCppCodeImpl(
      const string& function_name) const {
    rInfo("Starting to generate CPP code for CompositionScorer");
    ostringstream hpp_code;
    for (int index = 0; index < size(); ++index) {
      ostringstream scorer_name;
      scorer_name <<  function_name << "_weakscorer_" << index;
      hpp_code << at(index).scorer->generateCppCode(scorer_name.str());
    }
    hpp_code << predictions_aggregator_->generateCppCode(function_name
      + "_aggregator");
    hpp_code << "inline double " << function_name
      << "(const std::vector<double>& features) {\n";
    hpp_code << "  vector<double> weights(" << size()
      << "), labels(" << size() << ");\n";
    for (int index = 0; index < size(); ++index) {
      ostringstream scorer_name;
      scorer_name <<  function_name << "_weakscorer_" << index;
      hpp_code << "  weights[" << index << "] = "
        << at(index).weight << ";\n";
      hpp_code << "  labels[" << index << "] = "
        << scorer_name.str() << "(features);\n";
    }
    hpp_code << "  return " << function_name
      << "_aggregator(labels, weights);\n}\n";
    return hpp_code.str();
  }
};
};
