// Copyright 2012 Yandex

#ifndef LTR_UTILITY_TWO_CLASS_PREDICTED_LABEL_CONVERTER_H_
#define LTR_UTILITY_TWO_CLASS_PREDICTED_LABEL_CONVERTER_H_

#include <stdexcept>
#include <string>

namespace ltr {
namespace utility {
template <typename TElement>
class TwoClassPredictedLabelConverter {
 public:
  void convert(DataSet<TElement>* data_set, double splitter = 0.);
 private:
  void convert(Object* object, double splitter = 0.);
  double to_positive;
  double to_negative;
};

template <typename TElement>
void TwoClassPredictedLabelConverter<TElement>::convert(
  DataSet<TElement>* data_set,
  double splitter) {
  for (int index = 0; index < data_set->size(); ++index) {
    PerObjectAccessor<TElement> per_object_accessor(&data_set->at(index));
    for (int object_index = 0;
         object_index < per_object_accessor.object_count();
         ++object_index) {
      convert(&per_object_accessor.object(object_index), splitter);
    }
  }
}

template <typename TElement>
void TwoClassPredictedLabelConverter<TElement>::convert(
  Object* object,
  double splitter) {
  if (object->predicted_label() > splitter) {
    object->set_predicted_label(1);
  } else {
    object->set_predicted_label(-1);
  }
}
};
};
#endif  // LTR_UTILITY_TWO_CLASS_PREDICTED_LABEL_CONVERTER_H_
