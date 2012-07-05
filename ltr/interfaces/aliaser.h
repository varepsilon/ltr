// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_ALIASER_H_
#define LTR_INTERFACES_ALIASER_H_

#include <string>

using std::string;

namespace ltr {
/**
* \brief Inherit from this class in order to use alias.
*
* Alias for example can be used for logging.
*/
class Aliaser {
 public:
  virtual ~Aliaser() {}
  /**
  * \return alias if it's was set or default alias otherwise
  */
  virtual string alias() const {
    if (!alias_.empty()) {
      return alias_;
    } else {
      return getDefaultAlias();
    }
  }
  /**
  * sets alias
  */
  void set_alias(const string& alias) {
    alias_ = alias;
  }
 private:
  virtual string getDefaultAlias() const = 0;
  string alias_;
};
};

#endif  // LTR_INTERFACES_ALIASER_H_
