// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>

#include "ltr/scorers/linear_composition_scorer.h"

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
    std::string hpp_code;

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
        append(boost::lexical_cast< std::string >(scorers_[i].weight)).
        append(" * ").
        append(scorers_[i].scorer->getDefaultSerializableObjectName()).
        append("(features);\n");
    }

    hpp_code.
      append("\treturn result;\n").
      append("};\n");

    return hpp_code;
  }

  std::string LinearCompositionScorer::brief() const {
    std::string result =
      boost::lexical_cast<std::string>(scorers_.size())
      + " scorers combination.";
    for (size_t i = 0; i < scorers_.size(); ++i) {
      result.append("\n" + boost::lexical_cast<std::string>(i) + ": "
        + scorers_[i].scorer->brief() + "\n"
        + "    weight=" + boost::lexical_cast<std::string>(scorers_[i].weight));
    }
    return result;
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
