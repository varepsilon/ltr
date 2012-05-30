// Copyright 2011 Yandex

#ifndef LTR_CLIENT_CONFIGURATOR_H_
#define LTR_CLIENT_CONFIGURATOR_H_

#include <string>
#include <boost/unordered_map.hpp>  //NOLINT
#include <boost/unordered_set.hpp>  //NOLINT
#include <list>

#include "tinyxml/tinyxml.h"

#include "ltr/parameters_container/parameters_container.h"

using std::string;
using std::auto_ptr;

class TExecutor;

typedef boost::unordered_map<string, TExecutor*> TStrExecMap;

struct TDataInfo {
  TDataInfo() { }
  // C++11 -> we have no initializer, so we MUST write
  // stupid conctructor manually
  TDataInfo(const string& nm, const string& app,
            const string& fmt, const string& fn):
    name(nm), approach(app), format(fmt), file_name(fn) { }
  string name;
  string approach;
  string format;
  string file_name;
};
string ToString(const TDataInfo &Info);

struct TXmlTokenDependency {
  // C++11 -> we have no initializer, so we MUST write
  // stupid conctructor manually
  explicit TXmlTokenDependency(const string& nm): parameter_name(nm) { }
  string parameter_name;
};

class TXmlTokenSpec;
typedef std::list<const TXmlTokenSpec*> TXmlTokenSpecList;
class TXmlTokenSpecPrivate;
class TXmlTokenSpec {
 public:
  TXmlTokenSpec();
  TXmlTokenSpec(const TXmlTokenSpec& other);
  TXmlTokenSpec& operator=(const TXmlTokenSpec& other);
  ~TXmlTokenSpec();

  const string& tagName() const;
  const string& name() const;
  const string& type() const;
  const string& approach() const;
  const ltr::ParametersContainer& parameters() const;

  const TXmlTokenSpecList& dependencySpecs() const;

 private:
  TXmlTokenSpecPrivate* const d;

  friend class Configurator;
  friend class TOnGeneralXmlToken;
};
string ToString(const TXmlTokenSpec& Info);


struct TTrainInfo {
  TTrainInfo() {}
  // C++11 -> we have no initializer, so we MUST write stupid
  // conctructor manually
  TTrainInfo(const string& nm,
             const string& dt,
             const string& lr):
    name(nm), data(dt), learner(lr) { gen_cpp = false; }
  string name;
  string data;
  string learner;
  boost::unordered_set<string> predicts;
  bool gen_cpp;
};
string ToString(const TTrainInfo& Info);


struct TCrossvalidationInfo {
  TCrossvalidationInfo() { }
  // C++11 -> we have no initializer, so we MUST write stupid
  // conctructor manually
  explicit TCrossvalidationInfo(const string& fd):fold(fd) { }
  string fold;
  boost::unordered_set<string> learners;
  boost::unordered_set<string> measures;
  boost::unordered_set<string> datas;
};
string ToString(const TCrossvalidationInfo& Info);


class ConfiguratorPrivate;
class Configurator {
 public:
  typedef boost::unordered_map<string, TDataInfo> TDataInfos;
  typedef boost::unordered_map<string, TXmlTokenSpec> TXmlTokenSpecs;
  typedef boost::unordered_map<string, TTrainInfo> TTrainInfos;
  typedef boost::unordered_map<string, TCrossvalidationInfo>
                                                          TCrossvalidationInfos;

  Configurator();
  ~Configurator();
  void loadConfig(const string& file_name);
  // bool isValid() const;

  const TDataInfos& dataInfos() const;
  TDataInfos& dataInfos();
  const TXmlTokenSpecs& xmlTokenSpecs() const;
  TXmlTokenSpecs& xmlTokenSpecs();
  const TTrainInfos& trainInfos() const;
  TTrainInfos& trainInfos();
  const TCrossvalidationInfos& crossvalidationInfos() const;
  TCrossvalidationInfos& crossvalidationInfos();
  const TXmlTokenSpec& findLearner(const string &name) const;
  const TDataInfo& findData(const string &name) const;

  const string& rootPath() const;

 private:
  TStrExecMap handlers_;
  TExecutor* general_xml_token;

  auto_ptr<TiXmlDocument> document_;
  TiXmlElement* root_;
  string root_path_;
  Configurator::TDataInfos data_infos_;
  Configurator::TXmlTokenSpecs xml_token_specs;
  Configurator::TTrainInfos train_infos;
  Configurator::TCrossvalidationInfos crossvalidation_infos;
};

#endif  // LTR_CLIENT_CONFIGURATOR_H_
