// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_OBLIVIOUS_TREE_SPLITTER_H_
#define LTR_LEARNERS_DECISION_TREE_OBLIVIOUS_TREE_SPLITTER_H_

#include <vector>
#include <string>

#include "ltr/learners/decision_tree/base_splitter.h"

#include "ltr/utility/boost/shared_ptr.h"

using std::vector;

namespace ltr {
namespace decision_tree {
/**
 * \class ObliviousTreeSplitter 
 * \brief Implements ObliviousTree splitting aplorithm.
 * 
 * Checks all the possible features and determines the one that
 * has the highest layer quality.
 * For continious features try all possible thresholds
 * between two adjacent values of a feature.
 */
class ObliviousTreeSplitter : public BaseSplitter {
 public:
  typedef ltr::utility::shared_ptr<ObliviousTreeSplitter> Ptr;

  ObliviousTreeSplitter() {}

  explicit ObliviousTreeSplitter(const ParametersContainer& parameters);

  string toString() const;

  virtual void generateNextLayer(const vector<VertexWithData>& layer,
                                 vector<VertexWithData>* next_layer) const;
  void findBestThresholdWithQualityByFeature(
                                const vector<VertexWithData>& layer,
                                vector<Threshold>& thresholds,
                                int feature_index,
                                double* layer_split_quality) const;
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_OBLIVIOUS_TREE_SPLITTER_H_
