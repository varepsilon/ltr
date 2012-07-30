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

class TagHandler;

typedef boost::unordered_map<string, TagHandler*> TagHandlers;

struct TDataInfo {
  TDataInfo() { }
  // C++11 -> we have no initializer, so we MUST write
  // stupid conctructor manually
  TDataInfo(const string& name_, const string& approach_,
            const string& format_, const string& file_name_):
    name(name_), approach(approach_), format(format_), file(file_name_) { }
  string name;
  string approach;
  string format;
  string file;
};
string ToString(const TDataInfo &Info);

struct TXmlTokenDependency {
  // C++11 -> we have no initializer, so we MUST write
  // stupid conctructor manually
  explicit TXmlTokenDependency(const string& parameter_name_)
    : parameter_name(parameter_name_) { }
  string parameter_name;
};


struct TrainLaunchInfo {
  TrainLaunchInfo() {}
  // C++11 -> we have no initializer, so we MUST write stupid
  // conctructor manually
  TrainLaunchInfo(const string& nm,
             const string& dt,
             const string& lr):
    name(nm), data(dt), learner(lr) { gen_cpp = false; }
  string name;
  string data;
  string learner;
  boost::unordered_set<string> predicts;
  bool gen_cpp;
};
string ToString(const TrainLaunchInfo& Info);


struct CrossvalidationLaunchInfo {
  CrossvalidationLaunchInfo() { }
  // C++11 -> we have no initializer, so we MUST write stupid
  // conctructor manually
  explicit CrossvalidationLaunchInfo(const string& fold_)
    : splitter(fold_) { }
  string splitter;
  boost::unordered_set<string> learners;
  boost::unordered_set<string> measures;
  boost::unordered_set<string> datas;
};
string ToString(const CrossvalidationLaunchInfo& Info);

class ParametrizedInfo;



class ConfigParser {
 public:
  typedef boost::unordered_map<string, TDataInfo> TDataInfos;
  typedef boost::unordered_map<string, ParametrizedInfo> TXmlTokenSpecs;
  typedef boost::unordered_map<string, TrainLaunchInfo> TTrainInfos;
  typedef boost::unordered_map<string, CrossvalidationLaunchInfo>
                                                          TCrossvalidationInfos;

  ConfigParser();
  ~ConfigParser();
  void parseConfig(const string& file_name);

  const TDataInfos& dataInfos() const;
  TDataInfos& dataInfos();
  const TXmlTokenSpecs& xmlTokenSpecs() const;
  TXmlTokenSpecs& xmlTokenSpecs();
  const TTrainInfos& trainInfos() const;
  TTrainInfos& trainInfos();
  const TCrossvalidationInfos& crossvalidationInfos() const;
  TCrossvalidationInfos& crossvalidationInfos();
  const ParametrizedInfo& findLearner(const string &name) const;
  const TDataInfo& findData(const string &name) const;

  const string& rootPath() const;

 private:
  TagHandlers tag_handlers_;
  TagHandler* general_xml_token_;

  auto_ptr<TiXmlDocument> document_;
  TiXmlElement* root_;
  string root_path_;
  ConfigParser::TDataInfos data_infos_;
  ConfigParser::TXmlTokenSpecs xml_token_specs;
  ConfigParser::TTrainInfos train_infos;
  ConfigParser::TCrossvalidationInfos crossvalidation_infos;
};

class ParametrizedInfo;
typedef std::list<const ParametrizedInfo*> TXmlTokenSpecList;
class ParametrizedInfo {
 public:
  ParametrizedInfo();
  ParametrizedInfo(const string& tag_name,
                   const string& name,
                   const string& type,
                   const string& approach,
                   ltr::ParametersContainer parameters);
  ParametrizedInfo(const ParametrizedInfo& other);
  ParametrizedInfo& operator=(const ParametrizedInfo& other);
  ~ParametrizedInfo();

  const string& getTagName() const;
  const string& getName() const;
  const string& getType() const;
  const string& getApproach() const;
  const ltr::ParametersContainer& getParameters() const;

  const TXmlTokenSpecList& dependencySpecs() const;
  void fillDependencyList(
      const ConfigParser::TXmlTokenSpecs& token_specifications);

 private:
  string tag_name_;
  string name_;
  string type_;
  string approach_;
  ltr::ParametersContainer parameters_;
  TXmlTokenSpecList dependency_specs_;

  friend class ConfigParser;
  friend class OnGeneralXmlToken;
};
string ToString(const ParametrizedInfo& Info);

#endif  // LTR_CLIENT_CONFIGURATOR_H_
