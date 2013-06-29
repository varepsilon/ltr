// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_BASE_SPLITTER_H_
#define LTR_LEARNERS_DECISION_TREE_BASE_SPLITTER_H_

#include <vector>
#include <string>

#include "ltr/data/data_set.h"
#include "ltr/data/utility/data_set_statistics.h"

#include "ltr/learners/decision_tree/vertex/vertex_with_data.h"

#include "ltr/interfaces/parameterized.h"

#include "ltr/utility/boost/shared_ptr.h"

using std::vector;

using ltr::utility::LabelStatisticComputer;

namespace ltr {
namespace decision_tree {
struct Threshold {
  Threshold() {}

  Threshold(int feature_index,
            double value)
  : feature_index(feature_index),
    value(value) {}

  int feature_index;
  double value;
};
/*
 * BaseSplitter is a base class for splitters.
 * For given decision tree layer(vector of VertexWithData)
 * generates next layer by performing split in each Vertex.
 */
class BaseSplitter : public Parameterized {
  ALLOW_SHARED_PTR_ONLY_CREATION(BaseSplitter)
 public:
  virtual string toString() const = 0;
  /**
   * Function used to generate the next tree layer.
   * Returns empty vector if can't generate.
   */
  virtual void generateNextLayer(const vector<VertexWithData>& layer,
                                  vector<VertexWithData>* next_layer) const = 0;

 protected:
  void findBestThresholdWithQuality(const DataSet<Object>& data,
                                    int feature_index,
                                    Threshold* best_threshold,
                                    double* quality) const;

  void splitLayer(const vector<VertexWithData>& layer,
                  const vector<Threshold>& thresholds,
                  vector<VertexWithData>* next_layer) const;

 private:
  void splitVertex(const VertexWithData& vertex,
                   const Threshold& threshold,
                   vector<VertexWithData>* children) const;

  double computeQuality(
    const LabelStatisticComputer& all_label_stat_computer,
    const LabelStatisticComputer& first_part_computer,
    const LabelStatisticComputer& second_part_computer) const;
};

class FakeSplitter : public BaseSplitter {
 public:
  void generateNextLayer(const vector<VertexWithData>& layer,
                         vector<VertexWithData>* next_layer) const {
    next_layer->clear();
  }
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_BASE_SPLITTER_H_
