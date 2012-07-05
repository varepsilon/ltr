// Copyright 2012 Yandex

#ifndef LTR_MEASURES_NORMALIZED_MEASURE_H_
#define LTR_MEASURES_NORMALIZED_MEASURE_H_

#include <sstream>
#include <string>

#include "ltr/utility/numerical.h"
#include "ltr/measures/measure.h"

using std::string;

namespace ltr {
/**
 * Normalized measure. Can be used for any approach.
 * Returns lineal normalisation of the given weak measure.
 * Best and worst values of the rasulting measure are given in parameters.
 * Can't normalize infinity measure.
 */
template<class TElement>
class NormalizedMeasure : public Measure<TElement> {
 public:
  typedef boost::shared_ptr<NormalizedMeasure> Ptr;

  explicit NormalizedMeasure(const ParametersContainer& parameters) {
    this->setParameters(parameters);
  }

  explicit NormalizedMeasure(
      typename Measure<TElement>::Ptr weak_measure,
      double worst = -1.0, double best = 1.0) {
    weak_measure_ = weak_measure;
    worst_ = worst;
    best_ = best;
  }

  GET_SET(double, worst);
  GET_SET(double, best);

  void setWeakMeasure(typename Measure<TElement>::Ptr weak_measure) {
    weak_measure_ = weak_measure;
  }

  template<class TMeasure>
  void setWeakMeasure(const TMeasure& weak_measure) {
    weak_measure_ =
      typename Measure<TElement>::Ptr(new TMeasure(weak_measure));
  }

  virtual void checkParameters() {
    if (best_ == worst_)
      throw std::logic_error("Best and worst values must be different");
  }

  virtual void setDefaultParameters() {
    worst_ = -1.0;
    best_ = 1.0;
  }

  string toString() const {
    std::stringstream str;
    std::fixed(str);
    str.precision(2);
    if (weak_measure_ == NULL) {
      str << "Normalized measure without setted weak measure";
    } else {
      str << "Normalized measure over { ";
      str << *weak_measure_;
      str << " }";
    }
    str << " with parameters: BEST = ";
    str << best_;
    str << ", WORST = ";
    str << worst_;
    return str.str();
  }
  virtual string getDefaultAlias() const {return "NormalizedMeasure";}

 private:
  typename Measure<TElement>::Ptr weak_measure_;
  double worst_, best_;

  virtual void setParametersImpl(const ParametersContainer& parameters) {
    weak_measure_
      = parameters.Get<typename Measure<TElement>::Ptr>("WEAK_MEASURE");
    worst_ = parameters.Get<double>("WORST");
    best_ = parameters.Get<double>("BEST");
  }

  virtual double get_measure(const TElement& element) const {
    if (weak_measure_ == NULL)
      throw std::logic_error("No weak measure");
    if (weak_measure_->best() == utility::Inf ||
        weak_measure_->best() == -utility::Inf ||
        weak_measure_->worst() == utility::Inf ||
        weak_measure_->worst() == -utility::Inf) {
      throw std::logic_error("can't normalize infinity measure");
    }
// using best_ directly?
    double best = best_;
    double worst = worst_;
    return (weak_measure_->value(element) - weak_measure_->worst()) *
              (best - worst) /
                (weak_measure_->best() - weak_measure_->worst())
            + worst;
  }
};
}
#endif  // LTR_MEASURES_NORMALIZED_MEASURE_H_