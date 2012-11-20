// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_DATA_SET_CONVERTERS_H_
#define LTR_DATA_UTILITY_DATA_SET_CONVERTERS_H_

#include <cstdlib>
#include <vector>

#include "ltr/utility/boost/shared_ptr.h" //NOLINT

#include "ltr/data/data_set.h"

#include "ltr/data/object.h"

#include "ltr/data/per_object_accessor.h"

using std::vector;

using ltr::Object;

namespace ltr {
namespace utility {
/**
 * Class is used to convert data from ObjectList to the list of ObjectPair.
 */
class ListToPairConvertionStrategy {
 public:
  typedef ltr::utility::shared_ptr<ListToPairConvertionStrategy> Ptr;
  /**
   * Function converts data from ObjectList to the list of ObjectPair.
   * @param list - list to convert
   * @param result - pointer to the vector to save result in
   */
  virtual void operator()(const ObjectList& list,
                          vector<ObjectPair>* result) = 0;
  virtual ~ListToPairConvertionStrategy() {}
};

class ListToAllPairsConverter : public ListToPairConvertionStrategy {
  public:
  void operator()(const ObjectList& list, vector<ObjectPair>* result);
};

/**
 * Function converts dataset from one type to another.
 * @param data - dataset to convert
 * @param strategy - strategy of convertion.
 * Used only in List to Pair convertion
 */
template< class TElementTo, class TElementFrom >
DataSet<TElementTo> convertDataSet(const DataSet<TElementFrom>& data,
    ListToPairConvertionStrategy::Ptr strategy =
        ListToPairConvertionStrategy::Ptr(new ListToAllPairsConverter));

/**
 * Function extracts all objects from DataSet of any type
 */
template<class TElement>
DataSet<Object> extractAllObjects(const DataSet<TElement>& data) {
  DataSet<Object> result;
  for (int element_index = 0; element_index < data.size(); ++element_index) {
    PerObjectAccessor<const TElement> per_object_accessor(&data[element_index]);
    for (int object_index = 0;
         object_index < per_object_accessor.object_count();
         ++object_index) {
      result.add(per_object_accessor.object(object_index));
    }
  }
  return result;
}
};
};


#endif  // LTR_DATA_UTILITY_DATA_SET_CONVERTERS_H_
