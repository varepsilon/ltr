// Copyright 2011 Yandex

#include <sstream>

#include "ltr/scorers/linear_composition_scorer.h"

using std::string;

namespace ltr {
  double LinearCompositionScorer::scoreImpl(const Object& obj) const {
    double result = 0.0;
    for (size_t i = 0; i < scorers_.size(); ++i) {
      result += scorers_[i].weight * ((*scorers_[i].scorer)(obj));
    }
    return result;
  }

  string LinearCompositionScorer::generateCppCodeImpl
      (const string& function_name) const {
    string hpp_code;

    for (size_t i = 0; i < scorers_.size(); ++i)
      hpp_code.append(scorers_[i].scorer->generateCppCode());

    hpp_code.
      append("inline double ").
      append(function_name).
      append("(const std::vector< double >& features) {\n").
      append("\tdouble result = 0.0;\n");

    for (size_t i = 0; i < scorers_.size(); ++i) {
      hpp_code.
        append("\tresult += ").
        append(boost::lexical_cast< string >(scorers_[i].weight)).
        append(" * ").
        append(scorers_[i].scorer->getDefaultSerializableObjectName()).
        append("(features);\n");
    }

    hpp_code.
      append("\treturn result;\n").
      append("};\n");

    return hpp_code;
  }

  string LinearCompositionScorer::toString() const {
    std::stringstream str;
    std::fixed(str);
    str.precision(2);
    str << "Linear composition of " << scorers_.size() << "scorers: {\n";
    for (int i = 0; i < scorers_.size(); ++i) {
      str << scorers_[i].scorer->toString();
      str << " with weight " << scorers_[i].weight << "\n";
    }
    str << "}";
    return str.str();
  }

  void LinearCompositionScorer::clear() {
    scorers_.clear();
  }

  size_t LinearCompositionScorer::size() const {
    return scorers_.size();
  }

  const LinearCompositionScorer::ScorerAndWeight&
      LinearCompositionScorer::at(size_t i) const {
    return scorers_.at(i);
  }
  LinearCompositionScorer::ScorerAndWeight&
      LinearCompositionScorer::at(size_t i) {
    return scorers_.at(i);
  }
  const LinearCompositionScorer::ScorerAndWeight&
      LinearCompositionScorer::operator[](size_t i) const {
    return at(i);
  }
  LinearCompositionScorer::ScorerAndWeight&
      LinearCompositionScorer::operator[](size_t i) {
    return at(i);
  }

  void LinearCompositionScorer::add
      (const ScorerAndWeight& scorer_and_weight) {
    scorers_.push_back(scorer_and_weight);
  }

  void LinearCompositionScorer::add
      (Scorer::Ptr scorer, double weight) {
    scorers_.push_back(ScorerAndWeight(scorer, weight));
  }
};
