// Copyright 2012 Yandex

#ifndef LTR_DATA_DATA_SET_H_
#define LTR_DATA_DATA_SET_H_

#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "ltr/utility/macros.h"

#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/utility/numerical.h"

#include "ltr/data/object.h"
#include "ltr/data/object_list.h"
#include "ltr/data/object_pair.h"
#include "ltr/data/feature_info.h"
#include "ltr/data/per_object_accessor.h"
#include "ltr/interfaces/aliaser.h"



namespace ltr {
/** 
 * The default weight for an element in data set.
 */
const double DEFAULT_ELEMENT_WEIGHT = 1.0;

using std::vector;
using std::map;
using ltr::utility::shared_ptr;
using ltr::utility::DoubleEqual;

using Eigen::VectorXd;
using Eigen::MatrixXd;

/**
 * \brief Template class that implements DataSet.
 *
 * \tparam Type of elements, which would be stored in the DataSet. The
 * elements should implement Object container interface. Those could be
 * Object, ObjectPair or ObjectList.
 */
template <typename TElement>
class DataSet : public Printable, public Aliaser {
 public:
  /**
   * Shared pointer type to the DataSet.
   */
  typedef ltr::utility::shared_ptr<DataSet> Ptr;

  /**
   * \param feature_info Information about features.
   * \param label_info Information about labels.
   */
  DataSet(
      const FeatureInfo& feature_info = FeatureInfo(),
      const LabelInfo& label_info = LabelInfo());

