// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_PER_FEATURE_LINEAR_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_PER_FEATURE_LINEAR_CONVERTER_H_

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

#include "ltr/feature_converters/feature_converter.h"

using std::string;
using std::vector;

namespace ltr {

class PerFeatureLinearConverter : public FeatureConverter {
  public:
  typedef boost::shared_ptr<PerFeatureLinearConverter> Ptr;

  explicit PerFeatureLinearConverter(size_t feature_count) :
    coefficient_(feature_count, 1),
    shift_(feature_count, 0) {}

  FeatureInfo getNewFeatureInfo() const;

  void setFeatureCount(size_t feature_count);
  double getCoefficient(size_t feature_idx) const;
  void setCoefficient(size_t feature_idx, double coefficient);
  double getShift(size_t feature_idx) const;
  void setShift(size_t feature_idx, double shift);

  void apply(const Object& argument, Object* value) const;

  virtual std::string generateCppCode(const std::string& function_name) const {
    string hpp_string;

    hpp_string.
      append("#include <vector>\n\nvoid ").
      append(function_name).
      append("(const std::vector<double>& features, ").
      append("std::vector<double>* result) {\n").
      append("  result->clear();\n");
      for (size_t i = 0; i < coefficient_.size(); i++) {
        hpp_string.
          append("  result->push_back(features[").
          append(boost::lexical_cast<string>(i)).
          append("] * ").
          append(boost::lexical_cast<string>(coefficient_[i])).
          append(" + ").
          append(boost::lexical_cast<string>(shift_[i])).
          append(");\n");
      }
    hpp_string.append("}\n");
    return hpp_string;
  };

  private:
  size_t featureCount() const;
  void checkFeatureCount(size_t checked_feature_count) const;

  vector<double> coefficient_;
  vector<double> shift_;
};
}

#endif  // LTR_FEATURE_CONVERTERS_PER_FEATURE_LINEAR_CONVERTER_H_
