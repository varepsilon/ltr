// Copyright 2011 Yandex

#ifndef LTR_SCORERS_SOFT_SCORER_H_
#define LTR_SCORERS_SOFT_SCORER_H_

#include <vector>
#include <string>

#include "scorers/scorer.h"


namespace ltr {
  typedef std::vector<double> Gradient;

  class SoftScorer: public Scorer {
   public:
    typedef boost::shared_ptr< SoftScorer > Ptr;

    SoftScorer(const std::string& alias,  const FeatureConverterArray&
        featureConverters = FeatureConverterArray()):
      Scorer(alias, featureConverters) {
    }

    virtual Gradient getGradient(const Object& obj) const = 0;

    virtual size_t getParametersCount() const = 0;

    virtual double getParameter(int parameter_index) const = 0;

    virtual void shiftParameters(const Gradient& shift) = 0;
  };

  class LinearScorer: public SoftScorer {
   public:
    typedef boost::shared_ptr<LinearScorer> Ptr;

    LinearScorer(const Gradient& parameters, const FeatureConverterArray&
        featureConverters = FeatureConverterArray());

    std::string brief() const;

    Gradient getGradient(const Object& obj) const;
    size_t getParametersCount() const;
    double getParameter(int parameter_index) const;
    void shiftParameters(const Gradient& shift);

    std::string generateCppCode(const std::string& class_name,
      int tabbing = 0) const;
    std::string generateJavaCode(const std::string& class_name,
      int tabbing = 0, bool is_static = false) const;

   private:
    std::vector<double> features_weights_;

    double scoreImpl(const Object& object) const;

    std::string generateCppCodeImpl(const std::string& class_name,
      int tabbing = 0) const;
    std::string generateJavaCodeImpl(const std::string& class_name,
      int tabbing = 0, bool is_static = false) const;
  };
};

#endif  // LTR_SCORERS_SOFT_SCORER_H_
