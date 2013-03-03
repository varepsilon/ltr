// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_LINEAR_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_LINEAR_CONVERTER_H_

#include <string>
#include <vector>

#include <Eigen/Dense>

#include "ltr/feature_converters/feature_converter.h"
#include "ltr/utility/boost/shared_ptr.h"

using std::string;
using std::vector;

using Eigen::MatrixXd;
using Eigen::VectorXd;

namespace ltr {
/**
 * \brief Perform linear transformation of dataset.
 *
 * features[i] ->  factors[i][j] * features[j] + shifts[i].
 */
class LinearConverter : public FeatureConverter {
 public:
  typedef ltr::utility::shared_ptr<LinearConverter> Ptr;

  explicit LinearConverter(
    const FeatureInfo& input_feature_info = FeatureInfo())
    : FeatureConverter(input_feature_info) {
  }

  virtual FeatureInfo convertFeatureInfo() const;

  GET_SET(MatrixXd, factor);
  GET_SET(VectorXd, shift);

  virtual string generateCppCode(const string& function_name) const;

  virtual string toString() const;
 private:
  virtual void applyImpl(const Object& input, Object* output) const;

  virtual string getDefaultAlias() const;

  /**
   * \brief Checks either input corresponds factor_ and shift_ size.
   * \param input object for checking.
   */
  void checkParameters(const Object& input) const;

  /**
   * \brief matrix of factor coefficients for linear transformation.
   */
  MatrixXd factor_;
  /**
   * \brief vector of shift coefficients for linear transformation.
   */
  VectorXd shift_;
};
};
#endif  // LTR_FEATURE_CONVERTERS_LINEAR_CONVERTER_H_
