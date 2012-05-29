// Copyright 2011 Yandex

#ifndef LTR_DATA_DATA_SET_H_
#define LTR_DATA_DATA_SET_H_

#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

#include "ltr/data/object.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/object_list.h"

using std::vector;

namespace ltr {
/** The default weight for an element in data set.
 */
const double DEFAULT_ELEMENT_WEIGHT = 1.0;

template <typename TElement> class DataSet;

namespace utility {
template <typename TElement>
DataSet<TElement> lightSubset(const DataSet<TElement>& data,
    const std::vector<int>& indexes);
}

/** \typedef Type for a data set that implements pointwise approach.
 */
typedef DataSet<Object> PointwiseDataSet;
/** \typedef Type for a data set that implements pairwise approach.
 */
typedef DataSet<ObjectPair> PairwiseDataSet;
/** \typedef Type for a data set that implements listwise approach.
 */
typedef DataSet<ObjectList> ListwiseDataSet;

/** \class \brief Template class that implements DataSet.
 *  \tparam type of elements, which would be stored in the DataSet. The
 *  elements should implement Object container interface. Those could be
 *  Object, ObjectPair or ObjectList.
 */
template <typename TElement>
class DataSet : public Printable {
  public:
  /** Shared pointer type to the DataSet.
   */
  typedef boost::shared_ptr< DataSet > Ptr;

  /** The constructor creates a data set to store objects with given
   * FeatureInfo.
   */
  DataSet(const FeatureInfo& featureInfo = FeatureInfo());
  virtual ~DataSet();
  /** Returns FeatureInfo, objects with same FeatureInfo are allowed to be
   * stored in the DataSet.
   */
  const FeatureInfo& featureInfo() const;
  FeatureInfo::Ptr featureInfoPtr() const;
  /** Returns the number of features in objects of the DataSet.
   */
  size_t featureCount() const;
  /** Adds an element(Object, ObjectPair, ObjectList etc.) to the DataSet.
   */
  DataSet& operator<<(const TElement& element);
  /** Adds an element(Object, ObjectPair, ObjectList etc.) with
   * DEFAULT_ELEMENT_WEIGHT to the DataSet.
   */
  void add(const TElement& element);
  /** Adds an element(Object, ObjectPair, ObjectList etc.) with given weight
   * to the DataSet.
   */
  void add(const TElement& element, double weight);
  /** Returns the number of elements in the DataSet.
   */
  size_t size() const;
  /** Deletes all elements from the DataSet.
   */
  void clear();
  /** Deletes the element with given index from the DataSet.
   */
  void erase(size_t i);
  /** Returns a constant link to the ith element of the DataSet.
   */
  const TElement& operator[](size_t i) const;
  /** Returns a link to the ith element of the DataSet.
   */
  TElement& operator[](size_t i);
  /** Returns a constant link to the ith element of the DataSet.
   */
  const TElement& at(size_t i) const;
  /** Returns a link to the ith element of the DataSet.
   */
  TElement& at(size_t i);
  /** Returns the weight of ith element.
   */
  double getWeight(size_t element_idx) const;
  /** Sets the weight of ith element.
   * \param element_idx index of the element to set up weight.
   * \param new weight value.
   */
  void setWeight(size_t element_idx, double weight) const;
  /** Creates a new DataSet cantaining the copies of the elements in the
   * DataSet.
   */
  DataSet<TElement> deepCopy() const;
  /** The function creates new DataSet, which contains easy copies of elements
   * with given indexes (easy means that they share objects' resources: feature
   * vectors, objects' meat information).
   * \param data the DataSet from which the subset is build.
   * \param indexes the indexes of elements from data based on those the subset
   * is build.
   */
  friend DataSet<TElement> utility::lightSubset< >(
      const DataSet<TElement>& data,
      const std::vector<int>& indexes);

  /** 
   * Function for serialization data set into string.
   */
  string toString() const;

  private:
  /** Shared pointer to the actual vector, in which the elements are stored.
   */
  boost::shared_ptr< std::vector<TElement> > p_Elements_;
  /** The information about objects that are stored in the DataSet.
   */
  FeatureInfo::Ptr featureInfo_;
  /** Shared pointer to the vector of elements' weights.
     */
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

template< typename TElement >
FeatureInfo::Ptr DataSet<TElement>::featureInfoPtr() const {
  return featureInfo_;
}

template <typename TElement>
size_t DataSet<TElement>::featureCount() const {
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
  TElement element_to_add = element.deepCopy();
  if (featureInfo_ == NULL || featureInfo_->getFeatureCount() == 0) {
    featureInfo_ = FeatureInfo::Ptr(
      new FeatureInfo(element[0].feature_info()));
  }
  for (size_t i = 0; i < element_to_add.size(); i++) {
    if (element[i].feature_info() != featureInfo())
      throw std::logic_error("can't add objects with another FeatureInfo.");
    element_to_add[i].feature_info_ = featureInfo_;
  }
  (*p_Elements_).push_back(element_to_add);
  (*p_Weights_).push_back(weight);
}

template <typename TElement>
size_t DataSet<TElement>::size() const {
  return (*p_Elements_).size();
}

template <typename TElement>
void DataSet<TElement>::clear() {
  (*p_Elements_).clear();
  (*p_Weights_).clear();
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

template< typename TElement >
string DataSet<TElement>::toString() const {
  std::stringstream str;
  for (size_t i = 0; i < size(); i++)
    str << (*this)[i].toString() << std::endl;
  return str.str();
}

template< typename TElement >
bool operator==(const DataSet<TElement>& left,
                const DataSet<TElement>& right) {
  if (left.featureInfo() != right.featureInfo())
    return false;
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
