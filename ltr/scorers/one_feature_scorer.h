// Copyright 2011 Yandex

#ifndef LTR_SCORERS_ONE_FEATURE_SCORER_H_
#define LTR_SCORERS_ONE_FEATURE_SCORER_H_

#include <string>
#include "ltr/scorers/scorer.h"
#include "ltr/feature_converters/feature_converter.h"

using ltr::FeatureConverterArray;

namespace ltr {
  /**
   * Simple ranker. Represent single feature.
   */
  class OneFeatureScorer : public Scorer {
  public:
    typedef boost::shared_ptr< OneFeatureScorer > Ptr;

    explicit OneFeatureScorer(size_t feature_index = 0,
        FeatureConverterArray featureConverterArray = FeatureConverterArray()) :
        Scorer("OneFeatureScorer", featureConverterArray) {};

    size_t getIndex() const {
      return index_;
    }

    void setIndex(size_t newIndex) {
      index_ = newIndex;
    }

    virtual std::string brief() const;

  private:
    double scoreImpl(const Object& obj) const {
      return obj[1].features().at(index_);
    }

    std::string generateCppCodeImpl(const std::string& class_name,
        int tabbing = 0) const;

    size_t index_;
  };
};
#endif  // LTR_SCORERS_ONE_FEATURE_SCORER_H_
