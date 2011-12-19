// Copyright 2011 Yandex

#ifndef LTR_SCORERS_FAKE_SCORER_H_
#define LTR_SCORERS_FAKE_SCORER_H_

#include <string>
#include "ltr/scorers/scorer.h"

using std::string;

namespace ltr {
/**
 * Fake Scorer. Simply always return 0.0.
 */
class FakeScorer : public Scorer {
  public:
  typedef boost::shared_ptr< FakeScorer > Ptr;

  FakeScorer(
      const FeatureConverterArray& featureConverters = FeatureConverterArray())
  :Scorer("FakeScorer", featureConverters) {}

  string brief() const {
    return "All ranks are 0.";
  }

  private:
  double scoreImpl(const Object& obj) const {
    return 0.0;
  }

  string generateCppCodeImpl(const string& class_name,
      int tabbing) const {
    return "Not implemented.";
  }
};
}

#endif  // LTR_SCORERS_FAKE_SCORER_H_
