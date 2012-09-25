// Copyright 2012 Yandex

#ifndef LTR_DATA_UTILITY_DATA_SET_LABEL_STATISTICS_H_
#define LTR_DATA_UTILITY_DATA_SET_LABEL_STATISTICS_H_

#include <map>
#include <vector>
#include <functional>

#include "ltr/data/data_set.h"
#include "ltr/data/per_object_accessor.h"

#include "ltr/utility/numerical.h"

using std::map;

using ltr::DataSet;

namespace ltr {
namespace utility {
class DoubleLessComparator : public std::binary_function<double, double, bool> {
 public:
  bool operator()(double lhs, const double rhs) const {
    return DoubleLess(lhs, rhs);
  }
};

struct LabelStatistic {
  LabelStatistic(double label, int count, double weight)
  : label(label),
    count(count),
    weight(weight) {}

double label;
int count;
double weight;
};

class LabelStatisticComputer {
 public:
  LabelStatisticComputer() {}

  template <typename TElement>
  explicit LabelStatisticComputer(const DataSet<TElement>& data);

  template <typename TElement>
  void add(const TElement& element, double weight);

  void add(double label, double weight);

  template <typename TElement>
  void remove(const TElement& element, double weight);

  void remove(double label, double weight);

  void reset();

  double getMostCommonLabel() const;

  double getHeaviestLabel() const;

  int getHoldingDataSize() const;

  int getLabelsCount() const;

  int getLabelSizeById(int id) const;

  double getLabelWeightById(int id) const;

 private:
  map<double, int, DoubleLessComparator> labels_id_;
  vector<LabelStatistic> label_statistics_;
};

template <typename TElement>
LabelStatisticComputer::LabelStatisticComputer(const DataSet<TElement>& data) {
  for (int element_index = 0; element_index < data.size(); ++element_index) {
    this->add(data[element_index], data.getWeight(element_index));
  }
}

template <typename TElement>
void LabelStatisticComputer::add(const TElement& element, double weight) {
  PerObjectAccessor<const TElement> per_object_accessor(&element);
  for (int object_index = 0;
       object_index < per_object_accessor.object_count(); ++object_index) {
    double label = per_object_accessor.object(object_index).actual_label();
    add(label, weight);
  }
}

template <typename TElement>
void LabelStatisticComputer::remove(const TElement& element, double weight) {
  PerObjectAccessor<const TElement> per_object_accessor(&element);
  for (int object_index = 0;
       object_index < per_object_accessor.object_count(); ++object_index) {
    double label = per_object_accessor.object(object_index).actual_label();
    remove(label, weight);
  }
}
};
};

#endif  // LTR_DATA_UTILITY_DATA_SET_LABEL_STATISTICS_H_
