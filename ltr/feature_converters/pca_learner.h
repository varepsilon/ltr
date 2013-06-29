// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_PCA_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_PCA_LEARNER_H_

#include <vector>
#include <limits>
#include <string>
#include <sstream>

#include "contrib/include_Eigen.h"

#include "ltr/feature_converters/linear_converter.h"
#include "ltr/feature_converters/feature_converter_learner.h"

#include "ltr/data/utility/data_set_statistics.h"
#include "ltr/utility/numerical.h"
#include "ltr/utility/boost/lexical_cast.hpp"
#include "ltr/utility/boost/shared_ptr.h"

using std::vector;
using std::string;
using std::stringstream;
using std::logic_error;

using ltr::LinearConverter;
using ltr::utility::getFeaturesAverageValues;
using ltr::utility::shared_ptr;
using ltr::utility::lexical_cast;

using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::JacobiSVD;
using Eigen::ComputeThinU;

namespace ltr {
class NumberOfComponentsRule;
/**
 * \brief Learner for principal components analisys feature converter.
 */
template <class TElement>
class PCALearner
  : public BaseFeatureConverterLearner<TElement, LinearConverter> {
  ALLOW_SHARED_PTR_ONLY_CREATION(PCALearner)
 public:
  explicit PCALearner();

  explicit PCALearner(const ParametersContainer& parameters);

  virtual string toString() const;

  /**
   * Returns report about PCALearner.
   */
  string report() const;

  void setNumberOfComponentsRule(NumberOfComponentsRule* rule);

 private:
  virtual void learnImpl(const DataSet<TElement>& data_set,
                         LinearConverter::Ptr* linear_converter);

  virtual string getDefaultAlias() const;

  /**
   * Singular values of training dataset
   */
  VectorXd singular_values_;
  /**
   * Rule for optimal number of components calculation
   */
  shared_ptr<NumberOfComponentsRule> rule_;
};

/**
 * \brief Strategy for calculating number of principal components to be used.
 */
class NumberOfComponentsRule {
 public:
  /**
   * Returns number of principal components to be used.
   *
   * \param singular_values Vector of singular values correspond to each
   * principal component.
   */
  virtual int getNumberOfComponents(
      const VectorXd& singular_values) const = 0;
  virtual ~NumberOfComponentsRule() {}
};

/**
 * \brief Kaiser rule for optimal number of principal components calculation.
 *
 * It proposes that only components having singular values greater than average
 * of all singular values should be retained.
 *
 * \f$\lambda_i > \frac{1}{n} \mathit{tr} C\f$,
 * where \f$\mathit{tr} C = \sum_1^n{\lambda_j}\f$
 */
class KaiserRule: public NumberOfComponentsRule {
 public:
  int getNumberOfComponents(const VectorXd& singular_values) const;
};

/**
 * \brief Number of components rule which returns fixed number, given from
 * constructor of this class.
 */
class FixedNumberOfComponentsRule: public NumberOfComponentsRule {
 public:
  /**
   * Constructor sets expected number of components to be used. If value is
   * less or equal to zero exception will be thrown
   * 
   * \param components_count Number of components to be used.
   */
  explicit FixedNumberOfComponentsRule(int components_count);

  int getNumberOfComponents(const VectorXd& singular_values) const;

 private:
  /**
   * Expected number of components to be used
   */
  int components_count_;
};

/**
 * \brief Broken stick rule for optimal number of principal components
 * calculation.
 *
 * Let \f$l_i=\frac{1}{n}\sum_{j=i}^{n} \frac{1}{j}\f$ - parts of stick.
 * According to this rule, \f$k^{th}\f$ principal component will be retained if:
 * \f$\frac{\lambda_1}{\mathit{tr} C}>l_1 \; and \;
 * \frac{\lambda_2}{\mathit{tr} C}>l_2  \; and \; ...
 * \frac{\lambda_k}{\mathit{tr} C}>l_k \f$,
 * where \f$\mathit{tr} C = \sum_1^n{\lambda_j}\f$
 */
class BrokenStickRule: public NumberOfComponentsRule {
 public:
  int getNumberOfComponents(const VectorXd& singular_values_) const;

 private:
  /**
   * Calculates sizes of stick parts \f$l_i\f$ for broken stick method.
   *
   * \param parts_count Required count of stick parts (equal to number of
   * principal components).
   * \param stick_parts Vector of parts of the stick to be returned.
   */
  static void getStickParts(int parts_count, vector<double>* stick_parts);
};

// Template realizations

template <typename TElement>
PCALearner<TElement>::PCALearner(): rule_(new KaiserRule) {}


template <typename TElement>
PCALearner<TElement>::PCALearner(const ParametersContainer& parameters)
  :rule_(new KaiserRule) {
  this->setParameters(parameters);
}

template <typename TElement>
void PCALearner<TElement>::learnImpl(const DataSet<TElement>& data_set,
                                     LinearConverter::Ptr* linear_converter) {
  VectorXd eigen_average_features;
  getFeaturesAverageValues(data_set, &eigen_average_features);

  MatrixXd source_matrix = data_set.get_features_matrix();

  for (int object_index = 0;
       object_index < source_matrix.cols();
       ++object_index) {
    source_matrix.col(object_index) =
      source_matrix.col(object_index) - eigen_average_features;
  }

  JacobiSVD<MatrixXd> jacobi_svd = source_matrix.jacobiSvd(ComputeThinU);
  MatrixXd matrix_U = jacobi_svd.matrixU();
  singular_values_ = jacobi_svd.singularValues();
  int component_count = rule_->getNumberOfComponents(singular_values_);

  matrix_U.conservativeResize(matrix_U.rows(), component_count);
  (*linear_converter)->set_factor(matrix_U.transpose());

  VectorXd shift = VectorXd::Zero(component_count);
  (*linear_converter)->set_shift(shift);
}

template <typename TElement>
void PCALearner<TElement>::setNumberOfComponentsRule(
    NumberOfComponentsRule* rule) {
  rule_ = rule;
}

template <typename TElement>
string PCALearner<TElement>::toString() const {
  return "PCALearner";
}

template <typename TElement>
string PCALearner<TElement>::report() const {
  if (singular_values_.size() == 0)
    return "PCALearner haven't learned yet";
  stringstream out;
  out << "PCALearner. Singular values of dataset are:\n"
      << singular_values_ << "\n"
      << "Optimal number of principal components by current rule is "
      << rule_->getNumberOfComponents(singular_values_) << "\n";
  return out.str();
}

template <typename TElement>
string PCALearner<TElement>::getDefaultAlias() const {
  return "PCALearner";
}
};
#endif  // LTR_FEATURE_CONVERTERS_PCA_LEARNER_H_
