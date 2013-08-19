//  Copyright 2013 yandex

#ifndef LTR_OPTIMIZATION_POPULATION_UPDATERS_PER_POINT_UPDATER_HPP_
#define LTR_OPTIMIZATION_POPULATION_UPDATERS_PER_POINT_UPDATER_HPP_

#include <map>
#include <string>
#include "ltr/optimization/population_info/population_info.hpp"
#include "ltr/optimization/population_updaters/population_updater.h"
#include "ltr/optimization/population_updaters/one_point_updater.h"

namespace optimization {
typedef int PointId;
/**
 * \brief Class for update population via
 * updating all points in population independently.
 *
 * You should completely define one_point_updater before.
 * You can't change it after initialization of per_point_updater.
 *
 * \tparam TFunction is a type of function.
 *
 * \see OnePointUpdater.
 */
template<class TFunction>
class PerPointUpdater : public PopulationUpdater<TFunction> {
 public:
  explicit PerPointUpdater(
      typename OnePointUpdater<TFunction>::Ptr one_point_updater_sample)
      : one_point_updater_sample_(one_point_updater_sample) { }
  ~PerPointUpdater() { }
  typedef PopulationInfo<OnePointUpdater<TFunction> > OnePointUpdaterInfo;  // NOLINT
  typedef ltr::utility::shared_ptr<PerPointUpdater> Ptr;
  /**
   * Init data for future update of population.
   */
  virtual void init(Population* population,
                    typename TFunction::Ptr function,
                    Set::Ptr set);
  /**
   * Update population.
   */
  virtual void update(Population* population);

  virtual string getDefaultAlias() const;
 private:
  /**
   * PopulationInfo for updater.
   */
  typename OnePointUpdaterInfo::Ptr one_point_updaters_info_;
  /**
   * Sample of one point updater in per point updater.
   */
  typename OnePointUpdater<TFunction>::Ptr one_point_updater_sample_;
};

// template realization

template<class TFunction>
void PerPointUpdater<TFunction>::init(Population* population,
                                      typename TFunction::Ptr function,
                                      Set::Ptr set) {
  this->set_set(set);
  this->set_function(function);
  Point init_point;
  one_point_updater_sample_->init(init_point, function, set);
  one_point_updaters_info_ = new OnePointUpdaterInfo(population,
                                  one_point_updater_sample_);
}

template<class TFunction>
void PerPointUpdater<TFunction>::update(Population* population) {
  for (Population::Iterator it = population->begin();
       it != population->end();
       ++it) {
     Point updated_point;
     (one_point_updaters_info_->getInfoById(it.point_id()))->update(it.point(),
                                                         &updated_point);
     population->updatePoint(it.point_id(), updated_point);
  }
}

template<class TFunction>
string PerPointUpdater<TFunction>::getDefaultAlias() const {
  return "PerPointUpdater_with_" + one_point_updater_sample_->alias();
}
}
#endif  // LTR_OPTIMIZATION_POPULATION_UPDATERS_PER_POINT_UPDATER_HPP_
