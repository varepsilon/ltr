// Copyright 2012 Yandex

#ifndef LTR_CLIENT_FACTORY_H_
#define LTR_CLIENT_FACTORY_H_

#include <rlog/rlog.h>

#include <string>
#include <iostream>
#include <cassert>

#include <boost/unordered_map.hpp>  //NOLINT

#include "ltr/interfaces/parameterized.h"

#include "ltr/parameters_container/parameters_container.h"

#include "ltr/utility/shared_ptr.h"

using std::cout;
using std::endl;
using std::string;

using ltr::Parameterized;
using ltr::ParametersContainer;

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
    assert(name_creators_.find(name) == name_creators_.end());
    name_creators_[name] = AbstractCreator::Ptr(new Creator<Base, Derived>);
  }

  boost::any Create(const string& name,
                    const ParametersContainer& parameters) const {
    rInfo("Factory::Create: Requested creating of %s\n", name.c_str());
    NameCreatorHash::const_iterator it = name_creators_.find(name);
    assert(it != name_creators_.end());
    const AbstractCreator::Ptr creator = it->second;
    return creator->create(parameters);
  }

 private:
  class AbstractCreator {
   public:
    typedef ltr::utility::shared_ptr<AbstractCreator> Ptr;
    virtual ~AbstractCreator() {}

    virtual boost::any create(
      const ParametersContainer& parameters) const = 0;
  };

  template <class Base, class Derived>
  class Creator: public AbstractCreator {
   public:
    virtual boost::any create(const ParametersContainer& parameters) const {
      if (!parameters.empty()) {
        return ltr::utility::shared_ptr<Base>(new Derived(parameters));  // NOLINT
      } else {
        return ltr::utility::shared_ptr<Base>(new Derived());
      }
    }
  };

  typedef boost::unordered_map<string, AbstractCreator::Ptr> NameCreatorHash;

  Factory(const Factory&);

  Factory& operator=(const Factory&);

  NameCreatorHash name_creators_;

  static Factory* self_;
};

#endif  // LTR_CLIENT_FACTORY_H_
