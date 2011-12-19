// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_H_

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
#include "ltr/data/data_set.h"

namespace ltr {

class FeatureConverter {
    public:
    typedef boost::shared_ptr<FeatureConverter> Ptr;
    typedef boost::shared_ptr<const FeatureConverter> ConstPtr;

    virtual ~FeatureConverter() {}

    virtual FeatureInfo
    convertFeatureInfo(const FeatureInfo& oldFeatureInfo) const = 0;

    virtual void apply(const ltr::Object & argument, ltr::Object * value)
    const = 0;

    template <typename TElement>
    void apply(const DataSet<TElement> & argument,
            DataSet<TElement> * value) const;

    virtual std::string generateCppCode(const std::string& class_name,
            int tabbing = 0) const = 0;

    virtual std::string generateJavaCode(const std::string& class_name,
            int tabbing = 0, bool is_static = false) const = 0;
};

template <typename TElement>
void FeatureConverter::apply(
        const DataSet<TElement> & argument,
        DataSet<TElement> * value) const {
    DataSet<TElement> result(this->convertFeatureInfo(argument.featureInfo()));

    for (size_t elementIdx = 0; elementIdx < argument.size(); ++elementIdx) {
        for (size_t objIdx = 0;
                objIdx < argument[elementIdx].size();
                ++objIdx) {
            Object objToAdd;
            apply(argument[elementIdx][objIdx], &objToAdd);
            result << objToAdd;
        }
    }

    *value = result;
}

typedef std::vector< FeatureConverter::ConstPtr > FeatureConverterArray;
}

#endif  // LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_H_
