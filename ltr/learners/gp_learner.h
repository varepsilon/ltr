// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_H_
#define LTR_LEARNERS_GP_LEARNER_H_

#include <boost/lexical_cast.hpp>

#include <string>

#include "contrib/puppy/Puppy.hpp"

#include "ltr/learners/learner.h"
#include "ltr/learners/utility/gp_primitives.h"
#include "ltr/learners/utility/gp_functions.h"
#include "ltr/scorers/gp_scorer.h"
#include "ltr/measures/measure.h"
#include "ltr/interfaces/utility/parametrized_utility.h"

namespace ltr {
namespace gp {
/**
 \class GPLearner
 Implements genetic programming approach applied to learning to rank.
 \tparam TElement object container of those the dataset consists (it can be
 Object, ObjectPair, ObjectList).
 */
template <typename TElement>
class GPLearner : public Learner<TElement, GPScorer> {
  public:
  /** Constructor creates a GPLearner.
   * \param p_Measure shared pointer to the measure that would be maximized on
   * a dataset within learning.
   * \param parameters the ParametersContainer parameters from which would
   * overwrite the default parameters.
   */
  GPLearner(typename Measure<TElement>::Ptr p_Measure,
      const ParametersContainer& parameters = ParametersContainer())
  : featureCountInContext_(0),
  inPopulationBestTreeIdx_(0),
  Learner("GPLearner") {
    this->setMeasure(p_Measure);
    this->setDefaultParameters();
    this->copyParameters(parameters);
  }
  /** Constructor creates a GPLearner. But leaves p_measure uninitialized.
   * \param parameters the ParametersContainer parameters from which would
   * overwrite the default parameters.
   */
  GPLearner(const ParametersContainer& parameters = ParametersContainer())
  : featureCountInContext_(0),
  inPopulationBestTreeIdx_(0),
  Learner("GPLearner") {
    this->setDefaultParameters();
    this->copyParameters(parameters);
  }

  /** The function sets up default parameters for genetic learning process.
   */
  virtual void setDefaultParameters() {
    this->clearParameters();
    this->addIntParameter("POP_SIZE", 10);
    this->addIntParameter("NBR_GEN", 3);
    this->addIntParameter("NBR_PART", 2);
    this->addIntParameter("MAX_DEPTH", 35);
    this->addIntParameter("MIN_INIT_DEPTH", 2);
    this->addIntParameter("MAX_INIT_DEPTH", 5);
    this->addDoubleParameter("INIT_GROW_PROBA", 0.5);
    this->addDoubleParameter("CROSSOVER_PROBA", 0.9);
    this->addDoubleParameter("CROSSOVER_DISTRIB_PROBA", 0.5);
    this->addDoubleParameter("MUT_STD_PROBA", 0.05);
    this->addIntParameter("MUT_MAX_REGEN_DEPTH", 5);
    this->addDoubleParameter("MUT_SWAP_PROBA", 0.05);
    this->addDoubleParameter("MUT_SWAP_DISTRIB_PROBA", 0.5);
    this->addIntParameter("SEED", 1);
    this->addBoolParameter("USE_ADD", true);
    this->addBoolParameter("USE_SUB", true);
    this->addBoolParameter("USE_MUL", true);
    this->addBoolParameter("USE_DIV", true);
    this->addBoolParameter("USE_IF", true);
    this->addBoolParameter("USE_EFEM", true);
  }
  /** The method checks the correctness of the parameters in the parameters
   * container. If one of them is not correct it throws
   * std::logical_error(PARAMETER_NAME).
   */
  virtual void checkParameters() const {
    CHECK_INT_PARAMETER("POP_SIZE", X > 0);
    CHECK_INT_PARAMETER("NBR_GEN", X > 0);
    CHECK_INT_PARAMETER("NBR_PART", X > 1);
    CHECK_INT_PARAMETER("MAX_DEPTH", X > 0);
    CHECK_INT_PARAMETER("MIN_INIT_DEPTH", X > 0);
    CHECK_INT_PARAMETER("MUT_MAX_REGEN_DEPTH", X > 0);

    CHECK_INT_PARAMETER("MAX_INIT_DEPTH", \
      X > this->getIntParameter("MIN_INIT_DEPTH") - 1);

    CHECK_DOUBLE_PARAMETER("INIT_GROW_PROBA", X >= 0 && X <= 1);
    CHECK_DOUBLE_PARAMETER("CROSSOVER_PROBA", X >= 0 && X <= 1);
    CHECK_DOUBLE_PARAMETER("CROSSOVER_DISTRIB_PROBA", X >= 0 && X <= 1);
    CHECK_DOUBLE_PARAMETER("MUT_STD_PROBA", X >= 0 && X <= 1);
    CHECK_DOUBLE_PARAMETER("MUT_SWAP_PROBA", X >= 0 && X <= 1);
    CHECK_DOUBLE_PARAMETER("MUT_SWAP_DISTRIB_PROBA", X >= 0 && X <= 1);
  }
  /** The function recreates the context and reinitializes the population.
   */
  void reset() {
    this->initContext();
    this->initPopulation();
  }

