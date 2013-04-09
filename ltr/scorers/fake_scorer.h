// Copyright 2011 Yandex

#ifndef LTR_SCORERS_FAKE_SCORER_H_
#define LTR_SCORERS_FAKE_SCORER_H_

#include <string>
#include <sstream>

#include "ltr/utility/boost/lexical_cast.h"
#include "ltr/scorers/scorer.h"

using std::string;
using std::stringstream;

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

  private:
  double scoreImpl(const Object& obj) const {
    return score_value_;
  }

  string generateCppCodeImpl(const string& function_name) const {
    stringstream code;
    code
      << "inline double " << function_name
        << "(const std::vector<double>& features) { return "
        << ltr::utility::lexical_cast<string>(score_value_) << "; }\n";
    return code.str();
  }
  virtual string getDefaultAlias() const {return "FakeScorer";}

  string toStringImpl() const {
    return "All scores are constant (" +
    ltr::utility::lexical_cast<std::string>(score_value_) + ")";
  }

  double score_value_;
};
}

#endif  // LTR_SCORERS_FAKE_SCORER_H_
