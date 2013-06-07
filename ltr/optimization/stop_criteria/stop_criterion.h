// copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_STOP_CRITERIA_STOP_CRITERION_H_
#define LTR_OPTIMIZATION_STOP_CRITERIA_STOP_CRITERION_H_

#include <string>
#include "ltr/interfaces/aliaser.h"
#include "ltr/optimization/population_info/population.h"

namespace optimization {
/**
 * \brief Class for test population via
 *
 * \tparam is a type of function.
 *
 * \see PerPointStopCriterion.
 */
template<class TFunction>
class StopCriterion : public ltr::Aliaser {
 public:
  explicit StopCriterion() { }
  ~StopCriterion() { }

  typedef ltr::utility::shared_ptr<StopCriterion> Ptr;
  /**
   * Init data for future test of population.
   */
  virtual void init(Population* population) = 0;
  /**
   * Update data for future test of population.
   */
  virtual void update(const Population& population) = 0;
  /**
   * Is this stop criterion true.
   */
  virtual bool isTrue() = 0;

  virtual void set_function(typename TFunction::Ptr function) = 0;
  virtual typename TFunction::Ptr function() {
    return function_;
  }
  virtual string getDefaultAlias() const {
    return "StopCriterion";
  }

 protected:
  typename TFunction::Ptr function_;
};
}

#endif  // LTR_OPTIMIZATION_STOP_CRITERIA_STOP_CRITERION_H_
