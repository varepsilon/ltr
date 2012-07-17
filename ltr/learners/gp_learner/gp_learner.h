// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_GP_LEARNER_H_
#define LTR_LEARNERS_GP_LEARNER_GP_LEARNER_H_

#include <boost/lexical_cast.hpp>

#include <string>
#include <vector>
#include <functional>

#include "contrib/puppy/Puppy.hpp"

#include "ltr/learners/learner.h"
#include "ltr/learners/gp_learner/gp_primitives.h"
#include "ltr/learners/gp_learner/gp_functions.h"
#include "ltr/learners/gp_learner/strategies/population_handler.h"
#include "ltr/scorers/gp_scorer.h"
#include "ltr/measures/measure.h"
#include "ltr/measures/reciprocal_rank.h"

#include "ltr/parameters_container/parameters_container.h"

namespace ltr {
namespace gp {
/**
 \brief GPLearner
 Implements genetic programming approach applied to learning to rank.
 \tparam TElement object container of those the dataset consists (it can be
 Object, ObjectPair, ObjectList).
 */
template <typename TElement>
class GPLearner : public BaseLearner<TElement, GPScorer> {
 public:
  /** Constructor creates a GPLearner.
   * \param measure shared pointer to the measure that would be maximized on
   * a dataset within learning.
   * \param parameters the ParametersContainer parameters from which would
   * overwrite the default parameters.
   */
  explicit GPLearner(typename Measure<TElement>::Ptr measure,
      const ParametersContainer& parameters)
  : feature_count_(0),
    best_tree_index_(0) {
    measure_ = measure;
    this->setParameters(parameters);
  }
  /** Constructor creates a GPLearner. But leaves p_measure uninitialized.
   * \param parameters the ParametersContainer parameters from which would
   * overwrite the default parameters.
   */
  explicit GPLearner(const ParametersContainer& parameters)
  : feature_count_(0),
    best_tree_index_(0) {
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
    best_tree_index_(0),
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
    best_tree_index_(0),
    population_size_(population_size),
    number_of_generations_(number_of_generations),
    min_init_depth_(min_init_depth),
    max_init_depth_(max_init_depth),
    init_grow_probability_(init_grow_probability),
    seed_(seed) {}
  /** The function sets up default parameters for genetic learning process.
   */
  virtual void setDefaultParameters() {
    population_size_ = 10;
    number_of_generations_ = 3;
    min_init_depth_ = 2;
    max_init_depth_ = 5;
    init_grow_probability_ = 0.5;
    seed_ = 1;
  }
  /** The method checks the correctness of the parameters in the parameters
   * container. If one of them is not correct it throws
   * std::logical_error(PARAMETER_NAME).
   */
  virtual void checkParameters() const {
    CHECK(population_size_ > 0); // NOLINT
    CHECK(number_of_generations_ > 0); // NOLINT
    CHECK(min_init_depth_ > 0); // NOLINT
    CHECK(max_init_depth_ > min_init_depth_ - 1);
    CHECK(0.0 <= init_grow_probability_ && init_grow_probability_ <= 1.0);
  }
  /** The function recreates the context and reinitializes the population.
   */
  void reset() {
    this->initContext();
    this->initPopulation();
  }

  string toString() const {
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
  /** The function sets up context and population from the given GPScorer.
   * \param scorer GPScorer whose population and context would be set up.
   */
  void setInitialScorer(const GPScorer& scorer) {
    population_ = scorer.population_;
    context_ = scorer.context_;
    feature_count_ = scorer.feature_count_;
    best_tree_index_ = scorer.best_tree_index_;
  }

  void addPopulationHandler(const BasePopulationHandler::Ptr
                            new_population_handler) {
    population_handlers_.push_back(new_population_handler);
  }

  void addGPOperation(const BaseGPOperation::Ptr
                            new_gp_operation) {
    gp_operations_.push_back(new_gp_operation);
  }

  GET_SET(int, population_size);
  GET_SET(int, number_of_generations);
  GET_SET(int, min_init_depth);
  GET_SET(int, max_init_depth);
  GET_SET(double, init_grow_probability);
  GET_SET(int, seed);
  GET_SET(std::vector<BasePopulationHandler::Ptr>, population_handlers);
  GET_SET(std::vector<BaseGPOperation::Ptr>, gp_operations);

 private:
  virtual void setParametersImpl(const ParametersContainer& parameters) {
    population_size_ = parameters.Get<int>("POP_SIZE");
    number_of_generations_ = parameters.Get<int>("NBR_GEN");
    min_init_depth_ = parameters.Get<int>("MIN_INIT_DEPTH");
    max_init_depth_ = parameters.Get<int>("MAX_INIT_DEPTH");
    init_grow_probability_ = parameters.Get<double>("INIT_GROW_PROBA");
    seed_ = parameters.Get<int>("SEED");
  }

  virtual string getDefaultAlias() const {return "GPLeaner";}
  /** Method clears and adds primitives to the context.
   */
  void initContext() {
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
  /** Method creates initial population.
   */
  void initPopulation() {
    population_.clear();

    population_.resize(population_size_);
    Puppy::initializePopulation(population_, context_, init_grow_probability_,
                                min_init_depth_, max_init_depth_);
  }
  /** \brief This function implements the changes made in the population at each
   *  algorithm's iteration.
   */
  virtual void evaluationStepImpl() {
    for (int population_handler_index = 0;
         population_handler_index < (int) population_handlers_.size();
         ++population_handler_index) {
      population_handlers_[population_handler_index]->
        HandlePopulation(population_, context_);
    }
  }
  /** The implementation of genetic programming optimization approach.
   * \param data DataSet on which the measure would be maximized within the
   * learning procedure.
   */
  void learnImpl(const DataSet<TElement>& data, GPScorer* scorer) {
    if (data.feature_count() != feature_count_) {
      feature_count_ = data.feature_count();
      reset();
    }

    std::cout << "Evaluating data for the first time.\n";
    this->evaluatePopulation(data);

    std::cout << "The population looks like: \n";
    for (int tree_index = 0; tree_index < (int)population_.size(); ++tree_index) {
      using ::operator <<;
      std::cout << population_[tree_index] << std::endl;
    }

    std::cout << "Evolution begins.\n";
    for (int generationIdx = 0;
         generationIdx < number_of_generations_;
         ++generationIdx) {
      std::cout << "Generation "<< generationIdx << ".\n";

      std::cout << "Calling strategy\n";
      this->evaluationStepImpl();

      std::cout << "Evaluation.\n";
      this->evaluatePopulation(data);

      best_tree_index_ = 0;
      for (int tree_index = 1;
          tree_index < (int)population_.size(); ++tree_index) {
        if (population_[best_tree_index_].mFitness <
            population_[tree_index].mFitness) {
          best_tree_index_ = tree_index;
        }
      }

      std::cout
      << "The best one is number " << best_tree_index_ << ".\n";
      using ::operator <<;
      std::cout << population_[best_tree_index_] << std::endl;
      std::cout << "with fitness " <<
          population_[best_tree_index_].mFitness << "\n";
    }
    // \TODO ? rewrite with setters and getters
    *scorer = GPScorer(population_, context_,
                       feature_count_, best_tree_index_);
  }
  /** Method evaluates the population, it sets individ tree fitness to the
   *  average on the data set metric value.
   *  @param data data set for calculation of the average metric value
   */
  void evaluatePopulation(const DataSet<TElement>& data) {
    for (int tree_index = 0; tree_index < (int)population_.size(); ++tree_index) {
      if (population_[tree_index].mValid) {
        continue;
      }
      markDataSetWithTree<TElement>(data, &context_, &population_[tree_index]);

      // This line yields a topic for research. Why so?
      //
      double measureVal = this->measure_->weightedAverage(data);

      population_[tree_index].mFitness = static_cast<float>(measureVal);
      population_[tree_index].mValid = true;
    }
  }
  /** The number of features, to process dataset with the number of features
   * the context_ is constructed for.
   */
  int feature_count_;
  /** The index of the best Puppy::tree (formula, individ) in current
   * population.
   */
  int best_tree_index_;

  typename Measure<TElement>::Ptr measure_;

  int population_size_;
  int number_of_generations_;
  int min_init_depth_;
  int max_init_depth_;
  double init_grow_probability_;
  int seed_;

 protected:
  /** The set of Puppy::trees (formulas that represent the optimization space),
   * that represent a population within the learning procedure.
   */
  Population population_;
  /** Context that contains all the routine needed to build Puppy::trees within
   * the population.
   */
  Puppy::Context context_;

  std::vector<BasePopulationHandler::Ptr> population_handlers_;
  std::vector<BaseGPOperation::Ptr> gp_operations_;
  };
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_GP_LEARNER_H_
