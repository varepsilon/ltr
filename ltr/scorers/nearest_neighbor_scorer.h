// Copyright 2012 Yandex

#ifndef LTR_SCORERS_NEAREST_NEIGHBOR_SCORER_H_
#define LTR_SCORERS_NEAREST_NEIGHBOR_SCORER_H_

#include <string>
#include <vector>

#include "ltr/predictions_aggregators/predictions_aggregator.h"
#include "ltr/data/object.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/scorer.h"
#include "ltr/metrics/metric.h"
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/utility/neighbor_weighter.h"

using std::string;
using std::vector;

using ltr::PredictionsAggregator;
using ltr::DataSet;
using ltr::Object;
using ltr::utility::NeighborWeighter;

namespace ltr {
 /** \brief The class prepresents Scorer for NNLearner. It contains the learning data set,
 * pointers to aggregator, metric and neigbor weighter.
 */
class NNScorer : public Scorer {
  ALLOW_SHARED_PTR_ONLY_CREATION(NNScorer)
 public:
  typedef ltr::utility::shared_ptr<ltr::BaseMetric> BaseMetricPtr;
  explicit NNScorer() {
  }

  explicit NNScorer(BaseMetricPtr metric) : metric_(metric) {
  }

  explicit NNScorer(BaseMetricPtr metric,
           DataSet<Object> data,
           NeighborWeighter::Ptr neighbor_weighter,
           PredictionsAggregator::Ptr predictions_aggregator,
           int number_of_neighbors_to_process) :
    metric_(metric),
    data_(data),
    neighbor_weighter_(neighbor_weighter),
    predictions_aggregator_(predictions_aggregator),
    number_of_neighbors_to_process_(number_of_neighbors_to_process) {
  }

 private:
  /** The implementation of scoring function. It scores using the learning data set,
  * given metric, weighter, aggregator and number of nearest neighbors
  * that must be processed.
  */
  virtual double scoreImpl(const Object& object) const;
  /** the function generates code for the scorer as cpp code function
   * \param function_name the name for the function that would be created.
   */
  virtual string generateCppCodeImpl(const string& function_name) const;
  virtual string getDefaultAlias() const {return "NNscorer";}

  string toStringImpl() const;

  BaseMetricPtr metric_;
  DataSet<Object> data_;
  NeighborWeighter::Ptr neighbor_weighter_;
  PredictionsAggregator::Ptr predictions_aggregator_;
  int number_of_neighbors_to_process_;
};
};

#endif  // LTR_SCORERS_NEAREST_NEIGHBOR_SCORER_H_
