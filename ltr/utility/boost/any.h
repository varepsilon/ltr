// Copyright 2012 Yandex

#ifndef LTR_UTILITY_ANY_H_
#define LTR_UTILITY_ANY_H_

#include <algorithm>
#include <typeinfo>
#include <vector>
#include <string>

using std::vector;
using std::string;

namespace ltr {
namespace utility {

/**
* Class for storing variant data
*/
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

  template<typename ResultType, typename SourceType>
  friend class AnyCaster;

  PlaceHolder* content;
};


/**
* Exception thrown when any_cast fails
*/
class bad_any_cast: public std::bad_cast {
public:
  bad_any_cast(const string& from, const string& to): from_(from), to_(to) {}

  virtual const char* what() const throw() {
    string description = "ltr::utility::bad_any_cast: failed conversion from stored " +
        from_ + " to " + to_;
    return description.c_str();
  }

  virtual ~bad_any_cast() throw() {};
private:
  string from_;
  string to_;
};


template <typename ResultType, typename SourceType>
class AnyCaster {
public:
  static ResultType* toPointer(SourceType* operand) {
    return operand && operand->type() == typeid(ResultType) ?
      &static_cast<Any::Holder<ResultType>*>(operand->content)->held : 0;
  }

  static const ResultType* toPointer(const SourceType* operand) {
    return toPointer(const_cast<SourceType*>(operand));
  }

  static ResultType toValue(SourceType& operand) {
    // Warning: ResultType must not be reference! But it usually does not.
    ResultType* result = toPointer(&operand);
    if (!result) {
      throw(bad_any_cast(operand.type().name(), typeid(ResultType).name()));
    }
    return *result;
  }

  static ResultType toValue(const SourceType& operand) {
    return AnyCaster<const ResultType, SourceType>().toValue(const_cast<SourceType&>(operand));
  }
};


template <typename ResultType, typename SourceType>
class AnyCaster<vector<ResultType>, vector<SourceType> > {
public:
  static vector<ResultType> toValue (const vector<SourceType>& operand) {
    vector<ResultType> result(operand.size());
    for (int i = 0; i < result.size(); ++i) {
      result[i] = AnyCaster<ResultType, SourceType>().toValue(operand[i]);
    }
    return result;
  }
};


template <typename ResultType>
class AnyCaster<vector<ResultType>, Any> {
public:
  static vector<ResultType> toValue (const Any& operand) {
    if (operand.type() == typeid(vector<Any>)) {
      const vector<Any>* intermediate = AnyCaster<vector<Any>, Any>().toPointer(&operand);
      if (!intermediate) {
        throw(bad_any_cast(operand.type().name(), typeid(vector<ResultType>).name()));
      }
      return AnyCaster<vector<ResultType>, vector<Any> >().toValue(*intermediate);
    } else {
      const vector<ResultType>* result =
          AnyCaster<vector<ResultType>, Any>().toPointer(&operand);
      if (!result) {
        throw(bad_any_cast(operand.type().name(), typeid(vector<ResultType>).name()));
      }
      return *result;
    }
  }

  static vector<ResultType>* toPointer(Any* operand) {
    return operand && operand->type() == typeid(vector<ResultType>) ?
      &static_cast<Any::Holder<vector<ResultType> >*>(operand->content)->held : 0;
  }

  static const vector<ResultType>* toPointer(const Any* operand) {
    return toPointer(const_cast<Any*>(operand));
  }
};


/**
* Casts Any to the given type
*/
template<typename ResultType, typename SourceType>
ResultType* any_cast(SourceType* operand) {
  return AnyCaster<ResultType, SourceType>::toPointer(operand);
}

template<typename ResultType, typename SourceType>
inline const ResultType* any_cast(const SourceType* operand) {
  return AnyCaster<ResultType, SourceType>::toPointer(operand);
}

template<typename ResultType, typename SourceType>
ResultType any_cast(SourceType& operand) {
  return AnyCaster<ResultType, SourceType>::toValue(operand);
}

template<typename ResultType, typename SourceType>
inline ResultType any_cast(const SourceType& operand) {
  return AnyCaster<ResultType, SourceType>::toValue(operand);
}

}
}

#endif
