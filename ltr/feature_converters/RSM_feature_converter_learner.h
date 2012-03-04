// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_RSM_FEATURE_CONVERTER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_RSM_FEATURE_CONVERTER_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include <ctime>
#include <cmath>
#include <vector>
#include <algorithm>

#include "ltr/feature_converters/feature_converter_learner.h"
#include "ltr/feature_converters/feature_subset_chooser.h"

using std::vector;
using std::random_shuffle;
using std::copy;

namespace ltr {
/**
 * Produces FeatureSubsetChooser with random indices (with no duplication)
 */
template <typename TElement>
class RSMFeatureConverterLearner : public FeatureConverterLearner<TElement> {
  public:
  typedef boost::shared_ptr<RSMFeatureConverterLearner> Ptr;

  /**
   * @param parameters Standart LTR parameter container with double parameter
   * SELECTED_PART. SELECTED_PART is a part of features chosen by produced
   * FeatureSubsetChooser. Upper rounding is used, so never produces
   * FeatureSubsetChooser with 0 features.
   * By default SELECTED_PART = 0.3
   */
  explicit RSMFeatureConverterLearner(const ParametersContainer& parameters =
      ParametersContainer())
      : converter_(new FeatureSubsetChooser) {
    this->setDefaultParameters();
    this->copyParameters(parameters);
    this->checkParameters();
  }

  void learn(const DataSet<TElement>& data_set);
  FeatureConverter::Ptr make() const;

  void setDefaultParameters();
  void checkParameters() const;

  private:
  FeatureSubsetChooser::Ptr converter_;
};

// template realizations
template <typename TElement>
void RSMFeatureConverterLearner<TElement>::setDefaultParameters() {
  this->clearParameters();
  this->addDoubleParameter("SELECTED_PART", 0.3);
}

template <typename TElement>
void RSMFeatureConverterLearner<TElement>::checkParameters() const {
  CHECK_DOUBLE_PARAMETER("SELECTED_PART", X > 0 && X <= 1);
}

template <typename TElement>
void RSMFeatureConverterLearner<TElement>
    ::learn(const DataSet<TElement>& data_set) {
  int size = static_cast<int>(
    ceil(data_set.featureInfo().getFeatureCount()
      * this->getDoubleParameter("SELECTED_PART")));
  vector<int> indices(size);
  srand(unsigned(time(NULL)));

  vector<int> all_used(data_set.featureInfo().getFeatureCount());
  for (int index = 0; index < all_used.size(); ++index) {
    all_used[index] = index;
  }
  random_shuffle(all_used.begin(), all_used.end());
  copy(all_used.begin(), all_used.begin() + size, indices.begin());

  converter_->setChoosedFeaturesIndices(indices);
}

template <typename TElement>
FeatureConverter::Ptr RSMFeatureConverterLearner<TElement>::make() const {
  FeatureSubsetChooser::Ptr output(new FeatureSubsetChooser(*converter_));
  return output;
}
};
#endif  // LTR_FEATURE_CONVERTERS_RSM_FEATURE_CONVERTER_LEARNER_H_
