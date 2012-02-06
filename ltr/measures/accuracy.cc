// Copyright 2011 Yandex

#include "ltr/measures/accuracy.h"
#include "ltr/utility/numerical.h"
#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"

namespace ltr {
  template<>
  double Accuracy<Object>::get_measure(const Object& object) const {
      if (utility::DoubleEqual(object.predictedLabel(),
          object.actualLabel())) {
        return 1;
      } else {
        return 0;
      }
  }

  template<>
  double Accuracy<ObjectPair>::get_measure(
      const ObjectPair& object_pair) const {
    bool predicted_is_less = utility::DoubleLess(
      object_pair.first().predictedLabel(),
      object_pair.second().predictedLabel());
    bool actual_is_less = utility::DoubleLess(
      object_pair.first().actualLabel(),
      object_pair.second().actualLabel());

    bool predicted_is_more = utility::DoubleMore(
      object_pair.first().predictedLabel(),
      object_pair.second().predictedLabel());
    bool actual_is_more = utility::DoubleMore(
      object_pair.first().actualLabel(),
      object_pair.second().actualLabel());

    if ((predicted_is_less == actual_is_less)
        && (predicted_is_more == actual_is_more)) {
      return 1;
    } else {
      return 0;
    }
  }
};
