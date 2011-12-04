// Copyright 2011 Yandex

#ifndef LTR_SCORERS_ONE_FEATURE_SCORER_H_ 
#define LTR_SCORERS_ONE_FEATURE_SCORER_H_ 

#pragma once

#include "ltr/scorers/scorer.h"


namespace ltr {
  /**
   * Simple ranker. Represent single features multiplied by weight.
   */
  class OneFeatureScorer : public IScorer {
  public:
    typedef boost::shared_ptr< OneFeatureScorer > Ptr;

    /**
     * constructor.
     * @param feature_index - index of used feature.
     * @param feature_weight - weight of used feature.
     */
    OneFeatureScorer(size_t feature_index = 0, double feature_weight = 0);

    /**
     * Returns index of used feature.
     */
    size_t index() const;

    /**
     * Returns weight of used feature.
     */
    double weight() const;

    /**
     * Resets feature's index.
     * @param new_index - index to be set.
     */
    void setIndex(size_t new_index);

    /**
     * Resets used feature's weight.
     * @param new_weight - weight to be set.
     */
    void setWeight(double new_weight);

    virtual std::string brief() const;
    virtual double operator()(const Object& obj) const;
    virtual std::string generateCppCode
      (const std::string& class_name, int tabbing) const;
    virtual std::string generateJavaCode
      (const std::string& class_name, int tabbing, bool is_static) const;
  private:
    size_t index_;
    double weight_;
  };
};

#endif  // LTR_SCORERS_ONE_FEATURE_SCORER_H_ 
