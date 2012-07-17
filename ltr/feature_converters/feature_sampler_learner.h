// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_SAMPLER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_SAMPLER_LEARNER_H_

#include "ltr/utility/shared_ptr.h"

#include <set>
#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>

#include "ltr/feature_converters/feature_converter_learner.h"
#include "ltr/feature_converters/feature_sampler.h"
#include "ltr/utility/indices.h"

using std::string;
using std::set;
using std::logic_error;
using std::vector;

using ltr::utility::Indices;
using ltr::utility::IndicesPtr;
using ltr::utility::getIdPermutation;

namespace ltr {
/**
* \brief Produces FeatureSampler with specified indices.
* \param indices indices of features to sample
*/
template <typename TElement>
class FeatureSamplerLearner
    : public BaseFeatureConverterLearner<TElement, FeatureSampler> {
 public:
  typedef ltr::utility::shared_ptr<FeatureSamplerLearner> Ptr;

  /**
  * \param indices indices of features to sample
  */
  explicit FeatureSamplerLearner(IndicesPtr indices = IndicesPtr(new Indices));
  explicit FeatureSamplerLearner(const ParametersContainer& parameters);

  virtual void setDefaultParameters();
  virtual void checkParameters() const;

  virtual string toString() const;

  GET_SET(IndicesPtr, indices);
 private:
  virtual void learnImpl(const DataSet<TElement>& data_set,
                         FeatureSampler *feature_sampler);
  virtual void setParametersImpl(const ParametersContainer& parameters);
  virtual string getDefaultAlias() const {return "FeatureSamplerLearner";}
  IndicesPtr indices_;
};

// template realizations

template <typename TElement>
FeatureSamplerLearner<TElement>::FeatureSamplerLearner(IndicesPtr indices) {
  set_indices(indices);
}

template <typename TElement>
FeatureSamplerLearner<TElement>::FeatureSamplerLearner(
    const ParametersContainer& parameters) {
  this->setParameters(parameters);
}

template <typename TElement>
void FeatureSamplerLearner<TElement>::setDefaultParameters() {
  indices_ = IndicesPtr(new Indices);
}

template <typename TElement>
void FeatureSamplerLearner<TElement>::checkParameters() const {
  // DON'T NEED
}

template <typename TElement>
void FeatureSamplerLearner<TElement>::setParametersImpl(
    const ParametersContainer& parameters) {
  // \TODO(sameg) Create indices from string representation like [1:3], 5, 6
  indices_ = parameters.Get<IndicesPtr>("INDICES");
}

template <typename TElement>
void FeatureSamplerLearner<TElement>::learnImpl(
    const DataSet<TElement>& data_set, FeatureSampler *feature_sampler) {
  // \TODO(sameg) Is it logic?
  if (indices_->size() != 0) {
    feature_sampler->set_indices(*indices_);
  } else {
    Indices indices;
    getIdPermutation(&indices, data_set.feature_count());
    feature_sampler->set_indices(indices);
  }
}

template <typename TElement>
string FeatureSamplerLearner<TElement>::toString() const {
  std::stringstream str;
    str << "FeatureSamplerLearner: indices = [";
    for (int i = 0; i < indices_->size(); ++i) {
      if (i != 0) {
        str << ", ";
      }
      str << indices_->at(i);
    }
    str << "]";
    return str.str();
}
};
#endif  // LTR_FEATURE_CONVERTERS_FEATURE_SAMPLER_LEARNER_H_
