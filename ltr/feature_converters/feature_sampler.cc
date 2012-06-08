// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>

#include <string>
#include <stdexcept>
#include "ltr/feature_converters/feature_sampler.h"

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

void FeatureSampler::fillOutputFeatureInfo() {
  if (input_feature_info_.empty()) {
    return;
  }
  output_feature_info_.resize(getChoosedFeaturesCount());
  for (int i = 0; i < getChoosedFeaturesCount(); ++i) {
    FeatureType feature_type = input_feature_info_.getFeatureType(indices_[i]);
    output_feature_info_.setFeatureType(i, feature_type);
  }
}

void FeatureSampler::applyImpl(const Object& argument,
                               Object* value) const {
  Object result(output_feature_info());
  CheckMaxUsedFeature(max_used_feature_, argument.feature_count());
  for (int choosed_feature_index = 0;
      choosed_feature_index < getChoosedFeaturesCount();
      ++choosed_feature_index) {
    result.features()[choosed_feature_index] =
        argument.features()[indices_[choosed_feature_index]];
  }
  *value = result;
}
}
