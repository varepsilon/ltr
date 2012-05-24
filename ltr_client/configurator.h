// Copyright 2011 Yandex

#ifndef LTR_CLIENT_CONFIGURATOR_H_
#define LTR_CLIENT_CONFIGURATOR_H_

#include <string>
#include <boost/unordered_map.hpp>  //NOLINT
#include <boost/unordered_set.hpp>  //NOLINT
#include <list>

#include "ltr/parameters_container/parameters_container.h"

struct TDataInfo {
  TDataInfo() { }
  // C++11 -> we have no initializer, so we MUST write
  // stupid conctructor manually
  TDataInfo(const std::string &nm, const std::string &app,
            const std::string &fmt, const std::string &fn):
    name(nm), approach(app), format(fmt), file_name(fn) { }
  std::string name;
  std::string approach;
  std::string format;
  std::string file_name;
};
std::string ToString(const TDataInfo &Info);

struct TXmlTokenDependency {
  // C++11 -> we have no initializer, so we MUST write
  // stupid conctructor manually
  explicit TXmlTokenDependency(const std::string &nm): parameter_name(nm) { }
  std::string parameter_name;
};

class TXmlTokenSpec;
typedef std::list<const TXmlTokenSpec *> TXmlTokenSpecList;
class TXmlTokenSpecPrivate;
class TXmlTokenSpec {
  public:
  TXmlTokenSpec();
  TXmlTokenSpec(const TXmlTokenSpec &other);
  TXmlTokenSpec &operator=(const TXmlTokenSpec &other);
  ~TXmlTokenSpec();

  const std::string &tagName() const;
  const std::string &name() const;
  const std::string &type() const;
  const std::string &approach() const;
  const ltr::ParametersContainer &parameters() const;

  const TXmlTokenSpecList &dependencySpecs() const;

 private:
  TXmlTokenSpecPrivate * const d;
  friend class Configurator;
  friend class TOnGeneralXmlToken;
};
std::string ToString(const TXmlTokenSpec &Info);


struct TTrainInfo {
  TTrainInfo() { }
  // C++11 -> we have no initializer, so we MUST write stupid
  // conctructor manually
  TTrainInfo(const std::string &nm,
             const std::string &dt,
             const std::string &lr):
    name(nm), data(dt), learner(lr) { gen_cpp = false; }
  std::string name;
  std::string data;
  std::string learner;
  boost::unordered_set<std::string> predicts;
  bool gen_cpp;
};
std::string ToString(const TTrainInfo &Info);


struct TCrossvalidationInfo {
  TCrossvalidationInfo() { }
  // C++11 -> we have no initializer, so we MUST write stupid
  // conctructor manually
  explicit TCrossvalidationInfo(const std::string &fd):fold(fd) { }
  std::string fold;
  boost::unordered_set<std::string> learners;
  boost::unordered_set<std::string> measures;
  boost::unordered_set<std::string> datas;
};
std::string ToString(const TCrossvalidationInfo &Info);


class ConfiguratorPrivate;
class Configurator {
 public:
  typedef boost::unordered_map<std::string, TDataInfo> TDataInfos;
  typedef boost::unordered_map<std::string, TXmlTokenSpec> TXmlTokenSpecs;
  typedef boost::unordered_map<std::string, TTrainInfo> TTrainInfos;
  typedef boost::unordered_map<std::string, TCrossvalidationInfo>
                                                          TCrossvalidationInfos;

  Configurator();
  ~Configurator();
  void loadConfig(const std::string &file_name);
  // bool isValid() const;

  const TDataInfos &dataInfos() const;
  const TXmlTokenSpecs &xmlTokenSpecs() const;
  const TTrainInfos &trainInfos() const;
  const TCrossvalidationInfos &crossvalidationInfos() const;

  const TXmlTokenSpec &findLearner(const std::string &name) const;
  const TDataInfo &findData(const std::string &name) const;

  const std::string &rootPath() const;

 private:
  ConfiguratorPrivate * const d;
};

#endif  // LTR_CLIENT_CONFIGURATOR_H_
