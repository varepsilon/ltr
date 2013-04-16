// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_NAN_TO_AVERAGE_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_NAN_TO_AVERAGE_LEARNER_H_

#include <string>
#include <vector>
#include <limits>

#include "ltr/feature_converters/nan_to_neutral_converter.h"
#include "ltr/feature_converters/feature_converter_learner.h"

namespace ltr {
/**
 * \brief Produces NanToNeutralConverter with average features neutral object.
 */
template <typename TElement>
class NanToAverageConverterLearner
  : public BaseFeatureConverterLearner<TElement, NanToNeutralConverter> {
 public:
  explicit NanToAverageConverterLearner(const ParametersContainer& parameters) {
  }

  NanToAverageConverterLearner() {
  }

  virtual void learnImpl(const DataSet<TElement>& data_set,
                         NanToNeutralConverter* feature_converter);

  virtual string toString() const;

 private:
  virtual string getDefaultAlias() const;
};

template <typename TElement>
void NanToAverageConverterLearner<TElement>::learnImpl(
  const DataSet<TElement>& data_set, NanToNeutralConverter* feature_converter) {
    VectorXd average_features_values(data_set.feature_count());

    for (int feature_index = 0;
         feature_index < (int)data_set.feature_count(); ++feature_index) {
      double current_feature_average_value = 0.0;
      int number_of_non_nan_features = 0;
      for (int object_index = 0;
           object_index < (int)data_set.size(); ++object_index) {
        if (!isNaN(data_set[object_index][feature_index])) {
          current_feature_average_value +=
            data_set[object_index][feature_index];
          ++number_of_non_nan_features;
        }
      }
      if (number_of_non_nan_features == 0) {
        average_features_values[feature_index] =
          numeric_limits<double>::quiet_NaN();
      } else {
        average_features_values[feature_index] =
          current_feature_average_value / number_of_non_nan_features;
      }
    }

    Object average_features_object;
    average_features_object.set_eigen_features(average_features_values);
    feature_converter->set_neutral_object(average_features_object);
}

template <typename TElement>
string NanToAverageConverterLearner<TElement>::toString() const {
  return "NanToAverageConverterLearner";
}

template <typename TElement>
string NanToAverageConverterLearner<TElement>::getDefaultAlias() const {
  return "NanToAverageConverterLearner";
}
};
#endif  // LTR_FEATURE_CONVERTERS_NAN_TO_AVERAGE_LEARNER_H_
