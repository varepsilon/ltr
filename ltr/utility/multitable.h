// Copyright 2012 Yandex

#ifndef LTR_UTILITY_MULTITABLE_H_
#define LTR_UTILITY_MULTITABLE_H_

#define _ITERATOR_DEBUG_LEVEL 0  // for VS 2010

#include <algorithm>
#include <string>
#include <vector>
#include <exception>
#include <stdexcept>
#include "ltr/utility/boost/multi_array.h"
#include "ltr/utility/html.h"
#include "ltr/interfaces/printable.h"
#include "ltr/utility/boost/lexical_cast.hpp"

using std::max;
using std::sort;
using std::logic_error;
using std::string;
using std::stringstream;
using std::vector;
using std::cout;
using ltr::Printable;
using ltr::utility::MultiArray;
using ltr::utility::FileLink;
using ltr::utility::lexical_cast;
using ltr::utility::LexicalCastConfig;

namespace ltr {
namespace utility {
/**
 * Enumerates formats MultiTable can be exported to
 */
enum Format {
  PLAIN_TEXT,
  HTML
};
/**
 * This struct contains information about size of the dimension
 * of the MultiTable.
 */
struct DimensionSize {
  int index;
  int size;
};

/**
 * The less operator returns true if the left operand's field "size"
 * is less than right operand's field "size"
 * @param left - left operand
 * @param right - right operand
 * @returns boolean left.size < right.size
 */

inline bool operator<(const DimensionSize& left, const DimensionSize& right) {
  return left.size < right.size;
}

/**
 * Class intended to store the multidimensional tables.
 */
template <typename T, int N>
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
   * @param table_size - vector<int> multisize specification
   */
  explicit MultiTable(const vector<int>& table_size);
  /**
   * Operator [] to get the access to the element
   * by multiindex as by ordinary index.
   * @param multiIndex - vector<int> specifies the position
   * of the element
   * @returns T& - the constant datum reference of the stored type.
   */
  const T& operator[] (const vector<int>& multiIndex) const;
  /**
   * Non-constant implementation of the operator [].
   * @param multiIndex - vector<int> specifies the position
   * of the element
   * @returns T - the datum reference of the stored type.
   */
  T& operator[] (const vector<int>& multiIndex);
  /**
   * Sets axis label.
   * @param axis_index - int number of the axis to set label.
   * @param label - string label to set
   */
  void setAxisLabel(int axis_index, const string& label);
  /**
   * Sets tick label.
   * @param axis_index - int index of the axis to modify
   * @param tick_index - int index of the tick to set
   * @param label - string label to set
   */
  void setTickLabel(int axis_index,
                    int tick_index,
                    const string& label);
  virtual string toString() const;

  string toHTML() const;
  /**
   * Resizes the multitable.
   * @param multi_size - new size of the multitable
   */
  void resize(const vector<int>& multi_size);
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
  MultiArray<T, N>* table_contents_;
  vector<DimensionMetaInfo> table_meta_info_;
  void printMultiTableInner(const vector<DimensionSize>& multi_size,
                            vector<int>* multi_index,
                            int depth,
                            stringstream* const outstream,
                            Format format) const;
  void print2DTable(int first_axis_index,
                    int second_axis_index,
                    vector<int>* const multi_index,
                    stringstream* const out_stream) const;
  void print2DTableAsHTML(int first_axis_index,
                          int second_axis_index,
                          vector<int>* const multi_index,
                          stringstream* const out_stream) const;
  string convertToText(Format format) const;

  /**
   * Minimum witdh for a column in plain text output
   */
  static const int MIN_COL_WIDTH;
  /**
   * Number of spaces between columns in plain text output
   */
  static const int COL_SPACING;
};
template<typename T, int N>
const int MultiTable<T, N>::MIN_COL_WIDTH = 8;
template<typename T, int N>
const int MultiTable<T, N>::COL_SPACING = 1;

/**
 * Iterator for a MultiTable class.
 */
template<typename T, int N>
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
  const vector<int>& getMultiIndex() const;
  /**
   * @returns true if the iterators are at the same position
   */
  bool operator==(const Iterator& right);
  /**
   * @returns false if the iterators are at the same position
   */
  bool operator!=(const Iterator& right);
 private:
  vector<int> multi_index_;
  MultiTable<T, N>* instance;

