// Copyright 2011 Yandex

#include "ltr/measures/accuracy.h"
#include "ltr/utility/numerical.h"
#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"

namespace ltr {
  template<>
  double Accuracy<Object>::get_measure(const Object& object) const {
      if (utility::DoubleEqual(object.predicted_label(),
          object.actual_label())) {
        return 1;
      } else {
        return 0;
      }
  }

  template<>
  double Accuracy<ObjectPair>::get_measure(
      const ObjectPair& object_pair) const {
    bool predicted_is_less = utility::DoubleLess(
      object_pair.first.predicted_label(),
      object_pair.second.predicted_label());
    bool actual_is_less = utility::DoubleLess(
      object_pair.first.actual_label(),
      object_pair.second.actual_label());

    bool predicted_is_more = utility::DoubleMore(
      object_pair.first.predicted_label(),
      object_pair.second.predicted_label());
    bool actual_is_more = utility::DoubleMore(
      object_pair.first.actual_label(),
      object_pair.second.actual_label());

    if ((predicted_is_less == actual_is_less)
        && (predicted_is_more == actual_is_more)) {
      return 1;
    } else {
      return 0;
    }
  }

  template<>
  string Accuracy<Object>::toString() const {
    return "Pointwise accuracy measure";
  }

  template<>
  string Accuracy<ObjectPair>::toString() const {
    return "Pairwise accuracy measure";
  }
};
