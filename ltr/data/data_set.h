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
/**
 * The end element index of an iterator
 */
const int END_OBJECT_LINE_INDEX = -1;

using std::vector;
using std::map;
using ltr::utility::shared_ptr;
using ltr::utility::DoubleEqual;

using Eigen::VectorXd;
using Eigen::MatrixXd;

template <typename TElement>
class DataSetObjectIterator;

template<typename TElement>
class DataSetElementIterator;

template<typename TElement>
class ConstDataSetObjectIterator;

template<typename TElement>
class ConstDataSetElementIterator;

/**
 * \brief Template class that implements DataSet.
 *
 * \tparam Type of elements, which would be stored in the DataSet. The
 * elements should implement Object container interface. Those could be
 * Object, ObjectPair or ObjectList.
 */
template<typename TElement>
class DataSet : public Printable, public Aliaser {
 public:
  /**
   * Shared pointer type to the DataSet.
   */
  typedef ltr::utility::shared_ptr<DataSet> Ptr;
  /** Iterator over all objects in DataSet
   */
  typedef DataSetObjectIterator<TElement> object_iterator;
  typedef ConstDataSetObjectIterator<TElement> const_object_iterator;
  /** Iterator over elements of DataSet
   */
  typedef DataSetElementIterator<TElement> element_iterator;
  typedef ConstDataSetElementIterator<TElement> const_element_iterator;

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
  /**
   * Start of non-constant iterator over objects in data set.
   */
  object_iterator object_begin();
  /**
   * End of non-constant iterator over objects in data set.
   */
  object_iterator object_end();
  /**
   * Start of constant iterator over objects in data set.
   */
  const_object_iterator object_begin() const;
  /**
   * End of constant iterator over objects in data set.
   */
  const_object_iterator object_end() const;
  /**
   * Start of non-constant iterator over elements in data set.
   */
  element_iterator element_begin();
  /**
   * End of non-constant iterator over elements in data set.
   */
  element_iterator element_end();
  /**
   * Start of constant iterator over elements in data set.
   */
  const_element_iterator element_begin() const;
  /**
   * End of constant iterator over elements in data set.
   */
  const_element_iterator element_end() const;
  /**
   * Const access to whole feature matrix from inner representation
   */
  const MatrixXd& get_features_matrix() const;
 private:
  /**
   * Information about label value type.
   */
  ltr::utility::shared_ptr<LabelInfo> label_info_;
  /**
   * Shared pointer to the vector of elements' weights.
   */
  mutable vector<double> weights_;
  /**
   * Each element has its objects in the matrix ob lines, defined by this bounds.
   */
  vector<ElementBounds> element_bounds_;
  /**
   * Representation for objects storage.
   */
  shared_ptr<InnerRepresentation> inner_representation_;
  shared_ptr<FeatureInfo> feature_info_;
  /**
   * Vector of elements of the data set.
   */
  vector< shared_ptr<TElement> > element_vector_;
  /**
   * Returns default alias of class.
   */
  virtual string getDefaultAlias() const {
    return "DataSet";
  }

  friend class DataSetObjectIterator<TElement>;
  friend class ConstDataSetObjectIterator<TElement>;
  friend class DataSetElementIterator<TElement>;
  friend class ConstDataSetElementIterator<TElement>;
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

/**
 * \brief Iterator over objects in data set.
 */
template<typename TElement>
class DataSetObjectIterator {
 public:
  /**
   * Copy constructor
   */
  DataSetObjectIterator(const DataSetObjectIterator& other):
      data_set_(other.data_set_),
      element_index_(other.element_index_),
      current_object_(other.current_object_),
      object_line_index_(other.object_line_index_) { }

  /**
   * Constructor that creates iterator on certain position.
   *
   * \param data_set Data set over which we iterate.
   * \param element_number Number of starting element
   * (Object, ObjectPair, ObjectList).
   * \param current_object_line_index Index of starting object in data set. 
   */
  DataSetObjectIterator(DataSet<TElement>* data_set,
                        int element_number,
                        int current_object_line_index);

