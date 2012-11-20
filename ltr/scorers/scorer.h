// Copyright 2011 Yandex

#ifndef LTR_SCORERS_SCORER_H_
#define LTR_SCORERS_SCORER_H_

#include "ltr/utility/boost/lexical_cast.h"
#include <vector>
#include <string>

#include "ltr/interfaces/aliaser.h"
#include "ltr/data/object.h"
#include "ltr/feature_converters/feature_converter.h"
#include "ltr/interfaces/serializable_functor.h"
#include "ltr/interfaces/printable.h"
#include "ltr/utility/boost/shared_ptr.h"

using std::string;

namespace ltr {
/*
 * Scorer is a class that can for every object evaluate
 * a value - its rank, or score
 */
class Scorer : public SerializableFunctor<double>,
               public Printable,
               public Aliaser {
 public:
  typedef ltr::utility::shared_ptr<Scorer> Ptr;
  typedef ltr::utility::shared_ptr<Scorer> BasePtr;

  Scorer(const FeatureConverterArray&
            feature_converter = FeatureConverterArray()):
        feature_converter_(feature_converter) {}

  double value(const Object& object) const {
    return score(object);
  }

  double score(const Object& object) const;

  GET_SET_VECTOR_OF_PTR(FeatureConverter, feature_converter);

  using SerializableFunctor<double>::generateCppCode;

  string generateCppCode(const string& function_name) const {
    if (feature_converter_.size() == 0)
      return generateCppCodeImpl(function_name);
    string code;
    string implFunctionName(function_name);
    implFunctionName.append("Impl");
    code.append(generateCppCodeImpl(implFunctionName));

    for (size_t featureConverterIdx = 0;
        featureConverterIdx < feature_converter_.size();
        ++featureConverterIdx) {
      code.append(feature_converter_.at(
          featureConverterIdx)->generateCppCode());
    }

    code.append("double ");
    code.append(function_name);
    code.append("(const std::vector<double>& feature) {\n");

    string prevVectorName("feature");

    for (size_t featureConverterIdx = 0;
        featureConverterIdx < feature_converter_.size();
        ++featureConverterIdx) {
      string curVectorName = "feature" +
          ltr::utility::lexical_cast<string>(featureConverterIdx);
      string featureConverterFunctionName(feature_converter_.at(
          featureConverterIdx)->getDefaultSerializableObjectName());
      code.append("  std::vector<double> ");
      code.append(curVectorName);
      code.append(";\n  ");
      code.append(featureConverterFunctionName);
      code.append("(");
      code.append(prevVectorName);
      code.append(", &");
      code.append(curVectorName);
      code.append(");\n");

      prevVectorName = curVectorName;
    }
    code.append("  return ");
    code.append(implFunctionName);
    code.append("(");
    code.append(prevVectorName);
    code.append(");\n");
    code.append("}\n");
    return code;
  }

  template <class TElement>
  void predict(const DataSet<TElement>& elements) const;

  template <class TElement>
  void predict(const TElement& element) const;

  string generateLocalClassName(size_t index) const;

  virtual ~Scorer() {}

  virtual string toString() const;
 private:
  virtual double scoreImpl(const Object& obj) const = 0;

  virtual string generateCppCodeImpl(const string& function_name) const = 0;

  FeatureConverterArray feature_converter_;

  virtual string toStringImpl() const;
};
}
#endif  // LTR_SCORERS_SCORER_H_
