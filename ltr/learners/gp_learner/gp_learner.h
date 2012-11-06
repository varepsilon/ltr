// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_GP_LEARNER_H_
#define LTR_LEARNERS_GP_LEARNER_GP_LEARNER_H_

#include <boost/lexical_cast.hpp>

#include <string>
#include <sstream>
#include <vector>
#include <functional>

#include "contrib/puppy/Puppy.hpp"

#include "ltr/learners/learner.h"

#include "ltr/learners/gp_learner/gp_primitives.h"

#include "ltr/learners/gp_learner/strategies/population_handler.h"
#include "ltr/learners/gp_learner/strategies/default_selection_strategy.h"
#include "ltr/learners/gp_learner/strategies/default_crossover_strategy.h"
#include "ltr/learners/gp_learner/strategies/default_mutation_standart_strategy.h"
#include "ltr/learners/gp_learner/strategies/default_mutation_swap_strategy.h"

#include "ltr/scorers/gp_scorer.h"

#include "ltr/measures/measure.h"
#include "ltr/measures/reciprocal_rank.h"

#include "ltr/parameters_container/parameters_container.h"

namespace ltr {
namespace gp {
/**
 * \brief GPLearner
 * Implements genetic programming approach applied to learning to rank.
 * \tparam TElement object container of those the dataset consists (it can be
 * Object, ObjectPair, ObjectList).
 */
template <typename TElement>
class GPLearner : public BaseLearner<TElement, GPScorer> {
 public:
  /**
   * Constructor creates a GPLearner.
   * \param measure shared pointer to the measure that would be maximized on
   * a dataset within learning.
   * \param parameters the ParametersContainer parameters from which would
   * overwrite the default parameters.
   */
  explicit GPLearner(typename Measure<TElement>::Ptr measure,
      const ParametersContainer& parameters)
  : feature_count_(0),
    measure_(measure) {
    this->setParameters(parameters);
  }
  /**
   * Constructor creates a GPLearner. But leaves p_measure uninitialized.
   * \param parameters the ParametersContainer parameters from which would
   * overwrite the default parameters.
   */
  explicit GPLearner(const ParametersContainer& parameters)
  : feature_count_(0) {
    this->setParameters(parameters);
  }

  explicit GPLearner(typename Measure<TElement>::Ptr measure,
                     int population_size = 10,
                     int number_of_generations = 3,
                     int min_init_depth = 2,
                     int max_init_depth = 5,
                     double init_grow_probability = 0.5,
                     int seed = 1)
  : feature_count_(0),
    measure_(measure),
    population_size_(population_size),
    number_of_generations_(number_of_generations),
    min_init_depth_(min_init_depth),
    max_init_depth_(max_init_depth),
    init_grow_probability_(init_grow_probability),
    seed_(seed) {}

  explicit GPLearner(int population_size = 10,
                     int number_of_generations = 3,
                     int min_init_depth = 2,
                     int max_init_depth = 5,
                     double init_grow_probability = 0.5,
                     int seed = 1)
  : feature_count_(0),
    population_size_(population_size),
    number_of_generations_(number_of_generations),
    min_init_depth_(min_init_depth),
    max_init_depth_(max_init_depth),
    init_grow_probability_(init_grow_probability),
    seed_(seed) {}
  /**
   * The function sets up default parameters for genetic learning process.
   */
  virtual void setDefaultParameters();
  /**
   * The method checks the correctness of the parameters in the parameters
   * container. If one of them is not correct it throws
   * std::logical_error(PARAMETER_NAME).
   */
  virtual void checkParameters() const;
  /**
   * The function recreates the context and reinitializes the population.
   */
  void reset();

  string toString() const;
  /**
   * The function sets up context and population from the given GPScorer.
   * \param scorer GPScorer whose population and context would be set up.
   */
  void setInitialScorer(const GPScorer& scorer);

  void addPopulationHandler(BasePopulationHandler::Ptr
                            new_population_handler);

  void addGPOperation(BaseGPOperation::Ptr
                      new_gp_operation);

