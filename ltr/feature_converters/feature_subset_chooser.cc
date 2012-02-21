// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>

#include <string>
#include <stdexcept>
#include "ltr/feature_converters/feature_subset_chooser.h"

using std::logic_error;
using std::string;

namespace ltr {

FeatureInfo FeatureSubsetChooser::convertFeatureInfo(
    const FeatureInfo& oldFeatureInfo) const {
  FeatureInfo convertedFeatureInfo(countChoosedFeatures());

  for (size_t choosed_feature_index = 0;
      choosed_feature_index < countChoosedFeatures();
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
  if (max_used_feature_ > argument.featureCount()) {
    throw logic_error("Current object has "
      + boost::lexical_cast<string>(argument.featureCount())
      + " features while numer "
      + boost::lexical_cast<string>(max_used_feature_)
      + " was requested");
  }
  for (size_t choosed_feature_index = 0;
      choosed_feature_index < countChoosedFeatures();
      ++choosed_feature_index) {
    result <<
        argument.features()[indices_[choosed_feature_index]];
  }

  *value = result;
}
}
