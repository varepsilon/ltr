// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_POPULATION_UPDATERS_DIRECTION_CALCULATOR_HPP_
#define LTR_OPTIMIZATION_POPULATION_UPDATERS_DIRECTION_CALCULATOR_HPP_

#include <string>
#include "contrib/include_Eigen.h"
#include "ltr/utility/random_number_generator.h"
#include "ltr/utility/macros.h"
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/interfaces/aliaser.h"

using ltr::utility::randomizer;

namespace optimization {
typedef Eigen::VectorXd Point;
typedef Eigen::VectorXd Vector;
/** \brief Abstract class for calculate optimzal direction from
 * given point to minimize given function.
 *
 * \see DirectedUpdater.
 */
template<class TFunction>
class StepDirectionCalculator : public ltr::Aliaser {
 public:
  typedef ltr::utility::shared_ptr<StepDirectionCalculator> Ptr;
  explicit StepDirectionCalculator() { }
  ~StepDirectionCalculator() { }

  virtual void getStepDirection(const Point& point, Vector* direction) = 0;
  virtual typename StepDirectionCalculator<TFunction>::Ptr clone() = 0;
  virtual string getDefaultAlias() const {
    return "StepDirectionCalculator";
  }
  GET_SET(typename TFunction::Ptr, function);
 protected:
  typename TFunction::Ptr function_;
};
}
#endif  // LTR_OPTIMIZATION_POPULATION_UPDATERS_DIRECTION_CALCULATOR_HPP_