  friend class MultiTable;
};

template <typename T, int N>
struct MultiTable<T, N>::DimensionMetaInfo {
  string axis_label;
  vector<string> axis_ticks_labels;
};


template <typename T, int N>
void MultiTable<T, N>::resize(const vector<int>& multi_size) {
  delete table_contents_;
  table_contents_ = new MultiArray<T, N>(multi_size);
  table_meta_info_.resize(multi_size.size());
  for (int index = 0; index < multi_size.size(); ++index) {
    table_meta_info_[index].axis_ticks_labels.resize(multi_size[index]);
  }
}

template <typename T, int N>
void MultiTable<T, N>::clear() {
  vector<int> empty;
  for (int i = 0; i < N; ++i) {
    empty.push_back(0);
  }
  this->resize(empty);
}

template<typename T, int N>
void MultiTable<T, N>::print2DTable(int first_axis_index,
                                    int second_axis_index,
                                    vector<int>* const multi_index,
                                    stringstream* const out_stream) const {
  LexicalCastConfig::getInstance().restoreDefaults();

  int first_size = table_contents_->shape()[first_axis_index];
  int second_size = table_contents_->shape()[second_axis_index];

  *out_stream << "\n";
  for (int axis_index = 0;
       axis_index < (int)multi_index->size();
       ++axis_index) {
    int tick_index = (*multi_index)[axis_index];
    if (axis_index != first_axis_index && axis_index != second_axis_index) {
      *out_stream << table_meta_info_[axis_index].axis_label << ": "
                  << table_meta_info_[axis_index].
                     axis_ticks_labels[tick_index]
                  << "\n";
    }
  }
  *out_stream << "Table (row: "
       << table_meta_info_[first_axis_index].axis_label
       << ", column: "
       << table_meta_info_[second_axis_index].axis_label
       << ")\n";

  vector<vector<string> > output(first_size + 1,
                                 vector<string>(second_size + 1));

  for (int second_index = 0; second_index < (int)second_size; ++second_index) {
    output[0][second_index + 1] = lexical_cast<string>(
        table_meta_info_[second_axis_index].axis_ticks_labels[second_index]);
  }
  for (int first_index = 0; first_index < first_size; ++first_index) {
    (*multi_index)[first_axis_index] = first_index;
    output[first_index + 1][0] = lexical_cast<string>(
        table_meta_info_[first_axis_index].axis_ticks_labels[first_index]);
    for (int second_index = 0; second_index < second_size; ++second_index) {
      (*multi_index)[second_axis_index] = second_index;
      output[first_index + 1][second_index + 1] = lexical_cast<string>(
        table_contents_->operator()(*multi_index));
    }
  }

  vector<int> col_size(second_size + 1, MIN_COL_WIDTH);
  for (int first_index = 0; first_index < first_size + 1; ++first_index) {
    for (int second_index = 0; second_index < second_size + 1; ++second_index) {
      col_size[second_index] = max(col_size[second_index],
                                (int)output[first_index][second_index].size());
    }
  }
  for (int first_index = 0; first_index < first_size + 1; ++first_index) {
    for (int second_index = 0; second_index < second_size + 1; ++second_index) {
      string output_string = output[first_index][second_index];
      *out_stream << output_string
                  << string(col_size[second_index] - output_string.length(),
                            ' ')
                  // to add spacing between columns
                  << (second_index == second_size ? "\n" : " ");
    }
  }
}

