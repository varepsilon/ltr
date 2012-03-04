// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_H_

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
#include "ltr/data/data_set.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/interfaces/serializable.h"

namespace ltr {

class FeatureConverter : public Serializable {
  public:
  typedef boost::shared_ptr<FeatureConverter> Ptr;
  typedef boost::shared_ptr<const FeatureConverter> ConstPtr;

  virtual ~FeatureConverter() {}

  virtual FeatureInfo
  convertFeatureInfo(const FeatureInfo& oldFeatureInfo) const = 0;

  virtual void apply(const ltr::Object & argument, ltr::Object * value)
  const = 0;
};

typedef std::vector< FeatureConverter::ConstPtr > FeatureConverterArray;
}

#endif  // LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_H_