  GET_SET(int, population_size);
  GET_SET(int, number_of_generations);
  GET_SET(int, min_init_depth);
  GET_SET(int, max_init_depth);
  GET_SET(double, init_grow_probability);
  GET_SET(int, seed);
  GET_SET(std::vector<BasePopulationHandler::Ptr>, population_handlers);
  GET_SET(std::vector<BaseGPOperation::Ptr>, gp_operations);

 private:
  virtual void setParametersImpl(const ParametersContainer& parameters);

  virtual string getDefaultAlias() const {return "GPLeaner";}
  /**
   * Method clears and adds primitives to the context.
   */
  void initContext();
  /**
   * Method creates initial population.
   */
  void initPopulation();
  /**
   * \brief This function implements the changes made in the population at each
   * algorithm's iteration.
   */
  virtual void evaluationStepImpl();
  /**
   * The implementation of genetic programming optimization approach.
   * \param data DataSet on which the measure would be maximized within the
   * learning procedure.
   */
  void learnImpl(const DataSet<TElement>& data, GPScorer* scorer);
  /**
   * Method evaluates the population, it sets individ tree fitness to the
   * average on the data set metric value.
   * @param data data set for calculation of the average metric value
   */
  void evaluatePopulation(const DataSet<TElement>& data);
  /**
   * The number of features, to process dataset with the number of features
   * the context_ is constructed for.
   */
  int feature_count_;

  typename Measure<TElement>::Ptr measure_;

  int population_size_;
  int number_of_generations_;
  int min_init_depth_;
  int max_init_depth_;
  double init_grow_probability_;
  int seed_;

 protected:
  /**
   * The set of Puppy::trees (formulas that represent the optimization space),
   * that represent a population within the learning procedure.
   */
  Population population_;
  /**
   * Context that contains all the routine needed to build Puppy::trees within
   * the population.
   */
  Puppy::Context context_;
  /**
   * The best Puppy::tree(formula,
   * individ) in the population.
   */
  Puppy::Tree best_tree_;

