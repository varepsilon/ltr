// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_POPULATION_INITERS_NAIVE_INITER_H_
#define LTR_OPTIMIZATION_POPULATION_INITERS_NAIVE_INITER_H_

#include <string>
#include "ltr/optimization/population_initers/population_initer.h"

namespace optimization {
  /**
   * \brief Class for i.i.d initializing population of points.
   *
   * points initialized randomly.
   */
class NaiveIniter : public PopulationIniter {
 public:
  typedef ltr::utility::shared_ptr<PopulationIniter> Ptr;
  explicit NaiveIniter() : PopulationIniter() { }
  ~NaiveIniter() { }

  /**
   * Initialize population that consist of population_size points
   * for optimizate function on set.
   */
  virtual void populationInit(int population_size,
                              const Set& set,
                              Population* population) const;

  string getDefaultAlias() const { return "NaiveIniter"; }
};
}

#endif  // LTR_OPTIMIZATION_POPULATION_INITERS_NAIVE_INITER_H_
