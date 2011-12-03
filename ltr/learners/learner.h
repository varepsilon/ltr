// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_LEARNER_H_
#define LTR_LEARNERS_LEARNER_H_


#include <stdexcept>
#include <string>
#include <vector>

#include "ltr/interfaces.h"
#include "data/data_set.h"
#include "scorers/scorer.h"
//#include "feature_converters/feature_converter.h"
#include "parameters_container/parameters_container.h"

namespace ltr {

template< class TElement, class TScorer >
class Learner : public Reporter, public Aliaser, public Parameterized {
  public:
    typedef boost::shared_ptr<Learner> Ptr;

	  TScorer::Ptr learn(const DataSet<TElement>& data,
		  const TScorer& initial_scorer = TScorer()) const;

	  // void addFeatureConverter(FeatureConverter::ConstPtr input_feature_converter);

	  virtual ~Learner();

  protected:
	  virtual TScorer::Ptr learnImpl(const DataSet<TElement>& data,
		  const TScorer& initial_scorer) const = 0;

  private:
    // std::vector<FeatureConverter::Ptr> feature_converters_;
};

typedef Learner<Object> PointwiseLearner;
typedef Learner<ObjectPair> PairwiseLearner;
typedef Learner<ObjectList> ListwiseLearner;
};



namespace ltr {

template< class TElement, class TScorer >
TScorer::Ptr Learner< TElement, TScorer >::learn(const DataSet<TElement>& data,
	const TScorer& initial_scorer = TScorer()) const
{
    // apply feature converters
    return learnImpl(data, initial_scorer);
}

// old implementation, could be useful
/*template <class TElement>
void ILearner<TElement>::learn(const DataSet<TElement> & data) {

    DataSet<TElement> sourceData;
    DataSet<TElement> preprocessedData;
    data.DeepCopy(sourceData);

    for (typename std::vector< typename FeatureConverter::Ptr >::const_iterator preprocessor_itr = feature_converters_.begin();
            preprocessor_itr != feature_converters_.end();
            ++preprocessor_itr) {
        (**preprocessor_itr).apply(sourceData, &preprocessedData);
        using std::swap;
        swap(sourceData, preprocessedData);
    }

    learnImpl(sourceData);
}*/

};

#endif  // LTR_LEARNERS_LEARNER_H_
