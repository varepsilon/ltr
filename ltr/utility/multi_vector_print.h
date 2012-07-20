#include <vector>
#include <iostream>
#include <string>
#include <stdexcept>

using std::cout;
using std::string;
using std::vector;

struct SizeOverIndex {
  int indexVal;
  size_t sizeVal;
};

template <typename T>
int getSize(const T& value) {
  return 0;
}

template <typename T>
int getSize(const vector<T>& array) {
  return array.size();
}

template <typename T>
void getMultiSize(const T& value, vector<SizeOverIndex>* const result) {}

template <typename T>
void getMultiSize(const vector<T>& array, vector<SizeOverIndex>* const result) {
  SizeOverIndex new_item = {result->size(), array.size()};
  result->push_back(new_item);
  for (size_t i = 0; i < array.size(); ++i) {
    if (getSize(array[i]) != getSize(array[0])) {
      throw std::logic_error(
            "In getMultiSize the array is not a table!");
    }
  }
  getMultiSize(array[0], result);
}

bool operator < (const SizeOverIndex& left, const SizeOverIndex& right) {
  return left.sizeVal < right.sizeVal;
}

template <typename T>
void printMultiArray(const vector<T>& multi_array,
                     vector<string>* dimNames) {
  vector<SizeOverIndex> multi_size;
  getMultiSize(multi_array, &multi_size);
  vector<int> multi_index(multi_size.size(), 0);
  sort(multi_size.begin(), multi_size.end());
  reverse(multi_size.begin(), multi_size.end());
  printMultiArrayInner(&multi_size, dimNames, multi_array, &multi_index, 0);
}

template <typename T>
struct MultiArrayTraits {
  typedef T Type;
};

template <typename T>
struct MultiArrayTraits<vector<T> > {
  typedef typename MultiArrayTraits<T>::Type Type;
};

template <typename T>
typename MultiArrayTraits<T>::Type getValueByMultiIndex(
    vector<int>* const multi_index,
    const vector<T>& multi_array) {
  if (multi_index->size() != 1) {
    throw std::logic_error("Bad multiindex!");
  }
  return multi_array[multi_index->back()];
}

template <typename T>
typename MultiArrayTraits<vector<T> >::Type
  getValueByMultiIndex(vector<int>* const multi_index,
                        const vector<vector<T> >& multi_array) {
  int current_index = multi_index->back();
  multi_index->pop_back();
  typename MultiArrayTraits<vector<T> >::Type result =
      getValueByMultiIndex(multi_index, multi_array[current_index]);
  multi_index->push_back(current_index);
  return result;
}

template <typename T>
void printMultiArrayInner(vector<SizeOverIndex>* multi_size,
                          vector<string>* dim_names,
                          const vector<T>& multi_array,
                          vector<int>* multi_index,
                          const int depth) {
  if (multi_size->size() == 1) {
    int index_no_to_increase = multi_size->back().indexVal;
    int size_to_print = multi_size->back().sizeVal;
    multi_size->pop_back();
    string current_name = dim_names->back();
    dim_names->pop_back();
    for (int i = 0; i < size_to_print; ++i) {
      (*multi_index)[multi_index->size() - 1 - index_no_to_increase] = i;
      cout << getValueByMultiIndex(multi_index, multi_array) << "\t";
    }
    SizeOverIndex back_to_push = {index_no_to_increase, size_to_print};
    multi_size->push_back(back_to_push);
    dim_names->push_back(current_name);
  } else if (multi_size->size() == 2) {
    int indexNoToIncrease = multi_size->back().indexVal;
    int sizeToPrint = multi_size->back().sizeVal;
    multi_size->pop_back();
    string current_name = dim_names->back();
    dim_names->pop_back();
    for (int repetition = 0; repetition < depth; ++repetition) {
      cout << "|";
    }
    cout << "Table VERT: " << current_name << " HORIZ: "
         << dim_names->back() << "\n";
    for (int index = 0; index < sizeToPrint; ++index) {
      (*multi_index)[multi_index->size() - 1 - indexNoToIncrease] = index;
      for (int repetition = 0; repetition < depth; ++repetition) {
        cout << "|";
      }
      printMultiArrayInner(multi_size, dim_names, multi_array, multi_index, depth + 1);
      cout << "\n";
    }
    SizeOverIndex back_to_push = {indexNoToIncrease, sizeToPrint};
    multi_size->push_back(back_to_push);
    dim_names->push_back(current_name);
  } else {
    int index_no_to_increase = multi_size->back().indexVal;
    int size_to_print = multi_size->back().sizeVal;
    multi_size->pop_back();
    string current_name = dim_names->back();
    dim_names->pop_back();
    for (int index = 0; index < size_to_print; ++index) {
      for (int repetition = 0; repetition < depth; ++repetition) {
        cout << "|";
      }
      cout << "/" << current_name << " " << index + 1 << " of " << size_to_print << "\n";
      (*multi_index)[multi_index->size() - 1 - index_no_to_increase] = index;
      printMultiArrayInner(multi_size, dim_names, multi_array, multi_index, depth + 1);
      for (int repetition = 0; repetition < depth; ++repetition) {
        cout << "|";
      }
      cout << "\\" << current_name << " end\n";
    }
    SizeOverIndex back_to_push = {index_no_to_increase, size_to_print};
    multi_size->push_back(back_to_push);
    dim_names->push_back(current_name);
  }
}
