// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_SUBSET_CHOOSER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_SUBSET_CHOOSER_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include <set>
#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>

#include "ltr/feature_converters/feature_converter_learner.h"
#include "ltr/feature_converters/feature_subset_chooser.h"

using std::string;
using std::set;
using std::logic_error;
using std::vector;

namespace ltr {
/**
 * Produces FeatureSubsetChooser with specific indices. Duplication of indices
 * is denied (throws while checking parameters)
 */
template <typename TElement>
class FeatureSubsetChooserLearner
    : public FeatureConverterLearner<TElement, FeatureSubsetChooser> {
  public:
  typedef boost::shared_ptr<FeatureSubsetChooserLearner> Ptr;

  /**
   * @param parameters Standart LTR parameter container with list parameter
   * INDICES. INDICES is a list of indices of features to be used in produced
   * FeatureConverter. If INDICES is empty an equivalent to FakeFeatureConverter
   * for train dataset is produced.
   * By default INDICES is empty
   */
  explicit FeatureSubsetChooserLearner(const ParametersContainer& parameters =
      ParametersContainer()) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
    this->checkParameters();
  }

  void learn(const DataSet<TElement>& data_set);
  FeatureSubsetChooser make() const;

  void setDefaultParameters();
  void checkParameters() const;

  string toString() const;
  private:
  FeatureSubsetChooser converter_;
};

// template realizations
template <typename TElement>
void FeatureSubsetChooserLearner<TElement>::setDefaultParameters() {
  this->clearParameters();
  vector<int> empty;
  this->addNewParam("INDICES", empty);
}

template <typename TElement>
void FeatureSubsetChooserLearner<TElement>::checkParameters() const {
  vector<int> indices = this->parameters().
                        template Get<std::vector<int> >("INDICES");
  set<int> used_features;
  for (int index = 0; index < indices.size(); ++index) {
    int current_object = indices[index];
    if (used_features.find(current_object) == used_features.end()) {
      used_features.insert(current_object);
    } else {
      throw logic_error("Indicies array contains equal elements");
    }
  }
}

template <typename TElement>
string FeatureSubsetChooserLearner<TElement>::toString() const {
  std::stringstream str;
    str << "Feature subset chooser feature converter learner"
      << "with parameter INDICES = {";
    vector<int> indices = this->parameters().template GetRef<std::vector<int> >(
                "INDICES");
    for (int i = 0; i < indices.size(); ++i) {
      if (i != 0) {
        str << ", ";
      }
      str << indices[i];
    }
    str << "}";
    return str.str();
}

template <typename TElement>
void FeatureSubsetChooserLearner<TElement>
    ::learn(const DataSet<TElement>& data_set) {
  converter_.setFeatureInfo(data_set.feature_info());
  const ParametersContainer &params = this->parameters();
  if (params.GetRef<std::vector<int> >("INDICES").size() == 0) {
    vector<int> all_used(data_set.feature_info().get_feature_count());
    for (int index = 0; index < all_used.size(); ++index) {
      all_used[index] = index;
    }
    converter_.setChoosedFeaturesIndices(all_used);
  } else {
    converter_.setChoosedFeaturesIndices(
      params.GetRef<std::vector<int> >("INDICES"));
  }
}

template <typename TElement>
FeatureSubsetChooser FeatureSubsetChooserLearner<TElement>::make() const {
  return converter_;
}
};
#endif  // LTR_FEATURE_CONVERTERS_FEATURE_SUBSET_CHOOSER_LEARNER_H_
