// Copyright 2013 Yandex

#include "ltr/feature_converters/pca_learner.h"
#include <algorithm>

using std::min;

namespace ltr {
int KaiserRule::getNumberOfComponents(
    const VectorXd& singular_values) const {
  double average_singular_value = singular_values.mean();

  int number_of_components = 0;
  for (int value_index = 0;
       value_index < (int)singular_values.size();
       ++value_index) {
    if (singular_values[value_index] > average_singular_value) {
      ++number_of_components;
    }
  }
  return number_of_components;
}

FixedNumberOfComponentsRule::FixedNumberOfComponentsRule(int components_count) {
  CHECK(components_count > 0);
  components_count_ = components_count;
}

int FixedNumberOfComponentsRule::getNumberOfComponents(
    const VectorXd& singular_values) const {
  return min(components_count_, (int)singular_values.size());
}

void BrokenStickRule::getStickParts(int parts_count,
                                    vector<double>* stick_parts) {
  stick_parts->resize(parts_count);
  for (int stick_index = 0; stick_index < parts_count; ++stick_index) {
    (*stick_parts)[stick_index] = 0;
    for (int n = stick_index + 1; n <= parts_count; ++n) {
      (*stick_parts)[stick_index] += 1 / static_cast<double>(n);
    }
    (*stick_parts)[stick_index] /= parts_count;
  }
}

int BrokenStickRule::getNumberOfComponents(
    const VectorXd& singular_values) const {
  int components_count = singular_values.size();
  vector<double> stick_parts;
  getStickParts(components_count, &stick_parts);
  double trace = singular_values.sum();

  for (int component_index = 0;
       component_index < components_count;
       ++component_index) {
    if (singular_values[component_index] / trace < stick_parts[component_index])
      return component_index;
  }
  return components_count;
}
};
