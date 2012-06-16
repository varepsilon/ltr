// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_LEARNER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_LEARNER_H_

#include <boost/shared_ptr.hpp>

#include "ltr/data/data_set.h"
#include "ltr/feature_converters/feature_converter.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/interfaces/printable.h"
#include "ltr/parameters_container/parameters_container.h"

namespace ltr {
/**
 * \brief Learns input dataset and produces a FeatureConverter.
 *
 * Sometimes before training a Scorer it's usefull to make some preprocessing
 * of object features, e.g. normalize, filter out non significant features, etc. \n
 * In order to do this one need to add FeatureConverterLearner into the Learner
 * (see BaseLearner<TElement>::addFeatureConverterLearner).
 *
 * If you want to make your own FeatureConverterLearner it's more convinient
 * to inherit from BaseFeatureConverterLearner.
 *
 * \see BaseLearner, FeatureConverterLearner, BaseFeatureConverterLearner
 */
template <class TElement>
class FeatureConverterLearner : public Parameterized,
                                public Printable,
                                public Aliaser {
 public:
  typedef boost::shared_ptr<FeatureConverterLearner> Ptr;
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
* \sa FeatureConverterLearner, Learner
*/
// \TODO(sameg) Learner -> Trainer ?
template <class TElement, class TFeatureConverter>
class BaseFeatureConverterLearner : public FeatureConverterLearner<TElement> {
 public:
  typedef boost::shared_ptr<BaseFeatureConverterLearner> Ptr;
  /**
  * \note Don't forget to learn() before makeSpecific().
  * \return pointer to <em>new instance</em> of feature converter
  */
  typename TFeatureConverter::Ptr makeSpecific() const {
    return typename TFeatureConverter::Ptr(
        new TFeatureConverter(feature_converter_));
  }
  virtual FeatureConverter::Ptr make() const {
    return FeatureConverter::Ptr(makeSpecific());
  }
  virtual void learn(const DataSet<TElement>& data_set,
                     bool check_parameters = true) {
    if (check_parameters) {
      this->checkParameters();
    }
    feature_converter_.set_input_feature_info(data_set.feature_info());
    learnImpl(data_set, &feature_converter_);
    feature_converter_.fillOutputFeatureInfo();
  }
 private:
  /**
  * \brief Learn input dataset and train feature converter.
  * \param[in] data_set dataset to study
  * \param[out] feature_converter feature converter to train
  */
  virtual void learnImpl(const DataSet<TElement>& data_set,
                         TFeatureConverter* feature_converter) = 0;
  TFeatureConverter feature_converter_;
};
};

#endif  // LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_LEARNER_H_