  std::vector<BasePopulationHandler::Ptr> population_handlers_;
  std::vector<BaseGPOperation::Ptr> gp_operations_;
};

// template realizations

template <typename TElement>
void GPLearner<TElement>::setDefaultParameters() {
  population_size_ = 10;
  number_of_generations_ = 3;
  min_init_depth_ = 2;
  max_init_depth_ = 5;
  init_grow_probability_ = 0.5;
  seed_ = 1;
}

template <typename TElement>
void GPLearner<TElement>::checkParameters() const {
  CHECK(population_size_ > 0); // NOLINT
  CHECK(number_of_generations_ > 0); // NOLINT
  CHECK(min_init_depth_ > 0); // NOLINT
  CHECK(max_init_depth_ > min_init_depth_ - 1); // NOLINT
  CHECK(0.0 <= init_grow_probability_ && init_grow_probability_ <= 1.0); // NOLINT
}

template <typename TElement>
void GPLearner<TElement>::reset() {
  this->initContext();
  this->initPopulation();
}

template <typename TElement>
string GPLearner<TElement>::toString() const {
  std::stringstream str;
  std::fixed(str);
  str.precision(2);
  str << "Genetic programming learner with parameters: ";
  str << "population_size = " << population_size_ << ", ";
  str << "number_of_generations = " << number_of_generations_ << ", ";
  str << "min_init_depth = " << min_init_depth_ << ", ";
  str << "max_init_depth = " << max_init_depth_ << ", ";
  str << "init_grow_probability = " << init_grow_probability_ << ", ";
  str << "seed = " << seed_;
  return str.str();
}

template <typename TElement>
void GPLearner<TElement>::setInitialScorer(const GPScorer& scorer) {
  best_tree_ = scorer.best_tree_;
  context_ = scorer.context_;
  feature_count_ = scorer.feature_count_;
}

template <typename TElement>
void GPLearner<TElement>::addPopulationHandler(
  BasePopulationHandler::Ptr new_population_handler) {
    population_handlers_.push_back(new_population_handler);
}

template <typename TElement>
void GPLearner<TElement>::addGPOperation(
  BaseGPOperation::Ptr new_gp_operation) {
    gp_operations_.push_back(new_gp_operation);
}

template <typename TElement>
void GPLearner<TElement>::
  setParametersImpl(const ParametersContainer& parameters) {
    population_size_ = parameters.Get<int>("POPULATION_SIZE");
    number_of_generations_ = parameters.Get<int>("NUMBER_OF_GENERATIONS");
    min_init_depth_ = parameters.Get<int>("MIN_INIT_DEPTH");
    max_init_depth_ = parameters.Get<int>("MAX_INIT_DEPTH");
    init_grow_probability_ = parameters.Get<double>("INIT_GROW_PROBABILITY");
    seed_ = parameters.Get<int>("SEED");
}

template <typename TElement>
void GPLearner<TElement>::initContext() {
  Puppy::Context newContext;
  context_= newContext;

  context_.mRandom.seed(seed_);

  for (int gp_operations_index = 0;
       gp_operations_index < (int)gp_operations_.size();
       ++gp_operations_index) {
    context_.insert(gp_operations_[gp_operations_index].get());
  }

  if (gp_operations_.empty()) {
    context_.insert(new Add);
    context_.insert(new Subtract);
    context_.insert(new Multiply);
    context_.insert(new Divide);
  }

  for (int feature_index = 0;
       feature_index < feature_count_;
       ++feature_index) {
    std::string feature_name = "feature[";
    feature_name += boost::lexical_cast<string>(feature_index);
    feature_name += "]";
    context_.insert(new Puppy::TokenT<double>(feature_name));
  }
}

template <typename TElement>
void GPLearner<TElement>::initPopulation() {
  population_.clear();

  population_.resize(population_size_);
  Puppy::initializePopulation(population_, context_, init_grow_probability_,
                              min_init_depth_, max_init_depth_);
}

template <typename TElement>
void GPLearner<TElement>::evaluationStepImpl() {
  if (population_handlers_.empty()) {
    population_handlers_.push_back(new DefaultSelectionStrategy);
    population_handlers_.push_back(new DefaultCrossoverStrategy);
    population_handlers_.push_back(new DefaultMutationStandartStrategy);
    population_handlers_.push_back(new DefaultMutationSwapStrategy);
  }

  for (int population_handler_index = 0;
       population_handler_index < (int) population_handlers_.size();
       ++population_handler_index) {
    population_handlers_[population_handler_index]->
      handlePopulation(population_, context_);
  }
}

template <typename TElement>
void GPLearner<TElement>::
  learnImpl(const DataSet<TElement>& data, GPScorer* scorer) {
    if (data.feature_count() != feature_count_) {
      feature_count_ = data.feature_count();
      reset();
    }

    rDebug("Evaluating data for the first time");
    this->evaluatePopulation(data);

    rDebug("Evolution begins.\n");
    for (int generationIdx = 0;
         generationIdx < number_of_generations_;
         ++generationIdx) {
      rDebug("Generation %d", generationIdx);

      rDebug("Calling strategy");
      this->evaluationStepImpl();

      rDebug("Evaluation");
      this->evaluatePopulation(data);

      int best_tree_index = 0;
      for (int tree_index = 1;
           tree_index < (int)population_.size(); ++tree_index) {
        if (population_[best_tree_index].mFitness <
            population_[tree_index].mFitness) {
          best_tree_index = tree_index;
        }
      }

      rDebug("The best one is number %d", best_tree_index);
      using ::operator <<;
      stringstream log_out;
      best_tree_ = population_[best_tree_index];
      log_out << best_tree_ << " | ";
      log_out << "with fitness " << best_tree_.mFitness;
      rDebug(log_out.str().c_str());
    }
    // \TODO ? rewrite with setters and getters
    *scorer = GPScorer(best_tree_, context_, feature_count_);
}

template <typename TElement>
void GPLearner<TElement>::evaluatePopulation(const DataSet<TElement>& data) {
  for (int tree_index = 0;
       tree_index < (int)population_.size(); ++tree_index) {
    if (population_[tree_index].mValid) {
      continue;
    }
    GPScorer gp_scorer(population_[tree_index], context_, feature_count_);

    gp_scorer.markDataSet(data);

    // This line yields a topic for research. Why so?
    //
    double measureVal = this->measure_->average(data);

    population_[tree_index].mFitness = static_cast<float>(measureVal);
    population_[tree_index].mValid = true;
  }
}
};
};
#endif  // LTR_LEARNERS_GP_LEARNER_GP_LEARNER_H_
