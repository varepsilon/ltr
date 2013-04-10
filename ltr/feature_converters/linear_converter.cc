// Copyright 2011 Yandex

#include "ltr/feature_converters/linear_converter.h"

#include <stdexcept>
#include <sstream>

#include "ltr/utility/eigen_converters.h"

using std::logic_error;
using std::stringstream;

using ltr::utility::lexical_cast;
using ltr::utility::StdVectorToEigenVector;
using ltr::utility::EigenVectorToStdVector;

namespace ltr {
FeatureInfo LinearConverter::convertFeatureInfo() const {
  FeatureInfo output_feature_info = input_feature_info_;
  output_feature_info.resize(factor_.rows());
  return output_feature_info;
}

string LinearConverter::generateCppCode(const string& function_name) const {
  stringstream code;
  code
    << "#include <vector>\n"
    << "\n"
    << "void " << function_name << "(const std::vector<double>& features,\n"
    << "    std::vector<double>* result) {\n"
    << "  result->clear();\n"
    << "  double new_feature;\n";
    for (int row_index = 0; row_index < (int)factor_.rows(); ++row_index) {
      code << "  new_feature = 0;\n";
      for (int col_index = 0; col_index < (int)factor_.cols(); ++col_index) {
        code << "  new_feature += "
          << lexical_cast<string>(factor_(row_index, col_index))
          << " * features[" << lexical_cast<string>(col_index) << "];\n";
      }
      code << "  new_feature += " << lexical_cast<string>(shift_[row_index])
        << ";\n" << "  result->push_back(new_feature);\n";
    }
  code << "}\n";
  return code.str();
}

void LinearConverter::applyImpl(const Object& input, Object* output) const {
  checkParameters(input);
  VectorXd input_eigen = StdVectorToEigenVector(input.features());
  VectorXd output_eigen = factor_ * input_eigen + shift_;
  *output = input.deepCopy();
  EigenVectorToStdVector(output_eigen, &output->features());
}

string LinearConverter::getDefaultAlias() const {
  return "LinearConverter";
}

string LinearConverter::toString() const {
  stringstream str;
  str << "Linear feature converter with factors:\n" << factor_
      << "\n  and shifts:\n" << shift_ << "\n"; 
  return str.str();
}

void LinearConverter::checkParameters(const Object& input) const {
  CHECK(input.feature_count() == factor_.cols());
  CHECK(shift_.size() == factor_.rows());
}
};
