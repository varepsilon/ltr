// Copyright 2011 Yandex

#ifndef LTR_SCORERS_ONE_FEATURE_SCORER_H_
#define LTR_SCORERS_ONE_FEATURE_SCORER_H_

#include <string>
#include "ltr/scorers/scorer.h"
#include "ltr/feature_converters/feature_converter.h"

using ltr::FeatureConverterArray;
using std::string;

namespace ltr {
  /**
   * Simple ranker. Represent single feature.
   */
  class OneFeatureScorer : public Scorer {
  public:
    typedef boost::shared_ptr<OneFeatureScorer> Ptr;

    explicit OneFeatureScorer(size_t feature_index = 0,
      FeatureConverterArray featureConverterArray = FeatureConverterArray()) :
        Scorer("OneFeatureScorer", featureConverterArray),
        index_(feature_index) {}

    size_t getIndex() const {
      return index_;
    }

    void setIndex(size_t new_index) {
      index_ = new_index;
    }

    virtual string brief() const;

  private:
    double scoreImpl(const Object& obj) const {
      return obj[1].features().at(index_);
    }

    string generateCppCodeImpl(const string& function_name) const;

    size_t index_;
  };
};
#endif  // LTR_SCORERS_ONE_FEATURE_SCORER_H_
