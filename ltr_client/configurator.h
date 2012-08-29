// Copyright 2012 Yandex

#ifndef LTR_CLIENT_CONFIGURATOR_H_
#define LTR_CLIENT_CONFIGURATOR_H_

#include <string>
#include <list>

#include <boost/unordered_map.hpp> // NOLINT
#include <boost/unordered_set.hpp> // NOLINT

#include "tinyxml/tinyxml.h"

#include "ltr/parameters_container/parameters_container.h"

#include "ltr_client/utility/crossvalidation_launch_info.h"
#include "ltr_client/utility/data_info.h"
#include "ltr_client/utility/train_launch_info.h"
#include "ltr_client/utility/parameterized_info.h"

using std::string;
using std::auto_ptr;

class TagHandler;

typedef boost::unordered_map<string, TagHandler*> TagHandlers;

/**
 * Performs parsing of the XML config file
 */
class ConfigParser {
 public:
  typedef boost::unordered_map<string, DataInfo> DataInfos;
  typedef boost::unordered_map<string, ParametrizedInfo> ParameterizedInfos;
  typedef boost::unordered_map<string, TrainLaunchInfo> TrainInfos;
  typedef boost::unordered_map<string, CrossvalidationLaunchInfo>
    CrossvalidationInfos;
  /**
   * Basic constructor for ConfigParser
   */
  ConfigParser();
  /**
   * Basic destructor for ConfigParser
   */
  ~ConfigParser();
  /**
   * Performs the parsing of a given file.
   * @param file_name - string containing file name.
   */
  void parseConfig(const string& file_name);

  /**
   * Constant accessor to the data_info_ field
   * @returns data_info_ constant link
   */
  const DataInfos& dataInfos() const;
  /**
   * Accessor to the data_info_ field
   * @returns data_info_ link
   */
  DataInfos& dataInfos();
  /**
   * Constant accessor to the data_info_ field
   * @returns xml_token_specs_ constant  link
   */
  const ParameterizedInfos& xmlTokenSpecs() const;
  /**
   * Accessor to the xml_token_specs_ field
   * @returns xml_token_specs_ link
   */
  ParameterizedInfos& xmlTokenSpecs();
  /**
   * Constant accessor to the train_infos_ field
   * @returns train_infos_ constant link
   */
  const TrainInfos& trainInfos() const;
  /**
   * Accessor to the train_infos_ field
   * @returns train_infos_ link
   */
  TrainInfos& trainInfos();
  /**
   * Constant accessor to the crossvalidation_infos_ field
   * @returns crossvalidation_infos_ constant link
   */
  const CrossvalidationInfos& crossvalidationInfos() const;
  /**
   * Accessor to the crossvalidation_infos_ field
   * @returns crossvalidation_infos_ link
   */
  CrossvalidationInfos& crossvalidationInfos();
  /**
   * Performs a search in the xml tokens for the Parametrized
   * object with given name
   * @param name - string with the name of the learner
   * @returns ParametrizedInfo constant link
   */
  const ParametrizedInfo& findParametrized(const string& name) const;
  /**
   * Performs a search in the xml tokens for the dataset
   * with given name
   * @param name - string with the name of the dataset
   * @returns TDataInfo constant link
   */
  const DataInfo& findData(const string& name) const;
  /**
   * Root path constant accessor
   * @returns constant link to the root_path variable
   */
  const string& rootPath() const;

 private:
  TagHandlers tag_handlers_;
  TagHandler* general_xml_token_;

  auto_ptr<TiXmlDocument> document_;
  TiXmlElement* root_;
  string root_path_;
  ConfigParser::DataInfos data_infos_;
  ConfigParser::ParameterizedInfos xml_token_specs;
  ConfigParser::TrainInfos train_infos;
  ConfigParser::CrossvalidationInfos crossvalidation_infos;
};

class ParametrizedInfo;

#endif  // LTR_CLIENT_CONFIGURATOR_H_
