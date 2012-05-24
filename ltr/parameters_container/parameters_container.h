// Copyright 2011 Yandex

#ifndef LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_H_
#define LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_H_

#include <boost/unordered_map.hpp>
#include <boost/any.hpp>
#include <stdexcept>
#include <string>
#include <list>

namespace ltr {

class ParametersContainer {
 public:
  typedef boost::unordered_map<std::string, boost::any> StringAnyHash;

  ParametersContainer();
  ParametersContainer(const ParametersContainer &other);
  ParametersContainer &operator=(const ParametersContainer &other);

  void Clear();

  StringAnyHash::const_iterator begin() const {
    return name_value_hash_.begin();
  }
  StringAnyHash::const_iterator end() const {
    return name_value_hash_.end();
  }

  template <class T>
  struct NameValue {
    NameValue(const std::string &nm, const T &v): name(nm), value(v) { }
    std::string name;
    T value;
  };
  template <class T>
  std::list<NameValue<T> > getValuesByType() const {
    std::list<NameValue<T> > result;
    for (StringAnyHash::const_iterator it = name_value_hash_.begin();
        it != name_value_hash_.end();
        ++it) {
      T *value = boost::any_cast<T>(&it->second);
      if (!value) continue;
      result.push_back(NameValue<T>(it->first, *value));
    }
    return result;
  }

  bool Contains(const std::string &name) const;

  template <class T>
  bool TypeCoincides(const std::string &name) const {
    const StringAnyHash::const_iterator It = name_value_hash_.find(name);
    if (It == name_value_hash_.end())
      throw std::logic_error("No such paramater name: " + name);
    return It->second.type() == typeid(T);
  }

  template<class T>
  T Get(const std::string &name) const
      throw(std::logic_error, std::bad_cast) {
    const StringAnyHash::const_iterator It = name_value_hash_.find(name);
    if (It == name_value_hash_.end())
      throw std::logic_error("No such paramater name: " + name);

    try {
      return boost::any_cast<T>(It->second);
    } catch(const boost::bad_any_cast &exc) {
      throw std::logic_error(std::string(exc.what()) +
                                         "\nParameter name: " + name +
                                      "\nRequested type: " + typeid(T).name() +
                             "\nactual type: " + It->second.type().name());
    }
  }

  template<class StoredType, class DesiredType>
  DesiredType Get(const std::string &name) const
      throw(std::logic_error, std::bad_cast) {
    const StringAnyHash::const_iterator It = name_value_hash_.find(name);
    if (It == name_value_hash_.end())
      throw std::logic_error("No such paramater name: " + name);

    try {
      const StoredType &stored = boost::any_cast<StoredType>(It->second);
      DesiredType desired = dynamic_cast<DesiredType>(stored); //NOLINT
      return desired;
    } catch(const boost::bad_any_cast &exc) {
      throw std::logic_error(std::string(exc.what()) +
                                         "\nParameter name: " + name +
                                      "\nRequested type: " +
                             typeid(StoredType).name() +
                             "\nactual type: " + It->second.type().name());
    } catch(const std::bad_cast &exc) {
      throw std::logic_error(std::string(exc.what()) +
                                         "\nParameter name: " + name);
    }
  }

  template<class T>
  const T &GetRef(const std::string &name) const
      throw(std::logic_error, std::bad_cast) {
    const StringAnyHash::const_iterator It = name_value_hash_.find(name);
    if (It == name_value_hash_.end())
      throw std::logic_error("No such paramater name: " + name);

    try {
      return *boost::any_cast<T>(&It->second);
    }
    catch(const boost::bad_any_cast &exc) {
      throw std::logic_error(std::string(exc.what()) +
                                         "\nParameter name: " + name +
                                      "\nRequested type: " + typeid(T).name());
    }
  }

  template<class T>
  T &GetRef(const std::string &name)
      throw(std::logic_error, std::bad_cast) {
    const StringAnyHash::iterator It = name_value_hash_.find(name);
    if (It == name_value_hash_.end())
      throw std::logic_error("No such paramater name: " + name);

    try {
      return *boost::any_cast<T>(&It->second);
    }
    catch(const boost::bad_any_cast &exc) {
      throw std::logic_error(std::string(exc.what()) +
                                         "\nParameter name: " + name +
                                      "\nRequested type: " + typeid(T).name());
    }
  }


  // Without checks
  template<class T>
  void Set(const std::string &name, const T &value) {
    name_value_hash_[name] = value;
  }

  // Perform check
  template<class T>
  void SetExisting(const std::string &name, const T &value) {
    const StringAnyHash::iterator It = name_value_hash_.find(name);
    if (It == name_value_hash_.end())
      throw std::logic_error("No such paramater name: " + name);
    It->second = value;
  }

  // Perform check
  template<class T>
  void AddNew(const std::string &name, const T &value) {
    const StringAnyHash::const_iterator It = name_value_hash_.find(name);
    if (It != name_value_hash_.end())
      throw std::logic_error("There is already exist such parameter: " + name);
    name_value_hash_[name] = value;
  }

  void Copy(const ParametersContainer &parameters);

  std::string ToString() const;

 private:
  StringAnyHash name_value_hash_;
};
}


#endif  // LTR_PARAMETERS_CONTAINER_PARAMETERS_CONTAINER_H_
