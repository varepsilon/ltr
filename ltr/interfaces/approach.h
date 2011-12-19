// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_APPROACH_H_
#define LTR_INTERFACES_APPROACH_H_

#include <string>

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/object_list.h"

using std::string;

namespace ltr {
  template <class TElement>
  struct Approach {
    static string name() {
      return "unknown";
    }
  };
  #define LISTWISE "listwise"
  #define POINTWISE "pointwise"
  #define PAIRWISE "pairwise"

  template<> string Approach<ltr::Object>::name();
  template<> string Approach<ltr::ObjectPair>::name();
  template<> string Approach<ltr::ObjectList>::name();
};

#endif  // LTR_INTERFACES_APPROACH_H_
