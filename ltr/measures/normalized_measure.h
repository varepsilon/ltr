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
  private:
    typename Measure<TElement>::Ptr weak_measure_;

    virtual double get_measure(const TElement& element) const {
      if (weak_measure_ == NULL)
        throw std::logic_error("No weak measure");
      if (weak_measure_->best() == utility::Inf ||
          weak_measure_->best() == -utility::Inf ||
          weak_measure_->worst() == utility::Inf ||
          weak_measure_->worst() == -utility::Inf) {
        throw std::logic_error("can't normalize infinity measure");
      }
      double best = this->getDoubleParameter("BEST");
      double worst = this->getDoubleParameter("WORST");
      return (weak_measure_->value(element) - weak_measure_->worst()) *
                (best - worst) /
                  (weak_measure_->best() - weak_measure_->worst())
              + worst;
    }

  public:
    typedef boost::shared_ptr<NormalizedMeasure> Ptr;

    explicit NormalizedMeasure(
        const ParametersContainer& parameters = ParametersContainer())
        : Measure<TElement>("Normalized measure") {
      this->setDefaultParameters();
      this->copyParameters(parameters);
    }

    NormalizedMeasure(double worst, double best)
        : Measure<TElement>("Normalized measure") {
      this->setDefaultParameters();
      this->setDoubleParameter("WORST", worst);
      this->setDoubleParameter("BEST", best);
      this->checkParameters();
    }

    explicit NormalizedMeasure(
        typename Measure<TElement>::Ptr weak_measure,
        const ParametersContainer& parameters = ParametersContainer())
        : Measure<TElement>("Normalised measure") {
      this->setWeakMeasure(weak_measure);
      this->setDefaultParameters();
      this->copyParameters(parameters);
    }

    NormalizedMeasure(
        typename Measure<TElement>::Ptr weak_measure,
        double worst, double best) {
      this->setWeakMeasure(weak_measure);
      this->setDefaultParameters();
      this->setDoubleParameter("BEST", best);
      this->setDoubleParameter("WORST", worst);
    }

    void setWeakMeasure(typename Measure<TElement>::Ptr weak_measure) {
      weak_measure_ = weak_measure;
    }

    template<class TMeasure>
    void setWeakMeasure(const TMeasure& weak_measure) {
      weak_measure_ =
        typename Measure<TElement>::Ptr(new TMeasure(weak_measure));
    }

    virtual void checkParameters() {
      if (this->getDoubleParameter("BEST") ==
          this->getDoubleParameter("WORST"))
        throw std::logic_error("Best and worst values must be different");
    }

    virtual void setDefaultParameters() {
      this->addDoubleParameter("BEST", 1);
      this->addDoubleParameter("WORST", -1);
    }

    virtual double worst() const {
      return this->getDoubleParameter("WORST");
    }
    virtual double best() const {
      return this->getDoubleParameter("BEST");
    }

    string toString() const {
      std::stringstream str;
      std::fixed(str);
      str.precision(2);
      if (weak_measure_ == NULL) {
        str << "Normalized measure without setted weak measure";
      } else {
        str << "Normalized measure over { ";
        str << weak_measure_->toString();
        str << " }";
      }
      str << " with parameters: BEST = ";
      str << this->getDoubleParameter("BEST");
      str << ", WORST = ";
      str << this->getDoubleParameter("WORST");
      return str.str();
    }
};
}
#endif  // LTR_MEASURES_NORMALIZED_MEASURE_H_
