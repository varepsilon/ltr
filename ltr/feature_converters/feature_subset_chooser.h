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
/**
 * Choses a subset from inputted object's features. Throws while converting
 * if inputted object has too little features (if one of features to be chosen
 * has index with no feature for this index in inputted object). Duplication of
 * resulted feature's indices is allowed, but FeatureConverterLearners usually
 * produces FeatureSubsetChooser with no dublication in feature's indices
 */
class FeatureSubsetChooser : public FeatureConverter {
  public:
  typedef boost::shared_ptr<FeatureSubsetChooser> Ptr;

  FeatureSubsetChooser() {}
  /**
   * @param input_indices - indices of features to be chosen from inputted object
   */
  explicit FeatureSubsetChooser(const vector<int>& input_indices)
      : indices_(input_indices) {
    max_used_feature_ = *max_element(indices_.begin(), indices_.end());
  }
  /**
   * Sets indices of features to be chosen
   * @param input_indices - indices of features to be chosen from inputted object
   */
  void setChoosedFeaturesIndices(const vector<int>& input_indices) {
    indices_ = input_indices;
    max_used_feature_ = *max_element(indices_.begin(), indices_.end());
  }
  /**
   * Returns indices of features to be chosen
   */
  const vector<int>& getChoosedFeaturesIndices() const {
    return indices_;
  }
  /**
   * Returns number of features to be chosen
   */
  int getChoosedFeaturesCount() const {
    return indices_.size();
  }

  FeatureInfo convertFeatureInfo(const FeatureInfo& oldFeatureInfo) const;

  void apply(const Object& source_object,
    Object* preprocessed_object) const;

  virtual string generateCppCode(
      const string& function_name) const {
    return "Not implemented.";
  }

  private:
  vector<int> indices_;
  int max_used_feature_;
};
}
#endif  // LTR_FEATURE_CONVERTERS_FEATURE_SUBSET_CHOOSER_H_