template<typename T, int N>
void MultiTable<T, N>::print2DTableAsHTML(int first_axis_index,
                                          int second_axis_index,
                                          vector<int>* const multi_index,
                                          stringstream* const
                                              out_stream) const {
  int first_size = table_contents_->shape()[first_axis_index];
  int second_size = table_contents_->shape()[second_axis_index];

  *out_stream << "<p>\n";
  for (int axis_index = 0;
       axis_index < (int)multi_index->size();
       ++axis_index) {
    int tick_index = (*multi_index)[axis_index];
    if (axis_index != first_axis_index && axis_index != second_axis_index) {
      *out_stream << table_meta_info_[axis_index].axis_label << ": "
                  << FileLink(table_meta_info_[axis_index].
                              axis_ticks_labels[tick_index])
                  << "<br/>\n";
    }
  }
  *out_stream << "\n<table border=\"1\" cellpadding=\"5\" "
              << "cellspacing=\"0\">\n"
              << "\t<tr align=\"center\">\n"
              << "\t\t<th>"
              << table_meta_info_[first_axis_index].axis_label
              << " \\ "
              << table_meta_info_[second_axis_index].axis_label
              << "</th>\n";
  for (int second_index = 0; second_index < (int)second_size; ++second_index) {
    *out_stream << "\t\t<th>"
               << FileLink(table_meta_info_[second_axis_index].
                           axis_ticks_labels[second_index])
               << "</th>\n";
  }
  *out_stream << "\t</tr>\n";

  for (int first_index = 0; first_index < first_size; ++first_index) {
    (*multi_index)[first_axis_index] = first_index;
    *out_stream << "\t<tr align=\"center\">\n"
                << "\t\t<th>"
                << FileLink(table_meta_info_[first_axis_index].
                            axis_ticks_labels[first_index])
                << "</th>\n";
    for (int second_index = 0; second_index < second_size; ++second_index) {
      (*multi_index)[second_axis_index] = second_index;
      *out_stream << "\t\t<td>"
                  << table_contents_->operator()(*multi_index)
                  << "</td>\n";
    }
    *out_stream << "\t</tr>\n";
  }
  *out_stream << "</table>\n</p>";
}

template <typename T, int N>
void MultiTable<T, N>::printMultiTableInner(
    const vector<DimensionSize>& axis_print_order,
    vector<int>* multi_index,
    int depth,
    stringstream* const out_stream,
    Format format) const {
  if (depth + 2 == axis_print_order.size()) {
    if (format == PLAIN_TEXT) {
      print2DTable(axis_print_order[depth].index,
                   axis_print_order[depth + 1].index,
                   multi_index,
                   out_stream);
    } else if (format == HTML) {
      print2DTableAsHTML(axis_print_order[depth].index,
                   axis_print_order[depth + 1].index,
                   multi_index,
                   out_stream);
    }
  } else {
    int index_number_to_increase = axis_print_order[depth].index;
    int ticks_count = axis_print_order[depth].size;
    for (int tick_index = 0; tick_index < ticks_count; ++tick_index) {
      (*multi_index)[index_number_to_increase] = tick_index;
      printMultiTableInner(axis_print_order,
                           multi_index, depth + 1, out_stream, format);
    }
  }
}

template<typename T, int N>
MultiTable<T, N>::MultiTable(const vector<int>& table_size)
  : table_contents_(new MultiArray<T, N>())
  , table_meta_info_() {
  if (table_size.size() != N) {
    throw logic_error("Bad number of input sizes in multitable constructor\n");
  }
  delete table_contents_;
  table_contents_ = new MultiArray<T, N>(table_size);
  table_meta_info_.resize(table_size.size());
  for (int index = 0; index < table_size.size(); ++index) {
    table_meta_info_[index].axis_ticks_labels.resize(table_size[index]);
  }
}

template<typename T, int N>
MultiTable<T, N>::MultiTable()
  : table_contents_(new MultiArray<T, N>())
  , table_meta_info_() {}

