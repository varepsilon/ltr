// Copyright 2012 Yandex

#ifndef LTR_UTILITY_MULTITABLE_H_
#define LTR_UTILITY_MULTITABLE_H_

#define _ITERATOR_DEBUG_LEVEL 0  // for VS 2010

#include <algorithm>
#include <string>
#include <vector>
#include <exception>
#include <iostream>
#include "boost/multi_array.hpp"
#include "ltr/interfaces/printable.h"

using std::sort;
using std::logic_error;
using std::string;
using std::stringstream;
using std::vector;
using std::cout;
using ltr::Printable;

namespace ltr {
namespace utility {
/**
 * This struct contains information about size of the dimension
 * of the MultiTable.
 */
struct DimensionSize {
  int index;
  size_t size;
};

/**
 * The less operator returns true if the left operand's field "size"
 * is less than right operand's field "size"
 * @param left - left operand
 * @param right - right operand
 * @returns boolean left.size < right.size
 */

bool operator<(const DimensionSize& left, const DimensionSize& right) {
  return left.size < right.size;
}

/**
 * Class intended to store the multidimensional tables.
 */
template <typename T, size_t N>
class MultiTable : public Printable {
 public:
  /**
   * Iterator to iterate over the elements
   */
  class Iterator;
  /**
   * Basic constructor
   */
  MultiTable();
  /**
   * Constructor with specified table_size
   * (by default it is zero-vector)
   * @param table_size - vector<size_t> multisize specification
   */
  explicit MultiTable(const vector<size_t>& table_size);
  /**
   * Operator [] to get the access to the element
   * by multiindex as by ordinary index.
   * @param multiIndex - vector<size_t> specifies the position
   * of the element
   * @returns T& - the constant datum reference of the stored type.
   */
  const T& operator[] (const vector<size_t>& multiIndex) const;
  /**
   * Non-constant implementation of the operator [].
   * @param multiIndex - vector<size_t> specifies the position
   * of the element
   * @returns T - the datum reference of the stored type.
   */
  T& operator[] (const vector<size_t>& multiIndex);
  /**
   * Sets axis label.
   * @param axis_index - size_t number of the axis to set label.
   * @param label - string label to set
   */
  void setAxisLabel(const size_t axis_index, const string& label);
  /**
   * Sets tick label.
   * @param axis_index - size_t index of the axis to modify
   * @param tick_index - size_t index of the tick to set
   * @param label - string label to set
   */
  void setTickLabel(const size_t axis_index,
                    const size_t tick_index,
                    const string& label);
  virtual string toString() const;
  /**
   * Resizes the multitable.
   * @param multi_size - new size of the multitable
   */
  void resize(const vector<size_t>& multi_size);
  /**
   * Clears multitable
   */
  void clear();
  /**
   * @returns iterator to the beginning of the multitable
   */
  Iterator begin();
  /**
   * @returns iterator to the end of the multitable
   */
  Iterator end();
 private:
  struct DimensionMetaInfo;
  boost::multi_array<T, N>* table_contents_;
  vector<DimensionMetaInfo> table_meta_info_;
  void printMultiTableInner(const vector<DimensionSize>& multi_size,
                            vector<size_t>* multi_index,
                            const int depth,
                            stringstream* const outstream) const;
  void print2DTable(const size_t first_axis_index,
                    const size_t second_axis_index,
                    vector<size_t>* const multi_index,
                    stringstream* const out_stream) const;
};

/**
 * Iteartor for a MultiTable class.
 */
template<typename T, size_t N>
class MultiTable<T, N>::Iterator {
 public:
  /**
   * Basic copy constructor
   * @param _instance - instance to copy
   */
  Iterator(MultiTable<T, N>* _instance);
  /**
   * Constant operator *
   * @returns value of type T (resolved iterator)
   * T is stored type
   */
  const T& operator*() const;
  /**
   * Operator *
   * @returns value of type T (resolved iterator)
   * T is stored type
   */
  T& operator*();
  /**
   * operator ->
   * @returns the pointer to the stored type
   */
  T* operator->();
  /**
   * movement operator
   * @returns Iterator moved to the next position
   */
  Iterator operator++();
  /**
   * @returns a multiindex from Iterator instance
   */
  const vector<size_t>& getMultiIndex() const;
  /**
   * @returns true if the iterators are at the same position
   */
  bool operator==(const Iterator& right);
  /**
   * @returns false if the iterators are at the same position
   */
  bool operator!=(const Iterator& right);
 private:
  vector<size_t> multi_index_;
  MultiTable<T, N>* instance;

