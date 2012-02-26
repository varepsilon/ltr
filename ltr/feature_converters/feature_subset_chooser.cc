// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>

#include <string>
#include <stdexcept>
#include "ltr/feature_converters/feature_subset_chooser.h"

using std::logic_error;
using std::string;

// local support function, not included to namespace ltr
void CheckMaxUsedFeature(int max_used_feature, int argument_feature_count) {
  if (max_used_feature > argument_feature_count) {
    throw logic_error("Current object has "
      + boost::lexical_cast<string>(argument_feature_count)
      + " features while number "
      + boost::lexical_cast<string>(max_used_feature)
      + " was requested");
  }
}

namespace ltr {

FeatureInfo FeatureSubsetChooser::convertFeatureInfo(
    const FeatureInfo& oldFeatureInfo) const {
  FeatureInfo convertedFeatureInfo(getChoosedFeaturesCount());

  CheckMaxUsedFeature(max_used_feature_, oldFeatureInfo.getFeatureCount());
  for (int choosed_feature_index = 0;
      choosed_feature_index < getChoosedFeaturesCount();
      ++choosed_feature_index) {
    convertedFeatureInfo.setFeatureType(choosed_feature_index,
        oldFeatureInfo.getFeatureType(
            indices_[choosed_feature_index]));
  }

  return convertedFeatureInfo;
}

void FeatureSubsetChooser::apply(const Object& argument,
    Object* value) const {
  Object result;
  CheckMaxUsedFeature(max_used_feature_, argument.featureCount());
  for (int choosed_feature_index = 0;
      choosed_feature_index < getChoosedFeaturesCount();
      ++choosed_feature_index) {
    result <<
        argument.features()[indices_[choosed_feature_index]];
  }

  *value = result;
}
}
