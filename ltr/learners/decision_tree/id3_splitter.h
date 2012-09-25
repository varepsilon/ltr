// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_ID3_SPLITTER_H_
#define LTR_LEARNERS_DECISION_TREE_ID3_SPLITTER_H_

#include <vector>
#include <string>

#include "ltr/learners/decision_tree/base_splitter.h"

#include "ltr/utility/shared_ptr.h"

using std::vector;

namespace ltr {
namespace decision_tree {
/**
 * \class ID3Splitter Implements ID3 splitting aplorithm.
 * For each vertex at given layer checks all the possible features
 * and determines the one that has the highest quality.
 * For continious features try all possible thresholds
 * between two adjacent values of a feature.
 */
class ID3Splitter : public BaseSplitter {
 public:
  typedef ltr::utility::shared_ptr<ID3Splitter> Ptr;

  ID3Splitter() {}

  string toString() const;

  virtual void generateNextLayer(const vector<VertexWithData>& layer,
                                 vector<VertexWithData>* next_layer) const;
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_ID3_SPLITTER_H_
