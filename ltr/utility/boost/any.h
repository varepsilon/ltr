// Copyright 2012 Yandex

#ifndef LTR_UTILITY_ANY_H_
#define LTR_UTILITY_ANY_H_

#include <algorithm>
#include <typeinfo>

namespace ltr {
namespace utility {

class Any {
public:
  Any(): content(0) {
  }

  template<typename ValueType>
  Any(const ValueType& value)
    : content(new Holder<ValueType>(value)) {
  }

  Any(const Any& other)
    : content(other.content ? other.content->clone() : 0) {
  }

  ~Any() {
    delete content;
  }

  Any& swap(Any& rhs) {
    std::swap(content, rhs.content);
    return *this;
  }

  template<typename ValueType>
  Any& operator = (const ValueType& rhs) {
    Any(rhs).swap(*this);
    return *this;
  }

  Any& operator = (Any rhs) {
    rhs.swap(*this);
    return *this;
  }

  bool empty() const {
    return !content;
  }

  const std::type_info & type() const {
    return content ? content->type() : typeid(void);
  }

private:
  class PlaceHolder {
  public:
    virtual ~PlaceHolder() {
    }

    virtual const std::type_info& type() const = 0;
    virtual PlaceHolder* clone() const = 0;
  };

  template<typename ValueType>
  class Holder : public PlaceHolder {
  public:
    Holder(const ValueType& value)
      : held(value) {
    }

    virtual const std::type_info& type() const {
      return typeid(ValueType);
    }

    virtual PlaceHolder* clone() const {
      return new Holder(held);
    }

    ValueType held;

  private: // intentionally left unimplemented
    Holder & operator=(const Holder &);
  };

  template<typename ValueType>
  friend ValueType * any_cast(Any *);

  template<typename ValueType>
  friend ValueType * unsafe_any_cast(Any *);

  PlaceHolder* content;
};


class bad_any_cast: public std::bad_cast {
public:
  virtual const char* what() const throw() {
    return "ltr::utility::bad_any_cast: failed conversion using ltr::utility::any_cast";
  }
};

template<typename ValueType>
ValueType* any_cast(Any* operand) {
  return operand && operand->type() == typeid(ValueType) ?
      &static_cast<Any::Holder<ValueType>*>(operand->content)->held : 0;
}

template<typename ValueType>
inline const ValueType* any_cast(const Any* operand) {
  return any_cast<ValueType>(const_cast<Any*>(operand));
}

template<typename ValueType>
ValueType any_cast(Any& operand) {
  // Warning: ValueType must not be reference! But it usually does not.
  ValueType* result = any_cast<ValueType>(&operand);
  if (!result) {
    throw(bad_any_cast());
  }
  return *result;
}

template<typename ValueType>
inline ValueType any_cast(const Any& operand) {
  return any_cast<const ValueType>(const_cast<Any&>(operand));
}

}
}

#endif
