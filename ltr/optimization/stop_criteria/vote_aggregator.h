// copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_STOP_CRITERIA_VOTE_AGGREGATOR_H_
#define LTR_OPTIMIZATION_STOP_CRITERIA_VOTE_AGGREGATOR_H_

#include <string>
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/utility/macros.h"

namespace optimization {
/**
 * \brief Aggregator which returns true if part of
 * true answers from one point stop criteria more
 * than minimal_part_of_true_answers.
 */
class VoteAggregator : public ltr::Aliaser {
 public:
  ltr::utility::shared_ptr<VoteAggregator> Ptr;

  explicit VoteAggregator()
    : true_answers_(0),
      false_answers_(0) { }
  ~VoteAggregator() { }

  /**
   * Add yet another answer.
   */
  void push(bool answer);
  /**
   * Clear accumulated answers.
   */
  void reset();
  /**
   * Aggregate accumulated answers to the final one via voting.
   *
   * \return True if stopping condition is satisfied for more 
   * than given part of points in population.
   */
  bool isTrue(double true_answers_threshold = 1.0);

  GET(int, true_answers);
  GET(int, false_answers);

  string getDefaultAlias() const { return "VoteAggregator"; }
 private:
  int true_answers_;
  int false_answers_;
};
}

#endif  // LTR_OPTIMIZATION_STOP_CRITERIA_VOTE_AGGREGATOR_H_
