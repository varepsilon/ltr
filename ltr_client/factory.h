// Copyright 2012 Yandex

#ifndef LTR_CLIENT_FACTORY_H_
#define LTR_CLIENT_FACTORY_H_

#include <rlog/rlog.h>

#include <string>
#include <iostream>
#include <cassert>
#include <map>

#include "ltr/interfaces/parameterized.h"
#include "ltr/parameters_container/parameters_container.h"
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/utility/boost/string_utils.h"

using std::cout;
using std::endl;
using std::string;
using std::logic_error;

using ltr::Parameterized;
using ltr::ParametersContainer;
using ltr::utility::Any;
using ltr::utility::shared_ptr;
using ltr::utility::to_lower;

class Factory {
 public:

  Factory() {
    assert(!self_);
    self_ = this;
  }

  ~Factory() {}
  static Factory* instance() {
    assert(self_);
    return self_;
  }

  template <class Base, class Derived>
  void registerType(const string& name) {
    string lower_case_name = to_lower(name);
    assert(name_creators_.find(lower_case_name) == name_creators_.end());
    name_creators_[lower_case_name] =
        AbstractCreator::Ptr(new Creator<Base, Derived>);
  }

  Any Create(const string& name, const ParametersContainer& parameters) const {
    rInfo("Factory::Create: Requested creating of %s\n", name.c_str());
    NameCreatorHash::const_iterator it = name_creators_.find(name);
    if (it == name_creators_.end()) {
      throw logic_error("unable to create " + name + ": name unknown");
    }
    const AbstractCreator::Ptr creator = it->second;
    return creator->create(parameters);
  }

 private:
  class AbstractCreator {
   public:
    typedef shared_ptr<AbstractCreator> Ptr;
    virtual ~AbstractCreator() {}

    virtual Any create(
      const ParametersContainer& parameters) const = 0;
  };

  template <class Base, class Derived>
  class Creator: public AbstractCreator {
   public:
    virtual Any create(const ParametersContainer& parameters) const {
      if (!parameters.empty()) {
        return shared_ptr<Base>(new Derived(parameters));
      } else {
        return shared_ptr<Base>(new Derived());
      }
    }
  };

  typedef std::map<string, AbstractCreator::Ptr> NameCreatorHash;

  Factory(const Factory&);

  Factory& operator=(const Factory&);

  NameCreatorHash name_creators_;

  static Factory* self_;
};

#endif  // LTR_CLIENT_FACTORY_H_
