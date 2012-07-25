// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_NAN_TO_AVERAGE_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_NAN_TO_AVERAGE_LEARNER_H_

#include <string>
#include <vector>
#include <limits>

#include "ltr/feature_converters/nan_to_average_converter.h"
#include "ltr/feature_converters/feature_converter_learner.h"

namespace ltr {
/**
 * Produces NanToAverageConverter
 */
template <typename TElement>
class NanToAverageConverterLearner
  : public BaseFeatureConverterLearner<TElement, NanToAverageConverter> {
 public:
  virtual void learnImpl(const DataSet<TElement>& data_set,
                         NanToAverageConverter* feature_converter);

  virtual string toString() const {
    return "NanToAverageConverterLearner";
  }

 private:
  virtual string getDefaultAlias() const {
    return "NanToAverageConverterLearner";
  }
};

template <typename TElement>
void NanToAverageConverterLearner<TElement>::learnImpl(
  const DataSet<TElement>& data_set, NanToAverageConverter* feature_converter) {
    vector<double> average_features_values(data_set.feature_count());

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

    feature_converter->set_average_features_values(average_features_values);
}
};
#endif  // LTR_FEATURE_CONVERTERS_NAN_TO_AVERAGE_LEARNER_H_
