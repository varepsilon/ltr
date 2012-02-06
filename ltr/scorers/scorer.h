// Copyright 2011 Yandex

#ifndef LTR_SCORERS_SCORER_H_
#define LTR_SCORERS_SCORER_H_

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

#include "ltr/interfaces/aliaser.h"
#include "ltr/data/object.h"
#include "ltr/feature_converters/feature_converter.h"
#include "ltr/interfaces/serializable_functor.h"

using std::string;

namespace ltr {
/*
 * Scorer is a class that can for every object evaluate
 * a value - its rank, or score
 */
class Scorer : public Aliaser, public SerializableFunctor<double> {
  public:
  typedef boost::shared_ptr<Scorer> Ptr;
  typedef boost::shared_ptr<Scorer> BasePtr;

  Scorer(const string& alias,
      const FeatureConverterArray& featureConverters = FeatureConverterArray()):
        Aliaser(alias),
        featureConverters_(featureConverters) {}

  double value(const Object& obj) const {
    return score(obj);
  }

  double score(const Object& obj) const;

  virtual string brief() const = 0;

  const FeatureConverterArray& getFeatureConverters() const {
    return featureConverters_;
  }
  void setFeatureConverters(const FeatureConverterArray& featureConverters) {
    this->featureConverters_ = featureConverters;
  }

  virtual ~Scorer() {}

  private:
  virtual double scoreImpl(const Object& obj) const = 0;

  FeatureConverterArray featureConverters_;
};
}
#endif  // LTR_SCORERS_SCORER_H_
