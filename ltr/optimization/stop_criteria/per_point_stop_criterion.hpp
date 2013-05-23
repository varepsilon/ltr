//  Copyright 2013 yandex

#ifndef LTR_OPTIMIZATION_STOP_CRITERIA_PER_POINT_STOP_CRITERION_HPP_
#define LTR_OPTIMIZATION_STOP_CRITERIA_PER_POINT_STOP_CRITERION_HPP_

#include <map>
#include <string>
#include "ltr/optimization/stop_criteria/one_point_stop_criterion.h"
#include "ltr/optimization/stop_criteria/stop_criterion.h"
#include "ltr/optimization/stop_criteria/vote_aggregator.h"
#include "ltr/optimization/population_initers/population.h"
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
      typename OnePointStopCriterion<TFunction>::Ptr one_point_stop_criterion)
      : StopCriterion<TFunction>(one_point_stop_criterion->function()),
        one_point_stop_criterion_(one_point_stop_criterion) { }
  ~PerPointStopCriterion() { }

  typedef ltr::utility::shared_ptr<PerPointStopCriterion> Ptr;
  /**
   * Init data for future test of population.
   */
  virtual void init(const Population& population);
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

  GET_SET(double, aggregator_threshold);
  virtual string getDefaultAlias() const;
 private:
  /**
   * Add OnePointStopCriterion to point with point_id.
   */
  virtual void addOnePointStopCriterion(const Point& point, PointId point_id);
  /**
   * Remove OnePointStopCriterion to point with point_id.
   */
  virtual void removeOnePointStopCriterion(PointId point_id);
  map<PointId, typename OnePointStopCriterion<TFunction>::Ptr > one_point_stop_criteria_;  // NOLINT
  typedef typename map<PointId, typename OnePointStopCriterion<TFunction>::Ptr >::iterator Iterator;  // NOLINT
  VoteAggregator aggregator_;
  typename OnePointStopCriterion<TFunction>::Ptr one_point_stop_criterion_;
  double aggregator_threshold_;
};

// template realization

template<class TFunction>
void PerPointStopCriterion<TFunction>::init(const Population& population) {
  typename OnePointStopCriterion<TFunction>::Ptr stop_criterion =
    one_point_stop_criterion_->clone();
  for (Population::Iterator iterator = population.begin();
       iterator != population.end();
       ++iterator) {
     stop_criterion->init(iterator.point());
     one_point_stop_criteria_[iterator.point_id()] = stop_criterion->clone();
  }
}

template<class TFunction>
void PerPointStopCriterion<TFunction>::update(const Population& population) {
  /**
   * Deleting old stopcriteria, adding new stopcriteria for new points in population
   * via 2 iterators like in merge-step in merge-sort.
   */
  Population::Iterator population_iterator = population.begin();
  Iterator stop_criteria_iterator = one_point_stop_criteria_.begin();

  aggregator_.reset();
  while (stop_criteria_iterator != one_point_stop_criteria_.end() &&
          population_iterator != population.end()) {
    PointId stop_criteria_id = stop_criteria_iterator->first;
    PointId point_id = population_iterator.point_id();

    if (stop_criteria_id < point_id) {
      // Delete old points from one_point_stop_criteria_.
      ++stop_criteria_iterator;
      removeOnePointStopCriterion(stop_criteria_id);
    } else if (stop_criteria_id == point_id) {
      // Update ordinary points in one_point_stop_criteria_.
      one_point_stop_criteria_[stop_criteria_id]->update(population_iterator.point());  // NOLINT

      bool answer = one_point_stop_criteria_[stop_criteria_id]->isTrue();
      aggregator_.push(answer);

      ++population_iterator;
      ++stop_criteria_iterator;
    } else {
      // Add new points to one_point_stop_criteria_.
      addOnePointStopCriterion(population_iterator.point(), point_id);

      bool answer = one_point_stop_criteria_[point_id]->isTrue();
      aggregator_.push(answer);

      ++population_iterator;
    }
  }

  if (stop_criteria_iterator == one_point_stop_criteria_.end()) {
    // add points
    while (population_iterator != population.end()) {
      PointId point_id = population_iterator.point_id();
      addOnePointStopCriterion(population_iterator.point(), point_id);

      bool answer = one_point_stop_criteria_[point_id]->isTrue();
      aggregator_.push(answer);

      ++population_iterator;
    }
  } else {
    // delete points
    while (stop_criteria_iterator != one_point_stop_criteria_.end()) {
      one_point_stop_criteria_.erase(
          (++stop_criteria_iterator)->first);
    }
  }
}

template<class TFunction>
bool PerPointStopCriterion<TFunction>::isTrue() {
  return aggregator_.isTrue(aggregator_threshold_);
}

template<class TFunction>
void PerPointStopCriterion<TFunction>::addOnePointStopCriterion(const Point& point,  // NOLINT
                                                                PointId point_id) {  // NOLINT
  typename OnePointStopCriterion<TFunction>::Ptr stop_criterion =
    one_point_stop_criterion_->clone();
  stop_criterion->init(point);
  one_point_stop_criteria_[point_id] = stop_criterion;
}

template<class TFunction>
void PerPointStopCriterion<TFunction>::removeOnePointStopCriterion(PointId point_id) {  // NOLINT
  one_point_stop_criteria_.erase(point_id);
}

template<class TFunction>
string PerPointStopCriterion<TFunction>::getDefaultAlias() const {
  return "PerPointStopCriterion_with_" + one_point_stop_criterion_->alias();
}
}

#endif  // LTR_OPTIMIZATION_STOP_CRITERIA_PER_POINT_STOP_CRITERION_HPP_
