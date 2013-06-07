// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_POPULATION_INITERS_POPULATION_INITER_H_
#define LTR_OPTIMIZATION_POPULATION_INITERS_POPULATION_INITER_H_

#include <string>

#include "contrib/include_Eigen.h"

#include "ltr/interfaces/aliaser.h"
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/optimization/population_info/population.h"
#include "ltr/optimization/sets/set.h"

namespace optimization {
/**
 * \brief Abstract class for initializing population of points.
 */
class PopulationIniter : public ltr::Aliaser {
 public:
  typedef ltr::utility::shared_ptr<PopulationIniter> Ptr;

  explicit PopulationIniter() { }
  virtual ~PopulationIniter() { }

  /**
   * Initialize population that consist of population_size points
   * for optimizate function on set.
   */
  virtual void populationInit(int population_size,
                              const Set& set,
                              Population* population) const = 0;

  virtual string getDefaultAlias() const {
    return "PopulationIniter";
  }
};
}

#endif  // LTR_OPTIMIZATION_POPULATION_INITERS_POPULATION_INITER_H_
