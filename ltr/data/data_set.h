// Copyright 2011 Yandex

#ifndef LTR_DATA_DATA_SET_H_
#define LTR_DATA_DATA_SET_H_

#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "ltr/data/object.h"
#include "ltr/data/object_list.h"
#include "ltr/data/object_pair.h"

using std::vector;

namespace ltr {
/** The default weight for an element in data set.
 */
const double DEFAULT_ELEMENT_WEIGHT = 1.0;

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
  typedef boost::shared_ptr<DataSet> Ptr;

  /** The constructor creates a data set to store objects with the given
   * FeatureInfo.
   */
  DataSet(const FeatureInfo& feature_info = FeatureInfo());
  /** Destructor
   */
  virtual ~DataSet();
  /** Returns FeatureInfo, objects with the same FeatureInfo are allowed to be
   * stored in the DataSet.
   */
  const FeatureInfo& feature_info() const;
  /** Simple feature info setter
   */
  void set_feature_info(const FeatureInfo &feature_info);
  /** Returns the number of features in objects of the DataSet.
   */
  size_t feature_count() const;
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
  void erase(size_t element_index);
  /** Returns a constant link to the ith element of the DataSet.
   */
  const TElement& operator[](size_t element_index) const;
  /** Returns a link to the ith element of the DataSet.
   */
  TElement& operator[](size_t element_index);
  /** Returns a constant link to the ith element of the DataSet.
   */
  const TElement& at(size_t element_index) const;
  /** Returns a link to the ith element of the DataSet.
   */
  TElement& at(size_t element_index);
  /** Returns the weight of ith element.
   */
  double getWeight(size_t element_index) const;
  /** Sets the weight of ith element.
   * \param element_idx index of the element to set up weight.
   * \param new weight value.
   */
  void setWeight(size_t element_index, double weight) const;
  /** Creates a new DataSet containing the copies of the elements in the
   * DataSet.
   */
  DataSet<TElement> deepCopy() const;
  /** Creates new DataSet, which contains easy copies of elements
   * with given indexes (easy means that they share objects' resources: feature
   * vectors, objects' meat information).
   * \param indices the indices of elements from data based on those the subset
   * is build.
   */
  DataSet<TElement> lightSubset(const vector<int>& indices) const;

  /** 
   * Function for serialization dataset into string.
   */
  virtual string toString() const;

 private:
  /** Shared pointer to the actual vector, in which the elements are stored.
   */
  boost::shared_ptr<vector<TElement> > elements_;
  /** The information about objects that are stored in the DataSet.
   */
  FeatureInfo::Ptr feature_info_;
  /** Shared pointer to the vector of elements' weights.
     */
  boost::shared_ptr<vector<double> > weights_;
};

/** \typedef Type for a data set that implements pointwise approach.
 */
typedef DataSet<Object> PointwiseDataSet;
/** \typedef Type for a data set that implements pairwise approach.
 */
typedef DataSet<ObjectPair> PairwiseDataSet;
/** \typedef Type for a data set that implements listwise approach.
 */
typedef DataSet<ObjectList> ListwiseDataSet;

template <typename TElement>
DataSet<TElement>::DataSet(const FeatureInfo& feature_info)
                   : feature_info_(new FeatureInfo(feature_info)),
                     elements_(new vector<TElement>()),
                     weights_(new vector<double>()) {}

template<typename TElement>
DataSet<TElement>::~DataSet() {}

template<typename TElement>
const FeatureInfo& DataSet<TElement>::feature_info() const {
  return *feature_info_;
}

template<typename TElement>
void DataSet<TElement>::set_feature_info(const FeatureInfo &feature_info) {
  feature_info_ = FeatureInfo::Ptr(new FeatureInfo(feature_info));
}

template <typename TElement>
size_t DataSet<TElement>::feature_count() const {
  return this->feature_info().feature_count();
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
  if (feature_info_ == NULL || feature_info_->feature_count() == 0) {
    feature_info_ = FeatureInfo::Ptr(
      new FeatureInfo(element[0].feature_info()));
  }
  for (int object_index = 0;
       object_index < (int)element_to_add.size();
       ++object_index) {
    if (element[object_index].feature_info() != feature_info()) {
      throw std::logic_error("can't add objects with another FeatureInfo.");
    }
    element_to_add[object_index].feature_info_ = feature_info_;
  }
  (*elements_).push_back(element_to_add);
  (*weights_).push_back(weight);
}

template <typename TElement>
size_t DataSet<TElement>::size() const {
  return elements_->size();
}

template <typename TElement>
void DataSet<TElement>::clear() {
  elements_->clear();
  weights_->clear();
}

template <typename TElement>
void DataSet<TElement>::erase(size_t element_index) {
  elements_->erase(elements_->begin() + element_index);
  weights_->erase(weights_->begin() + element_index);
}

template <typename TElement>
const TElement& DataSet<TElement>::at(size_t element_index) const {
  return (*elements_)[element_index];
}

template <typename TElement>
TElement& DataSet<TElement>::at(size_t element_index) {
  return (*elements_)[element_index];
}

template <typename TElement>
const TElement& DataSet<TElement>::operator[](size_t element_index) const {
  return at(element_index);
}

template <typename TElement>
TElement& DataSet<TElement>::operator[](size_t element_index) {
  return at(element_index);
}

template <typename TElement>
double DataSet<TElement>::getWeight(size_t element_index) const {
  return (*weights_)[element_index];
}

template <typename TElement>
void DataSet<TElement>::setWeight(size_t element_index, double weight) const {
  (*weights_)[element_index] = weight;
}

template<typename TElement>
DataSet<TElement> DataSet<TElement>::deepCopy() const {
  DataSet<TElement> result(this->feature_info());
  for (int element_index = 0;
       element_index < (int)this->size();
       ++element_index) {
    result << this->at(element_index);
  }
  return result;
}

template<typename TElement>
string DataSet<TElement>::toString() const {
  std::stringstream str;
  for (int element_index = 0; element_index < (int)size(); ++element_index) {
    str << (*this)[element_index].toString() << std::endl;
  }
  return str.str();
}

template<typename TElement>
bool operator==(const DataSet<TElement>& lhs,
                const DataSet<TElement>& rhs) {
  if (lhs.feature_info() != rhs.feature_info()) {
    return false;
  }
  if (lhs.size() != rhs.size()) {
    return false;
  }
  for (int element_index = 0;
       element_index < (int)lhs.size();
       ++element_index) {
    if (lhs[element_index].size() != rhs[element_index].size()) {
      return false;
    }
    for (int object_index = 0;
         object_index < lhs[element_index].size();
         ++object_index) {
      if (lhs[element_index][object_index] !=
          rhs[element_index][object_index]) {
        return false;
      }
    }
  }
  return true;
}

template <typename TElement>
DataSet<TElement>
DataSet<TElement>::lightSubset(const vector<int>& indices) const {
  DataSet<TElement> resultDataSet(feature_info());

  for (size_t index_index = 0; index_index < indices.size(); ++index_index) {
    if (indices[index_index] > size()) {
      throw std::logic_error("Index is too large");
    }
    resultDataSet.elements_->push_back(elements_->at(indices[index_index]));
    resultDataSet.weights_->push_back(getWeight(indices[index_index]));
  }
  return resultDataSet;
}
}
#endif  // LTR_DATA_DATA_SET_H_
