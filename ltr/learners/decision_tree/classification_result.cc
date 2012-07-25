// Copyright 2011 Yandex

#include <cmath>
#include <stdexcept>

#include "logog/logog.h"

#include "ltr/learners/decision_tree/classification_result.h"

namespace ltr {

ClassificationResult::ClassificationResult(int n) {
  INFO("Resizing probability_ vector to %d elements.", n);
  probability_.resize(n, 0.0);
}

ClassificationResult::ClassificationResult(const vector<double>& probability) {
  probability_ = probability;
}

double& ClassificationResult::operator[](int id) {
  INFO("Geting element of probability with index %d.", id);
  return probability_[id];
}

const double& ClassificationResult::operator[](int id) const {
  INFO("Geting element of probability with index %d.", id);
  return probability_[id];
}

int ClassificationResult::size() const {
  INFO("Geting size of probability_ vector.");
  return probability_.size();
}

void ClassificationResult::normalize() {
  INFO("Normalizing classification result.");
  double sum = 0;
  for (int i = 0; i < size(); i++) {
    sum += std::fabs(probability_[i]);
  }
  if (sum == 0) {
    INFO("Sum of all probabilities is equal to zero.");
    for (int i = 0; i < size(); i++) {
      probability_[i] = 1.0 / size();
    }
    return;
  }
  for (int i = 0; i < size(); i++) {
    probability_[i] = std::fabs(probability_[i]) / sum;
  }
}

ClassificationResult operator+(const ClassificationResult& left,
                               const ClassificationResult& right) {
  INFO("Finding sum of two classification results.");
  if (left.size() != right.size())
    throw std::logic_error("can't add: different number of classes");
  ClassificationResult result(left.size());
  for (int i = 0; i < result.size(); i++) {
    result[i] = left[i] + right[i];
  }
  return result;
}

ClassificationResult operator*(const ClassificationResult& left,
                               double right) {
  INFO("Multiplying classification result by %lf", right);
  ClassificationResult result(left.size());
  for (int i = 0; i < result.size(); i++) {
    result[i] = right * left[i];
  }
  return result;
}

ClassificationResult operator*(double left,
                               const ClassificationResult& right) {
  INFO("Multiplying classification result by %lf", left);
  return right * left;
}

ClassificationResult operator/(const ClassificationResult& left,
                               double right) {
  INFO("Dividing classification result by %lf", right);
  return left * 1.0 / right;
}
}
