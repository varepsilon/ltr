// Copyright 2011 Yandex

#ifndef LTR_UTILITY_BOOST_TRIPLE_H_
#define LTR_UTILITY_BOOST_TRIPLE_H_

namespace ltr {
namespace utility {

template<class T0, class T1, class T2>
class triple {
 public:
  triple(const T0& value_0 = T0(),
         const T1& value_1 = T1(),
         const T2& value_2 = T2()):
    value_0_(value_0),
    value_1_(value_1),
    value_2_(value_2) {
  }

  T0 get_0() const {
    return value_0_;
  }

  T1 get_1() const {
    return value_1_;
  }

  T2 get_2() const {
    return value_2_;
  }

  bool operator < (const triple<T0, T1, T2>& t) const {
    if (t.value_0_ == value_0_) {
      if (t.value_1_ == value_1_)
        return value_2_ < t.value_2_;
      return value_1_ < t.value_1_;
    }
    return value_0_ < t.value_0_;
  }

  bool operator == (const triple<T0, T1, T2>& t) const {
    return value_0_ == t.value_0_ && value_1_ == t.value_1_ &&
      value_2_ == t.value_2_;
  }

  bool operator > (const triple<T0, T1, T2>& t) const {
    return !(*this < t || *this == t);
  }

 private:
  T0 value_0_;
  T1 value_1_;
  T2 value_2_;
};

template<class T0, class T1, class T2>
triple<T0, T1, T2> make_triple(T0 v0, T1 v1, T2 v2) {
  return triple<T0, T1, T2> (v0, v1, v2);
}
};
};

#endif  // LTR_UTILITY_BOOST_TRIPLE_H_