  friend class MultiTable;
};

template <typename T, size_t N>
struct MultiTable<T, N>::DimensionMetaInfo {
  string axis_label;
  vector<string> axis_ticks_labels;
};


template <typename T, size_t N>
void MultiTable<T, N>::resize(const vector<size_t>& multi_size) {
  delete table_contents_;
  table_contents_ = new boost::multi_array<T, N>(multi_size);
  table_meta_info_.resize(multi_size.size());
  for (size_t index = 0; index < multi_size.size(); ++index) {
    table_meta_info_[index].axis_ticks_labels.resize(multi_size[index]);
  }
}

template <typename T, size_t N>
void MultiTable<T, N>::clear() {
  vector<size_t> empty;
  for (size_t i = 0; i < N; ++i) {
    empty.push_back(0);
  }
  this->resize(empty);
}

template<typename T, size_t N>
void MultiTable<T, N>::print2DTable(const size_t first_axis_index,
                                    const size_t second_axis_index,
                                    vector<size_t>* const multi_index,
                                    stringstream* const out_stream) const {
  size_t first_size = table_contents_->shape()[first_axis_index];
  size_t second_size = table_contents_->shape()[second_axis_index];

  *out_stream << "\n";
  for (int axis_index = 0;
       axis_index < (int)multi_index->size();
       ++axis_index) {
    size_t tick_index = (*multi_index)[axis_index];
    if (axis_index != first_axis_index && axis_index != second_axis_index) {
      *out_stream << table_meta_info_[axis_index].axis_label << ":"
                  << table_meta_info_[axis_index].
                     axis_ticks_labels[tick_index]
                  << "\t";
    }
  }
  *out_stream << "\n";
  *out_stream << "Table ROW: "
       << table_meta_info_[first_axis_index].axis_label
       << " COLUMN: "
       << table_meta_info_[second_axis_index].axis_label
       << "\n";
  *out_stream << "\t";
  for (int second_index = 0; second_index < (int)second_size; ++second_index) {
    *out_stream
        << table_meta_info_
           [second_axis_index].axis_ticks_labels[second_index]
         << "\t";
  }
  *out_stream << "\n";
  for (size_t first_index = 0; first_index < first_size; ++first_index) {
    (*multi_index)[first_axis_index] = first_index;
    *out_stream << table_meta_info_
                   [first_axis_index].axis_ticks_labels[first_index]
                << "\t";
    for (size_t second_index = 0; second_index < second_size; ++second_index) {
      (*multi_index)[second_axis_index] = second_index;
      *out_stream << table_contents_->operator()(*multi_index) << "\t";
    }
    *out_stream << "\n";
  }
}

template <typename T, size_t N>
void MultiTable<T, N>::printMultiTableInner(
    const vector<DimensionSize>& axis_print_order,
    vector<size_t>* multi_index,
    const int depth,
    stringstream* const out_stream) const {
  if (depth + 2 == axis_print_order.size()) {
    print2DTable(axis_print_order[depth].index,
                 axis_print_order[depth + 1].index,
                 multi_index,
                 out_stream);
  } else {
    int index_number_to_increase = axis_print_order[depth].index;
    int ticks_count = axis_print_order[depth].size;
    for (int tick_index = 0; tick_index < ticks_count; ++tick_index) {
      (*multi_index)[index_number_to_increase] = tick_index;
      printMultiTableInner(axis_print_order,
                           multi_index, depth + 1, out_stream);
    }
  }
}

template<typename T, size_t N>
MultiTable<T, N>::MultiTable(const vector<size_t>& table_size)
  : table_contents_(new boost::multi_array<T, N>())
  , table_meta_info_() {
  if (table_size.size() != N) {
    throw logic_error("Bad number of input sizes in multitable constructor\n");
  }
  delete table_contents_;
  table_contents_ = new boost::multi_array<T, N>(table_size);
  table_meta_info_.resize(table_size.size());
  for (size_t index = 0; index < table_size.size(); ++index) {
    table_meta_info_[index].axis_ticks_labels.resize(table_size[index]);
  }
}

template<typename T, size_t N>
MultiTable<T, N>::MultiTable()
  : table_contents_(new boost::multi_array<T, N>())
  , table_meta_info_() {}

template<typename T, size_t N>
const T& MultiTable<T, N>::operator[] (
  const vector<size_t>& multi_index) const {
  if (multi_index.size() != N) {
    throw logic_error("Bad number of input sizes in multitable [] operator\n");
  }
  return (*table_contents_)(multi_index);
}

template<typename T, size_t N>
T& MultiTable<T, N>::operator[] (const vector<size_t>& multi_index) {
  if (multi_index.size() != N) {
    throw logic_error("Bad number of input sizes in multitable [] operator\n");
  }
  return (*table_contents_)(multi_index);
}

template<typename T, size_t N>
void MultiTable<T, N>::setAxisLabel(const size_t dim_index,
                                    const string& label) {
  table_meta_info_[dim_index].axis_label = label;
}

template<typename T, size_t N>
void MultiTable<T, N>::setTickLabel(
    const size_t dim_index, const size_t axis_index, const string& label) {
  table_meta_info_[dim_index].axis_ticks_labels[axis_index] = label;
}

template<typename T, size_t N>
string MultiTable<T, N>::toString() const {
  const int dimension_count = table_contents_->num_dimensions();
  vector<DimensionSize> multi_size;
  const size_t* dims_size = table_contents_->shape();
  for (int i = 0; i < dimension_count; ++i) {
    DimensionSize to_push = {i, dims_size[i]};
    multi_size.push_back(to_push);
  }
  sort(multi_size.begin(), multi_size.end());
  vector<size_t> multi_index(multi_size.size(), 0);
  stringstream out_stream;
  printMultiTableInner(multi_size, &multi_index, 0, &out_stream);
  return out_stream.str();
}

template<typename T, size_t N>
typename MultiTable<T, N>::Iterator MultiTable<T, N>::begin() {
  return Iterator(this);
}

template<typename T, size_t N>
typename MultiTable<T, N>::Iterator MultiTable<T, N>::end() {
  Iterator end_iter(this);
  for (size_t i = 0; i < N; ++i) {
    end_iter.multi_index_[i] = table_contents_->shape()[i];
  }
  return end_iter;
}

template<typename T, size_t N>
MultiTable<T, N>::Iterator::Iterator(MultiTable<T, N>* _instance)
  : multi_index_(vector<size_t>(N))
  , instance(_instance) {}

template<typename T, size_t N>
const T& MultiTable<T, N>::Iterator::operator*() const {
  return instance->table_contents_->operator()(multi_index_);
}

template<typename T, size_t N>
T& MultiTable<T, N>::Iterator::operator*() {
  return instance->table_contents_->operator()(multi_index_);
}

template<typename T, size_t N>
T* MultiTable<T, N>::Iterator::operator->() {
  return &(instance->*table_contents_(multi_index_));
}

template<typename T, size_t N>
typename MultiTable<T, N>::Iterator MultiTable<T, N>::Iterator::operator++() {
  vector<size_t> multi_size(N);
  for (int i = 0; i < N; ++i) {
    multi_size[i] = instance->table_contents_->shape()[i];
  }
  size_t index_number = 0;
  do {
    ++multi_index_[index_number];
    if (multi_index_[index_number] != multi_size[index_number]) {
      break;
    }
    multi_index_[index_number] = 0;
    ++index_number;
    if (index_number == multi_size.size()) {
      multi_index_ = multi_size;
      break;
    }
  } while (true);
  return *this;
}

template<typename T, size_t N>
bool MultiTable<T, N>::Iterator::operator==(const Iterator& right) {
  return (this->multi_index_ == right.multi_index_);
}

template<typename T, size_t N>
bool MultiTable<T, N>::Iterator::operator!=(const Iterator& right) {
    return !(*this == right);
}

template<typename T, size_t N>
const vector<size_t>& MultiTable<T, N>::Iterator::getMultiIndex() const {
  return this->multi_index_;
}
};
};

#endif  // LTR_UTILITY_MULTITABLE_H_