  /** The function sets up context and population from the given GPScorer.
   * \param in_scorer GPScorer whose population and context would be set up.
   */
  void setInitialScorer(const GPScorer& in_scorer) {
    population_ = in_scorer.population_;
    context_ = in_scorer.context_;
    featureCountInContext_ = in_scorer.featureCountInContext_;
    inPopulationBestTreeIdx_ = in_scorer.inPopulationBestTreeIdx_;
  }
  /** The function return trained GPscorer after learning process
   */
  GPScorer make() const {
    return GPScorer(this->population_, this->context_,
        this->featureCountInContext_, this->inPopulationBestTreeIdx_);
  }

  private:
  /** Method clears and adds primitives to the context.
   */
  void initContext() {
    Puppy::Context newContext;
    context_= newContext;

    context_.mRandom.seed(this->getIntParameter("SEED"));
    if (this->getBoolParameter("USE_ADD")) {
      context_.insert(new Add);
    }
    if (this->getBoolParameter("USE_SUB")) {
      context_.insert(new Subtract);
    }
    if (this->getBoolParameter("USE_MUL")) {
      context_.insert(new Multiply);
    }
    if (this->getBoolParameter("USE_DIV")) {
      context_.insert(new Divide);
    }
    if (this->getBoolParameter("USE_IF")) {
      context_.insert(new IfThenFunc);
    }
    if (this->getBoolParameter("USE_EFEM")) {
      context_.insert(new Ephemeral);
    }
    for (int featureIdx = 0;
        featureIdx < featureCountInContext_;
        ++featureIdx) {
      std::string featureName = "feature[";
      featureName += boost::lexical_cast<string>(featureIdx);
      featureName += "]";
      context_.insert(new Puppy::TokenT<double>(featureName));
    }
  }

  /** Method creates initial population.
   */
  void initPopulation() {
    population_.clear();
    population_.resize(this->getIntParameter("POP_SIZE"));
    Puppy::initializePopulation(population_, context_,
        this->getDoubleParameter("INIT_GROW_PROBA"),
        this->getIntParameter("MIN_INIT_DEPTH"),
        this->getIntParameter("MAX_INIT_DEPTH"));
  }

