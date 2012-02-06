// Copyright 2012 Yandex School Practice
#ifndef LTR_CLIENT_SPLITTER_INFO_H_
#define LTR_CLIENT_SPLITTER_INFO_H_

#include <string>

#include "ltr/crossvalidation/splitter.h"
#include "boost/variant.hpp"

using std::string;

/**
@class SplitterInfo
Class contains all necessary information about one splitter object.
*/
template< class TElement>
struct SplitterInfo {
  typename ltr::cv::Splitter<TElement>::Ptr splitter;
  ltr::ParametersContainer parameters;
  string type;
  string approach;
};

typedef boost::variant<SplitterInfo<ltr::ObjectList>,
                       SplitterInfo<ltr::ObjectPair>,
                       SplitterInfo<ltr::Object> > VSplitterInfo;
#endif  // LTR_CLIENT_SPLITTER_INFO_H_
