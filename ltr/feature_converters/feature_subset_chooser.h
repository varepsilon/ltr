// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_SUBSET_CHOOSER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_SUBSET_CHOOSER_H_

#include <boost/shared_ptr.hpp>

#include <algorithm>
#include <string>
#include <vector>

#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
#include "ltr/data/data_set.h"
#include "ltr/feature_converters/feature_converter.h"

using std::max_element;
using std::vector;
using std::string;
using ltr::Object;

namespace ltr {

class FeatureSubsetChooser : public FeatureConverter {
  public:
  typedef boost::shared_ptr<FeatureSubsetChooser> Ptr;

  explicit FeatureSubsetChooser(const vector<size_t>& input_indices)
      : indices_(input_indices) {
    max_used_feature_ = *max_element(indices_.begin(), indices_.end());
  }
  void setChoosedFeaturesIndices(const vector<size_t>& input_indices) {
    indices_ = input_indices;
    max_used_feature_ = *max_element(indices_.begin(), indices_.end());
  }
  const vector<size_t>& getChoosedFeaturesIndices() const {
    return indices_;
  }
  size_t countChoosedFeatures() const {
    return indices_.size();
  }

  FeatureInfo convertFeatureInfo(const FeatureInfo& oldFeatureInfo) const;

  void apply(const Object& source_object,
    Object* preprocessed_element) const;

  virtual string generateCppCode(
      const string& function_name) const {
    return "Not implemented.";
  }

  private:
  vector<size_t> indices_;
  int max_used_feature_;
};
}
#endif  // LTR_FEATURE_CONVERTERS_FEATURE_SUBSET_CHOOSER_H_