  /** \brief This function implements the changes made in the population at each
   *  algorithm's iteration.
   */
  virtual void evaluationStrategyStepImpl() {
    std::cout << "Tournament.\n";
    Puppy::applySelectionTournament(population_, context_,
        this->getIntParameter("NBR_PART"));

    std::cout << "Crossover.\n";
    Puppy::applyCrossover(population_, context_,
        this->getDoubleParameter("CROSSOVER_PROBA"),
        this->getDoubleParameter("CROSSOVER_DISTRIB_PROBA"),
        this->getIntParameter("MAX_DEPTH"));

    std::cout << "Mutation standart.\n";
    Puppy::applyMutationStandard(population_, context_,
        this->getDoubleParameter("MUT_STD_PROBA"),
        this->getIntParameter("MUT_MAX_REGEN_DEPTH"),
        this->getIntParameter("MAX_DEPTH"));

    std::cout << "Mutation swap.\n";
    Puppy::applyMutationSwap(population_, context_,
        this->getDoubleParameter("MUT_SWAP_PROBA"),
        this->getDoubleParameter("MUT_SWAP_DISTRIB_PROBA"));
  }

  /** The implementation of genetic programming optimization approach.
   * \param data DataSet on which the p_Measure would be maximized within the
   * learning procedure.
   */
  void learnImpl(const DataSet<TElement>& data) {
    if (data.featureCount() != featureCountInContext_) {
      featureCountInContext_ = data.featureCount();
      reset();
    }

    std::cout << "Evaluating data for the first time.\n";
    this->evaluatePopulation(data);

    std::cout << "The population looks like: \n";
    for (size_t treeIdx = 0; treeIdx < population_.size(); ++treeIdx) {
      using ::operator <<;
      std::cout << population_[treeIdx] << std::endl;
    }

    std::cout << "Evolution begins.\n";
    for (int generationIdx = 0;
        generationIdx < this->getIntParameter("NBR_GEN");
        ++generationIdx) {
      std::cout << "Generation "<< generationIdx << ".\n";

      std::cout << "Calling strategy\n";
      this->evaluationStrategyStepImpl();

      std::cout << "Evaluation.\n";
      this->evaluatePopulation(data);

      inPopulationBestTreeIdx_ = 0;
      for (size_t treeIdx = 1; treeIdx < population_.size(); ++treeIdx) {
        if (population_[inPopulationBestTreeIdx_].mFitness <
            population_[treeIdx].mFitness) {
          inPopulationBestTreeIdx_ = treeIdx;
        }
      }

      std::cout
      << "The best one is number " << inPopulationBestTreeIdx_ << ".\n";
      using ::operator <<;
      std::cout << population_[inPopulationBestTreeIdx_] << std::endl;
      std::cout << "with fitness " <<
          population_[inPopulationBestTreeIdx_].mFitness << "\n";
    }
  }

  /** Method evaluates the population, it sets individ tree fitness to the
   *  average on the data set metric value.
   *  @param data data set for calculation of the average metric value
   */
  void evaluatePopulation(const DataSet<TElement>& data) {
    for (size_t treeIdx = 0; treeIdx < population_.size(); ++treeIdx) {
      if (population_[treeIdx].mValid) {
        continue;
      }
      markDataSetWithTree<TElement>(data, &context_, &population_[treeIdx]);

      // This line yields a topic for research. Why so?
      //
      double measureVal = this->p_measure_->weightedAverage(data);

      population_[treeIdx].mFitness = static_cast<float>(measureVal);
      population_[treeIdx].mValid = true;
    }
  }

  /** Smart pointer to the measure object, the measure would be maximized
   * within the learning procedure.
   */
  typename Measure<TElement>::Ptr p_Measure_;
  size_t featureCountInContext_;
  /** The index of the best Puppy::tree (formula, individ) in current
   * population.
   */
  size_t inPopulationBestTreeIdx_;

  protected:
  /** The set of Puppy::trees (formulas that represent the optimization space),
   * that represent a population within the learning procedure.
   */
  Population population_;
  /** Context that contains all the routine needed to build Puppy::trees within
   * the population.
   */
  Puppy::Context context_;
  /** The number of features, to process dataset with the number of features
   * the context_ is constructed for.
   */
};

template class GPLearner<Object>;
template class GPLearner<ObjectPair>;
template class GPLearner<ObjectList>;
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_H_
