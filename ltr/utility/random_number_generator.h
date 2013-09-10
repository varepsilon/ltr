// Copyright 2012 Yandex

#ifndef LTR_UTILITY_RANDOM_NUMBER_GENERATOR_H_
#define LTR_UTILITY_RANDOM_NUMBER_GENERATOR_H_

#include <algorithm>

#include "contrib/random/Random.hpp"
#include "random/Random.hpp"

namespace ltr {
namespace utility {
class Randomizer {
 public:
  Randomizer() {
    generator_.Reseed(0);
  }

  double doubleRand(double min_value, double max_value) {
    return generator_.Float()*(max_value - min_value) + min_value;
  }

  int rand(int max_value = 2147483647) {
    return generator_.Integer(max_value);
  };

  void setSeed(int seed) {
    generator_.Reseed(seed);
  };

  int operator()(int max_value = 2147483647) {
    return rand(max_value);
  }

  RandomLib::Random generator_;
};
extern Randomizer randomizer;
};
};
#endif  // LTR_UTILITY_RANDOM_NUMBER_GENERATOR_H_
