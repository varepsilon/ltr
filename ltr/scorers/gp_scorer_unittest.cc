// Copyright 2012 Yandex
#include <gtest/gtest.h>
#include <vector>

#include "contrib/puppy/Puppy.hpp"
#include "ltr/scorers/gp_scorer.h"
#include "ltr/utility/numerical.h"

using std::vector;
using ltr::gp::GPScorer;
using ltr::utility::DoubleEqual;

typedef vector<Puppy::Tree> Population;

TEST(GPScorer, GPScorerTest) {
  Puppy::Context context;
  context.mRandom.seed(1);
  context.insert(new ltr::gp::Add);
  context.insert(new ltr::gp::Subtract);
  context.insert(new ltr::gp::Multiply);
  context.insert(new ltr::gp::Divide);
  context.insert(new ltr::gp::IfThenFunc);
  context.insert(new ltr::gp::Ephemeral);

  int features_count = 3;

  for (int feature_index = 0;
        feature_index < features_count;
        ++feature_index) {
    std::string feature_name = "feature[";
    feature_name += ltr::utility::lexical_cast<string>(feature_index);
    feature_name += "]";
    context.insert(new Puppy::TokenT<double>(feature_name));
  }

  Population population;
  int population_size = 10;
  population.resize(population_size);
  double init_grow_probability = 0.5;
  int min_init_depth = 2;
  int max_init_depth = 5;
  Puppy::initializePopulation(population,
                              context,
                              init_grow_probability,
                              min_init_depth,
                              max_init_depth);

  int best_tree_index = 0;

  GPScorer gp_scorer =
    GPScorer(population[best_tree_index], context, features_count);

  Object object1;
  object1 << 1.3 << 1.7 << 9.2;

  rInfo("%f", gp_scorer(object1));

  Object object2;
  object2 << 1.9 << 15.8 << 0.7;

  rInfo("%f", gp_scorer(object2));
}
