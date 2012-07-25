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
  GPLearner<Object> gp_learner(new ltr::TruePoint);
  ASSERT_NO_THROW(
    gp_learner.addPopulationHandler(new DefaultSelectionStrategy));

  ASSERT_NO_THROW(gp_learner.learn(learn_data));
}

TEST_F(GPLearnerTest, DefaultCrossoverStrategyTest) {
  GPLearner<Object> gp_learner(new ltr::TruePoint);
  ASSERT_NO_THROW(
    gp_learner.addPopulationHandler(new DefaultCrossoverStrategy));

  ASSERT_NO_THROW(gp_learner.learn(learn_data));
}

TEST_F(GPLearnerTest, DefaultMutationStandartStrategyTest) {
  GPLearner<Object> gp_learner(new ltr::TruePoint);
  ASSERT_NO_THROW(
    gp_learner.addPopulationHandler(new DefaultMutationStandartStrategy));

  ASSERT_NO_THROW(gp_learner.learn(learn_data));
}

TEST_F(GPLearnerTest, DefaultMutationSwapStrategyTest) {
  GPLearner<Object> gp_learner(new ltr::TruePoint);
  ASSERT_NO_THROW(
    gp_learner.addPopulationHandler(new DefaultMutationSwapStrategy));

  ASSERT_NO_THROW(gp_learner.learn(learn_data));
}

TEST_F(GPLearnerTest, SelectTopTreesStrategyTest) {
  GPLearner<Object> gp_learner(new ltr::TruePoint);
  ASSERT_NO_THROW(
    gp_learner.addPopulationHandler(new SelectTopTreesStrategy));

  ASSERT_NO_THROW(gp_learner.learn(learn_data));
}

TEST_F(GPLearnerTest, CrossoverAdjacentTreesStrategyTest) {
  GPLearner<Object> gp_learner(new ltr::TruePoint);
  ASSERT_NO_THROW(
    gp_learner.addPopulationHandler(new CrossoverAdjacentTreesStrategy));

  ASSERT_NO_THROW(gp_learner.learn(learn_data));
}
