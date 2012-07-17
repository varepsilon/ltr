// Copyright 2012 Yandex

#include <gtest/gtest.h>

#include <boost/filesystem.hpp>

#include <string>

#include "ltr/data/object.h"
#include "ltr/data/utility/io_utility.h"

#include "ltr/measures/dcg.h"
#include "ltr/measures/true_point.h"

#include "ltr/feature_converters/nan_to_zero_converter.h"

#include "ltr/learners/gp_learner/gp_learner.h"
#include "ltr/learners/gp_learner/gp_primitives.h"
#include "ltr/learners/gp_learner/strategies/population_handler.h"

#include "ltr/learners/gp_learner/strategies/default_selection_strategy.h"
#include "ltr/learners/gp_learner/strategies/default_crossover_strategy.h"
#include "ltr/learners/gp_learner/strategies/default_mutation_standart_strategy.h"
#include "ltr/learners/gp_learner/strategies/default_mutation_swap_strategy.h"

#include "ltr/learners/gp_learner/strategies/select_top_trees_strategy.h"
#include "ltr/learners/gp_learner/strategies/crossover_adjacent_trees_strategy.h"

using boost::filesystem::path;

using std::string;
using std::cout;

using ltr::DCG;
using ltr::DataSet;
using ltr::Object;
using ltr::ObjectList;
using ltr::Measure;

using ltr::io_utility::loadDataSet;

using ltr::NanToZeroConverter;

using ltr::gp::GPLearner;
using ltr::gp::GPScorer;
using ltr::gp::DefaultSelectionStrategy;
using ltr::gp::DefaultCrossoverStrategy;
using ltr::gp::DefaultMutationStandartStrategy;
using ltr::gp::DefaultMutationSwapStrategy;
using ltr::gp::SelectTopTreesStrategy;
using ltr::gp::CrossoverAdjacentTreesStrategy;

class GPLearnerTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    string learn_data_file_name =
      path("data/imat2009/imat2009_learning_small.txt").string();
    learn_data = loadDataSet<Object>(learn_data_file_name, "YANDEX");

    NanToZeroConverter::Ptr nan_to_zero_converter
    (new NanToZeroConverter(learn_data.feature_info()));
    nan_to_zero_converter->apply(learn_data, &learn_data);
  }

  DataSet<Object> learn_data;
};

TEST_F(GPLearnerTest, GPLearnerTest) {
  Measure<ObjectList>::Ptr pMeasure(new DCG());
  GPLearner<ObjectList> gp_learner(pMeasure);

  string learn_data_file_name =
    path("data/imat2009/imat2009_learning_small.txt").string();

  DataSet<ObjectList> learn_data_listwise =
    loadDataSet<ObjectList>(learn_data_file_name, "YANDEX");

  gp_learner.learn(learn_data_listwise);

  GPScorer::Ptr scorer = gp_learner.makeSpecific();

  cout << scorer->generateCppCode();
};

TEST_F(GPLearnerTest, DefaultSelectionStrategyTest) {
  DefaultSelectionStrategy::Ptr selection_strategy
    (new DefaultSelectionStrategy());

  GPLearner<Object> gp_learner(
    static_cast<ltr::TruePoint::Ptr>(new ltr::TruePoint));
  ASSERT_NO_THROW(gp_learner.addPopulationHandler(selection_strategy));

  ASSERT_NO_THROW(gp_learner.learn(learn_data));
}

TEST_F(GPLearnerTest, DefaultCrossoverStrategyTest) {
  DefaultCrossoverStrategy::Ptr crossover_strategy
    (new DefaultCrossoverStrategy());

  GPLearner<Object> gp_learner(
    static_cast<ltr::TruePoint::Ptr>(new ltr::TruePoint));
  ASSERT_NO_THROW(gp_learner.addPopulationHandler(crossover_strategy));

  ASSERT_NO_THROW(gp_learner.learn(learn_data));
}

TEST_F(GPLearnerTest, DefaultMutationStandartStrategyTest) {
  DefaultMutationStandartStrategy::Ptr mutation_standart_strategy
    (new DefaultMutationStandartStrategy());

  GPLearner<Object> gp_learner(
    static_cast<ltr::TruePoint::Ptr>(new ltr::TruePoint));
  ASSERT_NO_THROW(gp_learner.addPopulationHandler(mutation_standart_strategy));

  ASSERT_NO_THROW(gp_learner.learn(learn_data));
}

TEST_F(GPLearnerTest, DefaultMutationSwapStrategyTest) {
  DefaultMutationSwapStrategy::Ptr mutation_swap_strategy
    (new DefaultMutationSwapStrategy());

  GPLearner<Object> gp_learner(
    static_cast<ltr::TruePoint::Ptr>(new ltr::TruePoint));
  ASSERT_NO_THROW(gp_learner.addPopulationHandler(mutation_swap_strategy));

  ASSERT_NO_THROW(gp_learner.learn(learn_data));
}

TEST_F(GPLearnerTest, SelectTopTreesStrategyTest) {
  SelectTopTreesStrategy::Ptr multiply_top_trees_strategy
    (new SelectTopTreesStrategy());

  GPLearner<Object> gp_learner(
    static_cast<ltr::TruePoint::Ptr>(new ltr::TruePoint));
  ASSERT_NO_THROW(gp_learner.addPopulationHandler(
    multiply_top_trees_strategy));

  ASSERT_NO_THROW(gp_learner.learn(learn_data));
}

TEST_F(GPLearnerTest, CrossoverAdjacentTreesStrategyTest) {
  CrossoverAdjacentTreesStrategy::Ptr crossover_adjacent_trees_strategy
    (new CrossoverAdjacentTreesStrategy());

  GPLearner<Object> gp_learner(
    static_cast<ltr::TruePoint::Ptr>(new ltr::TruePoint));
  ASSERT_NO_THROW(gp_learner.addPopulationHandler(
    crossover_adjacent_trees_strategy));

  ASSERT_NO_THROW(gp_learner.learn(learn_data));
}