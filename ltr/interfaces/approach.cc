// Copyright 2011 Yandex

#include "ltr/interfaces/approach.h"

using std::string;

namespace ltr {
  template<> string Approach<ltr::Object>::name() {
    return POINTWISE;
  }

  template<> string Approach<ltr::ObjectPair>::name() {
    return PAIRWISE;
  }

  template<> string Approach<ltr::ObjectList>::name() {
    return LISTWISE;
  }
};
