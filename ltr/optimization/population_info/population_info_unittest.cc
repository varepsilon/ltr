// Copyright 2013 Yandex

#include <gtest/gtest.h>
#include "ltr/optimization/population_info/population.h"
#include "ltr/optimization/population_initers/naive_initer.h"
#include "ltr/optimization/population_info/population_info.hpp"

namespace optimization {
TEST(NoInitPopulationInfoTest, FirstNoInitPopulationInfoTest) {
  Population::Ptr population(new Population);
  int population_size = 10;
  for (int point_index = 0; point_index < population_size; ++point_index) {
    Point point(10);
    point.setConstant(1.0);
    population->addPoint(point);
  }
  NoInitPopulationInfo<int>::Ptr population_info;
  EXPECT_NO_THROW(population_info = new NoInitPopulationInfo<int>(
      population.get(), new int));
  for (Population::Iterator individual = population->begin();
       individual != population->end();
       ++individual) {
    EXPECT_NO_THROW(*population_info->getInfoById(individual.point_id()) = 3);
    EXPECT_EQ(*population_info->getInfoById(individual.point_id()), 3);
  }
  EXPECT_NO_THROW(population_info->removePoint(population->begin().point_id()));
  EXPECT_ANY_THROW(
      *population_info->getInfoById(population->begin().point_id()));
}
}
