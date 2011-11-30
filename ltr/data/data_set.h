// Copyright 2011 Yandex

#ifndef LTR_DATA_DATA_SET_H_
#define LTR_DATA_DATA_SET_H_

#include <vector>

#include "ltr/data/features_info.h"
#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/object_list.h"

namespace ltr {

template <typename TElement> class DataSet;

typedef DataSet<Object> PointwiseDataSet;
typedef DataSet<ObjectPair> PairwiseDataSet;
typedef DataSet<ObjectList> ListwiseDataSet;

template <typename TElement>
class DataSet {
    public:
    typedef boost::shared_ptr< DataSet > Ptr;

    DataSet(const FeatureInfo& featureInfo = FeatureInfo());
    virtual ~DataSet();

    const FeatureInfo& featureInfo() const;
    size_t featureCount() const;

    DataSet& operator<<(const TElement& element);
    void Add(const TElement& element);

    size_t size() const;
    void clear();
    void erase(size_t i);

    const TElement& operator[](size_t i) const;
    TElement& operator[](size_t i);
    const TElement& at(size_t i) const;
    TElement& at(size_t i);

    DataSet<TElement> deepCopy() const;

    friend bool operator==(const DataSet<TElement>& left,
            const DataSet<TElement>& right) {
        return true;
    }

    private:
    boost::shared_ptr< std::vector<TElement> > p_Elements_;
    FeatureInfo::Ptr featureInfo_;
};

template <typename TElement>
DataSet<TElement>::DataSet(const FeatureInfo& featureInfo)
    :featureInfo_(new FeatureInfo(featureInfo)),
     p_Elements_(new std::vector<TElement>()) {}

template< typename TElement >
DataSet< TElement >::~DataSet() {}

template< typename TElement >
const FeatureInfo& DataSet< TElement >::featureInfo() const {
    return *featureInfo_;
}

template <typename TElement>
size_t DataSet< TElement >::featureCount() const {
    return this->featureInfo().getFeatureCount();
}

template <typename TElement>
DataSet<TElement>& DataSet<TElement>::operator<<(const TElement& element) {
    (*p_Elements_).push_back(element.deepCopy());
    return *this;
}

template <typename TElement>
void DataSet<TElement>::Add(const TElement& element) {
    *this << element;
}

template <typename TElement>
size_t DataSet<TElement>::size() const {
    return (*p_Elements_).size();
}

template <typename TElement>
void DataSet<TElement>::clear() {
    (*p_Elements_).clear();
}

template <typename TElement>
void DataSet<TElement>::erase(size_t i) {
    (*p_Elements_).erase(i);
}

template <typename TElement>
const TElement& DataSet< TElement >::at(size_t i) const {
    return (*p_Elements_)[i];
}

template <typename TElement>
TElement& DataSet< TElement >::at(size_t i) {
    return (*p_Elements_)[i];
}

template <typename TElement>
const TElement& DataSet< TElement >::operator[](size_t i) const {
    return at(i);
}

template <typename TElement>
TElement& DataSet< TElement >::operator[](size_t i) {
    return at(i);
}

template< typename TElement >
DataSet<TElement> DataSet<TElement>::deepCopy() const {
    DataSet<TElement> result(this->featureInfo());
    for (size_t idx = 0; idx < this->size(); ++idx) {
        result << this->at(idx);
    }
    return result;
};
}
#endif  // LTR_DATA_DATA_SET_H_
