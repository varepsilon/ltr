// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_POPULATION_UPDATERS_PARTICLE_SWARM_UPDATER_H_
#define LTR_OPTIMIZATION_POPULATION_UPDATERS_PARTICLE_SWARM_UPDATER_H_

#include <vector>
#include <string>

#include "ltr/utility/macros.h"
#include "ltr/optimization/population_info/population.h"
#include "ltr/optimization/population_info/population_info.hpp"
#include "ltr/optimization/population_updaters/population_updater.h"
#include "ltr/utility/random_number_generator.h"

using ltr::utility::randomizer;

namespace optimization {
/**
 * \brief Class for update points from population in the particle swarm
 *        optimization algorithm.
 *
 * Get more information about the updater on
 * http://sci2s.ugr.es/EAMHCO/pdfs/ABC-algorithm-numerical-function-2007.pdf
 *
 * \tparam TFunction is a type of function.
 *
 * \see ParticleSwarmOptimization, PopulationUpdater
 */
template <typename TFunction>
class ParticleSwarmUpdater : public PopulationUpdater<TFunction> {
  ALLOW_SHARED_PTR_ONLY_CREATION(ParticleSwarmUpdater)
 public:
  explicit ParticleSwarmUpdater(
      int max_iteration,
      double inertia_weight_min,
      double inertia_weight_max,
      double individual_learning_factor,
      double cooperative_learning_factor)
      : max_iteration_(max_iteration),
        inertia_weight_min_(inertia_weight_min),
        inertia_weight_max_(inertia_weight_max),
        individual_learning_factor_(individual_learning_factor),
        cooperative_learning_factor_(cooperative_learning_factor) {}
  virtual void init(Population* population,
                    typename TFunction::Ptr function,
                    Set::Ptr set);
  virtual void update(Population* population);
  virtual void set_function(typename TFunction::Ptr function);
  virtual void set_set(Set::Ptr set);
  virtual string getDefaultAlias() const;
 private:
  NoInitPopulationInfo<Point>::Ptr velocity_, optimum_point_;
  Point global_optimum_point_;
  double inertia_weight_min_, inertia_weight_max_;
  double individual_learning_factor_;
  double cooperative_learning_factor_;
  int curr_iteration_, max_iteration_;
};

template <typename TFunction>
void ParticleSwarmUpdater<TFunction>::init(Population* population,
                                       typename TFunction::Ptr function,
                                       Set::Ptr set) {
  this->set_set(set);
  this->set_function(function);
  curr_iteration_ = 0;
  velocity_ = new NoInitPopulationInfo<Point>(population, new Point(
      function->dimension()));
  optimum_point_ = new NoInitPopulationInfo<Point>(population, new Point(
      function->dimension()));
  global_optimum_point_ = population->begin().point();
  for (Population::Iterator it = population->begin();
       it != population->end();
       ++it) {
    *optimum_point_->getInfoById(it.point_id()) = it.point();
    *velocity_->getInfoById(it.point_id()) = Point::Zero(
        this->function_->dimension());
    double current_point_optimum_value = function->computeValue(it.point());
    double global_optimum_value = function->computeValue(global_optimum_point_);
    if (current_point_optimum_value < global_optimum_value) {
      global_optimum_point_ = it.point();
    }
  }
}

template <typename TFunction>
void ParticleSwarmUpdater<TFunction>::set_function(
    typename TFunction::Ptr function) {
  this->function_ = function;
}

template <typename TFunction>
void ParticleSwarmUpdater<TFunction>::set_set(Set::Ptr set) {
  this->set_ = set;
}

template <typename TFunction>
string ParticleSwarmUpdater<TFunction>::getDefaultAlias() const {
  return "ParticleSwarmUpdater";
}

template <typename TFunction>
void ParticleSwarmUpdater<TFunction>::update(Population* population) {
  ++curr_iteration_;
  double inertia_weight = inertia_weight_max_ +
      (inertia_weight_min_ - inertia_weight_max_) *
      curr_iteration_ / max_iteration_;
  for (Population::Iterator it = population->begin();
       it != population->end();
       ++it) {
    Point& velocity = *velocity_->getInfoById(it.point_id());
    Point& optimum_point = *optimum_point_->getInfoById(it.point_id());
    velocity =  inertia_weight * velocity +
        individual_learning_factor_ * randomizer.doubleRand(0, 1) *
        (optimum_point - it.point()) +
        cooperative_learning_factor_ * randomizer.doubleRand(0, 1) *
        (global_optimum_point_ - it.point());
  }
  for (Population::Iterator it = population->begin();
       it != population->end();
       ++it) {
    Point& velocity = *velocity_->getInfoById(it.point_id());
    Point& optimum_point = *optimum_point_->getInfoById(it.point_id());
    Point new_point = it.point() + velocity;
    population->updatePoint(it.point_id(), new_point);
    if (!this->set_->isInside(new_point)) {
      continue;
    }
    double new_point_value = this->function_->computeValue(new_point);
    double current_point_optimum_value = this->function_->computeValue(
                                             optimum_point);
    double global_optimum_value = this->function_->computeValue(
                                      global_optimum_point_);
    if (new_point_value < current_point_optimum_value) {
      optimum_point = new_point;
    }
    if (new_point_value < global_optimum_value) {
      global_optimum_point_ = new_point;
    }
  }
}
}

#endif  // LTR_OPTIMIZATION_POPULATION_UPDATERS_PARTICLE_SWARM_UPDATER_H_
