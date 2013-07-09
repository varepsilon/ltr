// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_POPULATION_UPDATERS_BEE_COLONY_UPDATER_H_
#define LTR_OPTIMIZATION_POPULATION_UPDATERS_BEE_COLONY_UPDATER_H_

#include <vector>
#include <string>

#include "ltr/utility/macros.h"
#include "ltr/optimization/population_info/population.h"
#include "ltr/optimization/population_updaters/population_updater.h"
#include "ltr/utility/random_number_generator.h"

using std::vector;
using ltr::utility::randomizer;

namespace optimization {
/**
 * \brief Class for update points from population in the artificial bee
 *        colony algorithm.
 *
 * Get more information about the updater on
 * http://sci2s.ugr.es/EAMHCO/pdfs/ABC-algorithm-numerical-function-2007.pdf
 *
 * \tparam TFunction is a type of function.
 *
 * \see ArtificialBeeColony, PopulationUpdater
 */
template <typename TFunction>
class BeeColonyUpdater : public PopulationUpdater<TFunction> {
  ALLOW_SHARED_PTR_ONLY_CREATION(BeeColonyUpdater)
 public:
  explicit BeeColonyUpdater() {}
  virtual void init(Population* population,
                    typename TFunction::Ptr function,
                    Set::Ptr set);
  virtual void update(Population* population);
  virtual void set_function(typename TFunction::Ptr function);
  virtual void set_set(Set::Ptr set);
  virtual string getDefaultAlias() const;
 private:
  /**
   * Generates random point from points with index
   * different from current_point_index.
   */
  void getRandomDifferent(const vector<Point>& points,
                          int current_point_index,
                          Point* random_point) const;
};

template <typename TFunction>
void BeeColonyUpdater<TFunction>::init(Population* population,
                                       typename TFunction::Ptr function,
                                       Set::Ptr set) {
  this->set_set(set);
  this->set_function(function);
}

template <typename TFunction>
void BeeColonyUpdater<TFunction>::set_function(
    typename TFunction::Ptr function) {
  this->function_ = function;
}

template <typename TFunction>
void BeeColonyUpdater<TFunction>::set_set(Set::Ptr set) {
  this->set_ = set;
}

template <typename TFunction>
string BeeColonyUpdater<TFunction>::getDefaultAlias() const {
  return "BeeColonyUpdater";
}

template <typename TFunction>
void BeeColonyUpdater<TFunction>::update(Population* population) {
  CHECK_MSG(population->size() >= 2, "size of population must be at least 2");
  vector<Point> points;
  for (Population::Iterator it = population->begin();
       it != population->end();
       ++it) {
    points.push_back(it.point());
  }
  int current_point_index = 0;
  double sum_value = 0;
  for (Population::Iterator it = population->begin();
       it != population->end();
       ++it) {
    double alpha = randomizer.doubleRand(-1, 1);
    int dim_index = randomizer(this->function_->dimension());
    Point random_point;
    getRandomDifferent(points, current_point_index, &random_point);
    Point new_point = it.point();
    new_point[dim_index] += alpha * (new_point[dim_index] -
                                     random_point[dim_index]);
    if (this->set_->isInside(new_point) &&
        this->function_->computeValue(new_point) <
        this->function_->computeValue(it.point())) {
      population->updatePoint(it.point_id(), new_point);
    }
    sum_value += this->function_->computeValue(it.point());
    ++current_point_index;
  }
  for (Population::Iterator it = population->begin();
       it != population->end();
       ++it) {
    // probability to reject point
    double probability = this->function_->computeValue(it.point()) / sum_value;
    if (randomizer.doubleRand(0, 1) <= probability) {
      Point random_point;
      this->set_->sampleRandomPointInside(&random_point);
      population->updatePoint(it.point_id(), random_point);
    }
  }
}

template <typename TFunction>
void BeeColonyUpdater<TFunction>::getRandomDifferent(
    const vector<Point>& points,
    int current_point_index,
    Point* random_point) const {
  int n = points.size();
  int index = (current_point_index + randomizer(n - 1) + 1) % n;
  (*random_point) = points[index];
}
}

#endif  // LTR_OPTIMIZATION_POPULATION_UPDATERS_BEE_COLONY_UPDATER_H_