  /**
   * Iterator equality comparer.
   */
  bool operator==(const DataSetObjectIterator<TElement>& rhs) {
    return this->data_set_ == rhs.data_set_ &&
    this->object_line_index_ == rhs.object_line_index_ &&
    this->element_index_ == rhs.element_index_;
  }

  bool operator!=(const DataSetObjectIterator& rhs) {
    return !(this->operator==(rhs));
  }

  /**
   * Gets current object. Throws std::logic_error if iterator is at its end.
   */
  Object& operator*();
  /**
   * Increments iterator. Returns reference to itself
   */
  DataSetObjectIterator& operator++();

 private:
  /**
   * Generates object from given presentation and object line in it.
   * \param ptr Pointer to data set representation.
   * \param current_line Index of object to generate.
   */
  Object* generate_object(InnerRepresentation::Ptr ptr, int current_line);

  DataSet<TElement>* data_set_;
  int element_index_;
  int object_line_index_;

  shared_ptr<Object> current_object_;
};

/**
 * \brief Constant iterator over objects in data set.
 */
template<typename TElement>
class ConstDataSetObjectIterator {
 public:
  /**
   * Copy constructor
   */
  ConstDataSetObjectIterator(const ConstDataSetObjectIterator& other):
    data_set_(other.data_set_),
    element_index_(other.element_index_),
    current_object_(other.current_object_),
    object_line_index_(other.object_line_index_) { }

  /**
   * Constructor that creates iterator on certain position.
   *
   * \param data_set Data set over which we iterate.
   * \param element_number Number of starting element
   * (Object, ObjectPair, ObjectList).
   * \param current_object_line_index Index of starting object in data set. 
   */
  ConstDataSetObjectIterator(const DataSet<TElement>* data_set,
                       int element_number, int current_object_line_index);

  /**
   * Iterator equality comparer.
   */
  bool operator==(const ConstDataSetObjectIterator<TElement>& rhs) {
    return this->data_set_ == rhs.data_set_ &&
    this->object_line_index_ == rhs.object_line_index_ &&
    this->element_index_ == rhs.element_index_;
  }

  bool operator!=(const ConstDataSetObjectIterator& rhs) {
    return !(this->operator==(rhs));
  }

  /**
   * Gets current object. Throws std::logic_error if iterator is at its end.
   */
  const Object& operator*();

  /**
   * Increments iterator. Returns reference to itself
   */
  ConstDataSetObjectIterator& operator++();

 private:
  /**
   * Generates object from given presentation and object line in it.
   * \param ptr Pointer to data set representation.
   * \param current_line Index of object to generate.
   */
  Object* generate_object(InnerRepresentation::Ptr ptr, int current_line);

  const DataSet<TElement>* data_set_;
  int element_index_;
  int object_line_index_;

  shared_ptr<Object> current_object_;
};


/**
 * \brief Iterator over elements in data set.
 */
template<typename TElement>
class DataSetElementIterator {
 public:
  /**
   * Copy constructor
   */
  DataSetElementIterator(const DataSetElementIterator& other):
      data_set_(other.data_set_),
      element_index_(other.element_index_)
      { }

  /**
   * Constructor that creates iterator on certain position.
   *
   * \param data_set Data set over which we iterate.
   * \param element_number Number of starting element
   * (Object, ObjectPair, ObjectList).
   */
  DataSetElementIterator(DataSet<TElement>* data_set,
                         int element_number);

  /**
   * Iterator equality comparer.
   */
  bool operator==(const DataSetElementIterator<TElement>& rhs) {
    return this->data_set_ == rhs.data_set_ &&
    this->element_index_ == rhs.element_index_;
  }

  bool operator!=(const DataSetElementIterator& rhs) {
    return !(this->operator==(rhs));
  }

