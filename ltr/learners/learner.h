// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_LEARNER_H_
#define LTR_LEARNERS_LEARNER_H_

#include <vector>

#include "ltr/interfaces.h"
#include "ltr/data/data_set.h"
#include "ltr/feature_converters/feature_converter.h"
#include "ltr/parameters_container/parameters_container.h"

namespace ltr {

template< class TElement, class TScorer >
class Learner : public Reporter, public Aliaser, public Parameterized {
    public:
    typedef boost::shared_ptr<Learner> Ptr;

    void learn(const DataSet<TElement>& data);

    void addFeatureConverter(
            typename ltr::FeatureConverter::ConstPtr p_FeatureConverter);

    virtual void reset() = 0;
    virtual void setInitialScorer(const TScorer& in_scorer) = 0;
    virtual TScorer make() const = 0;

    virtual ~Learner();

    private:
    virtual void learnImpl(const DataSet<TElement>& data) = 0;

    FeatureConverterArray featureConverters_;
};

template< class TElement, class TScorer >
void Learner< TElement, TScorer >::addFeatureConverter(
        typename ltr::FeatureConverter::ConstPtr p_FeatureConverter) {
    featureConverters_.push_back(p_FeatureConverter);
}

template< class TElement, class TScorer >
void Learner< TElement, TScorer >::learn(const DataSet<TElement>& data) {
    DataSet<TElement> sourceData = data.deepCopy();
    DataSet<TElement> convertedData;
    for (size_t featureConverterIdx = 0;
            featureConverterIdx < featureConverters_.size();
            ++featureConverterIdx) {
        (*featureConverters_[featureConverterIdx]).apply(
                sourceData,
                &convertedData);
        sourceData = convertedData;
    }
    learnImpl(sourceData);
}
};
#endif  // LTR_LEARNERS_LEARNER_H_
