// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_DECISION_TREE_VERTEX_VERTEX_WITH_DATA_H_
#define LTR_LEARNERS_DECISION_TREE_VERTEX_VERTEX_WITH_DATA_H_

#include "ltr/data/data_set.h"

#include "ltr/learners/decision_tree/vertex/vertex.h"

namespace ltr {
namespace decision_tree {
/**
 * The Vertex with corresponding part of DataSet.
 */
struct VertexWithData {
  VertexWithData(Vertex::Ptr vertex, const DataSet<Object>& data)
  : vertex(vertex),
    data(data) {}

  VertexWithData(const VertexWithData& other)
  : vertex(other.vertex),
    data(other.data) {}

  Vertex::Ptr vertex;
  DataSet<Object> data;
};
};
};

#endif  // LTR_LEARNERS_DECISION_TREE_VERTEX_VERTEX_WITH_DATA_H_
