// Copyright 2013 Yandex

#include "ltr/optimization/population_initers/naive_initer.h"

namespace optimization {
void NaiveIniter::populationInit(int population_size,
                                 const Set& set,
                                 Population* population) const {
  Point random_point;
  for (int point_id = 0; point_id < population_size; ++point_id) {
    set.sampleRandomPointInside(&random_point);
    population->addPoint(random_point);
  }
}
}
