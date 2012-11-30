// Copyright 2012 Yandex

#ifndef LTR_UTILITY_MULTIARRAY_H_
#define LTR_UTILITY_MULTIARRAY_H_

#include <vector>

using std::vector;


namespace ltr {
namespace utility {

template<typename T, int DimensionsCount>
class MultiArray {
public:
  MultiArray(const vector<int>& dimensions = vector<int>(DimensionsCount, 0)):
    data_(*dimensions.begin(),
          MultiArray<T, DimensionsCount - 1>(dimensions.begin() + 1, dimensions.end())),
          dimensions_(dimensions.begin(), dimensions.end()) {
  }

  MultiArray(vector<int>::const_iterator dimensions_begin,
             vector<int>::const_iterator dimensions_end):
    data_(*dimensions_begin,
          MultiArray<T, DimensionsCount - 1>(dimensions_begin + 1, dimensions_end)),
          dimensions_(dimensions_begin, dimensions_end) {
  }

  T& operator () (const vector<int>& indices) {
    return get_element(indices.begin(), indices.end());
  }

  const T& operator () (const vector<int>& indices) const {
    return get_element(indices.begin(), indices.end());
  }

  T& get_element(vector<int>::const_iterator indices_begin,
                 vector<int>::const_iterator indices_end) {
    return data_[*indices_begin].get_element(indices_begin + 1, indices_end);
  }

  const T& get_element(vector<int>::const_iterator indices_begin,
                       vector<int>::const_iterator indices_end)  const {
    return data_[*indices_begin].get_element(indices_begin + 1, indices_end);
  }

  MultiArray<T, DimensionsCount - 1>& operator [] (int index) {
    return data_[index];
  }

  const MultiArray<T, DimensionsCount - 1>& operator [] (int index) const {
    return data_[index];
  }

  void resize(const vector<int>& dimensions) {
    resize(dimensions.begin(), dimensions.end());
  }

  void resize(vector<int>::const_iterator dimensions_begin,
              vector<int>::const_iterator dimensions_end) {
    dimensions_ = vector<int>(dimensions_begin, dimensions_end);
    data_.resize(*dimensions_begin);
    for (int i = 0; i < *dimensions_begin; ++i) {
      data_[i].resize(dimensions_begin + 1, dimensions_end);
    }
  }

  int dimensions_count() const {
    return DimensionsCount;
  }

  const vector<int>& shape() const {
    return dimensions_;
  }

private:
  vector<MultiArray<T, DimensionsCount - 1> > data_;
  vector<int> dimensions_;
};


template<typename T>
class MultiArray<T, 1> {
public:
  MultiArray(vector<int> dimensions = vector<int>(1, 0)):
    data_(1, *dimensions.begin()) {
  }

  MultiArray(vector<int>::const_iterator dimensions_begin,
             vector<int>::const_iterator dimensions_end):
    data_(1, *dimensions_begin) {
  }

  T& operator [] (int index) {
    return data_[index];
  }

  const T& operator [] (int index) const {
    return data_[index];
  }

  T& get_element(vector<int>::const_iterator indices_begin,
                 vector<int>::const_iterator indices_end) {
    return data_[*indices_begin];
  }

  const T& get_element(vector<int>::const_iterator indices_begin,
                       vector<int>::const_iterator indices_end)  const {
    return data_[*indices_begin];
  }

  void resize(const vector<int>& dims) {
    data_.resize(dims[0]);
  }

  void resize(vector<int>::const_iterator dimensions_begin,
              vector<int>::const_iterator dimensions_end) {
    data_.resize(*dimensions_begin);
  }

  int dimensions_count() const {
    return 1;
  }

  const vector<int>& shape() const {
    return vector<int>(1, data_.size());
  }
private:
  vector<T> data_;
};

}
}


#endif
