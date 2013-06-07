// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_POPULATION_UPDATERS_STEP_SIZE_CALCULATOR_HPP_
#define LTR_OPTIMIZATION_POPULATION_UPDATERS_STEP_SIZE_CALCULATOR_HPP_

#include <string>
#include "contrib/include_Eigen.h"
#include "ltr/utility/macros.h"
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/interfaces/aliaser.h"

namespace optimization {
typedef Eigen::VectorXd Point;
typedef Eigen::VectorXd Vector;
/** \brief Abstract class for calculate optimzal step size in given direction
 * to minimize given function.
 *
 * \see DirectedUpdater.
 */
template<class TFunction>
class StepSizeCalculator : public ltr::Aliaser {
 public:
  typedef ltr::utility::shared_ptr<StepSizeCalculator> Ptr;

  explicit StepSizeCalculator() { }
  ~StepSizeCalculator() { }
  /**
   * Calculate step size.
   */
  virtual double getStepSize(const Point& point, const Vector& direction) = 0;
  virtual string getDefaultAlias() const {
    return "StepSizeCalculator";
  }
  virtual typename StepSizeCalculator<TFunction>::Ptr clone() = 0;
  GET_SET(typename TFunction::Ptr, function);
 protected:
  typename TFunction::Ptr function_;
};
}
#endif  // LTR_OPTIMIZATION_POPULATION_UPDATERS_STEP_SIZE_CALCULATOR_HPP_
