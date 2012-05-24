// Copyright 2012 Yandex

#ifndef LTR_CLIENT_FACTORY_H_
#define LTR_CLIENT_FACTORY_H_

#include <string>
#include <iostream>
#include <cassert>
#include <boost/unordered_map.hpp>  //NOLINT

#include "ltr/interfaces/parameterized.h"
#include "ltr/parameters_container/parameters_container.h"

class Factory {
 public:
  Factory() {
    assert(!self_);
    self_ = this;
  }
  ~Factory() { }
  static Factory *instance() {
    assert(self_);
    return self_;
  }

  template <class T>
  void registerType(const std::string &name) {
    assert(name_creators_.find(name) == name_creators_.end());
    name_creators_[name] = new TCreator<T>;
  }
  ltr::Parameterized *Create(const std::string &name,
                             const ltr::ParametersContainer &parameters) const {
    std::cout << "Factory::Create: Requested creating of " << name << std::endl;
    TNameCreatorHash::const_iterator it = name_creators_.find(name);
    assert(it != name_creators_.end());
    const TAbstractCreator *creator = it->second;
    return creator->create(parameters);
  }

 private:
  class TAbstractCreator {
   public:
    virtual ~TAbstractCreator() { }
    virtual ltr::Parameterized *create(
        const ltr::ParametersContainer &parameters) const =0;
  };
  template <class T>
  class TCreator: public TAbstractCreator {
   public:
    virtual ltr::Parameterized *create(
        const ltr::ParametersContainer &parameters) const {
      return new T(parameters);
    }
  };

  typedef boost::unordered_map<std::string, TAbstractCreator *>
                                                              TNameCreatorHash;

  Factory(const Factory &);
  Factory &operator=(const Factory &);

  static Factory *self_;
  TNameCreatorHash name_creators_;
};

#endif  // LTR_CLIENT_FACTORY_H_
