// Copyright 2011 Yandex

#ifndef LTR_SCORERS_FAKE_SCORER_H_
#define LTR_SCORERS_FAKE_SCORER_H_

#include <boost/lexical_cast.hpp>

#include <string>
#include "ltr/scorers/scorer.h"

using std::string;

namespace ltr {
/**
 * Fake Scorer. Simply always returns a constant, whose default value is 0.0.
 */
class FakeScorer : public Scorer {
  public:
  typedef ltr::utility::shared_ptr< FakeScorer > Ptr;

  FakeScorer(double scoreValue = 0.0,
      const FeatureConverterArray& feature_converters = FeatureConverterArray())
  : Scorer(feature_converters), score_value_(scoreValue) {}

  string toString() const {
    return "All scores are constant (" +
    boost::lexical_cast<std::string>(score_value_) + ")";
  }

  private:
  double scoreImpl(const Object& obj) const {
    return score_value_;
  }

  string generateCppCodeImpl(const string& function_name) const {
    string code;
    code.
      append("inline double ").
      append(function_name).
      append("(const std::vector<double>& features) { return ").
      append(boost::lexical_cast<string>(score_value_)).
      append("; }\n");
    return code;
  }
  virtual string getDefaultAlias() const {return "FakeScorer";}

  double score_value_;
};
}

#endif  // LTR_SCORERS_FAKE_SCORER_H_
