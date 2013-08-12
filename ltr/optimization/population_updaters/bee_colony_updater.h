// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_POPULATION_UPDATERS_BEE_COLONY_UPDATER_H_
#define LTR_OPTIMIZATION_POPULATION_UPDATERS_BEE_COLONY_UPDATER_H_

#include <algorithm>
#include <vector>
#include <string>

#include "ltr/utility/macros.h"
#include "ltr/optimization/population_info/population.h"
#include "ltr/optimization/population_updaters/population_updater.h"
#include "ltr/utility/random_number_generator.h"

using std::vector;
using std::lower_bound;
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
  explicit BeeColonyUpdater(int no_update_limit) :
      no_update_limit_(no_update_limit) {}
  virtual void init(Population* population,
                    typename TFunction::Ptr function,
                    Set::Ptr set);
  virtual void update(Population* population);
  virtual void set_function(typename TFunction::Ptr function);
  virtual void set_set(Set::Ptr set);
  virtual string getDefaultAlias() const;
 private:
  /**
   * Generates random neighbour point.
   */
  void getRandomNeighbour(const vector<Point>& points,
                           int current_point_index,
                           Point* new_point) const;
  /**
   * Generates random point from points with index
   * different from current_point_index.
   */
  void getRandomDifferent(const vector<Point>& points,
                          int current_point_index,
                          Point* random_point) const;
  /**
   * Generates random point from points using distribution
   * defined by part_sums.
   */
  int getRandomWithDistribution(const vector<double>& part_sums) const;
  /**
   * Computes fitness of the point which is proportional to
   * the value at this point.
   */
  double computeFitness(const Point& point) const;

  NoInitPopulationInfo<int>::Ptr no_update_count_;
  int no_update_limit_;
};

template <typename TFunction>
void BeeColonyUpdater<TFunction>::init(Population* population,
                                       typename TFunction::Ptr function,
                                       Set::Ptr set) {
  this->set_set(set);
  this->set_function(function);
  no_update_count_ = new NoInitPopulationInfo<int>(population, new int(0)); //NOLINT
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
  vector<PointId> point_ids;
  vector<double> part_sums;
  for (Population::Iterator it = population->begin();
       it != population->end();
       ++it) {
    ++(*no_update_count_->getInfoById(it.point_id()));
    points.push_back(it.point());
    point_ids.push_back(it.point_id());
    double fitness = computeFitness(it.point());
    double sum = (part_sums.empty() ? fitness : part_sums.back() + fitness);
    part_sums.push_back(sum);
  }
  // employed bees
  for (int employed_index = 0;
       employed_index < population->size();
       ++employed_index) {
    Point neighbour_point;
    getRandomNeighbour(points, employed_index, &neighbour_point);
    Point new_point;
    this->set_->computeProjection(neighbour_point, &new_point);
    if (this->function_->computeValue(new_point) <=
        this->function_->computeValue(points[employed_index])) {
      population->updatePoint(point_ids[employed_index], new_point);
      (*no_update_count_->getInfoById(point_ids[employed_index])) = 0;
    } else {
      ++(*no_update_count_->getInfoById(point_ids[employed_index]));
    }
  }
  // onlooker bees
  for (int onlooker_index = 0;
       onlooker_index < population->size();
       ++onlooker_index) {
    int employed_index = getRandomWithDistribution(part_sums);
    Point neighbour_point;
    getRandomNeighbour(points, employed_index, &neighbour_point);
    Point new_point;
    this->set_->computeProjection(neighbour_point, &new_point);
    if (this->function_->computeValue(new_point) <=
        this->function_->computeValue(points[employed_index])) {
      population->updatePoint(point_ids[employed_index], new_point);
      *no_update_count_->getInfoById(point_ids[employed_index]) = 0;
    } else {
      ++(*no_update_count_->getInfoById(point_ids[employed_index]));
    }
  }
  // scout bees
  int max_index = 0;
  for (int bee_index = 1; bee_index < population->size(); ++bee_index) {
    int max_count = *no_update_count_->getInfoById(point_ids[max_index]);
    int current_count = (*no_update_count_->getInfoById(point_ids[bee_index]));
    if (max_count < current_count) {
      max_index = bee_index;
    }
  }
  if (*no_update_count_->getInfoById(point_ids[max_index]) > no_update_limit_) {
    Point random_point;
    this->set_->sampleRandomPointInside(&random_point);
    population->updatePoint(point_ids[max_index], random_point);
    *no_update_count_->getInfoById(point_ids[max_index]) = 0;
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

template <typename TFunction>
int BeeColonyUpdater<TFunction>::getRandomWithDistribution(
    const vector<double>& part_sums) const {
  double all_sums = part_sums.back();
  double random_number = randomizer.doubleRand(0, all_sums);
  return lower_bound(part_sums.begin(), part_sums.end(), random_number) -
         part_sums.begin();
}

template <typename TFunction>
void BeeColonyUpdater<TFunction>::getRandomNeighbour(
    const vector<Point>& points,
    int current_point_index,
    Point* new_point) const {
  double alpha = randomizer.doubleRand(-1, 1);
  int dim_index = randomizer(this->function_->dimension());
  Point random_point;
  getRandomDifferent(points, current_point_index, &random_point);
  *new_point = points[current_point_index];
  (*new_point)(dim_index) += alpha * ((*new_point)(dim_index) -
                             random_point(dim_index));
}

template <typename TFunction>
double BeeColonyUpdater<TFunction>::computeFitness(const Point& point) const {
  double value = this->function_->computeValue(point);
  if (value >= 0) {
    return 1.0 / (1.0 + value);
  }
  return 1.0 - value;
}
}

#endif  // LTR_OPTIMIZATION_POPULATION_UPDATERS_BEE_COLONY_UPDATER_H_
