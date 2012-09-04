// Copyright 2012 Yandex

#ifndef LTR_SCORERS_NEAREST_NEIGHBOR_SCORER_H_
#define LTR_SCORERS_NEAREST_NEIGHBOR_SCORER_H_

#include <string>
#include <vector>

#include "ltr/aggregators/aggregator.h"
#include "ltr/data/object.h"
#include "ltr/data/data_set.h"
#include "ltr/scorers/scorer.h"
#include "ltr/metrics/metric.h"
#include "ltr/utility/shared_ptr.h"
#include "ltr/utility/neighbor_weighter.h"

using std::string;
using std::vector;

using ltr::Aggregator;
using ltr::DataSet;
using ltr::Object;
using ltr::utility::NeighborWeighter;

namespace ltr {
 /** \class The class prepresents Scorer for NNLearner. It contains the learning data set,
 * pointers to aggregator, metric and neigbor weighter.
 */
class NNScorer : public Scorer {
 public:
  typedef ltr::utility::shared_ptr<NNScorer> Ptr;
  typedef ltr::utility::shared_ptr<ltr::BaseMetric> BaseMetricPtr;
  NNScorer() {
  }

  explicit NNScorer(BaseMetricPtr metric) : metric_(metric) {
  }

  NNScorer(BaseMetricPtr metric,
           DataSet<Object> data,
           NeighborWeighter::Ptr neighbor_weighter,
           Aggregator::Ptr aggregator,
           int number_of_neighbors_to_process) :
    metric_(metric),
    data_(data),
    neighbor_weighter_(neighbor_weighter),
    aggregator_(aggregator),
    number_of_neighbors_to_process_(number_of_neighbors_to_process) {
  }

  string toString() const;

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

  BaseMetricPtr metric_;
  DataSet<Object> data_;
  NeighborWeighter::Ptr neighbor_weighter_;
  Aggregator::Ptr aggregator_;
  int number_of_neighbors_to_process_;
};
};

#endif  // LTR_SCORERS_NEAREST_NEIGHBOR_SCORER_H_