  /**
   * Gets current element (Object, ObjectPair, ObjectList).
   * Throws std::logic_error if iterator is at its end.
   */
  TElement& operator*();

  /**
   * Increments iterator. Returns reference to itself
   */
  DataSetElementIterator& operator++();

 private:
  DataSet<TElement>* data_set_;
  int element_index_;
};

/**
 * \brief Constant iterator over elements in data set.
 */
template<typename TElement>
class ConstDataSetElementIterator {
 public:
  /**
   * Copy constructor
   */
  ConstDataSetElementIterator(const ConstDataSetElementIterator& other):
      data_set_(other.data_set_),
      element_index_(other.element_index_)
      { }

  /**
   * Constructor that creates iterator on certain position.
   *
   * \param data_set Data set over which we iterate.
   * \param element_number Number of starting element
   * (Object, ObjectPair, ObjectList).
   */
  ConstDataSetElementIterator(const DataSet<TElement>* data_set,
                              int element_number);

  /**
   * Iterator equality comparer.
   */
  bool operator==(const ConstDataSetElementIterator<TElement>& rhs) {
    return this->data_set_ == rhs.data_set_ &&
    this->element_index_ == rhs.element_index_;
  }

  bool operator!=(const ConstDataSetElementIterator& rhs) {
    return !(this->operator==(rhs));
  }

  /**
   * Gets current object. Throws std::logic_error if iterator is at its end.
   */
  const TElement& operator*();

  /**
   * Increments iterator. Returns reference to itself
   */
  ConstDataSetElementIterator& operator++();

 private:
  const DataSet<TElement>* data_set_;
  int element_index_;
};

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

