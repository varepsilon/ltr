// Copyright 2012 Yandex

#ifndef LTR_UTILITY_LEXICALCAST_H_
#define LTR_UTILITY_LEXICALCAST_H_

#include <typeinfo>
#include <exception>
#include <sstream>

using std::stringstream;

namespace ltr {
namespace utility {

class bad_lexical_cast: public std::exception {
public:
    bad_lexical_cast(): source(&typeid(void)), target(&typeid(void)) {
    }

    bad_lexical_cast(
        const std::type_info &source_type_arg,
        const std::type_info &target_type_arg) :
        source(&source_type_arg), target(&target_type_arg) {
    }

    const std::type_info &source_type() const {
        return *source;
    }
    const std::type_info &target_type() const {
        return *target;
    }

    virtual const char *what() const throw() {
        return "bad lexical cast: source type value could not be interpreted as target";
    }
    virtual ~bad_lexical_cast() throw() {
    }
private:
    const std::type_info *source;
    const std::type_info *target;
};


template <typename Target, typename Source>
inline Target lexical_cast(const Source& arg) {
  stringstream out;
  out.precision(12);
  fixed(out);
  Target target;
  out << arg;
  out >> target;
  if (!out || !out.eof()) {
    throw bad_lexical_cast(typeid(Source), typeid(Target));
  }
  return target;
}


}
}
#endif
