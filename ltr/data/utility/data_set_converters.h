// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_DATA_SET_CONVERTERS_H_
#define LTR_DATA_UTILITY_DATA_SET_CONVERTERS_H_

#include <boost/shared_ptr.hpp>
#include <stdlib.h>

#include <vector>

#include "ltr/data/data_set.h"

using std::vector;

namespace ltr {
namespace utility {

class ListToPairConvertionStrategy {
  public:
  typedef boost::shared_ptr<ListToPairConvertionStrategy> Ptr;
  virtual void operator()(const ObjectList& list,
                          vector<ObjectPair>* result) = 0;
  private:
    virtual ~ListToPairConvertionStrategy() {}
};

class DefaultConverter : public ListToPairConvertionStrategy {
  public:
  void operator()(const ObjectList& list, vector<ObjectPair>* result);
};

/**
 * Function converts data set from one type to another.
 */
template< class TElementTo, class TElementFrom >
DataSet< TElementTo > convertDataSet(const DataSet<TElementFrom>& data,
    ListToPairConvertionStrategy::Ptr strategy =
        ListToPairConvertionStrategy::Ptr(new DefaultConverter));
}
}

#endif  // LTR_DATA_UTILITY_DATA_SET_CONVERTERS_H_
