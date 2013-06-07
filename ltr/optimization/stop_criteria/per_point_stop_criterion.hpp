//  Copyright 2013 yandex

#ifndef LTR_OPTIMIZATION_STOP_CRITERIA_PER_POINT_STOP_CRITERION_HPP_
#define LTR_OPTIMIZATION_STOP_CRITERIA_PER_POINT_STOP_CRITERION_HPP_

#include <map>
#include <string>
#include "ltr/optimization/stop_criteria/one_point_stop_criterion.h"
#include "ltr/optimization/stop_criteria/stop_criterion.h"
#include "ltr/optimization/stop_criteria/vote_aggregator.h"
#include "ltr/optimization/population_info/population.h"
#include "ltr/optimization/population_info/population_info.hpp"
#include "ltr/interfaces/aliaser.h"
#include "ltr/optimization/test_problems/unconstrained_functions.h"
#include "ltr/utility/boost/shared_ptr.h"

namespace optimization {
typedef int PointId;
/**
 * \brief Class for test population via
 * testing all points in population independently.
 *
 * You should completely define one_point_stop_criterion before.
 * You can't change it after initialization of per_point_stop_criterion.
 *
 * \tparam TFunction is a type of function.
 *
 * \see StopCriterion, OnePointStopCriterion, VoteAggregator.
 */
template<class TFunction>
class PerPointStopCriterion : public StopCriterion<TFunction> {
 public:
  explicit PerPointStopCriterion(
      typename OnePointStopCriterion<TFunction>::Ptr one_point_stop_criterion_sample);  // NOLINT
  ~PerPointStopCriterion() { }
  typedef PopulationInfo<OnePointStopCriterion<TFunction> > StopCriterionInfo;  // NOLINT
  typedef ltr::utility::shared_ptr<PerPointStopCriterion> Ptr;
  /**
   * Init data for future test of population.
   */
  virtual void init(Population* population);
  /**
   * Update data for future test of population.
   */
  virtual void update(const Population& population);
  /**
   * Is this stop criterion true
   *
   * \return True if stopping condition is satisfied for more
   * than given part of points in population.
   */
  virtual bool isTrue();

  virtual void set_function(typename TFunction::Ptr function);
  GET_SET(double, aggregator_threshold);
  virtual string getDefaultAlias() const;
 private:
  /**
   * Sample of one point stop criterion for per point stop criterion.
   */
  typename OnePointStopCriterion<TFunction>::Ptr
    one_point_stop_criterion_sample_;
  /**
   * Population info for per point stop criterion.
   */
  typename StopCriterionInfo::Ptr stop_criterion_info_;
  VoteAggregator aggregator_;
  double aggregator_threshold_;
};

// template realization
template<class TFunction>
PerPointStopCriterion<TFunction>::PerPointStopCriterion(
      typename OnePointStopCriterion<TFunction>::Ptr one_point_stop_criterion_sample)  // NOLINT
      : one_point_stop_criterion_sample_(one_point_stop_criterion_sample),
        aggregator_threshold_(1.0) { }

template<class TFunction>
void PerPointStopCriterion<TFunction>::init(Population* population) {
  stop_criterion_info_ = new StopCriterionInfo(
      population,
      one_point_stop_criterion_sample_);
}

template<class TFunction>
void PerPointStopCriterion<TFunction>::update(const Population& population) {
  for (Population::Iterator it = population.begin();
       it != population.end();
       ++it) {
    typename OnePointStopCriterion<TFunction>::Ptr one_point_stop_criterion =
      stop_criterion_info_->getInfoById(it.point_id());
    one_point_stop_criterion->update(it.point());
  }
}

template<class TFunction>
bool PerPointStopCriterion<TFunction>::isTrue() {
  aggregator_.reset();
  for (typename StopCriterionInfo::Iterator it = stop_criterion_info_->begin();
       it != stop_criterion_info_->end();
       ++it) {
    aggregator_.push(it->second->isTrue());
  }
  return aggregator_.isTrue(aggregator_threshold_);
}

template<class TFunction>
string PerPointStopCriterion<TFunction>::getDefaultAlias() const {
  return "PerPointStopCriterion_with_" +
    one_point_stop_criterion_sample_->alias();
}
template<class TFunction>
void PerPointStopCriterion<TFunction>::set_function(
    typename TFunction::Ptr function) {
  one_point_stop_criterion_sample_->set_function(function);
  this->function_ = function;
}
}

#endif  // LTR_OPTIMIZATION_STOP_CRITERIA_PER_POINT_STOP_CRITERION_HPP_
