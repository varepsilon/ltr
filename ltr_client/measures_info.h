// Copyright 2011 Yandex School Practice
#ifndef LTR_CLIENT_MEASURES_INFO_H_
#define LTR_CLIENT_MEASURES_INFO_H_

#include <string>

#include "ltr/measures/measure.h"
#include "boost/variant.hpp"

using std::string;

/**
@class MeasureInfo
Class contains all necessary information about one measure object.
*/
template< class TElement>
struct MeasureInfo {
  MeasureInfo();
  typename ltr::Measure<TElement>::Ptr measure;
  string type;
  string approach;
};

typedef boost::variant<MeasureInfo<ltr::ObjectList>,
                       MeasureInfo<ltr::ObjectPair>,
                       MeasureInfo<ltr::Object> > VMeasureInfo;
#endif  // LTR_CLIENT_MEASURES_INFO_H_
