// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_DATA_SET_UTILITY_H_
#define LTR_DATA_UTILITY_DATA_SET_UTILITY_H_

#include <boost/lexical_cast.hpp>

#include <vector>
#include <map>
#include <string>

#include "ltr/data/data_set.h"

using std::string;
using std::vector;
using std::map;
using boost::lexical_cast;

namespace ltr {
namespace utility {
  void groupByMeta(const vector<Object>& objects,
                   string group_parameter,
                   map<string, vector<Object> >* result);

  void groupByIntMeta(const vector<Object>& objects,
                      string group_parameter,
                      map<int, vector<Object> >* result);

  void groupByFloatMeta(const vector<Object>& objects,
                        string group_parameter,
                        map<float, vector<Object> >* result);
};
};
#endif  // LTR_DATA_UTILITY_DATA_SET_UTILITY_H_
