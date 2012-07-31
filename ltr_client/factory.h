// Copyright 2012 Yandex

#ifndef LTR_CLIENT_FACTORY_H_
#define LTR_CLIENT_FACTORY_H_

#include <string>
#include <iostream>
#include <cassert>

#include <boost/unordered_map.hpp>  //NOLINT

#include "ltr/interfaces/parameterized.h"

#include "ltr/parameters_container/parameters_container.h"

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

  template <class T>
  void registerType(const string& name) {
    assert(name_creators_.find(name) == name_creators_.end());
    name_creators_[name] = new TCreator<T>;
  }

  Parameterized* Create(const string& name,
                        const ParametersContainer& parameters) const {
    cout << "Factory::Create: Requested creating of " << name << endl;
    TNameCreatorHash::const_iterator it = name_creators_.find(name);
    assert(it != name_creators_.end());
    const TAbstractCreator* creator = it->second;
    return creator->create(parameters);
  }

 private:
  class TAbstractCreator {
   public:
    virtual ~TAbstractCreator() {}

    virtual Parameterized* create(
      const ParametersContainer& parameters) const = 0;
  };

  template <class T>
  class TCreator: public TAbstractCreator {
   public:
    virtual Parameterized* create(const ParametersContainer& parameters) const {
      return new T(parameters);
    }
  };

  typedef boost::unordered_map<string, TAbstractCreator*> TNameCreatorHash;

  Factory(const Factory&);

  Factory& operator=(const Factory&);

  TNameCreatorHash name_creators_;

  static Factory* self_;
};

#endif  // LTR_CLIENT_FACTORY_H_
