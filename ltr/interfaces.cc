// Copyright 2011 Yandex

#include "ltr/interfaces.h"

namespace ltr {
  template<> std::string Approach<ltr::Object>::name() {
    return POINTWISE;
  }

  template<> std::string Approach<ltr::ObjectPair>::name() {
    return PAIRWISE;
  }

  template<> std::string Approach<ltr::ObjectList>::name() {
    return LISTWISE;
  }
};
