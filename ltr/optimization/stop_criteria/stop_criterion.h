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
  virtual void init(Population* population,
                    typename TFunction::Ptr function,
                    Set::Ptr set) = 0;
  virtual void init(Population* population,
                    typename TFunction::Ptr function) {
    init(population, function, NULL);
  }
  /**
   * Update data for future test of population.
   */
  virtual void update(const Population& population) = 0;
  /**
   * Is this stop criterion true.
   */
  virtual bool isTrue() = 0;

  virtual string getDefaultAlias() const {
    return "StopCriterion";
  }
  GET(typename TFunction::Ptr, function)
  GET(Set::Ptr, set)
 protected:
  SET(typename TFunction::Ptr, function)
  SET(Set::Ptr, set)
  typename TFunction::Ptr function_;
  typename Set::Ptr set_;
};
}

#endif  // LTR_OPTIMIZATION_STOP_CRITERIA_STOP_CRITERION_H_
