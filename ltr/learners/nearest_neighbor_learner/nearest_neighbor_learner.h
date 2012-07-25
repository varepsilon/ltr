// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_NEAREST_NEIGHBOR_LEARNER_NEAREST_NEIGHBOR_LEARNER_H_
#define LTR_LEARNERS_NEAREST_NEIGHBOR_LEARNER_NEAREST_NEIGHBOR_LEARNER_H_

#include <logog/logog.h>

#include <string>
#include <vector>

#include "ltr/aggregators/aggregator.h"
#include "ltr/learners/learner.h"
#include "ltr/metrics/metric.h"
#include "ltr/parameters_container/parameters_container.h"
#include "ltr/scorers/nearest_neighbor_scorer.h"
#include "ltr/utility/shared_ptr.h"
#include "ltr/utility/neighbor_weighter.h"

using ltr::Aggregator;
using ltr::BaseMetric;
using ltr::NNScorer;
using ltr::ParametersContainer;
using ltr::utility::NeighborWeighter;

namespace ltr {
template<class TElement>
/**
 \brief GPLearner
 Implements nearest neighbor approach.
 \tparam TElement object container of those the dataset consists (it should
 be Object).
 */
class NNLearner : public BaseLearner<TElement, NNScorer> {
 public:
  typedef ltr::utility::shared_ptr<NNLearner> Ptr;

  NNLearner() {
    // DO NOTHING
  }

  explicit NNLearner(const ParametersContainer& parameters) {
    metric_ = parameters.Get<BaseMetric::Ptr>("metric");
    neighbor_weighter_ =
      parameters.Get<NeighborWeighter::Ptr>("neighbor_weighter");
    aggregator_ = parameters.Get<Aggregator::Ptr>("aggregator");
    number_of_neighbors_to_process_ =
      parameters.Get<int>("number_of_neighbors_to_process");
  }

  NNLearner(BaseMetric::Ptr metric,
            NeighborWeighter::Ptr neighbor_weighter,
            Aggregator::Ptr aggregator,
            int number_of_neighbors_to_process) :
    metric_(metric),
    neighbor_weighter_(neighbor_weighter),
    aggregator_(aggregator),
    number_of_neighbors_to_process_(number_of_neighbors_to_process) {
  }

  GET_SET(typename BaseMetric::Ptr, metric);
  GET_SET(NeighborWeighter::Ptr, neighbor_weighter);
  GET_SET(Aggregator::Ptr, aggregator);
  GET_SET(int, number_of_neighbors_to_process);

 private:
  void learnImpl(const DataSet<TElement>& data, NNScorer* scorer);
  virtual string getDefaultAlias() const {return "NearestNeightborScorer";}

  BaseMetric::Ptr metric_;
  NeighborWeighter::Ptr neighbor_weighter_;
  Aggregator::Ptr aggregator_;
  int number_of_neighbors_to_process_;
};

/** The implementation of nearest neighbor approach.
  * \param data DataSet that will be stored in the scorer.
  */
template<class TElement>
void NNLearner<TElement>::learnImpl(const DataSet<TElement>& data,
                                    NNScorer* scorer) {
  INFO("Learning started");
  *scorer =  NNScorer(metric_,
                      data,
                      neighbor_weighter_,
                      aggregator_,
                      number_of_neighbors_to_process_);
}
};

#endif  // LTR_LEARNERS_NEAREST_NEIGHBOR_LEARNER_NEAREST_NEIGHBOR_LEARNER_H_