template<typename T, int N>
const T& MultiTable<T, N>::operator[] (
  const vector<int>& multi_index) const {
  if (multi_index.size() != N) {
    throw logic_error("Bad number of input sizes in multitable [] operator\n");
  }
  return (*table_contents_)(multi_index);
}

template<typename T, int N>
T& MultiTable<T, N>::operator[] (const vector<int>& multi_index) {
  if (multi_index.size() != N) {
    throw logic_error("Bad number of input sizes in multitable [] operator\n");
  }
  return (*table_contents_)(multi_index);
}

template<typename T, int N>
void MultiTable<T, N>::setAxisLabel(int dim_index,
                                    const string& label) {
  if (dim_index >= (int)table_meta_info_.size()) {
    throw logic_error("Axis label index out of range\n");
  }
  table_meta_info_[dim_index].axis_label = label;
}

template<typename T, int N>
void MultiTable<T, N>::setTickLabel(
    int dim_index, int axis_index, const string& label) {
  if (dim_index >= (int)table_meta_info_.size() ||
     axis_index >= (int)table_meta_info_[dim_index].axis_ticks_labels.size()) {
    throw logic_error("Axis tick label index out of range");
  }
  table_meta_info_[dim_index].axis_ticks_labels[axis_index] = label;
}

template<typename T, int N>
string MultiTable<T, N>::convertToText(Format format) const {
  int dimensions_count = table_contents_->dimensions_count();
  vector<DimensionSize> multi_size;
  vector<int> dims_size = table_contents_->shape();
  for (int i = 0; i < dimensions_count; ++i) {
    DimensionSize to_push = {i, dims_size[i]};
    multi_size.push_back(to_push);
  }
  sort(multi_size.begin(), multi_size.end());
  vector<int> multi_index(multi_size.size(), 0);
  stringstream out_stream;
  printMultiTableInner(multi_size, &multi_index, 0, &out_stream, format);
  return out_stream.str();
}

template<typename T, int N>
string MultiTable<T, N>::toString() const {
  return convertToText(PLAIN_TEXT);
}

template<typename T, int N>
string MultiTable<T, N>::toHTML() const {
  return convertToText(HTML);
}

template<typename T, int N>
typename MultiTable<T, N>::Iterator MultiTable<T, N>::begin() {
  return Iterator(this);
}

template<typename T, int N>
typename MultiTable<T, N>::Iterator MultiTable<T, N>::end() {
  Iterator end_iter(this);
  for (int i = 0; i < N; ++i) {
    end_iter.multi_index_[i] = table_contents_->shape()[i];
  }
  return end_iter;
}

template<typename T, int N>
MultiTable<T, N>::Iterator::Iterator(MultiTable<T, N>* _instance)
  : multi_index_(vector<int>(N))
  , instance(_instance) {}

template<typename T, int N>
const T& MultiTable<T, N>::Iterator::operator*() const {
  return instance->table_contents_->operator()(multi_index_);
}

template<typename T, int N>
T& MultiTable<T, N>::Iterator::operator*() {
  return instance->table_contents_->operator()(multi_index_);
}

template<typename T, int N>
T* MultiTable<T, N>::Iterator::operator->() {
  return &(instance->*table_contents_(multi_index_));
}

template<typename T, int N>
typename MultiTable<T, N>::Iterator MultiTable<T, N>::Iterator::operator++() {
  vector<int> multi_size(N);
  for (int i = 0; i < N; ++i) {
    multi_size[i] = instance->table_contents_->shape()[i];
  }
  int index_number = 0;
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

template<typename T, int N>
bool MultiTable<T, N>::Iterator::operator==(const Iterator& right) {
  return (this->multi_index_ == right.multi_index_);
}

template<typename T, int N>
bool MultiTable<T, N>::Iterator::operator!=(const Iterator& right) {
    return !(*this == right);
}

template<typename T, int N>
const vector<int>& MultiTable<T, N>::Iterator::getMultiIndex() const {
  return this->multi_index_;
}
};
};

#endif  // LTR_UTILITY_MULTITABLE_H_
