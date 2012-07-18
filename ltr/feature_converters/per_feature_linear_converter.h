// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_PER_FEATURE_LINEAR_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_PER_FEATURE_LINEAR_CONVERTER_H_

#include "ltr/utility/shared_ptr.h"

#include <string>
#include <vector>

#include "ltr/feature_converters/feature_converter.h"

using std::string;
using std::vector;

namespace ltr {

/**
* \brief Perform linear transformation of each feature.
*
* features[i] ->  factors[i] * features[i] + shifts[i]
* \param factors vector of factors
* \param shifts vector of shifts
*/
class PerFeatureLinearConverter : public FeatureConverter {
 public:
  typedef ltr::utility::shared_ptr<PerFeatureLinearConverter> Ptr;

  explicit PerFeatureLinearConverter(
    const FeatureInfo& input_feature_info = FeatureInfo()) {
    resize(input_feature_info);
  }

  virtual void fillOutputFeatureInfo();

  /**
  * change expected feature info of input object and resizes 
  * factors and shifts vectors
  */
  void resize(const FeatureInfo& input_feature_info);
  /**
  * get component of factors vector
  */
  double factor(size_t feature_index) const;
  /**
  * set component of factors vector
  */
  void set_factor(size_t feature_index, double coefficient);
  /**
  * get component of shifts vector
  */
  double shift(size_t feature_index) const;
  /**
  * set component of shifts vector
  */
  void set_shift(size_t feature_index, double shift);

  virtual string generateCppCode(const string& function_name) const;
 private:
  virtual void applyImpl(const Object& input, Object* output) const;
  virtual string getDefaultAlias() const {return "PerFeatureLinearConverter";}

  vector<double> factors_;
  vector<double> shifts_;
};
}

#endif  // LTR_FEATURE_CONVERTERS_PER_FEATURE_LINEAR_CONVERTER_H_
