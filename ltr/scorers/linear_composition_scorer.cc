// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>

#include "scorers/linear_composition_scorer.h"

namespace ltr {
  double LinearCompositionScorer::operator()(const Object& obj) const {
    double result = 0.0;
    for (size_t i = 0; i < scorers_.size(); ++i) {
      result += scorers_[i].weight * ((*scorers_[i].scorer)(obj));
    }
    return result;
  }

  string LinearCompositionScorer::generateCppCode
      (const string& function_name) const {
    std::string hpp_code;

    /*std::string tab_str(0, '\t');

    hpp_code.
      append(tab_str).
      append("class ").
      append(class_name).
      append(" {\n");

    for (size_t i = 0; i < scorers_.size(); ++i) {
      hpp_code.append(
        scorers_[i].scorer->generateCppCode(
        utility::GenerateLocalClassName(*(scorers_[i].scorer), i),
        tabbing + 1));
    }

    hpp_code.
      append(tab_str).
      append("public:\n").
      append(tab_str).
      append("\tstatic double score").
      append("(const std::vector< double >& features) {\n").
      append(tab_str).
      append("\t\tdouble result = 0.0;\n");

    for (size_t i = 0; i < scorers_.size(); ++i) {
      hpp_code.
        append(tab_str).
        append("\t\tresult += ").
        append(boost::lexical_cast< std::string >(scorers_[i].weight)).
        append(" * ").
        append(utility::GenerateLocalClassName(*(scorers_[i].scorer), i)).
        append("::score(features);\n");
    }

    hpp_code.
      append(tab_str).
      append("\t\treturn result;\n").
      append(tab_str).
      append("\t}\n").
      append(tab_str).
      append("};\n");
    */
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
