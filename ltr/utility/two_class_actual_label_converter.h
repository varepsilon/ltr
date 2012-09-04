// Copyright 2012 Yandex

#ifndef LTR_UTILITY_TWO_CLASS_ACTUAL_LABEL_CONVERTER_H_
#define LTR_UTILITY_TWO_CLASS_ACTUAL_LABEL_CONVERTER_H_

#include <stdexcept>
#include <string>
#include <set>

using std::set;

namespace ltr {
namespace utility {
/**
 * If we have the dataset with less than or equal to 2 different
 * actual labels, then we can use TwoClassActualLabelConverter to
 * convert this actual labels to {-1, 1}. For example, if we have
 * labels 2 and 5, then 2 became -1 and 5 became 1 after converting.
 *
 */
template <typename TElement>
class TwoClassActualLabelConverter {
 public:
  void convert(DataSet<TElement>* data_set);
 private:
  void searchForValues(const DataSet<TElement>& data_set);
  void convert(Object* object);
  double to_positive;
  double to_negative;
};

template <typename TElement>
void TwoClassActualLabelConverter<TElement>::convert(
  DataSet<TElement>* data_set) {
  searchForValues(*data_set);
  for (int index = 0; index < data_set->size(); ++index) {
    PerObjectAccessor<TElement> per_object_accessor(&data_set->at(index));
    for (int object_index = 0;
         object_index < per_object_accessor.object_count();
         ++object_index) {
      convert(&per_object_accessor.object(object_index));
    }
  }
}

template <typename TElement>
void TwoClassActualLabelConverter<TElement>::searchForValues(
  const DataSet<TElement>& data_set) {
  set<double> available_labels;
  for (int index = 0; index < data_set.size(); ++index) {
    PerObjectAccessor<const TElement> per_object_accessor(&data_set[index]);
    for (int object_index = 0;
         object_index < per_object_accessor.object_count();
         ++object_index) {
      available_labels.insert(
        per_object_accessor.object(object_index).actual_label());
      if (available_labels.size() > 2) {
        throw std::logic_error(
          "there are more than two different actual labels in dataset");
      }
    }
  }
  if (available_labels.size() == 1) {
    to_positive = *available_labels.begin();
  } else if (available_labels.size() == 2) {
    to_positive = *available_labels.rbegin();
    to_negative = *available_labels.begin();
  }
}

template <typename TElement>
void TwoClassActualLabelConverter<TElement>::convert(Object* object) {
  if (object->actual_label() == to_positive) {
    object->set_actual_label(1);
  } else {
    object->set_actual_label(-1);
  }
}
};
};
#endif  // LTR_UTILITY_TWO_CLASS_ACTUAL_LABEL_CONVERTER_H_