  virtual ~DataSet();
  /**
   * Returns FeatureInfo, objects with the same FeatureInfo are allowed to be
   * stored in the DataSet.
   */
  const FeatureInfo& feature_info() const;
  /** 
   * Simple feature info setter
   */
  void set_feature_info(const FeatureInfo& feature_info);
  /**
   * Getter for label value type information
   */
  const LabelInfo& label_info() const;
  /**
   * Setter for label value type information
   */
  void set_label_info(const LabelInfo& label_info);
  /**
   * Returns the number of features in objects of the DataSet.
   */
  int feature_count() const;
  /**
   * Adds an element(Object, ObjectPair, ObjectList etc.) to the DataSet.
   */
  DataSet<TElement>& operator<<(const TElement& element);
  /**
   * Adds an element(Object, ObjectPair, ObjectList etc.) with
   * DEFAULT_ELEMENT_WEIGHT to the DataSet.
   */
  void add(const TElement& element);
  /**
   * Adds an element(Object, ObjectPair, ObjectList etc.) with given weight
   * to the DataSet.
   */
  void add(const TElement& element, double weight);
  /**
   * Returns the number of elements in the DataSet.
   */
  int size() const;
  /**
   * Deletes all elements from the DataSet.
   */
  void clear();
  /**
   * Deletes the element with given index from the DataSet.
   */
  void erase(int element_index);
  /**
   * Returns a constant link to the ith element of the DataSet.
   */
  const TElement& operator[](int element_index) const;
  /**
   * Returns a link to the ith element of the DataSet.
   */
  TElement& operator[](int element_index);
  /**
   * Returns a constant link to the ith element of the DataSet.
   */
  const TElement& at(int element_index) const;
  /**
   * Returns a link to the ith element of the DataSet.
   */
  TElement& at(int element_index);
  /**
   * Returns the weight of ith element.
   */
  double getWeight(int element_index) const;
  /**
   * Sets the weight of ith element.
   *
   * \param element_index is an index of the element to set up weight.
   * \param weight is a new weight value.
   */
  void setWeight(int element_index, double weight) const;
  /**
   * Creates a new DataSet containing the copies of the elements in the
   * DataSet.
   */
  DataSet<TElement> deepCopy() const;
  /**
   * Creates new DataSet, which contains easy copies of elements
   * with given indices (easy means that they share objects' resources: feature
   * vectors, objects' meat information).
   *
   * \param indices the indices of elements from data based on those the subset
   * is build.
   */
  DataSet<TElement> lightSubset(const vector<int>& indices) const;
  /** 
   * Function for serialization dataset into string.
   */
  virtual string toString() const;
 private:
  /**
   * Information about label value type
   */
  ltr::utility::shared_ptr<LabelInfo> label_info_;
  /**
   * Shared pointer to the vector of elements' weights.
   */
  mutable vector<double> weights_;
  /** Each element has its objects in the matrix ob lines, defined by this bounds
   */
  vector<ElementBounds> element_bounds_;
  /** Representation for objects storage
   */
  shared_ptr<InnerRepresentation> inner_representation_;
  shared_ptr<FeatureInfo> feature_info_;
  /** Vector of elements of the data set
   */
  vector< shared_ptr<TElement> > element_vector_;
  /**
   * Returns default alias of class
   */
  virtual string getDefaultAlias() const {
    return "DataSet";
  }
};
/**
 * Type for a data set that implements pointwise approach.
 */
typedef DataSet<Object> PointwiseDataSet;
/**
 * Type for a data set that implements pairwise approach.
 */
typedef DataSet<ObjectPair> PairwiseDataSet;
/**
 * Type for a data set that implements listwise approach.
 */
typedef DataSet<ObjectList> ListwiseDataSet;

template <typename TElement>
DataSet<TElement>::DataSet(const FeatureInfo& feature_info,
                           const LabelInfo& label_info) {
  feature_info_ = new FeatureInfo(feature_info);
  label_info_ = new LabelInfo(label_info);
  inner_representation_ = new InnerRepresentation(0,
                                              feature_info_->feature_count());
}

template <typename TElement>
inline DataSet<TElement>::~DataSet() { }

template <typename TElement>
inline const FeatureInfo& DataSet<TElement>::feature_info() const {
  return *feature_info_;
}

template <typename TElement>
inline void DataSet<TElement>::set_feature_info(const FeatureInfo& info) {
  feature_info_ = FeatureInfo::Ptr(new FeatureInfo(info));
  inner_representation_->set_row_count(info.feature_count());
}

template <typename TElement>
int DataSet<TElement>::feature_count() const {
  return feature_info_->feature_count();
}

template<typename TElement>
const LabelInfo& DataSet<TElement>::label_info() const {
  return *label_info_;
}

template<typename TElement>
void DataSet<TElement>::set_label_info(
    const LabelInfo &label_info) {
  label_info_ = ltr::utility::shared_ptr<LabelInfo>(
      new LabelInfo(label_info));
}

template <typename TElement>
DataSet<TElement>& DataSet<TElement>::operator<<(const TElement& element) {
  this->add(element);
  return *this;
}

template <typename TElement>
inline void DataSet<TElement>::add(const TElement& element) {
  this->add(element, DEFAULT_ELEMENT_WEIGHT);
}

template <typename TElement>
inline void DataSet<TElement>::add(const TElement& element, double weight) {
  PerObjectAccessor<const TElement> accessor(&element);
  if ((bool)feature_info_ || feature_info_->feature_count() == 0) {
    feature_info_ = FeatureInfo::Ptr(
      new FeatureInfo(accessor.object(0).feature_count()));
    inner_representation_->set_row_count(feature_info_->feature_count());
  }

  int start_index = inner_representation_->column_count();

  inner_representation_->add_several_objects(accessor.object_count());

  for (int object_index = 0;
       object_index < accessor.object_count();
       ++object_index) {
    Object to_push = accessor.object(object_index);

    CHECK(to_push.feature_count() == inner_representation_->row_count())

    int presentation_index = start_index + object_index;

    inner_representation_->set_actual_label(presentation_index,
                                          to_push.actual_label());
    inner_representation_->set_predicted_label(presentation_index,
                                             to_push.predicted_label());
    inner_representation_->set_meta_info(presentation_index,
                                       to_push.presentation_->meta_info());
    inner_representation_->set_features_column(presentation_index,
                                          to_push.eigen_features());
  }

  element_bounds_.push_back(ElementBounds(start_index, start_index +
                                          accessor.object_count()));
  weights_.push_back(weight);

  element_vector_.push_back(
    new TElement(inner_representation_,
                 element_bounds_[element_bounds_.size() - 1]));
}

template <typename TElement>
void DataSet<TElement>::erase(int element_index) {
  // Warning! Currently does nothing!
}

template <typename TElement>
inline int DataSet<TElement>::size() const {
  return element_vector_.size();
}

template <typename TElement>
inline void DataSet<TElement>::clear() {
  element_vector_.clear();
  element_bounds_.clear();
  weights_.clear();
  inner_representation_ = new InnerRepresentation(0,
                                              feature_info().feature_count());
}

template <typename TElement>
inline const TElement& DataSet<TElement>::operator[](int element_index) const {
  return this->at(element_index);
}

template <typename TElement>
inline TElement& DataSet<TElement>::operator[](int element_index) {
  return this->at(element_index);
}

template <typename TElement>
inline const TElement& DataSet<TElement>::at(int element_index) const {
  return *(element_vector_[element_index]);
}

template <typename TElement>
inline TElement& DataSet<TElement>::at(int element_index) {
  return  *(element_vector_[element_index]);
}

template <typename TElement>
inline double DataSet<TElement>::getWeight(int element_index) const {
  return weights_[element_index];
}

template <typename TElement>
inline void DataSet<TElement>::setWeight(int element_index,
                                         double weight) const {
  weights_[element_index] = weight;
}

template <typename TElement>
inline DataSet<TElement> DataSet<TElement>::deepCopy() const {
  DataSet<TElement> result(this->feature_info());
  for (int element_index = 0;
       element_index < (int)this->size();
       ++element_index) {
    result.add(this->at(element_index), this->getWeight(element_index));
  }
  return result;
}

template <typename TElement>
DataSet<TElement>
DataSet<TElement>::lightSubset(const vector<int>& indices) const {
  DataSet result(feature_info());
  result.inner_representation_ = this->inner_representation_;

  for (int indices_index = 0; indices_index < (int)indices.size(); ++indices_index) {  // NOLINT
    CHECK(indices[indices_index] < size() && indices[indices_index] >= 0);
    result.element_bounds_.push_back(element_bounds_[indices[indices_index]]);
    result.element_vector_.push_back(
      new TElement(result.inner_representation_,
                   result.element_bounds_[indices_index]));
    result.weights_.push_back(weights_[indices[indices_index]]);
  }
  return result;
}

template <typename TElement>
inline string DataSet<TElement>::toString() const {
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
    PerObjectAccessor<const TElement> per_object_accessor1(&lhs[element_index]);
    PerObjectAccessor<const TElement> per_object_accessor2(&rhs[element_index]);
    if (per_object_accessor1.object_count() !=
        per_object_accessor2.object_count()) {
      return false;
    }
    for (int object_index = 0;
         object_index < per_object_accessor1.object_count();
         ++object_index) {
      if (per_object_accessor1.object(object_index) !=
          per_object_accessor2.object(object_index)) {
        return false;
      }
    }
  }
  return true;
}
};
#endif  // LTR_DATA_DATA_SET_H_
