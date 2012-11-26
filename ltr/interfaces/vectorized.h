// Copyright 2012 Yandex

#ifndef LTR_INTERFACES_VECTORIZED_H_
#define LTR_INTERFACES_VECTORIZED_H_

#include <vector>
#include <stdexcept>

using std::vector;

namespace ltr {
/**
* Vector for inheritance in classes. It provides methods of std::vector for
* custom classes.
* Usage: inherit your class from Vectorized<your_type> and add 
* Vectorized<your_type>(your_vector_name) call in all your constructors to
* associate your_vector_name vector with vector methods.
*/
template<typename T>
class Vectorized {
  private:
    vector<T> &items_;
  public:
    explicit Vectorized(vector<T> &items): items_(items) {} //NOLINT
    typedef typename vector<T>::iterator iterator;
    typedef typename vector<T>::const_iterator const_iterator;

    int count() const {
      return items_.size();
    }
    void add(const T& obj) {
      items_.push_back(obj);
    }
    const T& operator[](int index) const {
      return items_[index];
    }
    T& operator[](int index) {
      return items_[index];
    }
    const T& at(int index) const {
      return items_.at(index);
    }
    T& at(int index) {
      return items_.at(index);
    }

    iterator begin() {
      return items_.begin();
    }
    iterator end() {
      return items_.end();
    }
    const_iterator begin() const {
      return items_.begin();
    }
    const_iterator end() const {
      return items_.end();
    }

    void clear() {
      items_.clear();
    }

    void erase(typename vector<T>::const_iterator index) {
      items_.erase(index);
    }
    void erase(int index) {
      if (!(0 <= index && index < items_.size()))
        throw std::out_of_range("Vectorized::erase");
      items_.erase(items_.begin() + index);
    }
};
}
#endif  // LTR_INTERFACES_VECTORIZED_H_
