// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_CLASSIFICATION_RESULT_H_
#define LTR_LEARNERS_DECISION_TREE_CLASSIFICATION_RESULT_H_

#include <vector>

using std::vector;

namespace ltr {

class ClassificationResult {
  public:
    explicit ClassificationResult(int n);
    explicit ClassificationResult(const vector<double>& probability);
    double& operator[](int id);
    const double& operator[](int id) const;

    void normalize();
    int size() const;
  private:
    vector<double> probability_;
};

ClassificationResult operator+(const ClassificationResult& left,
                               const ClassificationResult& right);

ClassificationResult operator*(const ClassificationResult& left,
                               double right);

ClassificationResult operator*(double left,
                               const ClassificationResult& right);

ClassificationResult operator/(const ClassificationResult& left,
                               double right);
}

#endif  // LTR_LEARNERS_DECISION_TREE_CLASSIFICATION_RESULT_H_
