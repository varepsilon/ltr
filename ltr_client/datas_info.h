// Copyright 2011 Yandex School Practice
#ifndef LTR_CLIENT_DATAS_INFO_H_
#define LTR_CLIENT_DATAS_INFO_H_

#include <string>
#include "boost/variant.hpp"
#include "ltr/data/data_set.h"

/**
@class DataInfo
Class contains all necessary information about one data object.
*/
template <class TElement>
struct DataInfo {
  ltr::DataSet<TElement> data;
  string data_file;
  string format;
  string approach;
};

typedef boost::variant<DataInfo<ltr::ObjectList>,
                       DataInfo<ltr::ObjectPair>,
                       DataInfo<ltr::Object> > VDataInfo;
#endif  // LTR_CLIENT_DATAS_INFO_H_
