// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_LEARNER_H_

#include <vector>

#include "ltr/data/data_set.h"

#include "ltr/feature_converters/feature_converter.h"

#include "ltr/interfaces/parameterized.h"
#include "ltr/interfaces/printable.h"

#include "ltr/parameters_container/parameters_container.h"

#include "ltr/utility/boost/shared_ptr.h"

using std::vector;

namespace ltr {
/**
 * \brief Learns input dataset and produces a FeatureConverter.
 *
 * Sometimes before training a Scorer it's usefull to make some preprocessing
 * of object features, e.g. normalize, filter out non significant features, etc. \n
 * In order to do this one need to add FeatureConverterLearner into the BaseLearner
 * (see Learner<TElement>::addFeatureConverterLearner).
 *
 * If you want to make your own FeatureConverterLearner it's more convinient
 * to inherit from BaseFeatureConverterLearner.
 *
 * \see Learner, FeatureConverterLearner, BaseFeatureConverterLearner
 */
template <class TElement>
class FeatureConverterLearner : public Parameterized,
                                public Printable,
                                public Aliaser {
 public:
  typedef ltr::utility::shared_ptr<FeatureConverterLearner> Ptr;
  /**
   * \brief Learns input dataset.
   * \param data_set dataset to study
   * \param check_parameters whether perform Parameterized::checkParameters() before launch
   */
  virtual void learn(const DataSet<TElement>& data_set,
                     bool check_parameters = true) = 0;
  /**
   * \note Don't forget to learn() before make().
   * \return pointer to <em>new instance</em> of feature converter.
   */
  virtual FeatureConverter::Ptr make() const = 0;
};
/**
 * \brief A base class for FeatureConverter learners.
 *
 * If you want to make your own FeatureConverterLearner you should
 * inherit from BaseFeatureConverterLearner and implement learnImpl().
 *
 * \sa FeatureConverterLearner, BaseLearner
 */
// \TODO(sameg) BaseLearner -> Trainer ?
template <class TElement, class TFeatureConverter>
class BaseFeatureConverterLearner : public FeatureConverterLearner<TElement> {
  ALLOW_SHARED_PTR_ONLY_CREATION(BaseFeatureConverterLearner)
 public:
  BaseFeatureConverterLearner() :
  feature_converter_(new TFeatureConverter) {}
  /**
   * \note Don't forget to learn() before makeSpecific().
   * \return pointer to <em>new instance</em> of feature converter
   */
  typename TFeatureConverter::Ptr makeSpecific() const {
    return feature_converter_;
  }

  virtual FeatureConverter::Ptr make() const {
    return FeatureConverter::Ptr(makeSpecific());
  }

  virtual void learn(const DataSet<TElement>& data_set,
                     bool check_parameters = true) {
    if (check_parameters) {
      this->checkParameters();
    }

    feature_converter_->set_input_feature_info(data_set.feature_info());
    learnImpl(data_set, &feature_converter_);
  }

 private:
  /**
   * \brief Learn input dataset and train feature converter.
   * \param[in] data_set dataset to study
   * \param[out] feature_converter feature converter to train
   */
  virtual void learnImpl(
      const DataSet<TElement>& data_set,
      typename TFeatureConverter::Ptr* feature_converter) = 0;

  typename TFeatureConverter::Ptr feature_converter_;
};
};
#endif  // LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_LEARNER_H_
