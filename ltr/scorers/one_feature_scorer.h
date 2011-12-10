// Copyright 2011 Yandex

#ifndef LTR_SCORERS_ONE_FEATURE_SCORER_H_
#define LTR_SCORERS_ONE_FEATURE_SCORER_H_

#include <string>
#include "ltr/scorers/scorer.h"


namespace ltr {
  /**
   * Simple ranker. Represent single feature.
   */
  class OneFeatureScorer : public Scorer {
  public:
    typedef boost::shared_ptr< OneFeatureScorer > Ptr;

    explicit OneFeatureScorer(size_t feature_index = 0);

    size_t index() const;
    void setIndex(size_t new_index);



    virtual std::string brief() const;
    virtual double operator()(const Object& obj) const;
    virtual std::string generateCppCode
      (const std::string& class_name, int tabbing) const;
    virtual std::string generateJavaCode
      (const std::string& class_name, int tabbing, bool is_static) const;
  private:
    size_t index_;
  };
};

#endif  // LTR_SCORERS_ONE_FEATURE_SCORER_H_
