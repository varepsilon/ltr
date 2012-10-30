// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_LEARNER_H_
#define LTR_LEARNERS_LEARNER_H_

#include <rlog/rlog.h>

#include <vector>
#include <string>

#include "ltr/data/data_set.h"

#include "ltr/data_preprocessors/data_preprocessor.h"

#include "ltr/feature_converters/feature_converter_learner.h"

#include "ltr/interfaces/reporter.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/interfaces/parameterized.h"

#include "ltr/measures/measure.h"

#include "ltr/parameters_container/parameters_container.h"

#include "ltr/scorers/scorer.h"

#include "ltr/interfaces/printable.h"

#include "ltr/utility/shared_ptr.h"

using std::string;
using std::vector;

namespace ltr {
/**
 * \brief Learns input dataset and produces a Scorer.
 *
 * If you want to make your own Learner it's more convinient
 * to inherit from BaseLearner.
 *
 * \see Learner, BaseLearner, Scorer
 */
template<class TElement>
class Learner : public Reporter,
                public Aliaser,
                public Parameterized {
 public:
  typedef ltr::utility::shared_ptr<Learner> Ptr;
  typedef ltr::utility::shared_ptr<Learner> BasePtr;

  typedef vector<typename FeatureConverterLearner<TElement>::Ptr>
        FeatureConverterLearnerArray;
  typedef vector<typename DataPreprocessor<TElement>::Ptr>
        DataPreprocessorArray;

  /**
  * \brief Learns input dataset
  * \param data_set dataset to study
  * \param check_parameters whether perform Parameterized::checkParameters() before launch
  */
  virtual void learn(const DataSet<TElement>& data_set,
                     bool check_parameters = true) = 0;
  /**
  * \note Don't forget to learn() before make().
  * \return pointer to <em>new instance</em> of scorer.
  */
  virtual Scorer::Ptr make() const = 0;

  /**
  * \brief Reset internal state of Learner
  */
  virtual void reset() = 0;
  // \TODO Meaning is not clear

  GET_SET_VECTOR_OF_PTR(DataPreprocessor<TElement>, data_preprocessor);
  GET_SET_VECTOR_OF_PTR(FeatureConverter, feature_converter);
  GET_SET_VECTOR_OF_PTR(FeatureConverterLearner<TElement>,
      feature_converter_learner);

 protected:
  DataPreprocessorArray data_preprocessor_;

  FeatureConverterArray feature_converter_;
  FeatureConverterLearnerArray feature_converter_learner_;
};

template<class TElement, class TScorer>
class BaseLearner : public Learner<TElement> {
 public:
  /**
  * \note Don't forget to learn() before makeSpecific().
  * \return pointer to <em>new instance</em> of scorer
  */
  typename TScorer::Ptr makeSpecific() const;
  virtual Scorer::Ptr make() const;
  virtual void learn(const DataSet<TElement>& data_set,
                     bool check_parameters = true);
  // \TODO ? We don't need it in base class as not all Learners support it
  virtual void setInitialScorer(const TScorer& scorer);
  virtual void setDefaultScorer();
  virtual void reset();
 private:
  /**
  * \brief Learn input dataset and train scorer.
  * \param[in] data_set dataset to study
  * \param[out] scorer scorer to train
  */
  virtual void learnImpl(const DataSet<TElement>& data_set,
                         TScorer* scorer) = 0;

  using Learner<TElement>::data_preprocessor_;

  using Learner<TElement>::feature_converter_;
  using Learner<TElement>::feature_converter_learner_;

 protected:
  TScorer scorer_;
};

// template realization

// _________________BaseLearner_______________________

template<class TElement, class TScorer>
typename TScorer::Ptr BaseLearner<TElement, TScorer>::makeSpecific() const {
  return typename TScorer::Ptr(
      new TScorer(scorer_));
}

template<class TElement, class TScorer>
Scorer::Ptr BaseLearner<TElement, TScorer>::make() const {
  return Scorer::Ptr(makeSpecific());
}

template<class TElement, class TScorer>
void BaseLearner<TElement, TScorer>::reset() {
  setDefaultScorer();
}


template<class TElement, class TScorer>
void BaseLearner<TElement, TScorer>::setDefaultScorer() {
  TScorer scorer;
  scorer_ = scorer;
}

template<class TElement, class TScorer>
void BaseLearner<TElement, TScorer>::setInitialScorer(const TScorer& scorer) {
  scorer_ = scorer;
}

template<class TElement, class TScorer>
void BaseLearner<TElement, TScorer>::learn(const DataSet<TElement>& data_set,
                                           bool check_parameters) {
  if (check_parameters) {
    this->checkParameters();
  }
  setDefaultScorer();

  DataSet<TElement> source_data = data_set.deepCopy();
  DataSet<TElement> converted_data;

  for (int index = 0; index < (int)data_preprocessor_.size(); ++index) {
    data_preprocessor_[index]->apply(source_data, &converted_data);
    source_data = converted_data;
  }

  feature_converter_.clear();
  for (int index = 0;
       index < (int)feature_converter_learner_.size(); ++index) {
    feature_converter_learner_[index]->learn(source_data);
    feature_converter_.push_back(feature_converter_learner_[index]->make());
    feature_converter_[index]->apply(source_data, &converted_data);
    source_data = converted_data;
  }

  learnImpl(source_data, &scorer_);
  scorer_.set_feature_converter(feature_converter_);
}
};
#endif  // LTR_LEARNERS_LEARNER_H_
