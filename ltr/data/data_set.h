// Copyright 2011 Yandex

#ifndef LTR_DATA_DATA_SET_H_
#define LTR_DATA_DATA_SET_H_

#include <vector>

#include "ltr/data/feature_info.h"
#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/object_list.h"
#include "ltr/data/utility/data_set_utility.h"

namespace ltr {

const double DEFAULT_ELEMENT_WEIGHT = 1.0;

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
  void add(const TElement& element);
  void add(const TElement& element, double weight);

  size_t size() const;
  void clear();
  void erase(size_t i);

  const TElement& operator[](size_t i) const;
  TElement& operator[](size_t i);
  const TElement& at(size_t i) const;
  TElement& at(size_t i);

  double getWeight(size_t element_idx) const;
  void setWeight(size_t element_idx, double weight) const;

  DataSet<TElement> deepCopy() const;

  template <typename TSubsetCreationStrategy>
  DataSet<TElement> subset(TSubsetCreationStrategy strategy) const;

  private:
  boost::shared_ptr< std::vector<TElement> > p_Elements_;
  FeatureInfo::Ptr featureInfo_;
  boost::shared_ptr< std::vector<double> > p_Weights_;
};

template <typename TElement>
DataSet<TElement>::DataSet(const FeatureInfo& featureInfo)
  :featureInfo_(new FeatureInfo(featureInfo)),
  p_Elements_(new std::vector<TElement>()),
  p_Weights_(new std::vector<double>()) {}

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
  this->add(element);
  return *this;
}

template <typename TElement>
void DataSet<TElement>::add(const TElement& element) {
  this->add(element, DEFAULT_ELEMENT_WEIGHT);
}

template <typename TElement>
void DataSet<TElement>::add(const TElement& element, double weight) {
  (*p_Elements_).push_back(element.deepCopy());
  (*p_Weights_).push_back(weight);
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
  (*p_Elements_).erase((*p_Elements_).begin() + i);
  (*p_Weights_).erase((*p_Weights_).begin() + i);
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

template <typename TElement>
double DataSet< TElement >::getWeight(size_t element_idx) const {
  return (*p_Weights_)[element_idx];
}

template <typename TElement>
void DataSet< TElement >::setWeight(size_t element_idx, double weight) const {
  (*p_Weights_)[element_idx] = weight;
}

template< typename TElement >
DataSet<TElement> DataSet<TElement>::deepCopy() const {
  DataSet<TElement> result(this->featureInfo());
  for (size_t idx = 0; idx < this->size(); ++idx) {
    result << this->at(idx);
  }
  return result;
}

template<typename TElement>
template<typename TSubsetCreationStrategy>
DataSet<TElement> DataSet<TElement>::subset(
    TSubsetCreationStrategy strategy) const {
  DataSet<TElement> resultDataSet(this->featureInfo());
  for (size_t elementIdx = 0; elementIdx < this->size(); ++elementIdx) {
    if (strategy.takes((*this)[elementIdx], elementIdx)) {
      resultDataSet.p_Elements_->push_back((*this)[elementIdx]);
      resultDataSet.p_Weights_->push_back((*this).getWeight(elementIdx));
    }
  }
  return resultDataSet;
};

template < >
template < >
DataSet<Object> DataSet<Object>::subset< std::vector<size_t> >(
    std::vector<size_t> indexes) const;

template < >
template < >
DataSet<ObjectPair> DataSet<ObjectPair>::subset< std::vector<size_t> >(
    std::vector<size_t> indexes) const;

template < >
template < >
DataSet<ObjectList> DataSet<ObjectList>::subset< std::vector<size_t> >(
    std::vector<size_t> indexes) const;

template< typename TElement >
bool operator==(const DataSet<TElement>& left,
                const DataSet<TElement>& right) {
  if (left.size() != right.size())
    return false;
  for (int i = 0; i < left.size(); ++i) {
    if (left[i].size() != right[i].size()) {
      return false;
    }
    for (int j = 0; j < left[i].size(); ++j) {
      if (left[i][j] != right[i][j]) {
        return false;
      }
    }
  }
  return true;
}
}
#endif  // LTR_DATA_DATA_SET_H_
