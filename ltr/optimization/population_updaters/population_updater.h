//  Copyright 2013 yandex

#ifndef LTR_OPTIMIZATION_POPULATION_UPDATERS_POPULATION_UPDATER_H_
#define LTR_OPTIMIZATION_POPULATION_UPDATERS_POPULATION_UPDATER_H_

#include <string>
#include "ltr/utility/macros.h"
#include "ltr/optimization/sets/set.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/optimization/population_info/population.h"
#include "ltr/optimization/population_info/population_info.hpp"

namespace optimization {
typedef int PointId;
/**
 * \brief Class for update population.
 *
 * \tparam TFunction is a type of function.
 */
template<class TFunction>
class PopulationUpdater : public ltr::Aliaser {
 public:
  explicit PopulationUpdater() { }
  ~PopulationUpdater() { }

  typedef ltr::utility::shared_ptr<PopulationUpdater> Ptr;
  /**
   * Init data for future updates of population.
   */
  virtual void init(Population* population,
                    typename TFunction::Ptr function,
                    Set::Ptr set) = 0;
  /**
   * Update population.
   */
  virtual void update(Population* population) = 0;

  GET(typename TFunction::Ptr, function)
  GET(Set::Ptr, set)
  virtual string getDefaultAlias() const {return "PopulationUpdater"; }
 protected:
  SET(typename TFunction::Ptr, function)
  SET(Set::Ptr, set)
  typename TFunction::Ptr function_;
  Set::Ptr set_;
};
}

#endif  // LTR_OPTIMIZATION_POPULATION_UPDATERS_POPULATION_UPDATER_H_
