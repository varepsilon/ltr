// Copyright 2011 Yandex

#ifndef LTR_SCORERS_SCORER_H_
#define LTR_SCORERS_SCORER_H_

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

#include "ltr/interfaces/aliaser.h"
#include "ltr/data/object.h"
#include "ltr/feature_converters/feature_converter.h"

using std::string;

namespace ltr {

class Scorer : public Aliaser {
  public:
  typedef boost::shared_ptr<Scorer> Ptr;

  Scorer(const string& alias,
      const FeatureConverterArray& featureConverters = FeatureConverterArray()):
        Aliaser(alias),
        featureConverters_(featureConverters) {}

  double score(const Object& obj) const;
  double operator() (const Object& obj) const;

  virtual string brief() const = 0;

  string generateCppCode(const string& class_name,
      int tabbing = 0) const;

  const FeatureConverterArray& getFeatureConverters() const {
    return featureConverters_;
  }
  void setFeatureConverters(const FeatureConverterArray& featureConverters) {
    this->featureConverters_ = featureConverters;
  }

  virtual ~Scorer() {}

  private:
  virtual double scoreImpl(const Object& obj) const = 0;

  virtual string generateCppCodeImpl(const string& class_name,
      int tabbing = 0) const = 0;

  FeatureConverterArray featureConverters_;
};
}
#endif  // LTR_SCORERS_SCORER_H_
