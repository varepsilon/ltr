// Copyright 2011 Yandex

#ifndef LTR_CLIENT_CONSTANTS_H_
#define LTR_CLIENT_CONSTANTS_H_

#include <string>

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/object_list.h"

#define PW "pointwise"
#define LW "listwise"
#define PRW "pairwise"

/**
@struct Appoach
This struct is used to get string for given template approach.
*/
template<class TElement>
struct Approach {
  /**
  Function which returns string for template approach.
  */
  static std::string name() {return "Unknown";}
};
template <> std::string Approach<ltr::Object>::name();
template <> std::string Approach<ltr::ObjectPair>::name();
template <> std::string Approach<ltr::ObjectList>::name();

#endif  // LTR_CLIENT_CONSTANTS_H_
