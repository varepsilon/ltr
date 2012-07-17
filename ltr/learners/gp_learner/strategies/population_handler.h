// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_POPULATION_HANDLER_H_
#define LTR_LEARNERS_GP_LEARNER_POPULATION_HANDLER_H_

#include <boost/shared_ptr.hpp>

namespace ltr {
namespace gp {
class BasePopulationHandler {
 public:
  typedef boost::shared_ptr<BasePopulationHandler> Ptr;

  virtual void HandlePopulation(std::vector<Puppy::Tree>& population,
                                 Puppy::Context& context) = 0;
};
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_POPULATION_HANDLER_H_
