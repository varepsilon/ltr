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

namespace ltr {
namespace gp {

/**
 @class GPLearner
 Implements genetic programming approach applied to learning to rank.
 */
template <typename TElement>
class GPLearner : public Learner<TElement, GPScorer> {
  public:
  GPLearner(const ParametersContainer& parameters = ParametersContainer())
  : featureCountInContext_(0),
  inPopulationBestTreeIdx_(0) {
    this->setDefaultParameters();
    this->parameters().copyParameters(parameters);
  }
  GPLearner(typename Measure<TElement>::Ptr p_measure,
      const ParametersContainer& parameters = ParametersContainer())
  : featureCountInContext_(0),
  inPopulationBestTreeIdx_(0) {
    this->setMeasure(p_measure);
    this->setDefaultParameters();
    this->parameters().copyParameters(parameters);
  }

  void setDefaultParameters() {
    this->parameters().clear();
    this->parameters().setInt("POP_SIZE", 3);
    this->parameters().setInt("NBR_GEN", 3);
    this->parameters().setInt("NBR_PART", 2);
    this->parameters().setInt("MAX_DEPTH", 17);
    this->parameters().setInt("MIN_INIT_DEPTH", 2);
    this->parameters().setInt("MAX_INIT_DEPTH", 5);
    this->parameters().setDouble("INIT_GROW_PROBA", 0.5);
    this->parameters().setDouble("CROSSOVER_PROBA", 0.9);
    this->parameters().setDouble("CROSSOVER_DISTRIB_PROBA", 0.9);
    this->parameters().setDouble("MUT_STD_PROBA", 0.05);
    this->parameters().setInt("MUT_MAX_REGEN_DEPTH", 5);
    this->parameters().setDouble("MUT_SWAP_PROBA", 0.05);
    this->parameters().setDouble("MUT_SWAP_DISTRIB_PROBA", 0.5);
    this->parameters().setInt("SEED", 1);
    this->parameters().setBool("USE_ADD", true);
    this->parameters().setBool("USE_SUB", true);
    this->parameters().setBool("USE_MUL", true);
    this->parameters().setBool("USE_DIV", true);
    this->parameters().setBool("USE_IF", true);
    this->parameters().setBool("USE_EFEM", true);
  }

  void reset() {
    this->initContext();
    this->initPopulation();
  }

  void setInitialScorer(const GPScorer& in_scorer) {
    population_ = in_scorer.population_;
    context_ = in_scorer.context_;
    featureCountInContext_ = in_scorer.featureCountInContext_;
    inPopulationBestTreeIdx_ = in_scorer.inPopulationBestTreeIdx_;
  }

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

    context_.mRandom.seed(this->parameters().getInt("SEED"));
    if (this->parameters().getBool("USE_ADD")) {
      context_.insert(new Add);
    }
    if (this->parameters().getBool("USE_SUB")) {
      context_.insert(new Subtract);
    }
    if (this->parameters().getBool("USE_MUL")) {
      context_.insert(new Multiply);
    }
    if (this->parameters().getBool("USE_DIV")) {
      context_.insert(new Divide);
    }
    if (this->parameters().getBool("USE_IF")) {
      context_.insert(new IfThenFunc);
    }
    if (this->parameters().getBool("USE_EFEM")) {
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
    population_.resize(this->parameters().getInt("POP_SIZE"));
    Puppy::initializePopulation(population_, context_,
        this->parameters().getDouble("INIT_GROW_PROBA"),
        this->parameters().getInt("MIN_INIT_DEPTH"),
        this->parameters().getInt("MAX_INIT_DEPTH"));
  }

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
        generationIdx < this->parameters().getInt("NBR_GEN");
        ++generationIdx) {
      std::cout << "Generation "<< generationIdx << ".\n";

      std::cout << "Tournament.\n";
      Puppy::applySelectionTournament(population_, context_,
          this->parameters().getInt("NBR_PART"));

      std::cout << "Crossover.\n";
      Puppy::applyCrossover(population_, context_,
          this->parameters().getDouble("CROSSOVER_PROBA"),
          this->parameters().getDouble("CROSSOVER_DISTRIB_PROBA"),
          this->parameters().getInt("MAX_DEPTH"));

      std::cout << "Mutation standart.\n";
      Puppy::applyMutationStandard(population_, context_,
          this->parameters().getDouble("MUT_STD_PROBA"),
          this->parameters().getInt("MUT_MAX_REGEN_DEPTH"),
          this->parameters().getInt("MAX_DEPTH"));

      std::cout << "Mutation swap.\n";
      Puppy::applyMutationSwap(population_, context_,
          this->parameters().getDouble("MUT_SWAP_PROBA"),
          this->parameters().getDouble("MUT_SWAP_DISTRIB_PROBA"));

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
      double measureVal = this->p_measure_->average(data);
      // This line yields a topic for research. Why so?
      //
      population_[treeIdx].mFitness = static_cast<float>(measureVal);
    }
  }

  Population population_;
  Puppy::Context context_;
  size_t featureCountInContext_;
  size_t inPopulationBestTreeIdx_;
};
template class GPLearner<Object>;
template class GPLearner<ObjectPair>;
template class GPLearner<ObjectList>;
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_H_