  for (int indices_index = 0;
       indices_index < (int)indices.size();
       ++indices_index) {
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

template <typename TElement>
typename DataSet<TElement>::object_iterator DataSet<TElement>::object_begin() {
  if (size() == 0) {
    return object_iterator(this, 0, END_OBJECT_LINE_INDEX);
  } else {
    return object_iterator(this, 0, element_bounds_[0].start_line_index);
  }
}

template <typename TElement>
typename DataSet<TElement>::object_iterator DataSet<TElement>::object_end() {
  return object_iterator(this, size(), END_OBJECT_LINE_INDEX);
}

template <typename TElement>
inline typename DataSet<TElement>::const_object_iterator
  DataSet<TElement>::object_begin() const {
  if (size() == 0) {
    return const_object_iterator(this, 0, END_OBJECT_LINE_INDEX);
  } else {
    return const_object_iterator(this, 0,
                                 element_bounds_[0].start_line_index);
  }
}

template <typename TElement>
inline typename DataSet<TElement>::const_object_iterator
  DataSet<TElement>::object_end() const {
  return const_object_iterator(this, size(), END_OBJECT_LINE_INDEX);
}

template <typename TElement>
typename DataSet<TElement>::element_iterator
  DataSet<TElement>::element_begin() {
    return element_iterator(this, 0);
}

template <typename TElement>
typename DataSet<TElement>::element_iterator
  DataSet<TElement>::element_end() {
    return element_iterator(this, size());
}

template <typename TElement>
typename DataSet<TElement>::const_element_iterator
  DataSet<TElement>::element_begin() const {
    return const_element_iterator(this, 0);
}

template <typename TElement>
typename DataSet<TElement>::const_element_iterator
  DataSet<TElement>::element_end() const {
    return const_element_iterator(this, size());
}

template <typename TElement>
DataSetObjectIterator<TElement>::DataSetObjectIterator(
    DataSet<TElement>* data_set,
    int element_number,
    int current_object_line_index): data_set_(data_set),
                              element_index_(element_number),
                              object_line_index_(current_object_line_index) {
    current_object_ = generate_object(data_set_->inner_representation_,
                                      object_line_index_);
}

template <typename TElement>
ConstDataSetObjectIterator<TElement>::ConstDataSetObjectIterator(
    const DataSet<TElement>* data_set,
    int element_number,
    int current_object_line_index): data_set_(data_set),
                              element_index_(element_number),
                              object_line_index_(current_object_line_index) {
    current_object_ = generate_object(data_set_->inner_representation_,
                                      object_line_index_);
}

template <typename TElement>
DataSetElementIterator<TElement>::DataSetElementIterator(
    DataSet<TElement>* data_set,
    int element_number): data_set_(data_set),
                         element_index_(element_number) { }

template <typename TElement>
ConstDataSetElementIterator<TElement>::ConstDataSetElementIterator(
    const DataSet<TElement>* data_set,
    int element_number): data_set_(data_set),
                         element_index_(element_number) { }

template <typename TElement>
Object& DataSetObjectIterator<TElement>::operator*() {
  if (current_object_.get() == NULL)
    throw std::logic_error("Iterator is out of range");
  return *current_object_;
}

template <typename TElement>
const Object& ConstDataSetObjectIterator<TElement>::operator*() {
  if (current_object_.get() == NULL)
    throw std::logic_error("Iterator is out of range");
  return *current_object_;
}

template <typename TElement>
TElement& DataSetElementIterator<TElement>::operator*() {
  return (*data_set_)[element_index_];
}

template <typename TElement>
const TElement& ConstDataSetElementIterator<TElement>::operator*() {
  return (*data_set_)[element_index_];
}

template <typename TElement>
DataSetObjectIterator<TElement>&
DataSetObjectIterator<TElement>::operator++() {
  ++object_line_index_;
  int last_index = data_set_->element_bounds_[element_index_].end_line_index;
  if (object_line_index_ == last_index) {
    ++element_index_;
    if (element_index_ == data_set_->size()) {
      object_line_index_ = END_OBJECT_LINE_INDEX;
    } else {
      object_line_index_ =
        data_set_->element_bounds_[element_index_].start_line_index;
    }
  }
  current_object_ = generate_object(data_set_->inner_representation_,
                                    object_line_index_);
  return *this;
}

template <typename TElement>
ConstDataSetObjectIterator<TElement>&
ConstDataSetObjectIterator<TElement>::operator++() {
  ++object_line_index_;
  int last_index = data_set_->element_bounds_[element_index_].end_line_index;
  if (object_line_index_ == last_index) {
    ++element_index_;
    if (element_index_ == data_set_->size()) {
      object_line_index_ = END_OBJECT_LINE_INDEX;
    } else {
      object_line_index_ =
        data_set_->element_bounds_[element_index_].start_line_index;
    }
  }
  current_object_ = generate_object(data_set_->inner_representation_,
                                    object_line_index_);
  return *this;
}

template <typename TElement>
DataSetElementIterator<TElement>&
DataSetElementIterator<TElement>::operator++() {
  ++element_index_;
  return *this;
}

template <typename TElement>
ConstDataSetElementIterator<TElement>&
ConstDataSetElementIterator<TElement>::operator++() {
  ++element_index_;
  return *this;
}

template<typename TElement>
Object* DataSetObjectIterator<TElement>::generate_object(
  InnerRepresentation::Ptr ptr,
  int current_line) {
    if (current_line == END_OBJECT_LINE_INDEX) {
      return NULL;
    } else {
      return new Object(ptr, current_line);
    }
}

template<typename TElement>
Object* ConstDataSetObjectIterator<TElement>::generate_object(
  InnerRepresentation::Ptr ptr,
  int current_line) {
    if (current_line == END_OBJECT_LINE_INDEX) {
      return NULL;
    } else {
      return new Object(ptr, current_line);
    }
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

template<typename TElement>
const MatrixXd& DataSet<TElement>::get_features_matrix() const {
  return inner_representation_->get_features_matrix();
}
};
#endif  // LTR_DATA_DATA_SET_H_
